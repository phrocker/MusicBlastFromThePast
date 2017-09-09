#include <iostream>
#include <fstream>

using namespace std;

int main()
{


fstream inoutFile;
inoutFile.open("myfile.dat", ios::in|ios::out|ios::binary);
int data = 15;
int A[19]={0};
inoutFile.write((char *) & data, sizeof(int));
inoutFile.write((char *) A, 20*sizeof(int));
inoutFile.seekg(0L, ios::beg);
inoutFile.read((char *) data, sizeof(int));
inoutFile.read((char *) A, 20*sizeof(int));
    return 0;
}
