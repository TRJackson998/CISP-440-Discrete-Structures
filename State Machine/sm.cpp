/*
Title: State Machine Generator
Author: Terrence Jackson
Date: Nov 17 2021
Mod Date: Nov 19 2021
Purpose: 440 HW 12: State Machine Generator
         Write a C program to generate a state table for any arbitrary input string, 
         then use that state table to find all locations of that string in a large string.
Description: given source code, fill in generate_StateTable() function. 
         strncmp_olap(char* str1, char* str2, int n) particularly useful
         main() tests several strings from monkeydata.txt input

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
char nextState[STATES][2]; /*= {
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
char instr[80] = "abab";   //"abbbbbbaaaaaba";

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
   cout << "s\ta\tb\n\n";

   int len = strlen(instr) + 1;
   for (int s = 0; s < len; s++) {
      cout << s << "\t";
      for (int ch = 0; ch < 2; ch++)
         cout << (int)nextState[s][ch] << "\t";
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
     cout << "state " << State << " ";
     for(char i[2] = "a"; i[0] < 'c'; i[0]++) {
         got[State] = i[0];
         got[State+1] = 0;
        for(int j = 0; (j < num_states) && (j <= len_got); j++) {
           if(!strncmp_olap(instr, got, j)) {
              nextState[State][i[0]-97] = j;
            }
        }
        cout << got << " goto " << (int)nextState[State][i[0]-97] << "  ";
     }
     cout << endl;
  }
}


int main()
{
   ///////////////////Searching for string "abab" in text file "abab.txt"

   char inFile[20] = "abab.txt\0";
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

   ///////////////////Searching for string "abbbbbbaaaaaba" in text file "monkeydata.txt"
   ///////////////////	reset inFile
   char dummyF[20] = "monkeydata.txt\0";
   strncpy(inFile, dummyF, sizeof(dummyF));

   ///////////////////	reset inString
   char dummyS[25] = "abbbbbbaaaaaba\0";
   strncpy(instr, dummyS, sizeof(dummyS));

   cout << "Searching for string \"" << instr << "\" in text file \"" << inFile << "\"...\n";

   /// //////////////  CREATE THE STATE TABLE
   generate_StateTable();  // you write this function please
   print_StateTable();

   ///////////////////	RUN THE STATE MACHINE

   in.open(inFile);
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

   ///////////////////Searching for string "abbbbaabbbbaaa" in text file "monkeydata.txt"

   ///////////////////	reset inString
   char dummyS2[25] = "abbbbaabbbbaaa\0";
   strncpy(instr, dummyS2, sizeof(dummyS));

   cout << "Searching for string \"" << instr << "\" in text file \"" << inFile << "\"...\n";

   /// //////////////  CREATE THE STATE TABLE
   generate_StateTable();  // you write this function please
   print_StateTable();

   ///////////////////	RUN THE STATE MACHINE

   in.open(inFile);
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

   ///////////////////Searching for string "aaaaaaabaaaaaaa" in text file "monkeydata.txt"

   ///////////////////	reset inString
   char dummyS3[25] = "aaaaaaabaaaaaaa\0";
   strncpy(instr, dummyS3, sizeof(dummyS));

   cout << "Searching for string \"" << instr << "\" in text file \"" << inFile << "\"...\n";

   /// //////////////  CREATE THE STATE TABLE
   generate_StateTable();  // you write this function please
   print_StateTable();

   ///////////////////	RUN THE STATE MACHINE

   in.open(inFile);
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

   ///////////////////Searching for string "aabbbababaaababbaaaa" in text file "monkeydata.txt"

   ///////////////////	reset inString
   char dummyS4[25] = "aabbbababaaababbaaaa\0";
   strncpy(instr, dummyS4, sizeof(dummyS));

   cout << "Searching for string \"" << instr << "\" in text file \"" << inFile << "\"...\n";

   /// //////////////  CREATE THE STATE TABLE
   generate_StateTable();  // you write this function please
   print_StateTable();

   ///////////////////	RUN THE STATE MACHINE

   in.open(inFile);
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

   ///////////////////Searching for string "aaaabababaaaaabbaaaa" in text file "monkeydata.txt"

   ///////////////////	reset inString
   char dummyS5[25] = "aaaabababaaaaabbaaaa\0";
   strncpy(instr, dummyS5, sizeof(dummyS));

   cout << "Searching for string \"" << instr << "\" in text file \"" << inFile << "\"...\n";

   /// //////////////  CREATE THE STATE TABLE
   generate_StateTable();  // you write this function please
   print_StateTable();

   ///////////////////	RUN THE STATE MACHINE

   in.open(inFile);
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