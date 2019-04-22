#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

//variable for the maximum number of processes that can be run by program
#define maxProcesses 5

//global variable for foreground only mode
bool foregroundOnly = false;
//global variable to access the terminating signal from the sigint signal
int sigExit = 0;
//struct that stores all of a commands arguments, the count of arguments entered
//the input file specified, the output file if specified, and a boolean indicating 
//if the command is a foreground or background process
struct userCmd{
	char* args[512];
	int nArgs;
	char* inputFile;
	char* outputFile;
	bool foreground;
};

//struct that store the active background processes id values 
//and the count of the number of active background processes
struct activePids{
	pid_t pids[maxProcesses];
	int nPids;
};

//function to kill all running background processes
//no need to pass in exitMethod because we only call this function
//when exiting the shell
void killBackProcesses(struct activePids* processes){
	int n;
	int status = -5;

	//loop through all background processes, kill them, and restore their pid value to -1
	for(n = 0; n < processes->nPids; n++){
		kill(processes->pids[n], SIGTERM);
		waitpid(processes->pids[n], &status, 0);
		processes->pids[n] = -1;
	}
}

//helper function to make sure that I dont accidentally make too many processes
//is check before each fork() call
void checkPids(struct activePids* processes){
	
	//if adding another process would put the program over the maximum number of 
	//processes then print an error message, kill all background processes, and exit the current process
	if((processes->nPids + 1) > maxProcesses){
		killBackProcesses(processes);
		perror("Too many processes running at once\n");
		exit(1);
	}
}

//add a background process id to the struct that stores all background process process ids
//assumes that the checkPids func has been called already so we know we are in bounds of our array
void addPid(struct activePids* processes, pid_t pid){
	//add pid to the end of the array and increment the number of pids in the array
	processes->pids[processes->nPids] = pid;
	processes->nPids++;
}

//remove a process id from the struct that stores the background process ids
void removePid(struct activePids* processes, pid_t pid){
	int n, m;
	//loop through the list of process ids to find the relevant pid
	for(n = 0; n < processes->nPids; n++){
		//once found replace the current index of the pid with the next in line
		if(processes->pids[n] == pid){
			for(m = n; m < (processes->nPids - 1); m++){
				processes->pids[m] = processes->pids[m+1];
			}
			//replace the last pid stored with -1 because it has been copied in the previous slot
			//in the for loop above
			processes->pids[processes->nPids - 1] = -1;
			break;
		}
	}
	processes->nPids--;
}

//initiate the values for the struct that stores the process ids of all background processes
void initProcesses(struct activePids* processes){
	int n;
	//each pid is initialized to -1 so that it will throw an error if accessed
	for(n = 0; n < maxProcesses; n++){
		processes->pids[n] = -1;
	}
	//initial number of pids stored is zero
	processes->nPids = 0;
}

//helper function to get the number of characters in a string
//was having problems with accessing the built in strlen so I created my own
int strLen(char* string){
	int n = 0;
	//count the number of characters until the null terminator is reached
	while(string[n] != '\0'){
		n++;
	};
	return(n+1);
}

//reset the command values for each iteration of shell
void resetCmd(struct userCmd* command){
	command->nArgs = 0;
	command->inputFile = NULL;
	command->outputFile = NULL;
	command->foreground = true;
}

