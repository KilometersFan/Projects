#include <vector>
#include <iostream>

template <class T, class Comparator>
std::vector<std::pair<int,int>> findIndices (std::vector<T> &myArray, int k, Comparator comp){
	std::vector<std::pair<int, int>> indices;
	int range = myArray.size()-1;
	int i = 0;
	while(i <= range){
		if(range/k + i <= range){
			indices.push_back(std::make_pair(i, range/k + i));
			i += range/k + 1;
		}
		else{
			indices.push_back(std::make_pair(i, range));
			break;
		}
	}
	return indices;
}

template <class T, class Comparator>
void mergeSorter()

template <class T, class Comparator>
void mergeSort (std::vector<T>& myArray, int k, Comparator comp){
	std::vector<std::pair<int,int>> indices = findIndices(myArray, k, comp);
	for (unsigned int j = 0; j < indices.size(); j++)
		std::cout << indices[j].first << " " << indices[j].second<< std::endl;
	// for (unsigned int i = 0; i < indices.size(); i++)
	// 	mergeSort()

}
