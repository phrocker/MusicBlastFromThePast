#include <iostream>
#include <vector>

using namespace std;

int main()
{
	vector< vector<int> > rows;
	rows.resize(10);
	rows[0].resize(10);
	rows[0][1]=9;
	rows[9].resize(10);
	rows[9][0]=0;
	cout << rows[9][0] << "=" << rows[0][1] << endl;
}