#include "msort.h"
using namespace std;

int main(int argc, char const *argv[])
{
	vector<string> strings;
	strings.push_back("hj///");
	strings.push_back("he");
	strings.push_back("hk");
	strings.push_back("hc");
	strings.push_back("hm");
	strings.push_back("hd35");
	strings.push_back("ha");
	strings.push_back("hi");	
	strings.push_back("hb");
	strings.push_back("hh");
	strings.push_back("hg");
	strings.push_back("hl");
	NumStrComp comp1;
	mergeSort(strings, 3, comp1);
	for (int i = 0; i < strings.size(); i++)
		cout << strings[i] << endl;
	return 0;
}