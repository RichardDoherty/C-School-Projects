#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>

//global variables for easily changing the number of running processes or size of buffers
#define nProcesses 5
#define bufferSize 100000

//struct stores active child processes and sockets used
struct activePids{
	pid_t pids[nProcesses];
	int fd[nProcesses];
	int nPids;
};

/*
 * Preconditions:       client connection has been established
 * Postconditions:      reads characters from the server, and ensure that all content is read.
 * Return:              none
 */
void readSocket(char* key, int establishedConnectionFD){
	int charsRead = 0;
	//initialize buffer and set all of its contents to file terminators
	char buffer[bufferSize];
	memset(buffer, '\0', bufferSize);


	//while the file has not socket has not filled the buffer compelely (aside from terminator) keep recieving data
	while(charsRead != bufferSize-1){
		//track the position of chars already read in from server in the recv function
		charsRead += recv(establishedConnectionFD, buffer + charsRead, bufferSize-1 - charsRead, 0);
		//error handle if charsRead is given -1
		if (charsRead < 0){
			fprintf(stderr, "ERROR reading from socket");
			exit(1);
		}
	}
		//copy buffer back into data parameter
	strcpy(key, buffer);
}

/*
 * Preconditions:       An initialzed data pointer has been passed in as a parameter
 * Postconditions:      This function will send data to the server
 * Return:              none
 */
void writeSocket(char*  key, int socketFD){
	//initialize variables to be used and make sure that buffer is filled with terminating chars
	char buffer[bufferSize];
	int charsWritten = 0;
	memset(buffer, '\0', bufferSize);

	strcpy(buffer, key);

	//while the number of chars sent to the server is less then the number of chars in the buffer continue to send them
	while(charsWritten != bufferSize-1){
		//make sure to track the number of chars already sent and the position of the new chars to be sent
		charsWritten += send(socketFD, buffer + charsWritten, bufferSize-1 - charsWritten, 0); // Read the client's message from the socket
		//error handle if the sending process fails and charsWritten is given -1
		if (charsWritten < 0) {
			fprintf(stderr, "ERROR reading from socket\n");
			exit(1);
		}
	}
}


int convertEncInt(char msg){
  //if the acii values are equal to between 65 and 90 return their
  //value minus 65 for formated 0-25 int
 if( msg == ' '){
   return 26;
 }else{
   //the character must be a space so return 26
   return ((int)msg) - 65;
 }
}

/*
 * Preconditions:	a valid key and message has been put in parameters
 * Postconditions:	The function decrypts message to ciphertext
 * Return:	a pointer to a string of decrypted characters
 */

 /*
  * Preconditions:	a valid key and message has been put in parameters
  * Postconditions:	The function decrypts message to ciphertext
  * Return:	a pointer to a string of decrypted characters
  */
 char* decrypt(char* key,char* msg){
    //initialize variable for use and initialize and allocate memory for decrypted message
    int msgInt, keyInt, decryptedInt, n, msgSize = strlen(msg)+1;
    char* decryptedMsg = (char*) malloc(msgSize*sizeof(char));
    //loop through each char in message
    for(n = 0; n < msgSize-1; n++){
      msgInt = convertEncInt(msg[n]);
      keyInt = convertEncInt(key[n]);

      //calculate the decrypted char int value
      decryptedInt = (msgInt - keyInt) % 27;

      //if the value is negatie then add 27
      if(decryptedInt < 0){
        decryptedInt += 27;
      }

      //convert the ints back to chars and store in message
      if(decryptedInt == 26){
        decryptedMsg[n] = ' ';
      }else{
        decryptedMsg[n] = (char) (decryptedInt + 65);
      }


    }
    //return the decrypted message
    return decryptedMsg;
  }


	/*
	 * Preconditions:	valid data has been passed in with the '/' char seperating the key and message, key and message have been initialized and allocated
	 * Postconditions:	The buffer is seperated by the "/" char into the key and message
	 * Return:		none
	 */
void seperateData(char* data, char* key, char* msg){
	//initialize variables that will be used
	int n, m = 0;
 	int keySize = 0;
 	int msgSize = 0;
	//tracks when the key is being read still (ie the '/' has not yet been reached)
 	bool keyScan = true;
   int	dataLen = strlen(data)+1;
 	char keyBuffer[dataLen];
 	memset(keyBuffer, '\0', dataLen);
 	char msgBuffer[dataLen];
 	memset(msgBuffer, '\0', dataLen);
	//loop through all chars in data
 	for(n = 0; n < dataLen; n++){
		//if the '/' char is read then change the keyScan variable to false so we start to scan the message
 		if(data[n] == '/'){
 					keyScan = false;
		//scan and store the current char in the message or key string
 		}else{
			//if keyScan is true then store chars in the key
 			if(keyScan){
 				keyBuffer[n] = data[n];
 				keySize++;
			//else store the chars in the message
 			}else{
 				msgBuffer[m] = data[n];
 				msgSize++;
 				m++;
 			}
 		}
 	}
	//copy the buffer contents into the appropriate string pointer
 	strcpy(key, keyBuffer);
 	key[keySize] = '\0';

 	strcpy(msg, msgBuffer);
 	msg[msgSize] = '\0';
}



/*
 * Preconditions:	there are background processes running and value are store in the activePids arr
 * Postconditions:	This function will clean up the child processes.
 * Return:		none
 */
