#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main()
{
    char abc[27]=
{
'A',
'B',
'C',
'D',
'E',
'F',
'G',
'H',
'I',
'J',
'K',
'L',
'M',
'N',
'O',
'P',
'Q',
'R',
'S',
'T',
'U',
'V',
'W',
'X',
'Y',
'Z'
};
    for (int i = 0; i < 26; i++)
    {
    cout << abc[i] << endl;
//     continue;    
    ifstream sd("metaphoneAmerican.txt");
    string file = "";
    file=abc[i];
    file+= ".dat";
    ofstream ds(file.c_str());
    string blahd="";
    while(sd>>blahd)
    {

        if (blahd[0]==abc[i])
               ds << blahd << " ";
    }    
    sd.close();
    ds.close();
    }    
    return 0;
}
