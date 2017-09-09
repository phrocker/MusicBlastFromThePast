#include <iostream>
#include <string>
using namespace std;
#include "rijndael_encryption.h"

int main()
{
	rijndael_encryption *ds = new rijndael_encryption("ADegj2937gjaDeiF");
	string blah = "blah me";
	cout << ds->encrypt_text(blah) << endl;
	string dss = ds->encrypt_text(blah);
	for (int i=0; i < dss.length(); i++)
		cout << (int)dss.at(i) << " ";
cout << endl;
	delete ds;
	
}
