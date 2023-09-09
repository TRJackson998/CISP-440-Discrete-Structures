/*
Title: relations
Author: Terrence Jackson
Date: Sep 20 2021
Purpose: 440 HW 4: Relation Implementation
         matrix operations to determine the properties of a relation
Description: Source code from Prof Ross edited to fill in the required functions
         input from provided binary files
*/

/*
Reads in a matrix from a binary file and determines RST and EC's.

Dan Ross
   Original Sep 2014
   Fixes Sep 2020

The input files are binary files with the following format:
the first byte is the SIZE of the relation,
followed by a byte for each element of the matrix,
filling rows from left to right.

For example, a file with the following byte sequence:

   4, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1

represents the following matrix, R:

   1, 1, 0, 1,
   1, 1, 0, 1,
   0, 0, 1, 0,
   1, 1, 0, 1

Given a set A = {0, 1, 2, 3} we can label R as follows:

       0  1  2  3
   0   1, 1, 0, 1,
   1   1, 1, 0, 1,
   2   0, 0, 1, 0,
   3   1, 1, 0, 1

So, the above matrix represents the a relation R on set A where,

R = {(0,0) (0,1) (0,3) (1,0) (1,1) (1,3) (2,2) (2,1) (3,2) (3,3)}

Which is reflexive, symmetric and transitive.  This makes it an
equivalence relation with the following equivalence classes:

[0] = {0 1 3}
[2] = {2}

*/

#include <iostream>
#include <string.h>
#include <fstream>
#pragma warning( disable : 4267)
#define MAX 40

using namespace std;

// The Matrix.  No-one can be told what the Matrix is, you must be shown.
#define BLUEPILL
#define REDPILL
int R[MAX][MAX];   // a boolean array indicating members of a relation
int R2[MAX][MAX];   // a boolean array indicating members of a relation
int EC[MAX];       // a boolean array indicating representatives of equivalence classes
int SIZE;

/*
This prints a matrix
*/
void printMatrix(int R[MAX][MAX])
{
   int i, j;

   for (i = 0; i < SIZE; i++) {
      for (j = 0; j < SIZE; j++)
         cout << R[i][j] << " ";
      cout << endl;
   }
}

/*
Checks if a matrix is reflexive.

Checks the diagonals of a matrix.  All elements on the diagonal
must be 1.
*/
int IsRefx(int R[MAX][MAX])
{
   int reflexive = 1;
   for(int i = 0; i < SIZE; i++) {
      for(int j = 0; j < SIZE; j++) {
         if(!R[i][i]) {
            reflexive = 0; //not reflexive
         }
      }
   }
   return reflexive;  // returns a value if reflexive, 0 if not
}

/*
Checks is a matrix is symmetric
i,j == j,i for every cell of the matrix
*/
int IsSymt(int R[MAX][MAX])
{
   int symt = 1;
   for(int i = 0; i < SIZE; i++) {
      for(int j = 0; j < SIZE; j++) {
         if(R[i][j] != R[j][i]) {
            symt = 0; //not symmetric
         }
      }
   }
   return symt;  // returns a value if symmetric, 0 if not
}


/*
Squares a matrix

R2 = R x R
R2 i,j = sum of R i,k * R k,j
*/
void SquareMatrix(int R[MAX][MAX], int R2[MAX][MAX])
{
   int sum = 0;
   for(int i = 0; i < SIZE; i++) {
      for(int j = 0; j < SIZE; j++) {
         for(int k = 0; k < SIZE; k++) {
            sum += (R[i][k] * R[k][j]);
         }
         R2[i][j] = sum;
         sum = 0;
      }
   }
}


/*
Checks if a matrix is transitive.

First calculate the matrix squared
Every non-zero element of Rsquared must also be non zero in R
*/
int IsTrans(int R[MAX][MAX], int R2[MAX][MAX])
{
   //no need to multiply again bc R2 has already been filled from a call in main()
   int trans = 0;
   for(int i = 0; i < SIZE; i++) {
      for(int j = 0; j < SIZE; j++) {
         if(R[i][j]) { //if there's an element in R
            if(R2[i][j]) { //there's also an element in R2
               trans = 1;
            }
            else { //there's not an element in R2
               trans = 0;
            }
         }
      }
   }
   return trans;  // returns a value if transitive, 0 if not
}

/*
Finds equivalence classes and elects representatives of each class.

EC will contain a list of class representatives aka "captians" represented
as an array of boolean flags.

Each row in a matrix represents an equivalence class.  But there may be
several duplicate rows.  Our task is to find the captian of each class,
which will be the first member of the class.

ALGORITHM:
0 is always a captian

For elements 1 thru N, assign each as a captian
then, go to that elements row and look backwards for members less than this.
If found, demote the captian.
This works because captains are the first members of their class.  Think about it.
*/
void FindECs(int R[MAX][MAX], int EC[MAX])
{
   EC[0] = 1;
   for(int i = 1; i < SIZE; i++) {
      EC[i] = 1;
      for(int j = i-1; j > -1; j--) {
         if(R[j][i] != 0) {
            EC[i] = 0;
         }
      }
   }
}


/*
Iterate thru the captains array.  For each captian, go to that row of
the matrix and print the members of the class.
*/
void printECs(int R[MAX][MAX], int EC[MAX])
{
   for(int i = 0; i < SIZE; i++) {
      if(EC[i]) {
         cout << "{";
         for(int j = 0; j < SIZE; j++ ) {
            if(j < SIZE - 1) {
               cout << R[i][j] << ", ";
            }
            else {
               cout << R[i][j];
            }
         }
         cout << "}\n";
      }
   }
}

void prepMatrix(string fileName) {
   char c;
   // open source file
   // By default Visual Studio 2019 looks in same folder as cpp file
   ifstream fin(fileName, ios_base::binary);
   if (!fin) { cerr << "Input file could not be opened\n"; exit(1); }

   // get the matrix SIZE
   fin.read(&c, 1); SIZE = c;

   // fill the matrix from the file
   int i, j;
   for (i = 0; i < SIZE; i++)
      for (j = 0; j < SIZE; j++)
      {
         fin.read(&c, 1);
         R[i][j] = c;
      }

   // close file
   fin.close();

}


int main()
{
   string a ="./Relations/R";
   for(int i = 1; i <= 7; i++) { //loop through input files
      a += (i+48);
      a += ".bin";
      prepMatrix(a);
      cout << "Relation #" << i << endl;
      printMatrix(R);
      a ="./Relations/R";

      int reflx = 0;
      int symt = 0;
      int trans = 0;

      if(IsRefx(R)) {
         cout << "Reflexive\n";
         reflx = 1;
      }
      if(IsSymt(R)) {
         cout << "Symmetric\n";
         symt = 1;
      }

      cout << "The Product is:\n";
      SquareMatrix(R, R2);
      printMatrix(R2);

      if(IsTrans(R, R2)) {
         cout << "Transitive\n";
         trans = 1;
      }

      if(trans && symt && reflx) {
         cout << "This is an equivalence relation\n";
         cout << "The equivalence classes are:\n";
         FindECs(R, EC);
         printECs(R, EC);
      }
      else {
         cout << "This is not an equivalence relation\n";
      }

      cout << "\n";
   }
   return 0;
}