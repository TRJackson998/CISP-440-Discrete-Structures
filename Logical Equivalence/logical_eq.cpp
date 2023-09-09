/*
Title: logical equivalence
Author: Terrence Jackson
Date: Oct 22 2021
Purpose: 440 HW 8: Logical Equivalence Implementation
         Implement a C program to  test expressions for logical equivalence
Description: Implements several logical operators as functions, tests the equivalence of several expressions using truth tables
    a. bool Implication(bool a, bool b);   // a -> b
    b. bool Bicondition(bool a, bool b);   // a <-> b
    c. bool Maybe(bool a, bool b);         // a ? b
    d. bool Because(bool a, bool b);       // a @ b
    e. bool And(bool a, bool b);           // a & b
    f. bool Or(bool a, bool b);            // a | b
    g. bool Xor(bool a, bool b);           // a ^ b
    h. bool Not(bool a);                   // !a

    Testing in main, from example code from Prof Ross for part d
        a. p | q ≡ ~(~p & ~q)
        b. p & (q -> r) ≡ (p & q) ^ r
        c. p <-> q ≡ ~(p ^ q)
        d. (p <-> q) -> r ≡ p & (~q | r)
        e. ~(p -> (q & r)) ≡ p & ~(q & r)
        f. p | (p ? q) ≡ p & (p @ q)
*/

#include <iostream>
#include <iomanip>
#include <string.h>
#include <stdlib.h>
using namespace std;


/*
Returns boolean truth value of implication between two inputs
x   y   z
0   0   1
0   1   1
1   0   0
1   1   1
*/
bool Implication(bool a, bool b) {
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
bool Bicondition(bool a, bool b) {
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
bool Maybe(bool a, bool b) {
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
bool Because(bool a, bool b) {
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
bool And(bool a, bool b) {
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
bool Or(bool a, bool b) {
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
bool Xor(bool a, bool b) {
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
bool Not(bool a) {
    return !a;
}

/*
    Test (at least) the following expressions
    a. p | q ≡ ~(~p & ~q)
    b. p & (q -> r) ≡ (p & q) ^ r
    c. p <-> q ≡ ~(p ^ q)
    d. (p <-> q) -> r ≡ p & (~q | r)
    e. ~(p -> (q & r)) ≡ p & ~(q & r)
    f. p | (p ? q) ≡ p & (p @ q)
*/

int main() {
    bool LHS, RHS, equiv = true;

    ////////////////a. p | q ≡ ~(~p & ~q)////////////////
    cout << "p q | p OR q | ~(~p AND ~q)" << endl;
    equiv = true;
    for(int p = 0; p < 2; p++) {
        for(int q = 0; q < 2; q++) {
            LHS = Or(p, q);
            RHS = Not(And(Not(p), Not(q)));
            if(LHS!=RHS) equiv=false;
            cout << p << " " << q << " | " 
                 << setw(6)  << LHS << " | "
                 << setw(6) << RHS << endl;
        }
    }
    if (equiv)
    printf("These expressions are equivalent\n\n\n");
    else
    printf("These expressions are NOT equivalent\n\n\n");

    ////////////////b. p & (q -> r) ≡ (p & q) ^ r////////////////
    cout << "p q r | p AND (q -> r) | (p AND q) XOR r" << endl;
    equiv = true;
    for(int p = 0; p < 2; p++) {
        for(int q = 0; q < 2; q++) {
            for(int r = 0; r < 2; r++) {
                LHS = And(p, Implication(q, r));
                RHS = Xor(And(p, q), r);
                if(LHS!=RHS) equiv=false;
                cout << p << " " << q  << " " << r << " | " 
                    << setw(14)  << LHS << " | "
                    << setw(14) << RHS << endl;
            }

        }
    }
    if (equiv)
    printf("These expressions are equivalent\n\n\n");
    else
    printf("These expressions are NOT equivalent\n\n\n");

    ////////////////c. p <-> q ≡ ~(p ^ q)////////////////
    cout << "p q | p <-> q | ~(p XOR q)" << endl;
    equiv = true;
    for(int p = 0; p < 2; p++) {
        for(int q = 0; q < 2; q++) {
            LHS = Bicondition(p, q);
            RHS = Not(Xor(p, q));
            if(LHS!=RHS) equiv=false;
            cout << p << " " << q << " | " 
                 << setw(7)  << LHS << " | "
                 << setw(7) << RHS << endl;
        }
    }
    if (equiv)
    printf("These expressions are equivalent\n\n\n");
    else
    printf("These expressions are NOT equivalent\n\n\n");

    /*
    Example code from Prof Ross
    For example, the logic expression:

    d. ~(p -> (q & r)) ≡ p & ~(q & r)
    can be broken into Left Hand Side (LHS) and Right Hand Side (RHS),
    and re-written in functional notation as follows:
    LHS: Not((Implication(p, And(q, r))));
    RHS: And(p, Not(And(q, r)));
    Then tested for equivalence as shown below.
    */
    printf("p q r | ~(p -> (q & r)) | p & ~(q & r) \n");
    equiv = true;
    for(int p = 0; p < 2; p++) {
        for (int q = 0; q < 2; q++) {
            for (int r = 0; r < 2; r++) {
                LHS = Not((Implication(p, And(q, r))));
                RHS = And(p, Not(And(q, r)));
                if (RHS != LHS) equiv = false;
                cout << p << " " << q  << " " << r << " | " 
                    << setw(15)  << LHS << " | "
                    << setw(10) << RHS << endl;
            }
        }
    }
    if (equiv)
    printf("These expressions are equivalent\n\n\n");
    else
    printf("These expressions are NOT equivalent\n\n\n");

    ////////////////e. ~(p -> (q & r)) ≡ p & ~(q & r)////////////////
    cout << "p q r | ~(p -> (q AND r)) | p AND ~(q AND r)" << endl;
    equiv = true;
    for(int p = 0; p < 2; p++) {
        for(int q = 0; q < 2; q++) {
            for(int r = 0; r < 2; r++) {
                LHS = Not(Implication(p, And(q, r)));
                RHS = Xor(And(p, q), r);
                if(LHS!=RHS) equiv=false;
                cout << p << " " << q  << " " << r << " | " 
                    << setw(17)  << LHS << " | "
                    << setw(14) << RHS << endl;
            }
        }
    }
    if (equiv)
    printf("These expressions are equivalent\n\n\n");
    else
    printf("These expressions are NOT equivalent\n\n\n");

    ////////////////f. p | (p ? q) ≡ p & (p @ q)////////////////
    cout << "p q | (p MAYBE q) | p AND (p BECAUSE q)" << endl;
    equiv = true;
    for(int p = 0; p < 2; p++) {
        for(int q = 0; q < 2; q++) {
            LHS = Maybe(p, q);
            RHS = And(p, Because(p, q));
            if(LHS!=RHS) equiv=false;
            cout << p << " " << q << " | " 
                 << setw(11)  << LHS << " | "
                 << setw(11) << RHS << endl;
        }
    }
    if (equiv)
    printf("These expressions are equivalent\n\n\n");
    else
    printf("These expressions are NOT equivalent\n\n\n");

    return 0;
}
