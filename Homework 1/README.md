# Assignment 1
**Due by 11:59pm on Monday, 1/22/2018**

**Demos due by 11:59pm on Monday, 2/5/2018**

The goals of this assignment are to brush off your C++ skills by working with `struct`s and pointers and to start practicing some of the new things we're learning in this class, like file I/O.  Follow the directions below, and make sure your source code files (**no executable files, `.o` files, or other crufty stuff**) are committed and pushed back into your repository on GitHub to earn credit for the assignment.

## 1. Sign up for Piazza

Use this link to sign up for CS 162 on Piazza using your ONID email address: https://piazza.com/oregonstate/winter2018/cs162.

We'll be using Piazza in this course for Q&A because it's geared towards students helping other students with the class.  Anyone can post or answer a question on Piazza, even anonymously, and the instructor and TAs can revise and endorse student answers, which means you can be confident in the quality of the response.

You are *strongly encouraged* to post any class-related questions to Piazza first instead of emailing the instructor or TAs.  You should also get in the habit of checking in to Piazza to answer other students' questions.  This will not only enable everyone to get help quickly, but it will also help you improve your understanding of the material, since teaching someone else is the best way to learn something.

As an incentive to use Piazza, extra credit will be awarded to the most active Piazza participants at the end of the class.

## 2. Write a program to explore state/county economic data

Write a program to explore a small set of state and county economic data.  Specifically, your program should take a single command line argument which contains the path to a file containing state and county economic data.  For example, if your executable program is called `explore_econdata`, you should be able to run it like this:
```
./explore_econdata test_data.txt
```
Your program must read all of the data from the file that is specified and then allow the user to interactively explore that data.

### Data file format and storage

The file will contain the following data for some number of states:
  * Statewide 2007 unemployment rate
  * Statewide 2015 unemployment rate
  * Statewide median household income
  * For some number of counties in the state:
    * County 2007 unemployment rate
    * County 2015 unemployment rate
    * County median household income

It will have the following format:
```
n_states
state_name state_07_unemployed state_15_unemployed state_med_income n_counties
county_1_name county_1_07_unemployed county_1_15_unemployed county_1_med_income
...
county_n_name county_n_07_unemployed county_n_15_unemployed county_n_med_income
state_name state_07_unemployed state_15_unemployed state_med_income n_counties
...
```

For example, here's what a very small data file might look like:
```
2
Oregon 5.2 5.7 51088 3
Benton 4.1 4.3 54089
Linn 6.3 6.9 44358
Multnomah 4.9 5.0 53519
California 5.4 6.2 61927 2
Alameda 4.7 4.7 76996
San_Francisco 4.2 3.6 83788
```
A more extensive set of data is provided for you in `test_data.txt`.  You can use this file to test your program.

To hold this data, your program should use these two structures:

```
struct county {
  std::string name;
  float unemployed_2007;
  float unemployed_2015;
  int med_income;
};

struct state {
  std::string name;
  float unemployed_2007;
  float unemployed_2015;
  int med_income;
  struct county* counties;
  int n_counties;
};
```

Your program should use the first value in the file (`n_states`) to allocate a dynamic array of type `struct state` big enough to hold data about all of the states.  Then, it should read the info about the first state and dynamically allocate the state's array of counties based on its value of `n_counties`.  After the county array is allocated, you can read the county data for the state and store if in the county array.  Repeat this process for each of the states.

### Program features

After your program reads and stores the state and county data from the specified file, it should allow the user to interactively explore the data.  Specifically, you should allow the user to do these things:
  * Print the state with the highest median household income.
  * Print the state with the lowest median household income.
  * Print the state with the highest unemployment in 2015.
  * Print the state with the lowest unemployment in 2015.
  * Print the states in sorted order by change in unemployment from 2007 to 2015 (i.e. start with the largest decrease in unemployment and end with the largest increase in unemployment).
  * Print the states in sorted order based on median household income.
  * Select a state and then:
    * Print the county in that state with the highest median household income.
    * Print the county in that state with the lowest median household income.
    * Print the county in that state with the highest unemployment in 2015.
    * Print the county in that state with the lowest unemployment in 2015.
    * Print the counties in that state in sorted order by change in unemployment from 2007 to 2015 (i.e. start with the largest decrease in unemployment and end with the largest increase in unemployment).
    * Print the counties in that state in sorted order based on median household income.

