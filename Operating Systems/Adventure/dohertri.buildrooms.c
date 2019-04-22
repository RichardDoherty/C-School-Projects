#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

struct room{
  char* name;
  int nConnections;
  char* type;
  struct room* connections;
};


//Description: takes an array of integers and randomizes their order
//Parameters: array of integers
//Return: Nothing. Alters int array order
void randomizeOrder(int* order){
  int a, b, temp, n;
  srand(time(NULL));
	//randomly select two indexes to switch 30 times
  for (n = 0; n < 30; n++){
    do{
      a = rand() % 10;
      b = rand() % 10;
    }while(a==b);
	 	//swith values
    temp = order[a];
    order[a] = order[b];
    order[b] = temp;

  }
}

//Description: Assigns names to room structs in array rooms based on randomize int array order
//Parameters: struct room array and integer array with order
//Return: Nothing. Alters the name of rooms in rooms array
void assignNames(struct room* rooms, int* order){
  char names[10][9] = {"Dungeon", "Throne", "Entryway","Dining", "Torture", "Hallway", "Stairs", "Tower", "Armory", "Bathroom"};
  int start, end, n;
	
	//radomly select two room indices that will be the startfile and end file
  do {
      start = rand() % 7;
      end = rand() % 7;
  } while (start == end);

	//loop through all rooms and assign the names and types of each room
  for(n = 0; n < 7; n++){
    rooms[n].name = (char*) malloc(9*sizeof(char));
    strncpy(rooms[n].name, names[order[n]], sizeof(rooms[n].name));
    rooms[n].type = (char*) malloc(11*sizeof(char));
    if(n == start){
    	strncpy(rooms[n].type, "START_ROOM", 11*sizeof(char));
    } else if (n == end) {
    	strncpy(rooms[n].type, "END_ROOM", 11*sizeof(char));
		} else {
    	strncpy(rooms[n].type, "MID_ROOM", 11*sizeof(char));
		}  
		//initialze the number of connection, because none have been made it is zero
    rooms[n].nConnections = 0;
  } 
}

//Description: 	reallocates memory for room connections and connects a room to another
//							this function will be run for each of the rooms being connected
//Parameters: array of room structs and the indexs of the two rooms to be connected 
//Return: Nothing
void connect(struct room* rooms, int room1, int room2){

		//increment the number of connections for room one
    rooms[room1].nConnections++;

		//allocate or reallocate the connections array based on the number of connections
    int n = rooms[room1].nConnections;
    if(n == 1){
        rooms[room1].connections = (struct room*) malloc(sizeof(struct room));
    } else{
        rooms[room1].connections = (struct room*) realloc(rooms[room1].connections, n*sizeof(struct room));
    }
		//connect the rooms
    rooms[room1].connections[n-1] = rooms[room2];
}

//Description:	sees if two rooms are connected to eachother already 
//Parameters:		two struct rooms
//Return: 			boolean value. returns true if the rooms are connected and false if they are not
bool areConnected(struct room room1, struct room room2){
		
		int n;
		//loop through all connections and if the rooms are connected return true else return false
    for(n = 0; n < room1.nConnections; n++){
      int comparrison = strcmp(room1.connections[n].name, room2.name);
      if (comparrison == 0){
        return true;
      }
    }
    return false;

}

/*Description: 	loop through each room in array and connect it with a random room that has under 6 connections
            		Connections are made by adding corresponding room names to a connected rooms list of names
  Parameters: 	array of room structs
	Returns: 			Nothing. Alter the rooms array
*/
void makeConnections(struct room* rooms){
		
      int a, b, minConnections, selector;
			//randomize the min connections for an iteration as 3 or 4 (adds randomness)
      for(a = 0; a < 7; a++){
				selector = rand() % 1;
				if((a % 2) == selector){
					minConnections = 4;
				} else {
					minConnections = 3;
				}
				//while the min number of connections have not been met for a room keep making connections
        while(rooms[a].nConnections < minConnections){
					//keep selecting indices to connect while the indices are the same
          do {
            b = rand() % 7;
          } while((b == a) || (rooms[b].nConnections >= 6) || areConnected(rooms[a], rooms[b]));
					//connect the two rooms that were selected
          connect(rooms, a, b);
          connect(rooms, b, a);
        };
      }


}

