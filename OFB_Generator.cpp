//////////HEADER FILES ////////////
#include <iostream>
#include <cstdlib>
#include <string>
#include <time.h>
#include "DES.h"
using namespace std;

/////////GLOBAL VARIABLES /////////
int numOnes;
int numZeros; 

///////////////MAIN //////////////
int main ()
{
	
////////INITIALIZATION ///////////
string defaultKey, result, seed, old;
numOnes = numZeros = 0;
char randBinary [64];
char dummy; 
bool good = false; 

cout<<"OFB RANDOM NUM GENERATOR"<<endl<<"----------------"<<endl<<endl;

//prompt user
cout << "Seed for process will be chosen at random along with a standard key."<< endl;
cout << "Please enter any key and enter to continue...: ";
cin >> dummy;
cout<< "Processing Data Please Wait..."<<endl;

//seed the random number with time to ensure randomness
srand(time(NULL)); 

//generate 64 random bits
for(int i = 0; i < 64; i++)
{	
	//insert items in array as characters
	if((rand() % 2) == 0)
	 {
	 	randBinary[i] = '0';
	 }
	
	else
	{
		randBinary[i] = '1'; 
	}
} 
//convert to hex for DES function
seed = convertToHex(randBinary);

//set standard key 
defaultKey = "A0B1C2D3E4F56789";  

cout << endl; 

//Use DES encryption
result = DES(defaultKey, seed, numOnes, numZeros);

//set result
old = result; 

//loop to 2^20
for(int i = 0; i < 16384; i++)
{
  //call DES 
  result = DES(defaultKey, old, numOnes, numZeros);
  
  //set the new result to old
  old = result;     
}

cout << "RESULTS" <<endl<< "Seed: "<<seed<<"Default Key"<< defaultKey<<endl;
cout << endl << "Amount of ones generated: " << numOnes;
cout << endl << "Amount of zeroes generated: " << numZeros; 

return 0; 
}
