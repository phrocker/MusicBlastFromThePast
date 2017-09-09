#include <iostream>

#include "HashTable.h"
using namespace std;

unsigned int itoi(int &i)
{
	return (unsigned int)i;
}
void testhash()
{
	HashTable<int,int> *testHash;
	testHash = new HashTable<int,int>();
	testHash->Set(&itoi,1000,0);
	for (int i=0; i < 100000; i++)
        {
                (*testHash)[i] = i;
        }
	delete testHash;
}
int main()
{
	testhash();
	return 0;
}
