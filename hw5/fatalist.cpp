#include "msort.h"
using namespace std;

bool FatalistHypothesis(std::vector<std::pair<int,int>> &grades){
	NumPairFirstComp comp1;
	NumPairSecondComp comp2;
	mergeSort(grades, 2, comp1);
	for (unsigned int m = 0; m < grades.size(); m++)
		cout << grades[m].first << " " << grades[m].second << endl;
	for(unsigned int j = 0; j < grades.size() -1; j++){
		if(grades[j].first == grades[j+1].first)
			continue;
		if(grades[j].second < grades[j+1].second)
			return false;
	}
	mergeSort(grades, 2, comp2);
	for (unsigned int l = 0; l < grades.size(); l++)
		cout << grades[l].first << " " << grades[l].second << endl;
	for(unsigned int i = 0; i < grades.size() -1; i++){
		if(grades[i].second == grades[i+1].second)
			continue;
		if(grades[i].first < grades[i+1].first)
			return false;
	}
	return true;
}

int main(int argc, char const *argv[])
{
	std::vector<std::pair<int,int> > grades;
	grades.push_back(make_pair(60, 60));
	grades.push_back(make_pair(60, 80));
	grades.push_back(make_pair(60, 60));
	grades.push_back(make_pair(60, 70));
	grades.push_back(make_pair(70, 80));
	grades.push_back(make_pair(100, 90));
	grades.push_back(make_pair(60, 30));
	cout << FatalistHypothesis(grades) << endl;
	return 0;
}