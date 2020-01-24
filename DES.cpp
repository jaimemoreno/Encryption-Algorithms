//Header Files
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cmath>

using namespace std;

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

//global variable
int sBoxes[8][4][16];

string DES(string k, string s, int& one, int& zero) {

//initialization 
//input variables
string key;
string plainText;
char keyC[64];
char plainTextC[64];

//reference table variables
int PC1[56];
int PC2[48];
int ETable[48];
int shiftTable[16];
int IP[64];
int IP2[64];
int SPTable[32];

//resulting variables, catches results
char key56[56];
char Permutation[64];
char Cn[17][28];
char Dn[17][28];
char Ln[17][32];
char Rn[17][32];
char combineCD[16][56];
char subKeys[16][48];
char bigExpansion[481];
char outcome[32];
char RL[64];
char RLPermuted[64];
string encryptedMessage; 

//dummy variables used to navigate and capture results
int dummy;
int iteration;


key = k; 
     
//check to see if key is valid 
if(key.length() != 16)
{
	//output error message and end program
    cout << "Key is not of length 16, ending program";
    return 0; 
}
     
plainText = s; 
     
//check to see if plaintext is valid     
if(plainText.length() != 16)
{
	//output error message and end program 
	cout << "Plaintext is not of length 16, ending program";
	return 0; 
}

   
//convert the key from hex to dec   
key = convertToBin(key);

//convert from plaintext from hex to dec
plainText = convertToBin(plainText);

//copy the key to a character array for future manipulation
strcpy(keyC,key.c_str());

//copy the plaintext to a character array for future manipulation
strcpy(plainTextC, plainText.c_str());
   
//get the data from the files
//get table PC1  
getData(PC1,"PC1.txt");
//get table PC2
getData( PC2, "PC2.txt");
//get the IP table
getData(IP, "ip.txt");
//get the IP-1 table
getData(IP2, "ip-1.txt");
//get the Sboxes
getSBoxes(); 
//get the table indicating shifts
getData(shiftTable, "leftshifts.txt");
//get the expansion table   
getData( ETable, "expandtable.txt" );
//get permutation table 
getData( SPTable, "p.txt");

//Begin manipulation of key 
//permute the key, this will make it 56 bits
permFunc(key56, keyC, PC1, 56);
//seperate the two halves of the key Cn and Dn
seperateHalves( Cn[0], Dn[0], key56, 56);
//shift Cn and Dn according to the shiftTable
shiftFunc(Cn, Dn, shiftTable);
//combine Cn and Dn to from CDn
combine(Cn, Dn, combineCD);

//permute each of the subkeys
for(int i = 0; i < 16; i++)
{
//permute current key
permFunc(subKeys[i], combineCD[i], PC2, 48);
}

//Begin manipulation of the plaint text
//perform the initial permutation on plaintext
permFunc(Permutation, plainText.c_str(), IP, 64);

//seperate the plain text into to halves L and R
seperateHalves( Ln[0], Rn[0], Permutation, 64);   

//begin the 16 iterations of encryption
for(iteration = 0; iteration < 16; iteration++)
{
	//f(Rn-1, Kn); apply the f function to get the f value
    F(Rn[iteration],subKeys[iteration], bigExpansion,  ETable, outcome, SPTable);

      //XOR Ln-1 with f(Rn-1, Kn)
      XOR(Rn[iteration + 1], Ln[iteration], outcome, 32);

      //set Ln to Rn-1
      for(int i = 0; i < 32; i++)
         Ln[iteration + 1][i] = Rn[iteration][i];
}
    
//16 cycles have completed  
//combine the R16 and L16 
RLFunc(RL, Rn, Ln);

//apply the final permutation to RL16
permFunc(RLPermuted,RL,IP2,64);

////////////////////////////////////////////////////////////////////////

for(int i = 0; i < 64; i++)
{
	if(RLPermuted[i] == '1')
	{
		one++; 
	}
	
	else
	{
		zero++; 
	}
}

////////////////////////////////////////////////////////////////////////

//convert back to hexadecimal form
encryptedMessage = convertToHex(RLPermuted);

//return the encrypted message
return encryptedMessage;
}

//Function Implementation