//prompt the user for input and store the input appropriately in the command struct
void getUserCommand(struct userCmd* command){
	char* input;
	int n = 0;
	int sizeInput;
	bool boolInput = false;
	bool boolOutput = false;
	
	//initialize buffer size and intialize memory and contents
	int bufSize = 2048;
	char* buffer = (char*) malloc(bufSize*sizeof(char));
	memset(buffer, '\0', bufSize);

	//prompt the shell
	fflush(stdout);
	printf(": ");
	fflush(stdout);

	//recieve input
	clearerr(stdin);
	fgets(buffer, bufSize, stdin);

	//parse through the input seperated by spaces
	input = strtok(buffer, " \n");
	
	//if thers are values from the input continue parsing until there is no more
	if(input != NULL){
		while(input != NULL){
			//if the input does not indicate an input or output file allocate memory for the command struct and store
			//the input in args array
			if(strcmp(input, "$$") == 0){
				command->args[command->nArgs] = (char*) malloc(10*sizeof(char));
				memset(command->args[command->nArgs], '\0', 10);
				sprintf(command->args[command->nArgs], "%i", getpid());
				command->nArgs++;
			} else if ((strcmp(input, "<") != 0) && (strcmp(input, ">") != 0)){
				//printf("input: %s accessing arg: %i size of input: %i\n", input, command->nArgs, strLen(input)) ;	
				command->args[command->nArgs] = (char*) malloc(strLen(input)*sizeof(char));
				strcpy(command->args[command->nArgs], input);
				command->nArgs++;
			//the input indicates a input or output file
			} else {	
				//the input indicates an input or output file. parse one more time to get the name of the file 
				//then store it in the inputFile or outputFile variable. 
				if (strcmp(input, "<") == 0){
					input = strtok(NULL, " \n");
					command->inputFile = (char*) malloc(strLen(input)*sizeof(char));
					strcpy(command->inputFile, input);
				} else{
					input = strtok(NULL, " \n");
					command->outputFile = (char*) malloc(strLen(input)*sizeof(char));
					strcpy(command->outputFile, input);
				}
				
				//Parse again because the file names are not included in the exec function that will be called later
			}
			input = strtok(NULL, " \n");
		};
		
		//if there is an apperand in the last arg then remove it
		//if the program is not in foreground only mode then change the command
		//forground status to false indicating to run the command in the background
		if(strcmp(command->args[command->nArgs - 1], "&") == 0){
			free(command->args[command->nArgs - 1]);
			command->nArgs--;
			if(foregroundOnly == false){
				command->foreground = false;
			}
		}
	}

	//add the NULL value as the last argument and increment the number of args again
	command->args[command->nArgs] = NULL;
	command->nArgs++;
	free(buffer);	
}

//function to print the the last exit or signal status of our shell
void printStatus(int* exitMethod){
		//if the exit method from the last process was exited
		//print the exit status
		if(sigExit != 0){
			*exitMethod = sigExit;
			sigExit = 0;
		}
		if(WIFEXITED(*exitMethod)){
			printf("exit value %i\n", WEXITSTATUS(*exitMethod));
		}else{
		//if the process wasnt exited than is was terminated by a signal
		//print out the signal value
			if(WIFSIGNALED(*exitMethod)){
				printf("terminated by signal %i\n", WTERMSIG(*exitMethod));
			} else {
			//this else statement should never execute but is used just to be sure
			//nothing weird is happening
				printf("terminated for unknown reason\n");
			}
		}
}


//free the memory that is allocated to the command struct
void freeCommand(struct userCmd* command){
	int n; 
	//for the memory allocated for each argument
	for(n = 0; n < command->nArgs; n++){
		free(command->args[n]);
	}
	
	//if there was an input of output value stored then free the 
	//memory allcocated for its name
	if(command->inputFile != NULL){
		free(command->inputFile);
	}

	if(command->outputFile != NULL){
		free(command->outputFile);
	}
}


void printCommand(struct userCmd* command){
	int n; 
	for(n = 0; n < (command->nArgs - 1); n++){
		printf("command %i: %s\n",n+1, command->args[n]);
	}

	if(command->inputFile != NULL){
		printf("inputFile: %s\n", command->inputFile);
	}
	
	if(command->outputFile != NULL){
		printf("inputFile: %s\n", command->outputFile);
	}

	if(command->foreground){
		printf("printcommand: Operating in foreground\n");
	}else{
		printf("Operating in background\n");
	}
}


