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
void mergeSort (std::vector<T>& myArray, int k, Comparator comp){
	if(myArray.size() <= 1)
		return;
	std::vector<std::pair<int,int>> indices = findIndices(myArray, k, comp);
	for(int i = 0; i < indices.size(); i++){
		std::vector<T> kthArray;
		for(int j = indices[i].first; j <= indices[i].second; j++)
			kthArray.push_back(myArray[j]);
		for(int k = 0; k < kthArray.size(); k++)
			std::cout << kthArray[k] << std::endl;
		std::cout << "index " << indices[i].first << std::endl; 
		mergeSort(kthArray, k, comp);

	}

}
