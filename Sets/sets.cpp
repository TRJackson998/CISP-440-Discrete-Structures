/*
Title: sets
Author: Terrence Jackson
Date: Sep 8 2021
Mod Date: Sep 10 2021
Purpose: 440 HW 2.42: Set Implementation
         set operations in binary
Description: Source code from Prof Ross edited to fill in the required functions
*/

/*
Set operations

Dan Ross
   Original Feb 2013, 32bit extension Sep 2016
   Minor annotations, Sep 2020

Performs set operations.

Universe = {Bat, Cat, Chimp, Dog, Fish, Liger, Snake, Turtle}
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#pragma warning( disable : 4996) 
#pragma warning( disable : 4244) 

// Start with a small universe
char Universe[8][10] = { "Bat", "Cat", "Chimp", "Dog", "Fish", "Liger", "Snake", "Turtle" };
typedef unsigned char set;          // a set, by any other name, would smell as sweet.

// Then use this big universe
char BigUniverse[32][20] = {
    "Bat", "Cat", "Chimp", "Dog", "Fish", "Liger", "Snake", "Turtle",
    "Bear", "Dragon", "Horse", "Wolf", "Rat", "Gerbil", "Rabbit", "Monkey",
    "Donkey", "Llama", "Zebra", "Hippopotamus", "Rhinoceros", "Gecko", "Frog", "Sloth",
    "Deer", "Kangaroo", "Gorilla", "Alligator", "Panda", "Squirrel", "Duck", "Platypus" };
typedef unsigned long int set32;    // a set, but bigger


/*
Prints out a set in set-sequence notation
*/
void printSet(set A)
{
   printf("{ ");

   bool commaflag = false;
   int i = 0;
   unsigned char mask = 0x80;
   for (; mask; mask >>= 1, i++) {
      if (mask & A)
      {
         if (commaflag) printf(", ");
         printf(Universe[i]);
         commaflag = true;
      }
   }
   printf(" }");
}

/*
Prints out a set32 in set-sequence notation
*/
void printSet(set32 A)
{
   printf("{ ");

   bool commaflag = false;
   int i = 0;
   unsigned long mask = 0x80000000;
   for (; mask; mask >>= 1, i++) {
      if (mask & A)
      {
         if (commaflag) printf(", ");
         printf(BigUniverse[i]);
         commaflag = true;
      }
   }
   printf(" }");
}

/*
Prints each bit of a byte
*/
void print8bits(unsigned char x)
{
   for (unsigned char mask = 0x80; mask; mask >>= 1) {
      if (mask & x)
         printf("1");
      else
         printf("0");
   }
}

/*
Prints each bit of a 32 bit int
*/
void print32bits(unsigned long x)
{
	for(unsigned long mask = 0x80000000; mask; mask >>= 1) {
		if(mask & x) 
			printf("1");
		else 
			printf("0");
	}
}


/*
Inserts an element of the universe into a set

Hashing calculation example: insert(A, "Chimp");
Asci C = 67, i = 105.   67 + 105 % 20 = 12, element 12 in the lookup table is 2.
So the index of Chimp = 2 in the Universe table, so we insert a bit into the set at bit position 2.
*/
void insert(set& A, const char str[])
{
   // First we need to get the Universe index of this string
   // Use hashing instead of searching - cuz it is faster 
   // than searching, especially for a big universe.

   // You will have to modify this for the 32bit universe
   // If you do not know how to hash && do not want to learn now
   // then modify this to use a loop search lookup with strcmp

   // get a unique hash for each string
   int hash = (str[0] + str[2]) % 20;

   // map unique string hashes back to their Universe indexes
   //            0   1  2  3   4  5   6   7   8   9
   int g[20] = { 6, -1, 0, 1, -1, 4, -1, -1, -1, -1,    // 0     
                -1, 3, 2, -1, -1, -1, -1, -1, 7, 5 };   // 1

   int index = g[hash];

   // make a mask
   set mask = 0x80 >> index;

   // insert this element
   A = A | mask;
}