//function to execute a completed command struct
void executeCommand(struct userCmd* command, struct activePids* processes, int* exitMethod){
	
	//if there was no arguments added to the command then the first arg will be null
	//if the first character of the first arg is '#' then it is a comment
	//both of these cases result in nothing happening so return the function	
	if((command->args[0] == NULL) || (command->args[0][0] == '#')){
		return;
	//if the first argument is cd then we will be changing the current directory
	} else	if(strcmp(command->args[0], "cd") == 0){
		int status;

		//if there are only two arguments then cd was the only thing entered
		//thus we change to directory to the home directory
		if(command->nArgs == 2){
			//use the getenv function to get the name of the home directory and call the change directory function
		  status = chdir(getenv("HOME"));

			//check for error during directory change
			if(status != 0){
				fflush(stdout);
				printf("Error changing directory\n");
			}
		// if the command has three arguments then we are changing to a specfied directory
		} else if (command->nArgs == 3) {
			//first assume an absolute path has been entered
			status = chdir(command->args[1]);

			//if directory change failed then test is the path is a relative path
			//and add the extension of the current directory to the path
			if(status != 0){
				char cwd[256];
				memset(cwd, '\0',256);
				char relPath[512];
				memset(cwd, '\0',512);
				getcwd(cwd, sizeof(cwd));

				//copy in current dir path
				strcpy(relPath, cwd);
				//add forward slash after dir path
				strcat(relPath, "/");
				//add in desired path after the forward slash
				strcat(relPath, command->args[1]);
				//attempt to change directory again
				status = chdir(relPath);
			}
			//if the directory change failed again then the directory does not exist in the current dir
			if(status != 0){
				fflush(stdout);
				printf("No such file or directory\n");
			}
				
		}
	//if the first arg is exit then kill all background processes and then exit the shell
	} else if(strcmp(command->args[0], "exit") == 0){
			killBackProcesses(processes);
			freeCommand(command);
			printf("Exiting smallsh\n");
			exit(0);
		
	//if the first arg is status then print the exit status or teminating signal of the last process
	} else if(strcmp(command->args[0], "status") == 0){
			printStatus(exitMethod);
	}	else { //Command is a non built in function
		pid_t childPid = -5;
		pid_t spawnID = -5;
		int inputDesc, outputDesc, dupDesc;
		
		//make sure that there are not too many process running 
		checkPids(processes);
		spawnID = fork();
		
		if(spawnID == -1){ //error in fork
				perror("Error in fork\n");	
				*exitMethod = 1;
		} else if(spawnID == 0){ //child process
				*exitMethod = 0;
				//if there is an inut file or the process is running in the foreground then we have to 
				//alter the input path
				if((command->inputFile != NULL) || (command->foreground == false)){
					//indicated that the process is running in the background 
					if(command->inputFile == NULL){
							//route input from dev/null to ensure that nothing is written in but correctly 
							//exits process
							inputDesc = open("/dev/null", O_RDONLY);
					//there is a specified input file
					} else {
							//open the file for reading only
							inputDesc = open(command->inputFile, O_RDONLY);
					}
					//if the file could not be opened print error message and exit process with exit status 1
					if(inputDesc == -1){
						printf("cannot open %s for input\n",command->inputFile);
						fflush(stdout);
						*exitMethod = 1;
						exit(1);
					}
					
					//duplicate the input file descriptor
					dupDesc = dup2(inputDesc, 0);
					//error check for dup 2
					//exit with status 1 if error occured
					if(dupDesc == -1){
						perror("failed dup2 for input file\n");
						fflush(stdout);
						*exitMethod = 1;
						exit(1);
					}
					
					//close file for exec
					fcntl(inputDesc, F_SETFD, FD_CLOEXEC);
				}
				
				//if there is an output file or if the process is operating in the background
				if((command->outputFile != NULL) || (command->foreground == false)){
					//indicate background process
					//use dev/null to ensure that output is sent nowhere but the exit status indicates 
					//successful execution
					if(command->outputFile == NULL){
							outputDesc = open("/dev/null", O_WRONLY | O_CREAT | O_TRUNC, 0664);
					//there is a specified output file
					} else {
							//open output file for writing only
							//if no file matches path then create it
							outputDesc = open(command->outputFile, O_WRONLY | O_CREAT | O_TRUNC, 0664);
					}
					
					//error handle for opening file
					if(outputDesc == -1){
						printf("failed to open %s for output\n", command->outputFile);
						fflush(stdout);
						*exitMethod = 1;
						exit(1);
					}

					//duplicate file descriptor
					dupDesc = dup2(outputDesc, 1);
					//error handle for duplication
					if(dupDesc == -1){
						perror("failed dup2 for output file\n");
						fflush(stdout);
						*exitMethod = 1;
						exit(1);
					}
					
					//close file for exec
					fcntl(outputDesc, F_SETFD, FD_CLOEXEC);
				}
				
				//attempt execution of the command
				//if execvp returns -1 then the function failed due to invalid commands
				//print error message if needed
				if(execvp(command->args[0],command->args) == -1){
					if(command->nArgs == 2){
					printf("%s: no such file or directory\n", command->args[0]);
					} else {
					printf("invalid command\n");
					}
					fflush(stdout);
					*exitMethod = 1;
					exit(1);
				} 
					
		} else { //parent process
				//if the command is a foreground process wait until child has finished termination
				//before reprompting user
				if(command->foreground){
					childPid = waitpid(spawnID, exitMethod, 0);
				//command is a background process
				//add to array of background processes and print process id of child
				}	else {
					addPid(processes, spawnID);
					printf("Background PID: %i\n", spawnID);
					fflush(stdout);	
				}
		}
	}
}

