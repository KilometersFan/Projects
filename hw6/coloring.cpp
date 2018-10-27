#include <queue>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <utility>
#include "coloring.h" 
using namespace std;

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
	int numCountries, rows, cols;
	ifile >> numCountries;
	ifile >> rows;
	ifile >> cols;
	//store map into 2d array
	char** _map = new char*[rows];
	for (int i = 0; i < rows; i++)
		_map[i] = new char[cols];
	//fill in map
	for(int k = 0; k < rows; k++){
		for(int l = 0; l < cols; l++){
			ifile >> _map[k][l];
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
	vector<Country> countries;
	BFS(_map, visited, countries, rows, cols);
	for (int i = 0; i < countries.size(); i++){
		cout << countries[i].name << "-------"<< endl; 
		for(set<char>::iterator it = countries[i].neighbors.begin(); it != countries[i].neighbors.end(); it++)
			cout << *it << endl;
		cout << endl;
	}
	//delete memory
	for (int j = 0; j < rows; j++)
		delete [] visited[j];
	delete [] visited;
	for (int j = 0; j < rows; j++)
		delete [] _map[j];
	delete [] _map;
	return 0;
}

void BFS(char**& _map, bool**&visited, vector<Country>& countries, int & rows, int & cols){
	queue<pair<int, int>> searcher;
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < cols; j++){
			if(!visited[i][j]){
				searcher.push(make_pair(i,j));
				visited[i][j] = true;
				char delim = _map[i][j];
				Country newCountry;
				newCountry.name = delim;
				while(!searcher.empty()){
					if(searcher.front().first - 1 >= 0){
						if(_map[searcher.front().first - 1][searcher.front().second] == delim && !visited[searcher.front().first - 1][searcher.front().second]){
							searcher.push(make_pair(searcher.front().first - 1, searcher.front().second));
							visited[searcher.front().first - 1][searcher.front().second] = true;
						}
						else if(_map[searcher.front().first - 1][searcher.front().second] != delim){
							newCountry.neighbors.insert(_map[searcher.front().first - 1][searcher.front().second]);
						}
					}
					if(searcher.front().first + 1 < rows){
						if(_map[searcher.front().first + 1][searcher.front().second] == delim && !visited[searcher.front().first + 1][searcher.front().second]){
							searcher.push(make_pair(searcher.front().first + 1, searcher.front().second));
							visited[searcher.front().first + 1][searcher.front().second] = true;
						}
						else if(_map[searcher.front().first + 1][searcher.front().second] != delim){
							newCountry.neighbors.insert(_map[searcher.front().first + 1][searcher.front().second]);
						}
					}
					if(searcher.front().second - 1 >= 0){
						if(_map[searcher.front().first][searcher.front().second -1] == delim && !visited[searcher.front().first][searcher.front().second -1]){
							searcher.push(make_pair(searcher.front().first, searcher.front().second - 1));
							visited[searcher.front().first][searcher.front().second - 1] = true;
						}
						else if(_map[searcher.front().first][searcher.front().second - 1] != delim){
							newCountry.neighbors.insert(_map[searcher.front().first][searcher.front().second - 1]);
						}
					}
					if(searcher.front().second + 1 < cols){
						if(_map[searcher.front().first][searcher.front().second + 1] == delim && !visited[searcher.front().first][searcher.front().second + 1]){
							searcher.push(make_pair(searcher.front().first, searcher.front().second + 1));
							visited[searcher.front().first][searcher.front().second + 1] = true;
						}
						else if(_map[searcher.front().first][searcher.front().second + 1] != delim){
							newCountry.neighbors.insert(_map[searcher.front().first][searcher.front().second + 1]);
						}
					}
					searcher.pop();
				}
				countries.push_back(newCountry);
			}
		}
	}
}