After performing one of these tasks, your program should allow the user to select another task to perform, and it should continue asking until the user quits the program.

In addition, your program should exit with an error message if the user does not specify a valid filename.

For the sorting functionality, consider using C++'s built-in `sort()` function: http://www.cplusplus.com/reference/algorithm/sort/.

### Program design

It is up to you how to design an interface to allow the user to perform the tasks above.  A system of menus where the user can enter values (e.g. integer values) to select operations from those menus is one option.

Your program should contain at least the following functions (keeping these prototypes):
  * `struct state* allocate_states(int)` - Allocates an array of a specified number of states.
  * `void read_state_data(struct state*, int, std::ifstream&)` - Reads data for a specified number of states from an input file stream into a given (pre-allocated) array.
  * `struct county* allocate_counties(int)` - Allocates an array of a specified number of counties.
  * `void read_county_data(struct county*, int, std::ifstream&)` - Reads data for a specified number of counties from an input file stream into a given (pre-allocated) array.
  * `void free_state_data(struct state*, int)` - Releases all data (including county data) allocated to given array.  You must make sure to call this function when needed to make sure any allocated data is freed before it is lost or before the program exits.

In addition, you should write any other functions needed to implement the features described above.  Your program should be well modularized.  That is, your program should be factored into reasonably-sized (preferably small) functions that each perform some specific logical task.

You should separate your source code into interface and implementation by writing it using three source code files:
  * `econdata.hpp` - This should contain definitions for your structures and prototypes for your functions.
  * `econdata.cpp` - This should contain implementations of the functions prototyped in `econdata.hpp`.
  * `explore_econdata.cpp` - This should contain your `main()` function.

You should also write a `Makefile` that specifies the compilation of your program.

### Code style

You must include a header comment for each source code file that contains a description of the file (including how to run the program, command line arguments, etc. if the file contains your `main()` function), your name, and the date.  Your code should be well commented, including header comments for *all* functions describing what the function does, its parameters, and any pre- and post-conditions for the function.  You should appropriately use whitespace, newlines, and indentation.

Make sure you review the style guidelines for the course, and start trying to follow them:

http://web.engr.oregonstate.edu/~hessro/static/media/162_style_guideline.4812c1d9.pdf

## Submitting your program

To submit your program, you need to make sure the following files are committed into your git repository and pushed to your `master` branch on GitHub before the due date above:
  * `econdata.hpp`
  * `econdata.cpp`
  * `explore_econdata.cpp`
  * `Makefile`

Do not commit any other files (other than the ones that were already in your repository at the start of the assignment).  A good way to check whether your assignment is submitted is to simply look at your repo on github.com.  If your files appear there before the deadline, they they are submitted.

## Grading criteria

Your program **MUST** compile and run on `flip.engr.oregonstate.edu`, so make sure you have tested your work there before you make your final submission, since a program that compiles and runs in one environment may not compile and run in another.  **Assignments that do not compile on `flip` will receive a grade of 0.**  If you do not have an ENGR account, you can create one at https://teach.engr.oregonstate.edu/.

This assignment is worth 100 points total:
  * 20 points: signed up for Piazza
  * 70 points: your program meets the specifications above
      * 10 points: correctly allocates and frees state and county structs
      * 35 points: correctly reads file specified on command line into state and county structs
      * 10 points: program allows user to print state data (high/low median income, high/low unemployment, sorted by change in unemployment/median income)
      * 10 points: program allows user to print county data for a selected state (high/low median income, high/low unemployment, sorted by change in unemployment/median income)
      * 5 points: separated into header, implementation, & application with Makefile

  * 10 points: your code is appropriately commented and uses consistent and appropriate style
      * 4 points: function/file headers and other comments
      * 3 points: whitespace & indentation
      * 2 points: variable & function naming
      * 1 point: line length

Your work on this assignment will be graded by demoing it with a TA.  During your demo, you will compile and run your program for the TA and walk them through your source code, describing to them how it works.  It'll be your responsibility to sign up for a 10-minute demo slot with a TA by the demo due date above.  **You will receive a zero on the assignment if you don't demo it with a TA by the demo due date.**