//Description: 	Void funtion to create a directory to store room files in
//Parameters: 	Char pointer that will get the name of the directory that was 
//							created
//Return: 			Nothing
void makeDirectory(char* directoryName){
	//get and store the process ID
	int pID = getpid();

	//store the process ID (integer) as an array of chars
	char endfile[10];
	sprintf(endfile, "%ld" ,pID);
	
	//initialize the front section of the directory and connect the front and end of the directory
	char frontName[16] = "dohertri.rooms.";
	strcat(frontName,endfile);

	//create the directory
	int directory = mkdir(frontName, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	strncpy(directoryName, frontName, 26*sizeof(char));
}

//Description:	Adds the room data for a room to a file
//Parameters:		struct room and char array that hold the path to the file 
//							that the data will be added to
//Return: 			Nothing. Creates a file with room data
void addContents(struct room Room, char* filePath){
	int n;
	//open the given file
	FILE* file = fopen(filePath, "w");

	//write the names, connected room names and room type to file
	fprintf(file, "ROOM NAME: %s\n", Room.name);
	for(n = 0; n < Room.nConnections; n++){
		fprintf(file, "CONNECTION %i: %s\n", n+1 , Room.connections[n].name);
	}
	fprintf(file, "ROOM TYPE: %s\n", Room.type);
	//close the opened file
	fclose(file);
}

//Description: Void function to create and add all room files in a given directory
//Parameters: array of initialized room structs, int number of rooms in the array
//					  array of chars that holds the directory name
//Return: 		Nothing.
void createRoomFiles(struct room* rooms, int nRooms, char* directoryName){
	int n,i;
	//initialize char array that will hold the file path
	char filePath[26];
	//loop throught each room in rooms array
	for(n = 0; n < nRooms; n++){
		i = n;

		//construct the file path starting with the directory name, a slash, 
		//and the constructed file name
		strncpy(filePath, directoryName, sizeof(filePath));
		strcat(filePath, "/");
		strcat(filePath, rooms[i].name);
		strcat(filePath, "_room");
		addContents(rooms[n], filePath);

		//clear the contents for the next iteration
		memset(filePath, '\0', sizeof(filePath));
	}
	
}

//Description: 	frees the memory allocated to the connections, type name, and name
//							also frees memory allocated to the rooms array
//Parameters:		array of room structs and the number of rooms in the array
//Return: 			nothing. Alters the rooms array
void freeRoomMemory(struct room* rooms, int nRooms){
	int r, n;

	//clear the connections, type names, and names of the rooms in room array
	for(r = 0; r < nRooms; r++){
		free(rooms[r].connections);
		free(rooms[r].type);
		free(rooms[r].name);
	}
	//free the memory given to the rooms array
	free(rooms);
}

int main(int argc, char const *argv[]) {

	//initialize order array initially in order
  int order[10] = {0,1,2,3,4,5,6,7,8,9};

	//initialize variables for number of names and rooms 
  int nNames = 10;
  int nRooms = 7;

	//randomize the order of the order array
	randomizeOrder(order);
	
	//create an array of structs and assign them names and connect the rooms
  struct room* rooms = (struct room*) malloc(nRooms*sizeof(struct room));
  assignNames(rooms, order);
  makeConnections(rooms);

	//initialize char array and assign directory name to it
	char* directoryName = (char*) malloc(26*sizeof(char));
	makeDirectory(directoryName);

	//create the room files in the created directory
	createRoomFiles(rooms, nRooms, directoryName);	
  
	//free all the memory that has been allocated
	free(directoryName);
  freeRoomMemory(rooms, nRooms);
  return 0;
}
