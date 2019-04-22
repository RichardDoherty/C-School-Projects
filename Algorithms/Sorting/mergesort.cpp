#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib>

void merge(std::vector<int>& intVector, std::vector<int>& sortedVector, int left, int right, int end) {

  //store the values of the left and right index so the values can be incremented without affecting the for loop
  int i_left = left;
  int i_right = right;


     //Place integers in sorted order into the same indices in the sorted vector as in the unsorted vector.  At
     //at each step, choose the minimum value from the front of either the left or right sub vector, or if the
     //elements of one vector have all been moved then move elements from the other sub vector.  Increment
     //the index of the front of the sub vector from which the integer was taken


  for (int i = left; i < end; i++) {
    if (i_left >= right) {
      sortedVector[i] = intVector[i_right++];
    } else if (i_right >= end) {
      sortedVector[i] = intVector[i_left++];
    } else if (intVector[i_left] >= intVector[i_right]) {
      sortedVector[i] = intVector[i_left++];
    } else {
      sortedVector[i] = intVector[i_right++];
    }
  }
}

void mergeSort(std::vector<int>& intVector) {

  std::vector<int> sortedVector(intVector.size());

    // start with sub vector of length 1 and then double the length of the
    //sub vectors we’re merging until the whole vector is sorted.


  for (int n = 1; n < intVector.size(); n *= 2) {
    //Merge pairs of adjacent sub vectors. In this loop i is the start of the
    //left sub vector, i + len is the start of the right subvector
    //and i + 2 * len is the end of the right sub vector.  Make sure the last two are within
    //the bounds of the vector using std::min().
    for (int i = 0; i < intVector.size(); i += 2 * n) {
      merge(intVector, sortedVector, i, std::min<int>(i + n, intVector.size()), std::min<int>(i + 2 * n, intVector.size()));
    }
    //copy of the contents from one vector to the other
    intVector = sortedVector;
  }
}

void randomVector(std::vector<int>& intVector, int nInts){

    //clear the vector before adding new integers
    intVector.clear();
    int randInt;
    //add in the specified number of random integers
    for (int n = 0; n < nInts; n++) {
        //generate a random number from 0 to 10,000 and add it to the vector
        randInt = rand() % 10001;
        intVector.push_back(randInt);
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
      intVector = loadVector((char*)"input.txt");
      //print the list before sorting
      std::cout << "\nInitial List:\t" ;
      printVector(intVector);
      //call sorting algorithm
      mergeSort(intVector);
      //print sorted vector
      std::cout << "After insertion sort:\t" ;
      printVector(intVector);

      //output the sorted vector values to an .out file
      outputVector(intVector);

  return 0;
}
