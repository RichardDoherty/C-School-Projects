#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

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
      //number of integers in the vector
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
          insertionSort(intVector);
          double algorithmTime = (std::clock() - start)/ (double)(CLOCKS_PER_SEC / 1000);
          //output the time it took to sort the vector
          std::cout << "Time elapsed sorting " << nInts << " integers using the insert sort algorithm: " << algorithmTime << " milliseconds\n";
        }
      } while(nInts != 0);

  return 0;
}
