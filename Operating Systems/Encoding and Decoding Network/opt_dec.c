#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdbool.h>

#define bufferSize 100000

/*
 * Preconditions:	The file must exist
 * Postconditions:	The function will generate a dynamically allocated c-string containing the file contents.
 * Return:		char*
 */
char* readContents(char* fileName){
	FILE* fd = fopen(fileName, "r");

	//error handle if open failed
	if(fd < 0){
			fprintf(stderr, "Could not open %s\n", fileName);
			exit(1);
	}

	//initialize and clear buffer
	char buffer[bufferSize];
	char fileChar;
	memset(buffer, '\0', bufferSize);
	int n;

	//loop through the availablility of the buffer and store each data char from file
	for(n = 0; n < bufferSize-1; n++){
		fileChar = fgetc(fd);
		//if an terminator or newline is hit stop recording data from file
		if(fileChar == '\0' || fileChar == '\n')
						break;
		buffer[n] = fileChar;
	}
	//store data in properly allocated array and return it
	int bufferLen = strlen(buffer)+1;
	char* data = (char*) malloc(bufferLen*sizeof(char));
	strncpy(data, buffer, bufferLen);
	fclose(fd);
	return data;
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

/*
 * Preconditions:       client connection has been established
 * Postconditions:      reads characters from the server, and ensure that all content is read.
 * Return:              none
 */
void readSocket(char* key, int establishedConnectionFD){
	int charsRead = 0;
	char buffer[bufferSize];
	memset(buffer, '\0', bufferSize);


		//while the file has not socket has not filled the buffer compelely (aside from terminator) keep recieving data
	while(charsRead != bufferSize-1){
		//track the position of chars already read in from server in the recv function
		// Read client message from socket
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


/*
 * Preconditions:	an Acknowledgement has been sent by server
 * Postconditions: verifies that this program is communicating with the proper server
 * Return:		none
 */
void receiveAck(int socketFD, char* plaintext, char* key){
	//initialize variables and buffer
	int charsRead;
	char buffer[7];
	memset(buffer, '\0', sizeof(buffer));

	//read in verifier
	charsRead = recv(socketFD, buffer, 7, 0);
	//error handle if we failed to recieve data
	if(charsRead < 0){
		fprintf(stderr, "ERROR: Recieving Acknowledgement\n");
		free(plaintext);
		free(key);
		exit(1);
	}
	//exit and output error if the verifier does not match what we expected
	if(strcmp(buffer,"decrypt") != 0){
		free(plaintext);
		free(key);
		fprintf(stderr,"Invalid connection, cannot connect!\n");
		exit(1);
	}
}


int main(int argc, char *argv[]){
	//verify that the correct number of arguments were passed in
	if (argc < 4) {
                 fprintf(stderr, "Improper number of arguments!\n");
                 exit(1);
  }
	//read the contents of both the key and messge files
	char* plaintext = readContents(argv[1]);
	char* key = readContents(argv[2]);


	//check for bad contents in the files
	if(badContents(key) || badContents(plaintext)){
		fprintf(stderr,"CLIENT ERROR: Bad content in file\n");
		free(plaintext);
		free(key);
		exit(1);
	}
	//check that the key is larger than or equal to the message
	if(strlen(key) < strlen(plaintext)){
		fprintf(stderr,"Key file is shorter than plaintext file!\n");
		free(plaintext);
		free(key);
		exit(1);
	}

	//initialize socket and port variables
	int socketFD, port, charsWritten;
	struct sockaddr_in serverAddress;
	struct hostent* serverHostInfo;

	key[strcspn(key, "\n")] = '\0';
	plaintext[strcspn(plaintext, "\n")] = '\0';


	// Clear out the address struct
	memset((char*)&serverAddress, '\0', sizeof(serverAddress));
	// convert the port number to an integer from a string
	port = atoi(argv[3]);
	// Set up the server address
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);

	//get the name of our local IP as the host
	serverHostInfo = gethostbyname("localhost");
	if (serverHostInfo == NULL){
		fprintf(stderr, "CLIENT ERROR: ERROR, no such host\n");
		exit(0);
	}

	// Copy the address into the server address information
	memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length); // Copy in the address

	//create and initialize the socket we will use
	socketFD = socket(AF_INET, SOCK_STREAM, 0);
	if (socketFD < 0){
		fprintf(stderr, "CLIENT: ERROR opening socket\n");
		exit(1);
	}


	// Connect to server
	if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0){ // Connect socket to address
		fprintf(stderr, "CLIENT: ERROR connecting\n");
	}



	//recieve acknowledgement from the server
	receiveAck(socketFD, plaintext, key);

	//create a buffer and store the data from the key and message in it
	char buffer[bufferSize];
	memset(buffer, '\0', sizeof(buffer));
	strcpy(buffer, key);
	strcpy(buffer + strlen(key),"/");
	strcpy(buffer + strlen(key)+1,plaintext);

	//send the buffer to the server
	writeSocket(buffer, socketFD);

	//recieve ciphertext from server
	memset(plaintext, '\0', sizeof(plaintext));
	readSocket(plaintext, socketFD);
	printf("%s\n", plaintext);

 	// Close the socket
	close(socketFD);
	free(plaintext);
	free(key);
	return 0;
}