//check to see if any background processes have terminated with wait nohang
void waitBackground(struct activePids* processes, int* exitMethod){
	int backgroundPid;
	
	//check for a terminated process
	backgroundPid = waitpid(-1, exitMethod, WNOHANG);
	
	//if a process has been terminated print its process id, status, and indicator that it finished
	//also remove it from the background process array
	while(backgroundPid > 0){
		if(backgroundPid > 0){
			printf("background pid %i is done: ", backgroundPid);
			printStatus(exitMethod);		
			removePid(processes, backgroundPid);	
		}
		backgroundPid = waitpid(-1, exitMethod, WNOHANG);
	};
}

//signal handler function for sigint (%C)
//signal should allow all child processes of the current process to terminate
//on their own. the parent should then print the terminating signal for the child
//processes
void catchSIGINT(int signo){
				
	pid_t spawnID;
	int exitMethod;
		
		//wait to see if child processes have terminated
		//if there are no child processes for the current process
		//then wait returns -1 
		spawnID = wait(&exitMethod);

		//while there is a child process for the current process
		//print the terminating signal using the write command	
		if(spawnID == -1){
			printf("\n");
		} else {
			while(spawnID != -1){
				if(WIFSIGNALED(exitMethod)){
					char message[24];
					sprintf(message, "terminated by signal %i\n", WTERMSIG(exitMethod));
				 	write(STDOUT_FILENO, message, 24);	
				} else {
					write(STDOUT_FILENO, "terminated for unknown reason\n", 31);
				}
				spawnID = wait(&exitMethod);		
			}
		}
		sigExit = exitMethod;
}

//signal handler function for the sigtstp (%Z) signal
//should switch the shell in and out of foreground-only mode
void catchSIGTSTP(int signo){		
	//if the shell is in foreground only mode then switch it out and print
	//the indicator message to the user
	if(foregroundOnly){
		foregroundOnly = false;
		write(STDOUT_FILENO,"Exiting foreground-only mode\n", 29);
	} else {
	//if the shell is not i foreground only mode then switch in into it and 
	//print the indicator message to the user
		foregroundOnly = true;
		write(STDOUT_FILENO,"Entering foreground-only mode (& is now ignored)\n", 49);

	}
}

void printProcesses(struct activePids* processes){
	int n;
	printf("Process\tPID\n");
	for(n = 0; n < processes->nPids; n++){
		printf("%i\t%i\n",n+1, processes->pids[n]);
	} 
}

int main(){
	//initialize signal action stucts and set the signals to trigger
	//the signal handle functions instead of their default values
	struct sigaction actionSIGINT = {0};
	actionSIGINT.sa_handler = catchSIGINT;
	sigfillset(&actionSIGINT.sa_mask);
	actionSIGINT.sa_flags = 0;
	sigaction(SIGINT, &actionSIGINT, NULL);
	

	struct sigaction actionSIGTSTP = {0};
	actionSIGTSTP.sa_handler = catchSIGTSTP;
	sigfillset(&actionSIGTSTP.sa_mask);
	actionSIGTSTP.sa_flags = 0;
	sigaction(SIGTSTP, &actionSIGTSTP, NULL);
	
	//initialize the command struct and background process id struct
	struct userCmd command;
	struct activePids processes;
	int exitMethod = 0;	
	initProcesses(&processes);
	while(1){
		fflush(stdout);
		resetCmd(&command);
		//printProcesses(&processes);
		getUserCommand(&command);
		//printCommand(&command);
		executeCommand(&command, &processes, &exitMethod);
		waitBackground(&processes, &exitMethod);
		freeCommand(&command);
	};
	return(0);
}