//used to convert from hex to binary
string convertToBin(string arg)
{ 
//initialization 
char current;
string binary;

//loop through the 16 hex values
for(int i = 0; i < 16; i++) 
{
	//get the current value
    current = arg[i];
    
    //match the case 0-F
    //add the binary value to binary string
    switch(current)
	{
    case '0':
    binary += "0000";
    break;
    
    case '1':
    binary += "0001";
    break;
    
    case '2':
    binary += "0010";
    break;
    
    case '3':
    binary += "0011";
    break;
    
    case '4':
    binary += "0100";
    break;
    
    case '5':
    binary += "0101";
    break;
    
    case '6':
    binary += "0110";
    break;
    
    case '7':
	binary += "0111";
    break;
    
    case '8':
    binary += "1000";
    break;
    
    case '9':
    binary += "1001";
    break;
    
    case 'A':
    binary += "1010";
    break;
    
    case 'B':
	binary += "1011";
    break;
    
    case 'C':
    binary += "1100";
    break;
    
    case 'D':
    binary += "1101";
    break;
    
    case 'E':
    binary += "1110";
    break;
    
    case 'F':
	binary += "1111";
    break;
    }
}

//add the null character to end of string
binary += '\0';

//return the new binary string 
return binary;
}


//used to convert from binary to hex
string convertToHex(char binary [])
{
//initialization
string hex;   
//used to keep track if current index  
int k = 0;

//loop through the binary array 16 times
//for 16 hex values
for(int i = 0; i < 16; i++)
{
//string captures current value      
string current;

//capture the binary representation of hex value 	
for(int j = 0; j < 4; j++, k++)
    current += binary[k];

//using if and else statements
//because switch does not support strings
if(current == "0000")
	hex += "0";
	
else if(current == "0001")
    hex += "1";
    
else if(current == "0010")
    hex += "2";
    
else if(current == "0011")
    hex += "3";
    
else if(current == "0100")
    hex += "4";
    
else if(current == "0101")
    hex += "5";
    
else if(current == "0110")
    hex += "6";
    
else if(current == "0111")
    hex += "7";
    
else if(current == "1000")
    hex += "8";
    
else if(current == "1001")
    hex += "9";
    
else if(current == "1010")
    hex += "A";
    
else if(current == "1011")
    hex += "B";
    
else if(current == "1100")
    hex += "C";
    
else if(current == "1101")
    hex += "D";
    
else if(current == "1110")
    hex += "E";
    
else if(current == "1111")
    hex += "F";
   }
   
//return the string with the new hex values
return hex;
}

//used for Sbox row anc column values
int convertToDec(int num)
{ 
//initialization 
int decimal = 0;
int i = 0;
int remainder;

//loop until the number equals 0
while(num != 0)
{
//capture the remainder of modulo 10
remainder = num % 10; 
	
//correct the decimal 
decimal += remainder * pow(2,i);

//divide num by 10
num = num / 10;

//update iteration count 
i++;
}

return decimal;
}

//used in s 
void convertDecToBin (int dec, char bin[], int index) 
{ 
//initialization	
char temp[4];
int tempIndex = 3; 
int quotient  = dec / 2;
int rem = dec % 2;
dec = quotient;

 
//pad the temporary array with 0's 
for(int i = 0; i < 4; i++) 
{
temp[i] = '0';
}

//convert character to integer
temp[tempIndex] = rem + '0'; 

//while the decimal value is > 0
while(dec >= 1) 
{
quotient = dec / 2;
rem = dec % 2;
dec = quotient;

//decrement index
tempIndex--;

//convert char to int
temp[tempIndex] = rem + '0';
}

   //copy over to binary
   for(int i = 0; i < 4; i++,index++) {
      bin[index] = temp[i];
   }
}

//used to get data from files 
void getData( int data[], string file)
{ 
//intialization
ifstream fin;
int i = 0;

//open the file stream
fin.open(file.c_str());

//loop until the end of file
while(fin.good())
{ 
	//get data and put into data array
	fin >> data[i];
	//update index
    i++;
}
}

//used to get the data regarding the sboxes
void getSBoxes() 
{ 
//intialization
ifstream fin;
fin.open("sboxes.txt");
string dummy;   

//loop through the 8 boxes
for(int i = 0; i < 8; i++)
{

//get the dummy variable
fin >> dummy;

//loop through the boxes and store them according to index
for(int j = 0; j < 4; j++) 
	for(int k = 0; k < 16; k++) 
        fin >> sBoxes[i][j][k];
  
}
}

