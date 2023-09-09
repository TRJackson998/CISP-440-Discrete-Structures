/*
Title: relational database
Author: Terrence Jackson
Date: Oct 1 2021
Purpose: 440 HW 5.42: Relational DB Implementation
         select, join, project, union
Description: Source code from Prof Ross edited to fill in the required functions
         input from provided csv files
*/
/*

Homemade relational database.

Implements Select, Project, Join and Union operations.

Dan Ross
   Original Mar 2013
   Updated Mar 2017
   Updated some more Sep 2020

*/

#include <iostream>
#include <iomanip>
#include <string.h>
#include <fstream>
#pragma warning( disable : 4267)
#pragma warning( disable : 4996)

using namespace std;

// ****  THE TABLES  ****
// Use 800kB global tables cuz RAM is cheap and labor is not!
// Seriously, a more efficient memory implementation would 
// use pointers and dynamic memory (using new and/or malloc)
// but that often requires tricky pointer arithmetic
char T1[100][100][80];
char T2[100][100][80];


/*
Prints out a table
*/
void printTable(char T[100][100][80])
{
   int i = 0, j = 0;
   while (T[i][j][0]) {		// look for null char at T[i][j][0]
      while (T[i][j][0]) {
         cout << left << setw(20) << T[i][j];// string at T[i][j]
         j++;
      }
      cout << endl;
      i++; j = 0;
   }
   cout << endl;
}


/*
Erases a table
*/
void eraseTable(char T[100][100][80])
{
   // erase the destination array
   for (int i = 0; i < 100; i++)		  // rows
      for (int j = 0; j < 100; j++)	  // cols
         for (int k = 0; k < 80; k++)   // chars
            T[i][j][k] = 0;

}

/*
Erases an array
*/
void eraseArray(int A[], int size)
{
   for (int i = 0; i < size; i++)
      A[i] = 0;
}

/*
Reads a file into a table
*/
void filltable(const char filename[80], char Table[100][100][80])
{
   // open source file
   ifstream fin(filename);
   if (!fin) { cerr << "Input file could not be opened\n"; exit(1); }

   char line[80];
   char buf[80];

   // table rows and cols
   int row = 0; int col = 0;

   // Copy file into table
   fin.getline(line, 80);
   while (line[0]) {
      col = 0;   // reset col

      // parse this line
      int i = 0; int j = 0;
      int len = strlen(line);
      for (i = 0; i <= len; i++) {
         if ((line[i] == ',') || (line[i] == 0)) { // delimiters
            strncpy(buf, line + j, i - j);
            buf[i - j] = 0;  // null terminator
            j = i + 1;		 // scoot up j	

            // copy buffer to table array
            strcpy(Table[row][col], buf);
            col++;			
         }
      }

      // get another line
      fin.getline(line, 80);
      row++;
   }
}


/*
Performs a select operation on a table.

Receives a table.
Returns a table consisting of only the rows which have
the specified 'value' in the specified 'col'
*/
void select(char Tout[100][100][80], char Tin[100][100][80], int col, const char* value)
{
   // erase the destination array
   eraseTable(Tout);
   int i1 = 0, i2 = 0, j = 0;
   while(Tin[i1][0][0]) { //there is something in this row
       if(strcmp(value, Tin[i1][col]) == 0) { //value we're looking for is in this row @ the right col
           while(Tin[i1][j][0]) { //loop through the row
               strcpy(Tout[i2][j], Tin[i1][j]);
               j++;
           }
           i2++;
       }
       i1++;
       j = 0;
   }
}


/*
Performs a project operation on a table.

Receives a table.
Returns a table consisting of the specified cols.
The 'cols' parameter is a set of boolean flags where
true means we want this row in the resulting table
*/
void project(char Tout[100][100][80], char Tin[100][100][80], int cols[100])
{
   // erase the destination array
   eraseTable(Tout);
   int i = 0, j1 = 0, j2 = 0;
   while(Tin[i][0][0]) { //there is something in this row
       while(Tin[i][j1][0]) { //loop thru cols
           if(cols[j1]) { //cols is filled w/1's and 0's, if this col is selected
               strcpy(Tout[i][j2], Tin[i][j1]); //copy it over
               j2++; //scroll thru the column
           }
           j1++; //scroll through the cols from the input
       }
       i++;
       j1 = j2 = 0;
   }
}


/*
Performs a join operation on a table.

Receives 2 tables and joins them.
Returns only the rows where the value in table1's T1col
matches the value in table2's T2col
*/
void join(char Tout[100][100][80], char T1[100][100][80], char T2[100][100][80], int T1col, int T2col)
{
   // erase the destination array
   eraseTable(Tout);

   int i1, i2, io, j1, j2, jo;
   i1 = i2 = io = j1 = j2 = jo = 0;
   while(T1[i1][0][0]) {
       while(T2[i2][0][0]) {
           if(strcmp(T1[i1][T1col], T2[i2][T2col]) == 0) {
               while(T1[i1][j1][0]) { 
                  //scroll across T1 and copy to output
                   strcpy(Tout[io][jo], T1[i1][j1]);
                   j1++;
                   jo++;
               }
               while(T2[i2][j2][0]) { 
                  //scroll across T2 and copy to output
                   if(j2 != T2col) {
                       //not a duplicate
                       strcpy(Tout[io][jo], T2[i2][j2]);
                       jo++;
                       j2++;
                   }
                   else { //already added, move on
                       j2++;
                   }
               }
               io++; //ready to insert next row to output
           }
           j1 = j2 = jo = 0;
           i2++;
       }
       i1++;
       i2 = 0;
   }
}

