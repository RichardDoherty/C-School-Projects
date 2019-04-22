#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

void error(){
			perror("Insufficient arguments\n");
			exit(2);
}

bool intValidator(char* charInt){
	bool valid = false;
	int digitNumber, n = 0;
	while(charInt[n] != '\0'){
		digitNumber = (int) charInt[n];
		if(digitNumber > 57 || digitNumber < 48) 
						return false;
		n++;
	}
	return true;
}

int main(int argc, char* argv[]){
		
		if( (argc == 2) && (intValidator(argv[1])) ){
				srand(time(NULL));
				int sizeKey = (int) atoi(argv[1]);
				int n = 0;
				char key[sizeKey + 1];
				int randInt;// lastZero = -6;
				
				while(n < sizeKey){
							randInt = rand() % 27;
							//if( ((randInt == 26) && (n - lastZero > 5)) || (randInt < 26) ){
									if(randInt == 26){
										//lastZero = n;
										key[n] = ' ';
									}else{
										key[n] = (char) randInt + 65;						
									}
									n++;					
							//}
				}	
				key[sizeKey] = '\n';
			printf("%s",key);
		}else{
			error();
		}
		return 0;
}
