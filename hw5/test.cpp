#include "msort.h"
#include "functor.h"
using namespace std;

int main(int argc, char const *argv[])
{
	vector<string> strings;
	strings.push_back("hi");
	strings.push_back("hey");
	strings.push_back("hello");
	strings.push_back("hu");
	strings.push_back("ho");
	strings.push_back("hi");
	strings.push_back("hi");
	strings.push_back("hey");
	strings.push_back("hello");
	strings.push_back("hu");
	strings.push_back("ho");
	strings.push_back("hi");
	NumStrComp comp1;
	mergeSort(strings, 2, comp1);
	return 0;
}