//used to shift the subkeys
void shiftFunc(char Cn[][28], char Dn[][28], int shiftTable[])
{ 
//intialization
int leftshift;
int offset;
int temp;

for(int i = 0; i < 16; i++) 
{
//get the number of shifts from table 
leftshift = shiftTable[i];

for(int j = 0; j < 28; j++)
{
//subtract the current index from the shift value
offset = j - leftshift;

//if offset is negative, wrap
if(offset < 0) 
{
  offset = (offset % 28+ 28) % 28;
}

//increment i and set temp
temp = i + 1;

//capture the left and right value at the 
//subkey number and offset
Cn[temp][offset] = Cn[i][j];
Dn[temp][offset] = Dn[i][j];
}
}
}

//used to combine C and D
void combine(char Cn[][28], char Dn[][28], char CD[][56]) 
{ 
//intialization
int index;
int col; 
    
//loop through keys 1-17; 0 is not used
for(int i = 1; i < 17;i++)
{
//allows to reach previous key 
index = i - 1;

//get the C values
    for(int j = 0; j < 28; j++) 
    	CD[index][j] = Cn[i][j];
    
//get the D values
    for(int k = 0,col = 28; k < 28; k++, col++)
         CD[index][col] = Dn[i][k];
    
}
}

//used for XOR operations
void XOR(char result[], char left[], char right[], int size) 
{ 
//intialization
int tempA, tempB;
int xorResult;
int outcome;
   
//loop through the size of left and right
for( int i = 0; i < size; i++)
{
//capture values at left and right
tempA = (int) left[i];
tempB = (int) right[i];

//subtract for ascii correction   
tempA -= 48;
tempB -= 48;

//capture the result of xor
 xorResult = (tempA + tempB) % 2;

//add 48 for correction
outcome  = xorResult + 48;
 
//cast result as a char and store in result 
result[i] = (char) outcome;
}

}

//f function 
void F(char Rn[],char Kn[], char bigExpansion[], int ETable[], char outcome[], int SPTable[]) 
{ 
//initialization   
char temp[32];

//expand Rn-1 to 48 bits
permFunc(bigExpansion, Rn, ETable, 48);

//Xor expanded RN-1 with Kn
XOR(bigExpansion,bigExpansion,Kn,48);

//selction function
sFunc(bigExpansion, outcome);

//last permutation
permFunc(temp, outcome, SPTable, 32);

//store result in outcome array
for(int i = 0; i < 32; i++)
    outcome[i] = temp[i];
}

//permutation function 
void permFunc(char result[], const char original[], const int table[], int size)
{ 
//initialization
int temp;
int one = 0;
int zero = 0; 

//loop through table values
for(int i = 0; i < size; i++) 
{
//get the temp value
temp = table[i];
 
//decrement for correct position
temp--;

//store the value of original at temp in result 
result[i] = original[temp];
}


}



//selection function for sboxes
void sFunc( char bigExpansion[], char outcome[]) 
{ 
//intialization
string row; 
string col;
char box[8][6];
int dummy = 0;
int convRow, convCol;
int index;
int i,j;
int result;


//seperate into 8 blocks
for( i = 0; i < 8 ; i++) 
{
	//seperate into 6 bits
	for( j = 0; j < 6; j++) 
	{	         
	//capture in box
	box[i][j] = bigExpansion[dummy];
	//increment dummy 
	dummy++;
	}
}

//loop 8 times 
for( i = 0, index = 0; i < 8; i++, index += 4) 
{

//get first and last bit
row = box[i][0];
row += box[i][5];

//get central bits
col = box[i][1];
col += box[i][2]; 
col += box[i][3]; 
col += box[i][4]; 

//capture int
convRow = atoi(row.c_str());
convCol = atoi(col.c_str());  
  
//convert to decimal 
convRow = convertToDec(convRow);
convCol = convertToDec(convCol);

//get the sbox value
result = sBoxes[i][convRow][convCol];

//convert back to binary
convertDecToBin(result, outcome, index );     
}
}

//used to seperate data in halves
void seperateHalves(char left[], char right[], char original[], int size) 
{ 
//intialization
int mid = size / 2;
int dummy;
int rightVar;

//get the values for the first half
for( dummy = 0; dummy < mid; dummy++) 
    left[dummy] = original[dummy];

//get the values for teh second half 
for( dummy = mid, rightVar  = 0; dummy < size; dummy++, rightVar++)
      right[rightVar] = original[dummy];
}

//combine left and right
void RLFunc(char RL [], char Rn[][32], char Ln[][32])
{
//put R in front
for(int i = 0; i < 32; i++)
      RL[i] = Rn[16][i];
      
//put L in back 
for(int j = 0,  dummy = 32; j < 32; j++, dummy++)
	RL[dummy] = Ln[16][j];
}
