#include <botan/botan.h>
#include <fstream>
#include <iostream>
#include <string>
#define KEYL 32

using namespace std;

int main() {
	string data;
   try {
       Botan::LibraryInitializer init("thread_safe use_engines");

       Botan::byte i[KEYL];
       for(int x=0;x<KEYL;x++) i[x]=0;   //testing with a "0" key

       Botan::SymmetricKey key(i,KEYL);   //instantiate key
       Botan::Pipe pipe(Botan::get_cipher("AES/ECB", key, 
Botan::ENCRYPTION)); //create pipe !exeption here!

//       std::ofstream outfile("/tmp/plaintext");
  //     std::ifstream infile("/tmp/ciphered");
		char *bs = new char[48];
	   sprintf(bs,"yeah");		
	   for (int i=strlen(bs); i < 48; i++)
		   bs[i]=0;
       pipe.start_msg();
       //infile >> pipe;
	   //bs >> pipe;
	   pipe.write((const Botan::byte*) bs,48);
	   delete [] bs;
       pipe.end_msg();
       //outfile << pipe;
	   //cout << pipe << endl;
	   data = pipe.read_all_as_string(0);
	   cout << data << endl;
		pipe.reset();
		pipe.append(Botan::get_cipher("AES/ECB", key, Botan::DECRYPTION));	   

		
	   
       pipe.start_msg();
       //infile >> pipe;
	   //bs >> pipe;
	   pipe.write(data);
	   
       pipe.end_msg();
       //outfile << pipe;
	   //cout << pipe << endl;
	   data = pipe.read_all_as_string(1);
	   cout << data << endl;

   } catch (std::exception &e) {
       std::cerr << "exc caught: " << e.what() << std::endl;
   }
   return 0;
}
