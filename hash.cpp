#include <iostream>
#include <string>
#include "md5.h"
using namespace std;



int main()
{
        int d = 1234567890;
        char *dd= new char[sizeof(d)*8];
        sprintf(dd,"%i",d);
        string name = "name";
        string hsh = MD5String((char*)name.c_str());
        string temp="";
        cout << name << endl;
        cout << hsh << endl;
        for ( int i =0; i < hsh.length(); i++)
        {
            temp+=hsh[i];
            if (i < strlen(dd))
                temp+=dd[i];        
        }
        delete [] dd;
        string fhash=MD5String((char*)temp.c_str());
        cout << fhash << endl;
        return 0;
}