/*
Makes a 3rd table containing all the stuff in 2 other tables.
The 2 input tables must have the same schema.
*/
void Union(char Tout[100][100][80], char T1[100][100][80], char T2[100][100][80])
{
   // erase the destination array
   eraseTable(Tout);

   int i1, i2, io, j;
   i1 = i2 = io = j = 0;
   while(T1[i1][j][0]) {
      while(T1[i1][j][0]) {
         strcpy(Tout[io][j], T1[i1][j]);
         j++;
      }
      j = 0;
      i1++;
      io++;
   }
   while(T2[i2][j][0]) {
      while(T2[i2][j][0]) {
         strcpy(Tout[io][j], T2[i2][j]);
         j++;
      }
      j = 0;
      i2++;
      io++;
   }
}


/*
Reads in some tables and does operations on them
*/
int main(void)
{
   filltable("./RDBtables/Professors.txt", T1);
   filltable("./RDBtables/Students.txt", T2);

   cout << "Original Professors table:\n";
   printTable(T1);

   cout << "Original Students table:\n";
   printTable(T2);

   // select
   // locals are created in the stack (except statics)
   // so use a static to prevent stack overflow
   static char Temp1[100][100][80];
   select(Temp1, T2, 1, "555 Riley");
   cout << "\nSELECT TEST: Temp1 = Students[Address = 555 Riley]\n";
   printTable(Temp1);

   // project
   cout << "\nPROJECT TEST: Temp2 = Students[Name, Phone]\n";
   static char Temp2[100][100][80];
   // dynamic arrays are also easy on the stack
   // cuz they point to the heap
   int* cols = new int[100];  eraseArray(cols, 100);
   cols[0] = cols[2] = 1;
   project(Temp2, T2, cols);
   printTable(Temp2);

   // join
   cout << "\nJOIN TEST: Temp3 = Students[address = address]Professors\n";
   static char Temp3[100][100][80];
   join(Temp3, T1, T2, 1, 1);
   printTable(Temp3);

   // union
   cout << "\nUNION TEST: Temp4 = Students UNION MoreStudents\n";
   static char T3[100][100][80];
   filltable("./RDBtables/MoreStudents.txt", T3);
   static char Temp4[100][100][80];
   Union(Temp4, T2, T3);
   printTable(Temp4);

   eraseTable(Temp1);
   eraseTable(Temp2);
   eraseTable(Temp3);
   eraseTable(Temp4);
   eraseTable(T1);

   static char buyer[100][100][80];
   filltable("./RDBtables/buyer.txt", buyer);
   static char department[100][100][80];
   filltable("./RDBtables/department.txt", department);
   static char employee[100][100][80];
   filltable("./RDBtables/employee.txt", employee);
   static char supplier[100][100][80];
   filltable("./RDBtables/supplier.txt", supplier);

   //5a
   cout << "\nNames of all employees who work in departments that supply parts to JCN:\n";
   select(Temp1, buyer, 0, "JCN Electronics");
   eraseArray(cols, 100);
   cols[1] = 1;
   project(Temp2, Temp1, cols);
   join(Temp3, Temp2, supplier, 0, 1);
   project(Temp4, Temp3, cols);
   
   eraseTable(Temp1);
   eraseTable(Temp2);
   eraseTable(Temp3);

   join(Temp1, Temp4, department, 0, 0);
   project(Temp2, Temp1, cols);
   join(Temp3, Temp2, employee, 0, 2);

   eraseArray(cols, 100);
   cols[2] = 1;
   project(T1, Temp3, cols);
   printTable(T1);

   eraseTable(Temp1);
   eraseTable(Temp2);
   eraseTable(Temp3);
   eraseTable(Temp4);
   eraseTable(T1);

   //5b
   cout << "\nAll buyers who buy parts from departments managed by Jones:\n";
   select(Temp1, department, 1, "Jones");
   join(Temp2, Temp1, supplier, 0, 0);
   eraseArray(cols, 100);
   cols[2] = 1;
   project(Temp3, Temp2, cols);
   join(Temp4, Temp3, buyer, 0, 1);
   eraseArray(cols, 100);
   cols[1] = 1;
   project(T1, Temp4, cols);
   printTable(T1);

   eraseTable(Temp1);
   eraseTable(Temp2);
   eraseTable(Temp3);
   eraseTable(Temp4);
   eraseTable(T1);

   //5c
   cout << "\nAll buyers who buy parts produced in the dept where Suzuki works:\n";
   select(Temp1, employee, 1, "Suzuki");
   join(Temp2, Temp1, department, 2, 1);
   eraseArray(cols, 100);
   cols[3] = 1;
   project(Temp3, Temp2, cols);
   join(Temp4, Temp3, supplier, 0, 0);

   eraseTable(Temp1);
   eraseTable(Temp2);
   eraseTable(Temp3);

   eraseArray(cols, 100);
   cols[1] = 1;
   project(Temp1, Temp4, cols);
   join(Temp2, Temp1, buyer, 0, 1);
   eraseArray(cols, 100);
   cols[1] = 1;
   project(T1, Temp2, cols);
   printTable(T1);

   eraseTable(T1);
   eraseTable(Temp1);

   //5d
   cout << "\nUnion department table with more_departments table\n";
   filltable("./RDBtables/departmentsPlus.txt", T1);
   Union(Temp1, department, T1);
   printTable(Temp1);

   return 0;
}