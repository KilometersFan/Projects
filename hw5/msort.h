#include <vector>
#include <iostream>
#include <algorithm>
#include "functor.h"
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
void merge (std::vector<T>& myArray, std::vector<std::vector<T>> subArrays, std::vector<std::pair<int,int>> indices, Comparator comp){
	int myArrayIterator = 0;
	while(subArrays.size() > 1){
		int index = 0;
		for(unsigned int i = 0; i < subArrays.size(); i++){
			if(!subArrays[i].empty()){
				if(!comp(subArrays[index][0], subArrays[i][0]))
					index = i;
			}
		}
		std::swap(subArrays[index][0], myArray[myArrayIterator]);
		subArrays[index].erase(subArrays[index].begin());
		myArrayIterator++;
		if(subArrays[index].empty())
			subArrays.erase(subArrays.begin() + index);
	}
	for	(unsigned int k = 0; k < subArrays[0].size(); k++){
		std::swap(myArray[myArrayIterator], subArrays[0][k]);
		myArrayIterator++;
	}
}

template <class T, class Comparator>
void mergeSort (std::vector<T>& myArray, int k, Comparator comp){
	if(myArray.size() <= 1)
		return;
	std::vector<std::pair<int,int>> indices = findIndices(myArray, k);
	std::vector<std::vector<T>> kvectors;
	for(unsigned int j = 0; j < indices.size(); j++){
		std::vector<T> values;
		for(int i = indices[j].first; i <= indices[j].second; i++){
			values.push_back(myArray[i]);
		}
		// for(unsigned int m = 0; m < values.size(); m++)
		// 	std::cout << values[m] << std::endl;
		// std::cout << std::endl;
		kvectors.push_back(values);
	}
	for(unsigned int l = 0; l < kvectors.size(); l++){
		mergeSort(kvectors[l], k , comp);
	}
	merge(myArray, kvectors, indices, comp);

}
