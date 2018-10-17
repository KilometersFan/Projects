#include <vector>
#include <iostream>
#include <algorithm>

template <class T>
std::vector<std::pair<int,int>> findIndices(std::vector<T> &myArray, int k){
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
void merge (std::vector<T>& myArray, std::vector<std::vector<int>> &ranges, int k, Comparator comp){
	int minIndex = 0;
	int vectorCount = 0;
	for(unsigned int i = 0; i < ranges.size(); i++){
		std::cout << myArray[minIndex] << " " << myArray[ranges[i][0]] << std::endl;
		T lhs = myArray[minIndex];
		T rhs = myArray[ranges[i][0]];
		if(!ranges[i].empty()){
			if(!comp(lhs, rhs))
			{
				vectorCount = i;
				minIndex = ranges[i][0];
			}

		}
	}
	// std::swap(myArray[minIndex], myArray[0]);
	// ranges[vectorCount].erase(ranges[vectorCount].begin());
}

template <class T, class Comparator>
void mergeSort (std::vector<T>& myArray, int k, Comparator comp){
	if(myArray.size() <= 1)
		return;
	std::vector<std::pair<int,int>> indices = findIndices(myArray, k);
	std::vector<std::vector<int>> kvectors;
	for(unsigned int j = 0; j < indices.size(); j++){
		std::vector<int> kindices;
		for(int i = indices[j].first; i <= indices[j].second; i++){
			kindices.push_back(i);
		}
		for(unsigned int m = 0; m < kindices.size(); m++)
			std::cout << kindices[m] << std::endl;
		std::cout << std::endl;
		kvectors.push_back(kindices);
	}
	std::cout << "indices pushed" << std::endl;
	for(unsigned int l = 0; l < kvectors.size(); l++)
		mergeSort(kvectors[l], k , comp);
	merge(myArray, kvectors, k, comp);

}
