#include <iostream>
#include "dbase.h"
#include "Query.h"
#include "result_set.h"
#include "rijndael_encryption.h"
using namespace std;
int main()
{

    cout << MD5String("dog")<<endl;
    cout << MD5String("dogs")<<endl;
    cout << MD5String("dogss")<<endl;
    cout << MD5String("dogssdgasgdsgsdgdsgsadgasdgasgs")<<endl;
    cout << MD5String("ogssdgasgdsgsdgdsgsadjasglksajgklasjdgalksjdgklasjglkasjdglksjglksjgklsjgkl;sjg;lksjglsk;jglksjgalksjglkaj3asoigjasjslkjglskjglksjglksjdglkjsdgklsjgkldogssdgasgdsgsdgdsgsadjasglksajgklasjdgalksjdgklasjglkasjdglksjglksjgklsjgkl;sjg;lksjglsk;jglksjgalksjglkaj3asoigjasjslkjglskjglksjglksjdglkjsdgklsjgklgasdgasgsdogssdgasgdsgsdgdsgsadgasdgasgs")<<endl;
    return 0;
}
