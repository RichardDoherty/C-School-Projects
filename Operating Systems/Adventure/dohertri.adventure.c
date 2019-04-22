#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>


struct Room{
	char* name;
	int nConnections;
	struct Room* connections;
	char* type;
};


//Description:	Checks if a prefix char array is contained in another char array
//Parameters:		test char array and prefix char array we are checking for
//Returns: 			boolean value. True if prefix is contained in char array and false else
bool containsPrefix(char* test, char* prefix){
	int a, b, lenPrefix, lenTest;
	//get number of chars stored in arrays
	lenTest = strlen(test);
	lenPrefix = strlen(prefix);

	//initialize the match to false
	bool match = false;
	
	//loop through each character in the test arra
	for(a = 0; a < lenTest; a++){
		//if a matching char has been found check the following chars
		if(test[a] == prefix[0]){
			for(b = 1; b < lenPrefix; b++){
				match = true;
				if(prefix[b] != test[a + b]){
					match = false;
					break;
				}
			}
			//if match is true at this point then there was a char sequence in test array that
			//matches the chars in prefix array
			if(match == true){
				break;
			}
		}
	}

	return match;
}

//Description:	Find and returns the value of the newest directory
//Parameters:		None
//Returns: 			char array that contains the directory name
char* findDirectory(){
	//initialize the directory pointer and allocate and clear memory for present working directory
	//and the directory name
	char* pwdName = malloc(256*sizeof(char));
	memset(pwdName, '\0', 256);
	DIR* pwd;
	char* directoryName = (char*) malloc(256*sizeof(char));
	memset(directoryName, '\0', 256);

	//prefix for directory
	char* dirPrefix = "dohertri.rooms.";

	//initialize variables used for reading in files and
	//finding their characteristics (stats)
	struct stat dirStats;
	struct dirent* unknownFile;
	time_t minTime;
	bool firstChecked = true;
	
	//get the present working directory and store it in a char array
	getcwd(pwdName, 256*sizeof(char));
	if(pwdName == NULL){
		perror("Unable to connect to pwd");
		exit(1);
	}
	//open the directory
	pwd = opendir(pwdName);
	free(pwdName);
	
	//read each file in the directory. If a filename contains the directory prefix
	while((unknownFile = readdir(pwd)) != NULL){
			if(containsPrefix(unknownFile->d_name, dirPrefix)){
				stat(unknownFile->d_name, &dirStats);
				//if the directory contains the prefix either store its creation time or
				//compare it to the current newest creation time
				if((firstChecked) || (difftime(minTime, dirStats.st_mtime) < 0)){
					memset(directoryName, '\0', sizeof(directoryName));
					strncpy(directoryName, unknownFile->d_name, 256*sizeof(char));
					minTime = dirStats.st_mtime;
					firstChecked = false;
				}
			}
	}
	
	//if first checked is still true then there was no directory in the cwd that contained the prefix
	if(firstChecked == true){
		perror("No directory matching prefix: \"dohertri.rooms\"\n");
		exit(1);
	}
	//close the directory and return the directory name
	closedir(pwd);
	return directoryName;
}

//Description: 	Load names from room files into room structs
//Parameters:		char array that is the first line from a file, rooms array, and the index of the room 
//							being created
//Returns: 			Nothing. Alters the rooms array
void loadName(char* buffer, struct Room* rooms, int nRoom){
	int n;
	//there are three spaces in the line 
	//using strtok three times will get us to the room name
	char* roomName = strtok(buffer, " ");
	for(n = 0; n < 2; n++){
		roomName = strtok(NULL, " \n");
	}
	//clear and allocate memory for the room name
	//copy the name from the file and initialize the number of connections for the room
	//because no connections have been made yet it is set to zero
	rooms[nRoom].name = (char*) malloc(9*sizeof(char));
	memset(rooms[nRoom].name, '\0', 9);
	strcpy(rooms[nRoom].name, roomName);
	rooms[nRoom].nConnections = 0;
}

//Description: 	finds the index of a room in the rooms array based on its name
//Parameters:		array of rooms, char array containing name of room being searched for
//Returns: 			the index of the room in the rooms array
int findIndex(struct Room* rooms, char* name){
	int n;
	//loop through each room in rooms array and if a match is found then return its index
	for(n = 0; n < 7; n++){
		if(strcmp(rooms[n].name, name) == 0){
			return n;
		}
	}
	//print error if index was not found (should never occur)
	perror("Error finding room index in findIndex() function");
	exit(1);
}

//Description: 	reallocates memory for room connections and connects a room to another
//							this function will be run for each of the rooms being connected
//Parameters: array of room structs and the indexs of the two rooms to be connected 
//Return: Nothing
void connect(struct Room* rooms, int room1, int room2){

		//increment the number of connections for room one
    rooms[room1].nConnections++;

		//allocate or reallocate the connections array based on the number of connections
    int n = rooms[room1].nConnections;
    if(n == 1){
        rooms[room1].connections = (struct Room*) malloc(sizeof(struct Room));
    } else{
        rooms[room1].connections = (struct Room*) realloc(rooms[room1].connections, n*sizeof(struct Room));
    }
		//connect the rooms
    rooms[room1].connections[n-1] = rooms[room2];
}


