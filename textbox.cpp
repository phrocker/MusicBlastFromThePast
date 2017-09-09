#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "rijndael_encryption.h"
using namespace std;
vector<string> storing;

int blah;
int cnt;
void
remove_whitespace(string &message) // removes leading AND TRAILING WHTIESPACE
{
    string::size_type pos=0;
/*
    while(1)
    {
        pos = message.find(" <hid> ",0);
        if (pos==string::npos) break;
        string beg=message.substr(0,pos);
        cout << "beg " << beg << endl;
        string end=message.substr(pos+7,message.length()-(pos+7));
        cout << "end " << end << endl;
        message=beg;
        message+=end;
    
    }
*/
    int begin=-1;
    int end=-1;
    for (int i =0; i < message.length(); i++)
        if (message[i]!=32)
        {
            begin=i;
            break;
        }
    for (int i =message.length()-1; i >=0; i--)
        if (message[i]!=32)
        {
            end=i;
            break;
        }
//    cout << begin << " " << end << " " << message.length() << endl;
    message=message.substr(begin,(end-begin)+1);
}
void
find_encrypted( string &message)
{
    string id = "3293";
    int location=0;
    string::size_type pos = 0;
    string::size_type pos2 = 0;
    string bst2="";
    string hidden="";
    while(pos != string::npos)
    {
        
        pos = message.rfind("<hide>",message.length());
        pos2 = message.find("</hide>",pos); // 7 for </hide>
        if (pos==string::npos || pos2==string::npos) break;
        string bst=message.substr(location,pos);
        hidden = message.substr(pos+6,pos2-(pos+6));

        remove_whitespace(hidden);
        storing[blah] = hidden;
        cout << "-" << hidden << "-" << endl;
        blah++;
        bst2=message.substr(pos2+7,(message.length())-(pos2+7));

        message=bst;
        message+="<hid>";
        message+=bst2;

        
    }
    cout << message << endl;
}
void
print_hide(string &message)
{
    string::size_type pos = 0;
    string::size_type pos2 = 0;
    int location=0;
    while(pos != string::npos)
    {
        pos = message.find("<hid>",location);
        if (message.substr(location,pos-location)[0]==32)
        cout << message.substr(location+1,pos-location);        
        else
        cout << message.substr(location,pos-location);
if (pos==string::npos) break;
       location=pos+5;

    } 
}
void
print_show(string &message)
{
    string::size_type pos = 0;
    string::size_type pos2 = 0;
    int location=0;
///    cout << "--" << message << "--" << endl;
    while(1)
    {
        
        pos = message.find("<hid>",location);
        if (pos!=string::npos)
        {
            cout << message.substr(location,pos-location);
        if (cnt >=0)
        {
            print_show(storing[--cnt]);
//            print_show(storing[on]);
//            on--;
        }

        }
        else
        {
            cout << message.substr(location,message.length());
            break;
        }
        location=pos+5;

    } 
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
int main()
{
    storing.resize(10);
    blah=0;

    int status=1;
    string testing="";
    ifstream read("read.txt");

    string st;
    while(read>>st)
    {
     testing+=st;
     testing+=" ";
    }
    testing=testing.substr(0,testing.length()-1);
    
    rijndael_encryption encryptor("jsn39ghalq01957mbnsk3067hgnzadje");       
/*
    text="Hello my friends";

    cout << cc_encrypt(encryptor.encrypt_text(text)) << "-" << endl;
    cout << "for " << encryptor.encrypt_text(text) << endl;
    text =  cc_decrypt( cc_encrypt(encryptor.encrypt_text(text)));
    cout << text << " " << text.length() << endl;
    cout << encryptor.decrypt_text(text) << endl;
*/
    read.close();
    cout << testing << endl;
    testing =cc_decrypt(testing);

    testing=encryptor.decrypt_text(testing);


    cout << testing << endl;
    cout << "attempting to parse..." << endl;
    find_encrypted(testing);
    cout << "now printing values..." << endl;
    print_hide(testing);
    cout << endl << endl;
    cnt = blah;
    print_show(testing);
  //  for (int i = 0; i<= blah; i++)
    //{
//        cout << i << " " << storing[i] << endl;
//    }
    return 0;
}
