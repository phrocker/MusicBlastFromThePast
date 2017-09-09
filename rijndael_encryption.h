#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <iostream>
#include <string>
#include <stdio.h>
#include "rijndael.h"
#include "md5.h"
using namespace std;

#define KEYBITS 256

class rijndael_encryption
{
    public:
        rijndael_encryption(string);
        string encrypt_text(string &);
        string decrypt_text(string &);

        string encrypt_escape_text(string &);
        string decrypt_escape_text(string &);

    private:
        unsigned long rk[RKLENGTH(KEYBITS)];
        unsigned char key[KEYLENGTH(KEYBITS)];
};


rijndael_encryption::rijndael_encryption(string createKey)
{
    for (int i = 0; i < sizeof(key); i++)
        key[i]  = ( i >= createKey.length() ? 0 : (unsigned char)createKey[i] );

        
    

}

string
rijndael_encryption::encrypt_escape_text(string &plainText)
{
    string mystring = encrypt_text(plainText);
    string returnstring="";
    for (int i= 0; i<mystring.length(); i++)
    {
        returnstring += "\\";
        returnstring += mystring.at(i);
    }
    return returnstring;
}

string
rijndael_encryption::decrypt_escape_text(string &plainText)
{
    return plainText;
}


string
rijndael_encryption::encrypt_text(string &plainText)
{
//    cout << "we received " << plainText << endl;
    unsigned char plaintext[16];
    unsigned char ciphertext[16];
    string encryptedText="";
    int nrounds;
    nrounds = rijndaelSetupEncrypt(rk,key, 256);
    for (int i =0; i < plainText.length(); )
    {
//            cout << i << endl;
            int j;
            for (j =0; j<16; j++)
            {
                if (j+i >= plainText.length())
                    plaintext[j]=' ';
                else
                    plaintext[j]=(unsigned char)plainText.at(j+i);
            }

        rijndaelEncrypt(rk, nrounds, plaintext, ciphertext);

       for (int d = 0; d<16; d++)
        {
            encryptedText+=ciphertext[d];
        }
        i+=j;
    }
    return encryptedText;
}

string
rijndael_encryption::decrypt_text(string &cipherText)
{
//    cout << "we received " << cipherText << endl;
    int nrounds = rijndaelSetupDecrypt(rk, key, 256);
    if (cipherText.length()% 16 > 0) return "";
    unsigned char plaintext[16];
    unsigned char ciphertext[16];
    string decryptedText="";
    for (int i =0; i < cipherText.length(); )
    {
//            cout << i << endl;
            int j;
            for (j =0; j<16; j++)
            {
                if (j+i >= cipherText.length())
                    ciphertext[j]=' ';
                else
                    ciphertext[j]=(unsigned char)cipherText.at(j+i);
            }

       rijndaelDecrypt(rk, nrounds, ciphertext, plaintext);

       for (int d = 0; d<16; d++)
        {
            decryptedText+=plaintext[d];
        }
        i+=j;
    }
    return decryptedText;
}

string
cc_encrypt(string et)
{

    string mys="";
        char num[10];
    for (int i =0; i<et.length(); i++)
    {
        sprintf(num,"%i",et[i]);
        mys+=num;
        if (i+1 <et.length())
         mys+= " ";
    }
    return mys;

    
}

string
cc_decrypt(string et)
{
    string::size_type pos=0;
    int location = 0;
    string mys="";
    string myn="";
  while(1)
    {
        pos = et.find(" ",location);
        if (pos==string::npos)
        {
          myn+=(char)atoi(et.substr(location,pos-location).c_str());
         break;
        }
        myn += (char)atoi(et.substr(location,pos-location).c_str());
        location=pos+1;
    }  
    return myn;
}


#endif
