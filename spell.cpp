#include <iostream>
#include <fstream>
#include <string>
#include "stdafx.h"
#include "HashTable.h"
#include <vector>
using namespace std;
int main()
{
    vector< string > lists;
lists.resize(1);
    string strings;
//    cin >> strings;
    HashTable<string,string> hsh(&stoi,1,"--");
    
    CString one,two;
    MString ss(strings.c_str());
    //ss.DoubleMetaphone(one,two);
//    cout << " " << one.data << " " << two.data << endl;
        char ff[4];    
    int d=0;
    string filename="";
        string blahd="";
                string blahds="";
//    cin >> filename;

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
'Z',
};
    for (int i =10; i < 100; i+=5)

//    while(filename!="-1")
    {  
        blahds="american-words.";
        sprintf(ff,"%i",i);
        blahds+=ff;
//        string sd="F:\\Programming\\spell\final\english-words.";
        ifstream ifs(blahds.c_str());
        if (ifs)
        {

        cout << "reading " << blahds<< endl;
        while(ifs>>blahd)
        {
     //   cout << d << endl;
//            cout << "s";
    
            MString ss(blahd.c_str());
           ss.DoubleMetaphone(one,two);
            //  cout << " " << one.data << " " << two.data << endl;
            bool sl=false;
            for (int i =0 ; i < lists.size(); i++)
            {
                if (lists[i]==one.data)
                {
                 sl=true;
                 break;
                }
            }
            if (sl) continue;
//            file << one.data << " ";
            lists.resize( lists.size() + 1 );
            lists[d]=one.data;
        d++;
//            hsh[one.data]=one.data;
       // cout << d << endl;
        }
        ifs.close();
        }
        cout << "finished with " << filename << endl;

//        cin >> filename;
    }
    ofstream file("metaphone.txt");
            for (int i =0 ; i < lists.size(); i++)
            {
                file << lists[i] << " ";
            }
    file.close();
    cout << "we are finished" << endl;
    cout << d+1 << endl;
cin >> filename;
    while(filename!="-1")
    {
    if (hsh[filename].GetData()=="--") cout << "not found" << endl;
    else
        cout << "found" << endl;
        cin >> filename;
}    
    return 0;
}