void insert32(set32& A, const char str[])
{
   // First we need to get the Universe index of this string
   // You will have to modify this for the 32bit universe
   // If you do not know how to hash && do not want to learn now
   // then modify this to use a loop search lookup with strcmp
   bool found = false;
   int index = -1;
   do {
      index++;
      char test = BigUniverse[index][0];
      int ret = strcmp(str, BigUniverse[index]);
      if(ret == 0) {
         found = true;
      }
   }while(!found);

   /*
   // get a unique hash for each string
   int hash = (str[0] + str[2]) % 20;

   // map unique string hashes back to their Universe indexes
   //            0   1  2  3   4  5   6   7   8   9
   
   int g[20] = { 6, -1, 0, 1, -1, 4, -1, -1, -1, -1,    // 0     
                -1, 3, 2, -1, -1, -1, -1, -1, 7, 5 };   // 1

   int index = g[hash];
   */
   // make a mask
   set32 mask = 0x80000000 >> index;

   // insert this element
   A = A | mask;
}


/*
Calculates base raised to the power exp
*/
int my_pow(int base, int  exp)
{
   int x = 1;
   for (int i = 0; i < exp; i++)
      x *= base;

   return x;
}


////////////// YOU WRITE THE FOLLOWING /////////////////


/*
Union
*/
set Union(set A, set B)
{
   return A | B;
}
set32 Union(set32 A, set32 B)
{
   return A | B;
}

/*
Intersection
*/
set Intersection(set A, set B)
{
   return A & B;
}
set32 Intersection(set32 A, set32 B)
{
   return A & B;
}

/*
Complement
bitwise complement operator ~
*/
set Complement(set A)
{
   return ~A;
}
set32 Complement(set32 A)
{
   return ~A;
}

/*
Difference
*/
set Difference(set A, set B)
{
   set I = Intersection(A, B);
   A ^= I; //A = A bitwise xor I
   return A;
}
set32 Difference(set32 A, set32 B)
{
   set32 I = Intersection(A, B);
   A ^= I; //A = A xor I
   return A;
}

/*
Cardinality
*/
int Cardinality(set A)
{
   int count = 0;
   for (unsigned char mask = 0x80; mask; mask >>= 1) {
      if (mask & A)
         count++;
   }
   return count;
}
int Cardinality(set32 A)
{
   int count = 0;
   for (unsigned long mask = 0x80000000; mask; mask >>= 1) {
      if (mask & A)
         count++;
   }
   return count;
}


/*
PowerSet  Algorithm:

OriginalSet -> Compress
               permutationN -> decompress -> print

Example:   0010 0001 -> 11

                     -> 00 -> 0000 0000
                     -> 01 -> 0000 0001
                     -> 10 -> 0010 0000
                     -> 11 -> 0010 0001

Basically, just generate all the 'compressed' subsets B by counting from 0 to (2^|A|) - 1
Then decompress each B by inserting each bit (0 or 1) of the 'compressed' set into the next 'available' uncompressed slot in subA
*/
void printPowerSet(set A)
{
   int CardOfP = my_pow(2, Cardinality(A));
   set setB = 0;	// a lil 'compressed' set
   set subA = 0;   // some subset of A

   // generate each "compressed" subset
   for(; setB < CardOfP; setB++)
   //(loop: count on setB from 0 to (2 ^ | A | ) - 1)
   {
       // uncompress setB into subA
       unsigned char Amask = 0x01;  // start at decimal value 1
       unsigned char Bmask = 0x01;  // start at binary value 0000 0001,  (You could also start at octal value 1)

       // use Amask to check for "slots" in original setA
       for(int i = 1; i <= 8; Amask<<=1, i++)
       //(loop: try each bit position from decimal values 1 to 128)  // Hint:  bit shifting might work here
       {
           // use Amask to look for available slots in setA
           if(A & Amask)// (this slot is available in original setA)  // Hint:  bitwise AND might work here
           {
               // use Bmask to check if corresponding element exists in setB
               if(setB & Bmask)// (this element in set B)
               {
                   // use Amask to insert this element into subA.
                   subA |= Amask;
               }
               // shift Bmask left.
               Bmask <<= 1;
           }
           // shift Amask left. Could do that here, or in the loop conditional, depending on how you write the loop
       }

       printSet(subA); // print subA
       printf("\n");
       subA = 0; // reset subA to ZERO
       // next setB
   }
}

