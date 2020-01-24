#include <iostream>
#include <cstdlib>
#include <string.h>

using namespace std; 



/**
    Encodes a string of any size by substituting text characters by an offset given as input.
    Handles characters outside of the alphabet by not encoding them so that language retains syntax. .

    @plaintext: a string of characters that is not encoded .
    @key: the offset used to encode each character in the string.
    
*/
void encrypt(string &plaintext, int key)
{
	//modulo offset by alphabet length
	int offset = (key%26);	

    for (int i=0; i < plaintext.length(); i++)
    {	
	
    	//check if character is capital 
        if (plaintext[i] >= 65 && plaintext[i] <= 90 )
        {
        	//encrypt by shifting by key size
            plaintext[i] = (char)((((plaintext[i] - 65 ) + offset )% 26 )+65);
        }
 
        //check if character is lowercase 
        else if (plaintext[i] >= 97 && plaintext[i] <= 122 )
        {
        	//encrypt by shifting by key size
            plaintext[i] = (char)((((plaintext[i]  -97 ) + offset )% 26)+97);
        }
    }
}

/**
    Decodes a string of any size by substituting text characters by an offset given as input.
    Handles characters outside of the alphabet by not decoding them so that language retains syntax. 

    @enText: a string of characters that is encrypted.
    @key: the offset used to decode each character in the string.
    
*/
void decrypt(string &enText, int key)
{
	//modulo offset by alphabet length
	int offset = (key%26);	

    for (int i=0; i < enText.length(); i++)
    {	
	
    	//check if character is capital 
        if (enText[i] >= 65 && enText[i] <= 90 )
        {
        	//encrypt by shifting by key size
            enText[i] = (char)((((enText[i] -65) +26-offset) %26)+65) ;
        }

        //check if character is lowercase 
        else if (enText[i] >= 97 && enText[i] <= 122 )
        {
        	//encrypt by shifting by key size
            enText[i] = (char)((((enText[i] -97 ) +26-offset )% 26)+97);
        }
    }
}

int main()
{
	//intialization
	string encrypted, copytext;
	int key; 
	int length = 0; 

	//prompt for ciphertext
	cout<<"Ceasar Cipher Decryption Tool"<<endl;
	cout<<"============================="<<endl<<endl;
	cout << "ENTER ENCRYPTED TEXT TO BE DECODED: ";

	//get input
	getline(cin, encrypted); 
	copytext = encrypted;
	cout << endl;
	
	//run a loop to see all possible plaintext
	for(key =1; key<27; key++)
	{
		//copy original array to the new one being decoded.
		encrypted = copytext;

		//decrypt the encoded message 
		decrypt(encrypted, key);

		//output result
		cout<<"POSSIBLE DECRYPTION USING KEY: "<<key<<endl;
		cout<< encrypted<<endl<<endl;
	}



  return 0;

}

