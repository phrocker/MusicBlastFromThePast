#include <iostream>
#include <string>
using namespace std;
#include "ip_restrict.h"



int main()
{
    string ip = "";
    cout << "Enter ip address ";
    cin >> ip;
    cout << "You entered ip address : " << ip << endl;

    ip_restrict ip_range;

    if (ip_range.test(ip.c_str()))
        cout << "Your ip address is within our range";
    else
        cout << "Your ip address is beyond our range";

//    load_ips(ip_range);

    
    return 0;
}

