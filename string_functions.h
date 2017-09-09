#ifndef STRING_FUNCTIONS
#define STRING_FUNCTIONS
#include <iostream>
#include <string>
using namespace std;
class string_functions
{
    public:
    string_functions(){     }
        int find(char *,char *);
//        bool contains(string,string,bool);
	string random_string(unsigned int);
    unsigned int mul;
};

bool
contains(string &input,string findstring,bool caseSensitive =false) // if true case matters
{
    string comparator = input;
    if (caseSensitive==false) 
    {
        for (int i =0; i< comparator.length(); i++)
            if (comparator[i]>=65 && comparator[i]<=90)
                comparator[i]=char((int)comparator[i]+32);

        for (int i =0; i< findstring.length(); i++)
            if (findstring[i]>=65 && findstring[i]<=90)
                findstring[i]=char((int)findstring[i]+32);
    }
    string::size_type pos = comparator.find(findstring.c_str(),0);
    if (pos == string::npos) return false;
    return true;
}


bool
valid_query(string &input,string findstring,bool caseSensitive =false) // if true case matters
{
    string comparator = input;
    if (caseSensitive==false) 
    {
        for (int i =0; i< comparator.length(); i++)
            if (comparator[i]>=65 && comparator[i]<=90)
                comparator[i]=char((int)comparator[i]+32);

        for (int i =0; i< findstring.length(); i++)
            if (findstring[i]>=65 && findstring[i]<=90)
                findstring[i]=char((int)findstring[i]+32);
    }
    string::size_type pos = comparator.find(findstring.c_str(),0);
    if (pos == string::npos) return false;
    return true;
}

int
string_functions::find(char* T,char* fT)
{
	int tL = strlen(T),fL = strlen(fT);    
		
      if (fL  > tL)
		return -1;
	
	int d[255];

	int k=0; // general iterator

	for (k = 0; k < 255; k++) d[k] = 1; // set all values of d[] to 1

	for (k = 0; k < fL - 1; k++) d[fT[k]] = fL - k - 1;

	k = fL - 1;
	// begin moving through and checking chars from behind
	while (k < tL)
	{
		register int j = fL - 1;
		register int i = k;
		while (j >= 0 && T[i] == fT[j])
		{
			j--;
			i--;
		}
		if (j == -1) return i + 1;
        k += d[T[k]];
		
	}
	return -1;
}
string
string_functions::random_string(unsigned int size)
{
	string rn="";
	ifstream randomStream;
	randomStream.open("/dev/urandom");
	if (randomStream)
	{

	
	char r=0;
	for (int i=0; i< size; i++)
	{
		while(1)
		{
			randomStream.get(r);
				if ((r > 0x2F && r <= 0x39) || ((r|0x20) >= 0x61 && (r|0x20) <=0x7A))
			{rn+=r;	
			break;
		}
		}
	}
	}
	else
	{
		srand(time(NULL));
    	for (int i = 0; i < size; i++)
	      	rn += (char)((rand()% 2)==1 ? 32 : 0) + (rand() % 26+65);
	}
	return rn;
}

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


#endif
