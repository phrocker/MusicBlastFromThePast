#ifndef HIDDEN_H
#define HIDDEN_H
#include <vector>
#include <string>
#include "string_functions.h"
using namespace std;
class hide_text //nothing hidden here
{
    
    public:
        hide_text(){ cnt=0;         storing.resize(0); mys=""; }
        void find_encrypted(string &);
        void print_hide(string &);
        void print_show(string &);
        string fill(string &s)
        {
            mys="";
            print_show(s);
            return mys;
        }
        vector<string> storing;
        int cnt;
        string mys;
        
};

void
hide_text::find_encrypted( string &message)
{
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
        storing.resize(cnt+1);
        storing[cnt] = hidden;
        cout << "-" << storing.size() << "-" << endl;
        cnt++;
        bst2=message.substr(pos2+7,(message.length())-(pos2+7));

        message=bst;
        message+="<hid>";
        message+=bst2;

        
    }
}

void
hide_text::print_hide(string &message)
{
    string::size_type pos = 0;
    string::size_type pos2 = 0;
    int location=0;
    string rt;
    rt="";
    while(pos != string::npos)
    {
        pos = message.find("<hid>",location);
        if (message.substr(location,pos-location)[0]==32)
        rt+= message.substr(location+1,pos-location);        
        else
        rt += message.substr(location,pos-location);
if (pos==string::npos) break;
       location=pos+5;

    } 
}
void
hide_text::print_show(string &message)
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

           mys+= message.substr(location,pos-location);
        if (cnt >=0)
        {

            print_show(storing[--cnt]);
//            print_show(storing[on]);
//            on--;
        }

        }
        else
        {
            mys+= message.substr(location,message.length());
//            cout << message.substr(location,message.length());
//            rt += message.substr(location,message.length());
            break;
        }
        location=pos+5;

    } 
}




#endif


