/*
Title: argument validator
Author: Terrence Jackson
Date: Nov 2 2021
Mod Date: Nov 4 2021
Purpose: 440 HW 10: Argument Validator
         Implement a C program to check logical arguments for validity.
Description:
        uses functions from previous logic implementation
        generates and checks argument truth tables for validity
        hard coded examples
*/

/*
    Argument Validator

    Dan Ross Oct 2016

    Generates an argument in table format and checks the argument for validity.

    Given the propositions:
    
        p = "The moon is made of cheese."
        q = "Winter is coming."

    The following argument is VALID:
    
        "The moon is made of cheese or winter is coming.  The moon is not made of cheese. Therefor, winter is coming."
    
    Symbolically:

        P1:  pVq
        P2:  !p
         Q:  q

    As stored in Argument table for validation:

              P1   P2   Q
    p    q    pVq  !p   q

    0    0    0    1    0   //row0
    0    1    1    1    1   //row1
    1    0    1    0    0   //row2
    1    1    1    0    1   //row3

  //0    1    2    3    4

    HOWEVER, The following argument is INVALID:

    "The moon is made of cheese or winter is coming.  The moon is made of cheese. Therefor, winter is coming."

    Symbolically:

    P1:  pVq
    P2:  p
     Q:  q

    As stored in Argument table for validation:

              P1   P2   Q
    p    q    pVq  p    q

    0    0    0    0    0   //row0
    0    1    1    0    1   //row1
    1    0    1    1    0   //row2     INVALID HERE
    1    1    1    1    1   //row3
    
*/
#include <stdio.h>
#pragma warning( disable : 4805)
#pragma warning( disable : 4800)

// Required logic function prototypes, in operation precedence order.
// See previous homework for truth table definitions.
bool NOT(bool p);           //not
bool AND(bool p, bool q);   //and
bool OR(bool p, bool q);    //inclusive or
bool XOR(bool p, bool q);   //exclusive or
bool IMP(bool p, bool q);   //implication
bool BCN(bool p, bool q);   //bicondition
bool MAY(bool p, bool q);   //maybe
bool BEC(bool p, bool q);   //because

//functions copied from previous hw
/*
Returns boolean truth value of implication between two inputs
x   y   z
0   0   1
0   1   1
1   0   0
1   1   1
*/
bool IMP(bool a, bool b) {
    bool ret = true;
    if(a && !b) {
        ret = false;
    }
    return ret;
}

/*
Returns boolean truth value of bicondition between two inputs
x   y   z
0   0   1
0   1   0
1   0   0
1   1   1
*/
bool BCN(bool a, bool b) {
    bool ret = false;
    if(a==b) {
        ret = true;
    }
    return ret;
}

/*
Returns boolean truth value of maybe between two inputs
x   y   z
0   0   0
0   1   1
1   0   0
1   1   0
*/
bool MAY(bool a, bool b) {
    bool ret = false;
    if(!a && b) { //a==FALSE b==TRUE
        ret = true;
    }
    return ret;
}

/*
Returns boolean truth value of because between two inputs
x   y   z
0   0   0
0   1   0
1   0   1
1   1   1
*/
bool BEC(bool a, bool b) {
    bool ret = false;
    if(a) { //a==TRUE
        ret = true;
    }
    return ret;
}

/*
Returns boolean truth value of AND between two inputs
x   y   z
0   0   0
0   1   0
1   0   0
1   1   1
*/
bool AND(bool a, bool b) {
    return a&&b;
}

/*
Returns boolean truth value of OR between two inputs
x   y   z
0   0   0
0   1   1
1   0   1
1   1   1
*/
bool OR(bool a, bool b) {
    return a||b;
}

/*
Returns boolean truth value of XOR between two inputs
x   y   z
0   0   0
0   1   1
1   0   1
1   1   0
*/
bool XOR(bool a, bool b) {
    bool ret = false;
    if(a!=b) {
        ret = true;
    }
    return ret;
}

/*
Returns boolean truth value of XOR between two inputs
x   z
0   1
1   0
*/
bool NOT(bool a) {
    return !a;
}


/*
    The Argument table

    Column format is lil propositions, big (compound) propositions, conclusion:
    p, q, ... z, P1, P2, ... PN,  Q

    Row format is all pqr value permutations in binary count order:
    00
    01
    10
    11
    etc...

*/
#define LOTS 42
bool Argument[LOTS][LOTS];
int NLilProps;   // how many lil propositions, p thru z
int NBigProps;   // how many big propositions, P1 thru PN


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


