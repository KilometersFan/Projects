#include <queue>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <utility> 
using namespace std;

void BFS(char** &map,int & rows, int & cols);

int main(int argc, char const *argv[])
{
	// if(argc < 2){
	// 	cout << "Please enter a file to read." << endl;
	// 	return 1;
	// }
	ifstream ifile("test.txt");
	if(ifile.fail()){
		cout << "Could not open file." << endl;
		return 1;
	}
	//grab config info
	int numLetters, rows, cols;
	ifile >> numLetters;
	ifile >> rows;
	ifile >> cols;
	//store map into 2d array
	char** map = new char*[rows];
	for (int i = 0; i < rows; i++)
		map[i] = new char[cols];
	//fill in map
	for(int k = 0; k < rows; k++){
		for(int l = 0; l < cols; l++){
			ifile >> map[k][l];
		}
	}

	BFS(map, rows, cols);
	//delete memory
	for (int j = 0; j < rows; j++)
		delete [] map[j];
	delete [] map;
	return 0;
}

void BFS(char**& map,int & rows, int & cols){
	queue<pair<int, int>> searcher;
	int maxCountrySize = 0;
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < cols; j++){
			if(map[i][j] != '\0'){
				searcher.push(make_pair(i,j));
				char delim = map[i][j];
				int counter = 0;
				while(!searcher.empty()){
					map[searcher.front().first][searcher.front().second] = '\0';
					if(searcher.front().first - 1 >= 0){
						if(map[searcher.front().first - 1][searcher.front().second] == delim){
							searcher.push(make_pair(searcher.front().first - 1, searcher.front().second));
							map[searcher.front().first - 1][searcher.front().second] = '\0';
						}
					}
					if(searcher.front().first + 1 < rows){
						if(map[searcher.front().first + 1][searcher.front().second] == delim){
							searcher.push(make_pair(searcher.front().first + 1, searcher.front().second));
							map[searcher.front().first + 1][searcher.front().second] = '\0';
						}
					}
					if(searcher.front().second - 1 >= 0){
						if(map[searcher.front().first][searcher.front().second -1] == delim){
							searcher.push(make_pair(searcher.front().first, searcher.front().second - 1));
							map[searcher.front().first][searcher.front().second - 1] = '\0';
						}

					}
					if(searcher.front().second + 1 < cols){
						if(map[searcher.front().first][searcher.front().second + 1] == delim){
							searcher.push(make_pair(searcher.front().first, searcher.front().second + 1));
							map[searcher.front().first][searcher.front().second + 1] = '\0';
						}
					}
					counter++;
					searcher.pop();
				}
				maxCountrySize = max(maxCountrySize, counter);
			}
		}
	}
	cout << maxCountrySize << endl;
}