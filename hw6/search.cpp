#include <queue>
#include <iostream>
#include <fstream>
#include "search.h"
using namespace std;


int main(int argc, char const *argv[])
{
	if(argc < 2){
		cout << "Please enter a file to read." << endl;
		return 1;
	}
	ifstream ifile(argv[1]);
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
	bool**visited = new bool*[rows];
	for (int i = 0; i < rows; i++)
		visited[i] = new bool[cols];
	for(int k = 0; k < rows; k++){
		for(int l = 0; l < cols; l++){
			visited[k][l] = false;
		}
	}

	BFS(map, visited, rows, cols);
	//delete memory
	for (int j = 0; j < rows; j++)
		delete [] visited[j];
	delete [] visited;
	for (int j = 0; j < rows; j++)
		delete [] map[j];
	delete [] map;
	return 0;
}

void BFS(char**& map, bool**&visited, int & rows, int & cols){
	queue<pair<int, int>> searcher;
	int maxCountrySize = 0;
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < cols; j++){
			if(!visited[i][j]){
				searcher.push(make_pair(i,j));
				visited[i][j] = true;
				char delim = map[i][j];
				int counter = 0;
				while(!searcher.empty()){
					if(searcher.front().first - 1 >= 0){
						if(map[searcher.front().first - 1][searcher.front().second] == delim && !visited[searcher.front().first - 1][searcher.front().second]){
							searcher.push(make_pair(searcher.front().first - 1, searcher.front().second));
							visited[searcher.front().first - 1][searcher.front().second] = true;
						}
					}
					if(searcher.front().first + 1 < rows){
						if(map[searcher.front().first + 1][searcher.front().second] == delim && !visited[searcher.front().first + 1][searcher.front().second]){
							searcher.push(make_pair(searcher.front().first + 1, searcher.front().second));
							visited[searcher.front().first + 1][searcher.front().second] = true;
						}
					}
					if(searcher.front().second - 1 >= 0){
						if(map[searcher.front().first][searcher.front().second -1] == delim && !visited[searcher.front().first][searcher.front().second -1]){
							searcher.push(make_pair(searcher.front().first, searcher.front().second - 1));
							visited[searcher.front().first][searcher.front().second - 1] = true;
						}

					}
					if(searcher.front().second + 1 < cols){
						if(map[searcher.front().first][searcher.front().second + 1] == delim && !visited[searcher.front().first][searcher.front().second + 1]){
							searcher.push(make_pair(searcher.front().first, searcher.front().second + 1));
							visited[searcher.front().first][searcher.front().second + 1] = true;
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