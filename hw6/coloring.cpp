#include <queue>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <utility>
#include "coloring.h" 
using namespace std;

int main(int argc, char const *argv[])
{
	//file validation
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
	//create country list
	vector<Country*> countries;
	//find neighbors
	BFS(_map, visited, countries, rows, cols);
	std::sort(countries.begin(), countries.end(), compareASCII);
	//color the graph
	findColors(countries[0]);
	for (unsigned int i = 0; i < countries.size(); i++)
		cout << countries[i]->name << " " << countries[i]->color << endl;
	//delete memory
	for (unsigned int j = 0; j < countries.size(); j++)
		delete countries[j];
	for (int j = 0; j < rows; j++)
		delete [] visited[j];
	delete [] visited;
	for (int j = 0; j < rows; j++)
		delete [] _map[j];
	delete [] _map;
	return 0;
}

void BFS(char**& _map, bool**&visited, vector<Country*>& countries, int & rows, int & cols){
	//create queue to go through coordinates
	queue<pair<int, int>> searcher;
	//search through every coordinate in graph
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < cols; j++){
			//search through a country
			if(!visited[i][j]){
				searcher.push(make_pair(i,j));
				visited[i][j] = true;
				char delim = _map[i][j];
				//create new country based on character
				Country* newCountry = new Country;
				newCountry->name = delim;
				while(!searcher.empty()){
					//search through adjacent nodes, if node has different delim add to neighbors
					//search above node
					if(searcher.front().first - 1 >= 0){
						if(_map[searcher.front().first - 1][searcher.front().second] == delim && !visited[searcher.front().first - 1][searcher.front().second]){
							searcher.push(make_pair(searcher.front().first - 1, searcher.front().second));
							visited[searcher.front().first - 1][searcher.front().second] = true;
						}
						else if(_map[searcher.front().first - 1][searcher.front().second] != delim){
							for(unsigned int i = 0; i < countries.size(); i++){
								if(countries[i]->name == _map[searcher.front().first - 1][searcher.front().second]){
									countries[i]->neighbors.insert(newCountry);
									newCountry->neighbors.insert(countries[i]);
									break;
								}
							}
						}
					}
					//search below node
					if(searcher.front().first + 1 < rows){
						if(_map[searcher.front().first + 1][searcher.front().second] == delim && !visited[searcher.front().first + 1][searcher.front().second]){
							searcher.push(make_pair(searcher.front().first + 1, searcher.front().second));
							visited[searcher.front().first + 1][searcher.front().second] = true;
						}
						else if(_map[searcher.front().first + 1][searcher.front().second] != delim){
							for(unsigned int i = 0; i < countries.size(); i++){
								if(countries[i]->name == _map[searcher.front().first + 1][searcher.front().second]){
									countries[i]->neighbors.insert(newCountry);
									newCountry->neighbors.insert(countries[i]);
									break;
								}
							}
						}
					}
					//search left node
					if(searcher.front().second - 1 >= 0){
						if(_map[searcher.front().first][searcher.front().second -1] == delim && !visited[searcher.front().first][searcher.front().second -1]){
							searcher.push(make_pair(searcher.front().first, searcher.front().second - 1));
							visited[searcher.front().first][searcher.front().second - 1] = true;
						}
						else if(_map[searcher.front().first][searcher.front().second - 1] != delim){
							for(unsigned int i = 0; i < countries.size(); i++){
								if(countries[i]->name == _map[searcher.front().first][searcher.front().second - 1]){
									countries[i]->neighbors.insert(newCountry);
									newCountry->neighbors.insert(countries[i]);
									break;
								}
							}
						}
					}
					//search right node
					if(searcher.front().second + 1 < cols){
						if(_map[searcher.front().first][searcher.front().second + 1] == delim && !visited[searcher.front().first][searcher.front().second + 1]){
							searcher.push(make_pair(searcher.front().first, searcher.front().second + 1));
							visited[searcher.front().first][searcher.front().second + 1] = true;
						}
						else if(_map[searcher.front().first][searcher.front().second + 1] != delim){
							for(unsigned int i = 0; i < countries.size(); i++){
								if(countries[i]->name == _map[searcher.front().first][searcher.front().second + 1]){
									countries[i]->neighbors.insert(newCountry);
									newCountry->neighbors.insert(countries[i]);
									break;
								}
							}
						}
					}
					searcher.pop();
				}
				//add to countries list
				countries.push_back(newCountry);
			}
		}
	}
}

void findColors(Country* country){
	if(country->color > 0)
		return;
	//create set of possible colors
	set<int> colors;
	for(int i = 1; i <= 4; i++)
		colors.insert(i);
	//eliminate colors that can't be used
	for(set<Country*>::iterator it = country->neighbors.begin(); it != country->neighbors.end(); it++){
		colors.erase((*it)->color);
	}
	//set first available color
	country->color = *(colors.begin());
	for(set<Country*>::iterator it = country->neighbors.begin(); it != country->neighbors.end(); it++){
		findColors(*it);
	}
}

bool compareASCII(Country* left, Country* right){
	return ((int)left->name < (int)right->name);
}