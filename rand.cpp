#include <iostream>
#include <fstream>
#include <string>
using namespace std;
string
random_string(unsigned int size)
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

int main()
{
	cout << random_string(45) << endl;
	return 0;
}