//Description: 	Load connections into room from lines from room file
//Parameters:		char array buffer that contains the line from room file
//							array of rooms and index of room we are adding
//Returns: 			Nothing. Alters the rooms array
void loadData(char* buffer, struct Room* rooms, int nRoom){
	int n, nConnect;
	char type[11];
	memset(type, '\0', 11);
	//use strtok to find each group of characters taht arent a space or newling
	//store the first word that will identify the data as a connection or room type
	char* buff = strtok(buffer, " ");
	strcpy(type, buff);
	//running strtok twice more will get us either the connection name or the room type
	for(n = 0; n < 2; n++){
		buff = strtok(NULL, " \n");
	}
	
	//if the data type is a connection find the index of the room and connect them
	//else allocate and assign the room type
	if(strcmp(type, "CONNECTION") == 0){
		nConnect = findIndex(rooms, buff);
		connect(rooms, nRoom, nConnect);
	}else{
		rooms[nRoom].type = (char*) malloc(11*sizeof(char));
		memset(rooms[nRoom].type, '\0', 11);
		strcpy(rooms[nRoom].type, buff);
	}
}

//Description: 	Find which room is the start room 
//Parameters:		array of rooms
//Returns: 			pointer to the start room
struct Room* findStartRoom(struct Room* rooms){
	int n;
	struct Room* startRoom;
	//loop through all rooms and if the type is start room assign the rooms address
	//to the pointer and return it 
	for(n=0; n < 7; n++){
		if(strcmp(rooms[n].type, "START_ROOM") == 0){
				startRoom = &rooms[n];
		}
	}
	return startRoom;
}

//Description: 	reads data from room files stored in a directory and loads it into the room struct array
//Parameters:		array of room structs and the name of the directory we will be searching
//Returns: 			Nothing. Alters the rooms array
void readRoomData(struct Room* rooms, char* directoryName){
	//create the directory pointer and open the directory passed by directoryName
	DIR* directory;
	directory = opendir(directoryName);

	//initialize and clear char array buffer adn filepath
	char buffer[256];
	memset(buffer, '\0', 256);
	char filePath[256];
	memset(filePath, '\0', 256);

	//create the file pointer and the dirent to read each file in directory
	struct dirent* unknownFile;
	FILE* file;
	int n, lineCount, fileCount;

	//loop twice. In first loop the names and number of connections will be established for each room.
	//In the second loop and the connections and room type will be added (the number of connection is required for this)
	for(n = 0; n < 2; n++){
		//reopen the directory each time because we need to loop through each file twice
		directory = opendir(directoryName);
		if(directory == NULL){
			perror("Failed to open directory in parameter of void readRoomData(struct room*, char*)");
			exit(1);
		}
		fileCount = 0;
		//while we successfully still reading files (ignore current and previous directory files)
		//build the file path and open the file. Read each file line by line and pass them into the loadName
		//and the loadData functions
		while((unknownFile = readdir(directory)) != NULL){
				if((strcmp(unknownFile->d_name, ".") != 0) && (strcmp(unknownFile->d_name, "..") != 0)){
					getcwd(filePath, sizeof(filePath));
					strcat(filePath, "/");
					strcat(filePath, directoryName);
					strcat(filePath, "/");
					strcat(filePath, unknownFile->d_name);
					file = fopen(filePath, "r");
					if(file == NULL){
						perror("Cannot open file");
						exit(1);
					}
					if(n == 0){
						fgets(buffer, 256,file);
						loadName(buffer, rooms, fileCount);
						memset(buffer, '\0', 256);
					}else{
						//line count will keep us from getting the first line for the readData function
						lineCount = 0;
						while((fgets(buffer, 256, file)) != NULL){
							if(lineCount > 0){
									loadData(buffer, rooms, fileCount);
							}
							lineCount++;
							memset(buffer, '\0', 256);
						}
					}
					fclose(file);
					fileCount++;
				}

		}
	}
}

//Description: 	frees memory that is allocated for the rooms array
//Parameters:		rooms array and the number of rooms stored in the array
//Returns: 			Nothing. Alters the rooms array
void freeRoomMemory(struct Room* rooms, int nRooms){
	int r, n;
	//loop throught each room and free memory 
	for(r = 0; r < nRooms; r++){
		free(rooms[r].connections);
		free(rooms[r].type);
		free(rooms[r].name);
	}
	//free rooms memory
	free(rooms);
}

//Description:	Prints room information such as room name, connections. Used in game interface
//Parameters:		room pointer
//Returns: 			Nothing. 
void printRoomInfo(struct Room* room){
	int n;
	//function is used for current room 
	//print name and connections
	printf("CURRENT LOCATION: %s\n", room->name);
	printf("POSSIBLE CONNECTIONS: ");
	for(n = 0; n < room->nConnections; n++){
		printf("%s",room->connections[n].name);
		if(n == (room->nConnections - 1)){
			printf(".\n");
		}else{
			printf(", ");
		}
	}
}