void printPowerSet(set32 A)
{
   int CardOfP = my_pow(2, Cardinality(A));
   set32 setB = 0;	// a lil 'compressed' set
   set32 subA = 0;   // some subset of A

   // generate each "compressed" subset
   for(; setB < CardOfP; setB++)
   //(loop: count on setB from 0 to (2 ^ | A | ) - 1)
   {
       // uncompress setB into subA
       unsigned long Amask = 0x00000001;  // start at decimal value 1
       unsigned long Bmask = 0x00000001;  // start at binary value 0000 0001,  (You could also start at octal value 1)

       // use Amask to check for "slots" in original setA
       for(int i = 1; i <= 32; Amask<<=1, i++)
       //(loop: try each bit position from decimal values 1 to 128)  // Hint:  bit shifting might work here
       {

           // use Amask to look for available slots in setA
           if(A & Amask)// (this slot is available in original setA)  // Hint:  bitwise AND might work here
           {
              /*
               iteration 1 of this conditional would look like this if you worked it out with a pencil
               setA      0010 0001
               & Amask   0000 0001
               ___________________
               TRUE
               */

               // use Bmask to check if corresponding element exists in setB
               if(setB & Bmask)// (this element in set B)
               {
                  /*
                   iteration 1 of this conditional would look like this if you worked it out with a pencil
                   setB	   	00
                   & Bmask		01
                   ______________
                   FALSE
                   */

                   // use Amask to insert this element into subA.
                   subA |= Amask;
                   /*

                   iteration 1 of this insert operation would look like this if you worked it out with a pencil
                   subA      0000 0000
                   | Amask   0000 0001
                   ___________________
                   subA	     0000 0001
                   */
               }
               // shift Bmask left.
               Bmask <<= 1;
           }
           // shift Amask left. Could do that here, or in the loop conditional, depending on how you write the loop
       }

       printSet(subA); // print subA
       printf("\n");
       subA = 0; // reset subA to ZERO
       // next setB
   }
}


bool IsSubset(set ASubset, set ASet)
{
   set I = Intersection(ASubset, ASet);
   return I == ASubset;
}
bool IsSubset(set32 ASubset, set32 ASet)
{
   set32 I = Intersection(ASubset, ASet);
   return I == ASubset;
}


bool IsProperSubset(set ASubset, set ASet)
{
   set I = Intersection(ASubset, ASet);
   return (I == ASubset) && (ASubset != ASet);
}
bool IsProperSubset(set32 ASubset, set32 ASet)
{
   set32 I = Intersection(ASubset, ASet);
   return (I == ASubset) && (ASubset != ASet);
}

