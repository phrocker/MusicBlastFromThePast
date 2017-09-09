/***************************************************************************
 *            botanwrite.c
 *
 *  Fri Jun  3 22:46:49 2004
 *  Copyright  2004  User
 *  Email
 ****************************************************************************/


#include <botan/botan.h>
#include <fstream>
#include <iostream>
#define KEYL 18

int main() {
   try {
       Botan::LibraryInitializer init("thread_safe use_engines");

       Botan::byte i[KEYL];
       for(int x=0;x<KEYL;x++) i[x]=0;   //testing with a "0" key

       Botan::SymmetricKey key(i,KEYL);   //instantiate key
       Botan::Pipe pipe(Botan::get_cipher("AES/ECB", key, 
Botan::DECRYPTION)); //create pipe !exeption here!

       std::ofstream outfile("/tmp/plaintext");
       std::ifstream infile("/tmp/ciphered");

       pipe.start_msg();
       infile >> pipe;
       pipe.end_msg();
       outfile << pipe;

   } catch (std::exception &e) {
       std::cerr << "exc caught: " << e.what() << std::endl;
   }
   return 0;
}