/* 
Looks at filled Argument array and checks if it contains a valid argument

THE RULE IS:
For every row where all Premises, PX are true, then Conclusion Q is also true. 

YOU WRITE THIS
MUST work for ANY sized argument without modification
*/
bool IsValid()
{
    for (int row = 0, rows = my_pow(2, NLilProps); row < rows; row++)
    {
        for (int col = NLilProps, cols = NLilProps + NBigProps; col < cols; col++) 
        {
            if(!Argument[row][col] && ((col+1) < cols)) {
                //if there is a false and it isn't Q, don't worry about this row 
                break;
            }
            else {
                //if we get here then there wasn't a false thru all the P's
                if(!Argument[row][col]) { //if Q is false
                    return false;
                }
            }
        }
    }
    return true;
}


// Does what its name suggests
void PrintArgument()
{
    for (int row = 0, rows = my_pow(2, NLilProps); row < rows; row++)
    {
        for (int col = 0, cols = NLilProps + NBigProps; col < cols; col++)
            printf("%d    ", Argument[row][col]);
        printf("\n");
    }
}

/* 
    "I'd like to have an argument, please."
    "I'm not allowed to argue unless you've paid."
    "...No it isn't."
*/
int main()
{
//*********************************ARG 1*********************************//
    // A fancy interactive parser would determine these values from user console input
    NLilProps = 2;
    NBigProps = 3;   // includes Q

    // **** THIS IS THE BEGINNING OF ONE ARGUMENT VALIDITY CHECK ****

    int rows = my_pow(2, NLilProps);    // how many rows
    int row = 0;                        // current row

    /* 
    Stores pqr permutations and a series of Compound Propositions (an Argument) into the Argument table.
    Calculates the value of HARDCODED compound (big) propositions for each value tuple of (lil) propositions
    For the homework, write similar loops and modify them with hardcoded big P's and Q for each of the Arguments, 
    print the new argument table and the validity check.  Easy peasy.
    Repeat as necesary for each Argument.
    */
    while (row < rows)
    {
        bool q = row & 1; bool p = row & 2;
        
        // NOTE: An INTERACTIVE version could send string expressions to an evaluator here
        bool P1 = OR(p, q);   // something like P1 = eval(strP1, p, q);  where str1 is "pVq" entered by the user.
        bool P2 = p;
        bool Q = q;

        // Fill the argument table
        Argument[row][0] = p;
        Argument[row][1] = q;
        Argument[row][2] = P1;
        Argument[row][3] = P2;
        Argument[row][4] = Q;

        row++;
    }
    
    // print out each argument
    printf("          P1   P2   Q\n");
    printf("p    q    pVq  p    q\n\n");
    PrintArgument();
        
    // "Open the pod bay doors please HAL"
    // References are from the classic movie "2001: A Space Odyssey" REQUIRED viewing for all CS majors.
    if (IsValid())
        printf("Your argument is valid, Dave.  I enjoy working with humans.\n");
    else
        printf("I'm sorry Dave, but I'm afraid your argument is invalid.\n");

    // **** THIS IS THE END OF ONE ARGUMENT VALIDITY CHECK ****
    // COPY, PASTE, MODIFY FOR EACH ARGUMENT VALIDITY CHECK



//*********************************ARG 2*********************************//
// A fancy interactive parser would determine these values from user console input
    NLilProps = 3;
    NBigProps = 3;   // includes Q

    // **** THIS IS THE BEGINNING OF ONE ARGUMENT VALIDITY CHECK ****

    rows = my_pow(2, NLilProps);    // how many rows
    row = 0;                        // current row

    /* 
    Stores pqr permutations and a series of Compound Propositions (an Argument) into the Argument table.
    Calculates the value of HARDCODED compound (big) propositions for each value tuple of (lil) propositions
    For the homework, write similar loops and modify them with hardcoded big P's and Q for each of the Arguments, 
    print the new argument table and the validity check.  Easy peasy.
    Repeat as necesary for each Argument.
    */
    while (row < rows)
    {
        bool q = row & 1; bool p = row & 2; bool r = row & 4;
        
        // NOTE: An INTERACTIVE version could send string expressions to an evaluator here
        bool P1 = IMP(p, r);   // something like P1 = eval(strP1, p, q);  where str1 is "pVq" entered by the user.
        bool P2 = IMP(p, q);
        bool Q = IMP(p, AND(r, q));

        // Fill the argument table
        Argument[row][0] = p;
        Argument[row][1] = q;
        Argument[row][2] = r;
        Argument[row][3] = P1;
        Argument[row][4] = P2;
        Argument[row][5] = Q;

        row++;
    }
    
    // print out each argument
    printf("               P1   P2   Q\n");
    printf("p    q    r    p->r p->q p->(rAq)\n\n");
    PrintArgument();
        
    // "Open the pod bay doors please HAL"
    // References are from the classic movie "2001: A Space Odyssey" REQUIRED viewing for all CS majors.
    if (IsValid())
        printf("Your argument is valid, Dave.  I enjoy working with humans.\n");
    else
        printf("I'm sorry Dave, but I'm afraid your argument is invalid.\n");

    // **** THIS IS THE END OF ONE ARGUMENT VALIDITY CHECK ****

//*********************************ARG 3*********************************//
// A fancy interactive parser would determine these values from user console input
    NLilProps = 2;
    NBigProps = 2;   // includes Q

    // **** THIS IS THE BEGINNING OF ONE ARGUMENT VALIDITY CHECK ****

    rows = my_pow(2, NLilProps);    // how many rows
    row = 0;                        // current row

    /* 
    Stores pqr permutations and a series of Compound Propositions (an Argument) into the Argument table.
    Calculates the value of HARDCODED compound (big) propositions for each value tuple of (lil) propositions
    For the homework, write similar loops and modify them with hardcoded big P's and Q for each of the Arguments, 
    print the new argument table and the validity check.  Easy peasy.
    Repeat as necesary for each Argument.
    */
    while (row < rows)
    {
        bool q = row & 1; bool p = row & 2;
        
        // NOTE: An INTERACTIVE version could send string expressions to an evaluator here
        bool P1 = AND(p, NOT(p));   // something like P1 = eval(strP1, p, q);  where str1 is "pVq" entered by the user.
        bool Q = q;

        // Fill the argument table
        Argument[row][0] = p;
        Argument[row][1] = q;
        Argument[row][2] = P1;
        Argument[row][3] = Q;

        row++;
    }
    
    // print out each argument
    printf("          P1   Q\n");
    printf("p    q    pA!p q\n\n");
    PrintArgument();
        
    // "Open the pod bay doors please HAL"
    // References are from the classic movie "2001: A Space Odyssey" REQUIRED viewing for all CS majors.
    if (IsValid())
        printf("Your argument is valid, Dave.  I enjoy working with humans.\n");
    else
        printf("I'm sorry Dave, but I'm afraid your argument is invalid.\n");

    // **** THIS IS THE END OF ONE ARGUMENT VALIDITY CHECK ****

//*********************************ARG 4*********************************//
// A fancy interactive parser would determine these values from user console input
    NLilProps = 4;
    NBigProps = 3;   // includes Q

    // **** THIS IS THE BEGINNING OF ONE ARGUMENT VALIDITY CHECK ****

    rows = my_pow(2, NLilProps);    // how many rows
    row = 0;                        // current row

    /* 
    Stores pqr permutations and a series of Compound Propositions (an Argument) into the Argument table.
    Calculates the value of HARDCODED compound (big) propositions for each value tuple of (lil) propositions
    For the homework, write similar loops and modify them with hardcoded big P's and Q for each of the Arguments, 
    print the new argument table and the validity check.  Easy peasy.
    Repeat as necesary for each Argument.
    */
    while (row < rows)
    {
        bool q = row & 1; bool p = row & 2; bool r = row & 4; bool s = row & 8;
        
        // NOTE: An INTERACTIVE version could send string expressions to an evaluator here
        bool P1 = AND(IMP(p, q), IMP(r, s));   // something like P1 = eval(strP1, p, q);  where str1 is "pVq" entered by the user.
        bool P2 = OR(p, r);
        bool Q = OR(q, s);

        // Fill the argument table
        Argument[row][0] = p;
        Argument[row][1] = q;
        Argument[row][2] = r;
        Argument[row][3] = s;
        Argument[row][4] = P1;
        Argument[row][5] = P2;
        Argument[row][6] = Q;

        row++;
    }
    
    // print out each argument
    printf("                    P1              P2   Q\n");
    printf("p    q    r    s    (p->q)A(r->s)   pVr  qVs\n\n");
    PrintArgument();
        
    // "Open the pod bay doors please HAL"
    // References are from the classic movie "2001: A Space Odyssey" REQUIRED viewing for all CS majors.
    if (IsValid())
        printf("Your argument is valid, Dave.  I enjoy working with humans.\n");
    else
        printf("I'm sorry Dave, but I'm afraid your argument is invalid.\n");

    // **** THIS IS THE END OF ONE ARGUMENT VALIDITY CHECK ****

//*********************************ARG 5*********************************//
// A fancy interactive parser would determine these values from user console input
    NLilProps = 3;
    NBigProps = 3;   // includes Q

    // **** THIS IS THE BEGINNING OF ONE ARGUMENT VALIDITY CHECK ****

    rows = my_pow(2, NLilProps);    // how many rows
    row = 0;                        // current row

    /* 
    Stores pqr permutations and a series of Compound Propositions (an Argument) into the Argument table.
    Calculates the value of HARDCODED compound (big) propositions for each value tuple of (lil) propositions
    For the homework, write similar loops and modify them with hardcoded big P's and Q for each of the Arguments, 
    print the new argument table and the validity check.  Easy peasy.
    Repeat as necesary for each Argument.
    */
    while (row < rows)
    {
        bool q = row & 1; bool p = row & 2; bool r = row & 4;
        
        // NOTE: An INTERACTIVE version could send string expressions to an evaluator here
        bool P1 = XOR(p, q);   // something like P1 = eval(strP1, p, q);  where str1 is "pVq" entered by the user.
        bool P2 = BCN(q, r);
        bool Q = r;

        // Fill the argument table
        Argument[row][0] = p;
        Argument[row][1] = q;
        Argument[row][2] = r;
        Argument[row][3] = P1;
        Argument[row][4] = P2;
        Argument[row][5] = Q;

        row++;
    }
    
    // print out each argument
    printf("               P1   P2    Q\n");
    printf("p    q    r    p+q  p<->q r\n\n");
    PrintArgument();
        
    // "Open the pod bay doors please HAL"
    // References are from the classic movie "2001: A Space Odyssey" REQUIRED viewing for all CS majors.
    if (IsValid())
        printf("Your argument is valid, Dave.  I enjoy working with humans.\n");
    else
        printf("I'm sorry Dave, but I'm afraid your argument is invalid.\n");

    // **** THIS IS THE END OF ONE ARGUMENT VALIDITY CHECK ****

//*********************************ARG 6*********************************//
// A fancy interactive parser would determine these values from user console input
    NLilProps = 3;
    NBigProps = 3;   // includes Q

    // **** THIS IS THE BEGINNING OF ONE ARGUMENT VALIDITY CHECK ****

    rows = my_pow(2, NLilProps);    // how many rows
    row = 0;                        // current row

    /* 
    Stores pqr permutations and a series of Compound Propositions (an Argument) into the Argument table.
    Calculates the value of HARDCODED compound (big) propositions for each value tuple of (lil) propositions
    For the homework, write similar loops and modify them with hardcoded big P's and Q for each of the Arguments, 
    print the new argument table and the validity check.  Easy peasy.
    Repeat as necesary for each Argument.
    */
    while (row < rows)
    {
        bool q = row & 1; bool p = row & 2; bool r = row & 4;
        
        // NOTE: An INTERACTIVE version could send string expressions to an evaluator here
        bool P1 = MAY(p, r);   // something like P1 = eval(strP1, p, q);  where str1 is "pVq" entered by the user.
        bool P2 = BEC(p, q);
        bool Q = BCN(q, OR(r, p));

        // Fill the argument table
        Argument[row][0] = p;
        Argument[row][1] = q;
        Argument[row][2] = r;
        Argument[row][3] = P1;
        Argument[row][4] = P2;
        Argument[row][5] = Q;

        row++;
    }
    
    // print out each argument
    printf("               P1   P2   Q\n");
    printf("p    q    r    p?r  p@q  q<->(r+p)\n\n");
    PrintArgument();
        
    // "Open the pod bay doors please HAL"
    // References are from the classic movie "2001: A Space Odyssey" REQUIRED viewing for all CS majors.
    if (IsValid())
        printf("Your argument is valid, Dave.  I enjoy working with humans.\n");
    else
        printf("I'm sorry Dave, but I'm afraid your argument is invalid.\n");

    // **** THIS IS THE END OF ONE ARGUMENT VALIDITY CHECK ****


    return 0;

}