#include "DES.cpp"
#include <string>

//function prototypes
//conversion functions 
string convertToBin(string);
string convertToHex(char[]);
int convertToDec(int);
void convertDecToBin (int num, char arr[], int index);

//get data from files
void getData(int arg[], string name);
void getSBoxes();

//encryption functions
void shiftFunc(char [][28], char [][28], int []);
void combine(char [][28], char [][28], char [][56]);
void XOR(char[], char [], char [], int);
void F(char [], char[], char[], int[], char[], int[]);
void permFunc(char dest[],const char src[],const int [], int);
void sFunc(char [], char []);
void seperateHalves(char[], char[], char[], int);
void RLFunc(char[], char[][32], char[][32]);

string DES(string , string, int&, int&);