//Description: 	checks if a room has a connection
//Parameters:		room pointer and connection char array that contains name of connection
//Returns: 			boolean value. Returns true is a room has a connection and false if not
bool hasConnection(struct Room* room, char* connection){
	int n;
	//loop through each connection and if the name of connection is equal to connection name
	//return true
	for(n = 0; n < room->nConnections; n++){
		if(strcmp(connection, room->connections[n].name) == 0){
			return true;
		}
	}
	return false;
}

//Description:
//Parameters:
//Returns: 
void* storeAndPrintTime(void *param){
	
	//create and initialize mutex and lock it
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_lock(&mutex);

	//create fileName of file for storing time
	char* fileName = "currentTime.txt";
	//initialize timeBuffer to hold time info and current directory name
	char* timeBuffer = (char*) malloc(256*sizeof(char));
  char* currentDirectory = (char*) malloc(256*sizeof(char));
	
	//initialize variables for creating and writing to a file
	//and also to calculate time data
	FILE* file;
	struct tm* currentTime;
	time_t timeStats;
	time(&timeStats);
	currentTime = localtime(&timeStats);

	//create file path
	getcwd(currentDirectory, 256);
	strcat(currentDirectory, "/");
	strcat(currentDirectory, fileName);
	file = fopen(currentDirectory, "w");

	//get time data and store it in the currentTime.txt file and output to stdout
	strftime(timeBuffer,100,"%I:%M%p, %A, %B %d, %Y", currentTime);
	fprintf(file,"%s", timeBuffer);
	printf("%s\n\n", timeBuffer);

	//close the file free the char arrays and unlock/destroy mutex
	fclose(file);
	free(timeBuffer);
	free(currentDirectory);
	pthread_mutex_unlock(&mutex);
	pthread_mutex_destroy(&mutex);

}

//Description: 	Interface and decision making of the game 
//Parameters:		rooms array
//Returns: 			Nothing
void startGame(struct Room* rooms){
	char* roomList[256];
	char userInput[256];
	//create a pointer to the startRoom
	struct Room* currentRoom = findStartRoom(rooms);
	int n, newIndex, stepCount = 0;

	//used to keep file info from printing after time call
	bool time = false;

	//while the current room being pointed to is not of type endroom keep looping
	while(strcmp(currentRoom->type, "END_ROOM") != 0){
		if(!time){
			printRoomInfo(currentRoom);
		}
		time = false;

		//get user input
		printf("WHERE TO? >");
		scanf("%s", userInput);
		printf("\n");
	
		//if the user enters a room name that the current room is connected to
		//move the current room pointer to it, add the new room name to the roomList array
		//and increment the step counter
		if(hasConnection(currentRoom, userInput)){
			newIndex = findIndex(rooms, userInput);
			currentRoom = &rooms[newIndex];
			roomList[stepCount] = currentRoom->name;
			stepCount++;

		//if the user enters "time" then create a mutex and new thread
		//then lock the mutex. Then run the storeAndPrintTime function with the
		//thread. Then unlock the mutex and use pthread join to return to when the pthread
		//is done executing. Then destroy the mutex
		}else if(strcmp(userInput, "time") == 0){
			time = true;
			pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
			int resultCode;
			pthread_t threadID;
			pthread_mutex_lock(&mutex);
			resultCode = pthread_create(&threadID, NULL, storeAndPrintTime, NULL);
			if(resultCode != 0){
				perror("Error in pthread_create in function: startGame");
				freeRoomMemory(rooms, 7);
				exit(1);
			}
			pthread_mutex_unlock(&mutex);
			resultCode = pthread_join(threadID,NULL);
			if(resultCode != 0){
				perror("Error in pthread_join in function: startGame");
				freeRoomMemory(rooms, 7);
				exit(1);
			}
			pthread_mutex_destroy(&mutex);

		//if the user does not enter a valid connection or "time" print the error message
		}else{
			printf("HUH? I DON'T UNDERSTAND THAT ROOM. TRY AGAIN.\n\n");
		}
	}
	
	//print the congradulations and tell the user then step count and the rooms they visited
	//that are stored in the roomList
	printf("YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\nYOU TOOK %i STEPS. ", stepCount);
	printf("YOUR PATH TO VICTORY WAS:\n");
	for(n = 0; n < stepCount; n++){
		printf("%s\n", roomList[n]);
	}
}

int main(int argc, char const *argv[]) {
	//initialize a variable for number of rooms
	int nRooms = 7;
	//initialize memory for the array of structs to be created 
	struct Room* rooms = (struct Room*) malloc(nRooms*sizeof(struct Room));

	//find the newest directory
	char* directoryName = findDirectory();

	//read in data from room files in directory and store them in the rooms array
	readRoomData(rooms, directoryName);

	//start the adventure game
	startGame(rooms);

	//free the memory allocated 
	free(directoryName);
	freeRoomMemory(rooms, nRooms);
  return 0;
}
