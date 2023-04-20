#include <iostream>
#include <fstream>
#include <math.h>
using namespace std;

bool ArmstrongNum(int n){
    //CODE HERE
    return false; // Can change this line.
}
/*
int main(int argc, char** argv){
    const char* filename = argv[1];
    ifstream ifs;
	ifs.open(filename);
	int N;
	int res;
	try
	{
		ifs >> N >> res;
	}
	catch (char const* s)
	{
		printf("An exception occurred. Exception type: %s\n", s);
	}

	ifs.close();

	if (ArmstrongNum(N) == res) return 0;
	return 1;
}*/

int main(){
	cout << ArmstrongNum(15);
}
