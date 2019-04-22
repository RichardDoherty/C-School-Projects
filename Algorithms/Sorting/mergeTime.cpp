#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

void merge(std::vector<int>& intVector, std::vector<int>& sortedVector, int left, int right, int end) {

  //store the values of the left and right index so the values can be incremented without affecting the for loop
  int i_left = left;
  int i_right = right;


  //Place integers in sorted order into the same indices in the sorted vector as in the unsorted vector.  At
  //at each step, choose the minimum value from the front of either the left or right sub vector, or if the
  //elements of one vector have all been moved then move elements from the other sub vector. Increment
  //the index of the front of the sub vector from which the integer was taken


  for (int i = left; i < end; i++) {
    if (i_left >= right) {
      sortedVector[i] = intVector[i_right++];
    } else if (i_right >= end) {
      sortedVector[i] = intVector[i_left++];
    } else if (intVector[i_left] <= intVector[i_right]) {
      sortedVector[i] = intVector[i_left++];
    } else {
      sortedVector[i] = intVector[i_right++];
    }
  }
}

void mergeSort(std::vector<int>& intVector) {

  std::vector<int> sortedVector(intVector.size());

  // start with subarrays of length 1 and then double the length of the
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

int main(int argc, char const *argv[]) {

      //vector of integers
      std::vector<int> intVector;
      //the number of integers to generated
      int nInts;

      //keep prompting the user for vector sizes to keep testing multiple vectors
      do {
        //prompt user and store input
        std::cout << "\nHow many random integers would you like to include in vector to sort? Enter 0 to end the program\nNumber of integers in vector: " ;
        std::cin >> nInts;
        //if the user did not want to exit then create the vector of random integers  and time the sort function
        if(nInts != 0){
          randomVector(intVector, nInts);
          std::clock_t start = std::clock();
          mergeSort(intVector);
          double algorithmTime = (std::clock() - start)/ (double)(CLOCKS_PER_SEC / 1000);
          //output the time it took to sort the vector
          std::cout << "Time elapsed sorting " << nInts << " integers using the insert sort algorithm: " << algorithmTime << " milliseconds\n";
        }
      } while(nInts != 0);

  return 0;
}
