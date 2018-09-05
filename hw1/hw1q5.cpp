#include <iostream>
#include <sstream>
#include <cstring> 
#include <fstream>

using namespace std;

int main(int argc, char* argv[]){
	//Checking for invalid inputs
	if(argc < 2){
		cout << "Please input a file name to read" << endl;
		return 1;
	}
	if(argc > 2){
		cout <<"Please input one file at a time" << endl;
		return 1;
	}
	ifstream ifile (argv[1]);
	if (ifile.fail()){
		cout << "Couldn't open file" << endl;
		return 1;
	}
	int size = 0;
	ifile >> size;
	string* words = new string[size];
	string buff = "";
	//add strings in reverse order into words array
	for (int i = 0; i < size; i++){
		ifile >> buff;
		words[size - i - 1] = buff;
	}
	for (int i = 0; i < size; i++){
		cout << words[i] << endl;
	}
	ifile.close();
	delete [] words;
	return 0;
}
