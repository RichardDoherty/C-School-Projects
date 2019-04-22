#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib>


void insertionSort(std::vector<int>& intVector){
    //starting at the second element in the vector compare each integer to the integers before it
    //swapping values with larger integers until in the proper place in the sorted list
    for(int n = 1; n < intVector.size(); n++) {
      //index for the integer before the current
      int i = n - 1;
      //stores the value of the current integer allowing us to swap integers
      int temp = intVector[n];

      //as long as the index being observed is greater than or the integer we are sorting for is less than the
      //previous integers
      while(i >= 0 && temp < intVector[i]) {
          //push larger integers down the vector and decrement the index toward the smaller integers
          intVector[i + 1] = intVector[i];
          i--;;
      }
      intVector[i + 1] = temp;
    }
}


//input:
//output:
void printVector(std::vector<int> intVector) {
  //print each integer stored in the vector
  for (int n = 0; n < intVector.size(); n++) {
      std::cout << intVector[n] << ' ';
  }
  std::cout << std::endl;
}

//input: cstring of the input file name
//output: vector that stored all the values of the
std::vector<int> loadVector(char* fileName){
  //stores value of integer pulled from infile
  int currentInt;
  //vector to be returned
  std::vector<int> intVector;

  std::ifstream infile;
  infile.open(fileName);

  //while there file has not recieved an error or the end of the file
  while(infile.good()){
      //input integer value from the infile
      infile >> currentInt;
      //if the end of the file has not been reached yet add the value to the vector
      //this ensures that the last value is not inserted twice
      if(!infile.eof())
        intVector.push_back(currentInt);
    }
  //close the infile
  infile.close();
  return intVector;
}

void outputVector(std::vector<int> intVector) {
  //create outfile stream
  std::ofstream outfile;
  outfile.open("insert.out");
  //add all integers from vector to the output file
  for(int n = 0; n < intVector.size(); n++){
    outfile << intVector[n] << ' ';
  }
  outfile.close();
}

int main(int argc, char const *argv[]) {
  std::vector<int> intVector;

      //load vector from data input file
      intVector = loadVector((char*)"data.txt");
      //print the list before sorting
      std::cout << "\nInitial List:\t" ;
      printVector(intVector);
      //call sorting algorithm
      insertionSort(intVector);
      //print sorted vector
      std::cout << "After insertion sort:\t" ;
      printVector(intVector);

      //output the sorted vector values to an .out file
      outputVector(intVector);

  return 0;
}
