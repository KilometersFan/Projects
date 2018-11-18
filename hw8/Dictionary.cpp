#include "Dictionary.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <locale>
using namespace std;
//reads in every word in the dictionary file and adds it to a set
Dictionary::Dictionary(std::string dictionary_file_name){
	ifstream ifile(dictionary_file_name);
	string buff = "";
	while (ifile >> buff){
		transform(buff.begin(), buff.end(), buff.begin(), ::tolower);
		dictionary.insert(buff);
	}
}
Dictionary::~Dictionary(){

}
//tries to find an inputted word in the dictionary
bool Dictionary::isLegalWord (std::string const & word) const {
	if(dictionary.find(word) != dictionary.end())
		return true;
	return false;
}