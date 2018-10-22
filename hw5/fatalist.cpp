#include "functor.h"
#include <iostream>
#include <algorithm>
using namespace std;

bool FatalistHypothesis(std::vector<std::pair<int,int>> &grades){
	NumPairFirstComp comp1;
	NumPairSecondComp comp2;
	if(grades.empty())
		return true;
	sort(grades.begin(), grades.end(), comp1);
	for(unsigned int j = 0; j < grades.size() -1; j++){
		if(grades[j].first == grades[j+1].first)
			continue;
		if(grades[j].second < grades[j+1].second)
			return false;
	}
	sort(grades.begin(), grades.end(), comp2);
	for(unsigned int i = 0; i < grades.size() -1; i++){
		if(grades[i].second == grades[i+1].second)
			continue;
		if(grades[i].first < grades[i+1].first)
			return false;
	}
	return true;
}
