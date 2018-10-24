#include <vector>

void findIndices(std::vector<std::pair<int,int>> &indices, int range, int k){
	int i = 0;
	while(i <= range){
		if(range/k + i <= range){
			indices.push_back(std::make_pair(i, range/k + i));
			i += (range/k + 1);
		}
		else{
			indices.push_back(std::make_pair(i, range));
			break;
		}
	}
}

template <class T, class Comparator>
void merge (std::vector<T>& myArray, std::vector<std::vector<T>> &subArrays, Comparator comp){
	int myArrayIterator = 0;
	while(subArrays.size() > 1){
		int index = 0;
		for(unsigned int i = 0; i < subArrays.size(); i++){
			if(!comp(subArrays[index][0], subArrays[i][0]))
				index = i;
		}
		T temp = myArray[myArrayIterator];
		myArray[myArrayIterator] = subArrays[index][0];
		subArrays[index][0] = temp;
		subArrays[index].erase(subArrays[index].begin());
		myArrayIterator++;
		if(subArrays[index].empty())
			subArrays.erase(subArrays.begin() + index);
	}
	for	(unsigned int k = 0; k < subArrays[0].size(); k++){
		T temp = myArray[myArrayIterator];
		myArray[myArrayIterator] = subArrays[0][k];
		subArrays[0][k] = temp;
		myArrayIterator++;
	}
}

template <class T, class Comparator>
void mergeSort (std::vector<T>& myArray, int k, Comparator comp){
	if(myArray.size() <= 1)
		return;
	std::vector<std::pair<int,int>> indices;
	findIndices(indices, myArray.size() - 1, k);
	std::vector<std::vector<T>> kvectors;
	for(unsigned int j = 0; j < indices.size(); j++){
		std::vector<T> values;
		for(int i = indices[j].first; i <= indices[j].second; i++){
			values.push_back(myArray[i]);
		}
		kvectors.push_back(values);
	}
	for(unsigned int l = 0; l < kvectors.size(); l++){
		mergeSort(kvectors[l], k , comp);
	}
	merge(myArray, kvectors, comp);
}