void reapZombies(struct activePids processes){
				//initialize all variable for use
        int n, m;
        int child_exit_method =-5;
				//loop through all child processes and use waitpid to check if any have exited
				//do not wait for children to exit
        for(n = 0; n < processes.nPids; n++){
						//if a chile has exited then close the socket they were using and delete their pid from array
						//reorganize the other pids in array forward
          	if(waitpid(processes.pids[n], &child_exit_method, WNOHANG) != 0){
								close(processes.fd[n]);
                for(m = n; m < processes.nPids; m++){
                    processes.pids[m] = processes.pids[m+1];
										processes.fd[m] = processes.fd[m+1];
                }
								//set the empty pid values to represent a lack of process and decrement the number of pids in array
								processes.pids[processes.nPids] = -1;
								processes.fd[processes.nPids] = 0;
								processes.nPids--;
								//because the pids moved forward recheck the pid that took over the pid that was deleted
								n--;
            }
        }

}

/*
 * Preconditions:	a connection to client has already been made
 * Postconditions:	sends an decryption identifier to the client to identify whether or not it is otp_enc_d or otp_dec_d.
 * Return:		none
 */
void sendAck(int fd){
				//initialize identifier and charsWritten
        char* id = "decrypt";
        int charsWritten;
				//send the identifier to the client
        charsWritten = send(fd, id, 7, 0);
				//error handle if the send failed
				if(charsWritten < 0){
					fprintf(stderr, "ERROR: Failed to send identifier\n");
					exit(0);
				}
}

/*
 * Preconditions:	data file contains key or message data
 * Postconditions:	returns whether or not the file contains bad contents
 * Return: boolean value if data contains bad content or not
 */
bool badContents(char* data){
	//initialize variables for use
	int asciiChar, n = 0;
	//loop while the end of the data string has not been reached
	while(data[n] != '\0'){
		//store ascii value as int
		asciiChar = (int) data[n];
		//if int ascii value is not in (A-Z) or ' ' return true
		if( ((asciiChar < 65) || (asciiChar > 90)) && (asciiChar != 32)){
			return true;
		}
		n++;
	}
	//if the function has not returned yet all values are valid
	return false;
}

int main(int argc, char *argv[]){

	//check that more than one variable was entered in the command line
	if (argc < 2) {
		fprintf(stderr,"USAGE: %s port\n", argv[0]);
		exit(1);
	}

		//initialize serve and client variables
	int listenSocketFD, establishedConnectionFD, portNumber;
	socklen_t sizeOfClientInfo;
	struct sockaddr_in serverAddress, clientAddress;
	struct activePids processes;
	processes.nPids = 0;



	// Clear the address struct
	memset((char *)&serverAddress, '\0', sizeof(serverAddress));
	//convert the port number to an integer
	portNumber = atoi(argv[1]);
	// Set up the address struct for this process (the server)
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(portNumber);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	// create the socket
	listenSocketFD = socket(AF_INET, SOCK_STREAM, 0);
	//check to make sure that the socket was successfully created
	if (listenSocketFD < 0){
		fprintf(stderr, "ERROR opening socket\n");
		exit(1);
	}

	// ake the the socket begin listening for the client and error handle if it is not able to
	if (bind(listenSocketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0){ // Connect socket to port
		fprintf(stderr, "ERROR on binding\n");
		exit(1);
	}
	//listen for the client
	listen(listenSocketFD, nProcesses); // Flip the socket on - it can now receive up to 5 processes

	//when a client calls out to the server begin the infinite loop to communicate with client
	while(1){

		//clean up any child processes that have exited
		reapZombies(processes);

		//get size of the address of the connecting client
		sizeOfClientInfo = sizeof(clientAddress);
		//accept the connection
		establishedConnectionFD = accept(listenSocketFD, (struct sockaddr *)&clientAddress, &sizeOfClientInfo);

		//error handle if the connection failed
		if (establishedConnectionFD < 0)
			fprintf(stderr,"SERVER: ERROR on accept\n");


		//create child processes
		int spawnpid = fork();
		switch(spawnpid){
			//case of error occuring when creating child process
			case -1:
				perror("child process failed to spawn!\n");
				exit(1);
				break;

			//chile process commands
			case 0:

				//send an Acknowledgement to the client
				sendAck(establishedConnectionFD);

				//initialize key and message variables before client repsonds
				char* key = malloc((bufferSize/2)*sizeof(char));
				char* message = malloc((bufferSize/2)*sizeof(char));
				memset(key, '\0', sizeof(key));
				memset(message, '\0', sizeof(message));

				char buffer[bufferSize];
				memset(buffer, '\0', bufferSize);

				//read in response data from the child
				readSocket(buffer, establishedConnectionFD);

				//seperate the data that the client sent into a key and message
				seperateData(buffer, key, message);

				//check for bad contents in the files
				if(badContents(key) || badContents(message)){
					fprintf(stderr,"ERROR: Bad content in file\n");
					free(message);
					free(key);
					exit(1);
				}

				//create an decrypted message from the message message
				char* decryptedMsg = decrypt(key, message);



				//send the client the encrpted data
				writeSocket(decryptedMsg, establishedConnectionFD);

				//free the dynamically allocated variables and exit successfully
				free(decryptedMsg);
				free(message);
				free(key);
				exit(0);

			//parent commands
			default:
				//add child pids and socket connections to pids array
				processes.pids[processes.nPids] = spawnpid;
				processes.fd[processes.nPids] = establishedConnectionFD;
				processes.nPids++;
		}

	}
	//close(establishedConnectionFD); // Close the existing socket which is connected to the client
	close(listenSocketFD); // Close the listening socket
	return 0;
}