/*********************************************************
main - modify as necessary with various test data
*********************************************************/
int main()
{
   set A = 0;
   insert(A, "Bat");
   insert(A, "Cat");
   insert(A, "Chimp");
   insert(A, "Snake");

   printf("Set A: ");
   printSet(A);
   printf("\nCardinality: ");
   printf("%d", Cardinality(A));

   printf("\n\nPowerSet(A):\n");
   printPowerSet(A);

   set B = 0;
   insert(B, "Chimp");
   insert(B, "Fish");
   insert(B, "Liger");

   printf("\nSet B: ");
   printSet(B);

   set C = 0;
   insert(C, "Chimp");
   insert(C, "Fish");
   insert(C, "Liger");
   printf("\nSet C: ");
   printSet(C);

   printf("\n(A Union B) Inter ~C: ");
   set D = Intersection(Union(A, B), ~C);
   printSet(D);

   if (IsSubset(B, C))
      printf("\nB is a subset of C");
   else
      printf("\nB is NOT a subset of C");

   if (IsProperSubset(B, C))
      printf("\nB is a proper subset of C");
   else
      printf("\nB is NOT a proper subset of C");

   printf("\n");

   ////////////////////////////////////////HW PROBLEM 3: TESTING////////////////////////////////////////
   printf("\nProblem 3:");
   A = B = C = D = 0;
   insert(A, "Cat");
   insert(A, "Dog");
   insert(A, "Fish");

   insert(B, "Cat");
   insert(B, "Dog");
   insert(B, "Liger");

   insert(C, "Dog");
   insert(C, "Liger");
   insert(C, "Snake");
   insert(C, "Turtle");

   printf("\nSet A: ");
   printSet(A);
   printf("\nSet B: ");
   printSet(B);
   printf("\nSet C: ");
   printSet(C);

   printf("\na. (A Union B) Inter ~C: ");
   D = Intersection(Union(A, B), ~C);
   printSet(D);

   printf("\nb. A Union (B Inter C): ");
   D = Union(Intersection(B, C), A);
   printSet(D);

   printf("\nc. Complement (A Inter B): ");
   D = Complement(Intersection(A, B));
   printSet(D);

   printf("\nd. (~A Union ~B): ");
   D = Union(Complement(A), Complement(B));
   printSet(D);

   printf("\ne. A - B: ");
   D = Difference(A, B);
   printSet(D);

   printf("\n\nf. PowerSet(A):\n");
   printPowerSet(A);

   if (IsSubset(A, B))
      printf("\ng. A is a subset of B");
   else
      printf("\ng. A is NOT a subset of B");

   if (IsProperSubset(A, B))
      printf("\nh. A is a proper subset of B");
   else
      printf("\nh. A is NOT a proper subset of B");  

   printf("\ni. 2 more expressions");
   printf("\n~A - B: ");
   D = (Difference(Complement(A), B));
   printSet(D);

   printf("\n(B - ~C) Union ((B - ~A) Inter (C Union B)): ");
   D = Union((Difference(B, Complement(C))), Intersection(Difference(B, Complement(A)), Union(C, B)));
   printSet(D);
   printf("\n\n");

////////////////////////////////////////HW PROBLEM 4: TESTING////////////////////////////////////////
   printf("\nProblem 4:");
   A = B = C = D = 0;
   //"Bat", "Cat", "Chimp", "Dog", "Fish", "Liger", "Snake", "Turtle"
   insert(A, "Bat");
   insert(A, "Chimp");
   insert(A, "Liger");
   insert(A, "Snake");
   insert(A, "Turtle");

   insert(B, "Bat");
   insert(B, "Cat");
   insert(B, "Chimp");
   insert(B, "Dog");
   insert(B, "Fish");

   insert(C, "Dog");
   insert(C, "Fish");
   insert(C, "Liger");
   insert(C, "Snake");
   insert(C, "Turtle");

   printf("\nSet A: ");
   printSet(A);
   printf("\nSet B: ");
   printSet(B);
   printf("\nSet C: ");
   printSet(C);

   printf("\na. (A Union B) Inter ~C: ");
   D = Intersection(Union(A, B), ~C);
   printSet(D);

   printf("\nb. A Union (B Inter C): ");
   D = Union(Intersection(B, C), A);
   printSet(D);

   printf("\nc. Complement (A Inter B): ");
   D = Complement(Intersection(A, B));
   printSet(D);

   printf("\nd. (~A Union ~B): ");
   D = Union(Complement(A), Complement(B));
   printSet(D);

   printf("\ne. A - B: ");
   D = Difference(A, B);
   printSet(D);

   printf("\n\nf. PowerSet(A):\n");
   printPowerSet(A);

   if (IsSubset(A, B))
      printf("\ng. A is a subset of B");
   else
      printf("\ng. A is NOT a subset of B");

   if (IsProperSubset(A, B))
      printf("\nh. A is a proper subset of B");
   else
      printf("\nh. A is NOT a proper subset of B");  

   printf("\ni. 2 more expressions");
   printf("\n~A - B: ");
   D = (Difference(Complement(A), B));
   printSet(D);

   printf("\n(B - ~C) Union ((B - ~A) Inter (C Union B)): ");
   D = Union((Difference(B, Complement(C))), Intersection(Difference(B, Complement(A)), Union(C, B)));
   printSet(D);
   printf("\n\n");

////////////////////////////////////////HW PROBLEM 5: TESTING////////////////////////////////////////
   printf("\nProblem 5:");
   set32 a, b, c, d;
   a = b = c = d = 0;
   // A = { Bat, Dragon, Hippopotamus , Gecko, Sloth, Deer, Kangaroo}
   //B = { Hippopotamus , Gecko, Sloth, Bat, Rhinoceros, Squirrel, Platypus}
   //C = { Gecko, Sloth, Bat, Rhinoceros, Dog, Fish, Horse, Snake, Turtle, Donkey, Gorilla, Llama}


   insert32(a, "Bat");
   insert32(a, "Dragon");
   insert32(a, "Hippopotamus");
   insert32(a, "Gecko");
   insert32(a, "Sloth");
   insert32(a, "Deer");
   insert32(a, "Kangaroo");

   insert32(b, "Hippopotamus");
   insert32(b, "Gecko");
   insert32(b, "Sloth");
   insert32(b, "Bat");
   insert32(b, "Rhinoceros");
   insert32(b, "Squirrel");
   insert32(b, "Platypus");

   insert32(c, "Gecko");
   insert32(c, "Sloth");
   insert32(c, "Bat");
   insert32(c, "Rhinoceros");
   insert32(c, "Dog");
   insert32(c, "Fish");
   insert32(c, "Horse");
   insert32(c, "Snake");
   insert32(c, "Turtle");
   insert32(c, "Donkey");
   insert32(c, "Gorilla");
   insert32(c, "Llama");

   printf("\nSet A: ");
   printSet(a);
   printf("\nSet B: ");
   printSet(b);
   printf("\nSet C: ");
   printSet(c);

   printf("\na. (A Union B) Inter ~C: ");
   d = Intersection(Union(a, b), ~c);
   printSet(d);

   printf("\nb. A Union (B Inter C): ");
   d = Union(Intersection(b, c), a);
   printSet(d);

   printf("\nc. Complement (A Inter B): ");
   d = Complement(Intersection(a, b));
   printSet(d);

   printf("\nd. (~A Union ~B): ");
   d = Union(Complement(a), Complement(b));
   printSet(d);

   printf("\ne. A - B: ");
   d = Difference(a, b);
   printSet(d);

   printf("\n\nf. PowerSet(A):\n");
   printPowerSet(a);

   if (IsSubset(a, b))
      printf("\ng. A is a subset of B");
   else
      printf("\ng. A is NOT a subset of B");

   if (IsProperSubset(a, b))
      printf("\nh. A is a proper subset of B");
   else
      printf("\nh. A is NOT a proper subset of B");  

   printf("\ni. 2 more expressions");
   printf("\n~A - B: ");
   d = (Difference(Complement(a), b));
   printSet(d);

   printf("\n(B - ~C) Union ((B - ~A) Inter (C Union B)): ");
   d = Union((Difference(b, Complement(c))), Intersection(Difference(b, Complement(a)), Union(c, b)));
   printSet(d);
   printf("\n\n");

  return 0;
}


