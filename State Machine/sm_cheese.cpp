/*
Title: State Machine, String Cheese Generator
Author: Terrence Jackson
Date: Nov 19 2021
Mod Date: Nov 19 2021
Purpose: 440 HW 12: State Machine Generator EXTRA CREDIT
         Write a C program to generate a state table for any arbitrary input string, 
         then use that state table to find all locations of that string in a large string.
         Modify the program to find all occurrences of “cheese” in “extra.txt”
Description: copied sm.cpp, modified nextState array, print_StateTable(), generate_StateTable()
         to handle a-z, then used main() to search for string cheese

*/

/*
   Generates a state machine to find all occurances of an arbitrary string

   Dan Ross
   Original April 2013
   Improved Nov 2020

*/
#include <iostream>
#include <fstream>
#include <string.h>
#pragma warning( disable : 4996) 
#pragma warning( disable : 4244) 
using namespace std;

#define STATES 20
char nextState[STATES][26]; /*= {
//     a  b   //inputs
      1, 0,  //state 0
      1, 2,  //state 1
      3, 0,  //state 2
      1, 4,  //state 3
      3, 0,  //state 4
};
*/

char state = 0;
int Count = 0;

// hard code input string
char instr[80] = "cheese";

void process(char ch)
{
   // print current char
   //cout << ch;

   // move to next state
   state = nextState[state][ch - 'a'];

   // output count if we are back to initial state
   if (state == strlen(instr)) cout << Count + 1 << ", ";

   Count++;  //another ch bites the dust!
}


void print_StateTable()
{
   cout << "\nComplete state table for " << instr << "...\n";
   cout << "s  a  b  c  d  e  f  g  h  i  j  k  l  m  n  o  p  q  r  s  t  u  v  w  x  y  z\n\n";

   int len = strlen(instr) + 1;
   for (int s = 0; s < len; s++) {
      cout << s << "  ";
      for (int ch = 0; ch < 26; ch++)
         cout << (int)nextState[s][ch] << "  ";
      cout << endl;
   }
}


/*
Compares first n characters of str1,
with last n characters of str2.

Returns 0 (false) if they match

Copyright (c) 1981 Dan Ross
*/
int strncmp_olap(char* str1, char* str2, int n)
{
   int i, j;
   int l1 = strlen(str1);
   int l2 = strlen(str2);

   for (i = 0, j = l2 - n; i < n; i++, j++)
      if (str1[i] != str2[j])
         return true;

   return false;
}

/*
Generate a state table for a sequence detector
*/
void generate_StateTable(void)
{
   cout << "Generating state table for " << instr << "..." << endl;
   // you write this

   /* you may need these things...
   char got[80];
   got[0] = 0;         // the null terminator
   strncpy(...);       // handy, but it doesn't do this: got[zeroSpot] = 0;
   strcat(...);	    // for sticking on a's and b's
   strlen(...);		// is useful too
   strncmp_olap(..)	// an amazing function!  try it!

   some tricky s**t...
   char ch[2] = "a";
   ch[0]++; // increments the above string to the next character
   */

   char got[80];
   got[0] = 0;         // the null terminator

  int len_got = -1;
  int num_states = strlen(instr) + 1;
  for(int State = 0; State < num_states; State++) {
     got[State-1] = instr[State-1];
     got[State] = 0;
     len_got = State+1;
     cout << "state " << State << endl;
     for(char i[2] = "a"; i[0] < 'z'; i[0]++) {
         got[State] = i[0];
         got[State+1] = 0;
        for(int j = 0; (j < num_states) && (j <= len_got); j++) {
           if(!strncmp_olap(instr, got, j)) {
              nextState[State][i[0]-97] = j;
            }
        }
        cout << got << " goto " << (int)nextState[State][i[0]-97] << endl;
     }
     cout << "\n\n";
  }
}


int main()
{
   ///////////////////Searching for string "cheese" in text file "abab.txt"

   char inFile[20] = "extra.txt\0";
   cout << "Searching for string \"" << instr << "\" in text file \"" << inFile << "\"...\n";

   /// //////////////  CREATE THE STATE TABLE
   generate_StateTable();  // you write this function please
   print_StateTable();

   ///////////////////	RUN THE STATE MACHINE

   char ch;
   ifstream in(inFile);
   if (!in) {
      cout << "Error opening file";
      return 1;
   }

   cout << "\nThe string " << instr << " was found at locations: ";

   // read and process characters
   while (in) {
      in.get(ch);
      if (in)
         process(ch);
   }
   cout << "\n\n";
   in.close();

   
   return 0;
}