#include "stackint.h"
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

void parser(ifstream &ifile);

void reset_vals(bool &add, bool &multiply, int& int1, int& int2);

void add_or_multiply(bool &add, bool &multiply, int& int1, int& int2, int& final, string& prev);

int main(int argc, char const *argv[])
{
	if(argc < 1 || argc > 2){
		cout << "Please choose one input file." << endl;
		return 1;
	}
	ifstream ifile(argv[1]);
	if(ifile.fail()){
		cout << "Couldn't open file." << endl;
		return 1;
	}
	parser(ifile);
	return 0;
}
void parser(ifstream &ifile){
	const int OPEN_PAREN = -1, LEFT_SHIFT = -2, RIGHT_SHIFT = -3, ADD = -4, MULTIPLY = -5;
	StackInt stack;
	string s;
	while(getline(ifile,s)){
		char buff;
		int final = 0, int1 = 0, int2 = 0;
		bool add = false, multiply = false, malformed = false, close_paren = false;
		stringstream ss(s);
		while(ss >> buff){
			string prevOperation = "";
			if(buff == '('){
				if(!stack.empty()){
					if(stack.top() > -1){
						malformed = true;
						break;
					}
				}
				stack.push(OPEN_PAREN);
			}
			else if(buff == '*'){
				if(stack.empty()){
					malformed = true;
					break;
				}
				else{
					if(stack.top() < 0){
						malformed = true;
						break;
					}
				}
				stack.push(MULTIPLY);
			}
			else if(buff == '+'){
				if(stack.empty()){
					malformed = true;
					break;
				}
				else{
					if(stack.top() < 0){
						malformed = true;
						break;
					}
				}
				stack.push(ADD);
			}
			else if(buff == '<'){
				if(!stack.empty()){
					if(stack.top() > -1){
						malformed = true;
						break;
					}
				}
				stack.push(LEFT_SHIFT);
			}
			else if(buff == '>'){
				if(!stack.empty()){
					if(stack.top() > -1){
						malformed = true;
						break;
					}
				}
				stack.push(RIGHT_SHIFT);
			}
			else if(buff - '0' >= 0 && buff - '0' < 10){
				if (!stack.empty()) {
					if(stack.top() > -1){
						malformed = true;
						break;
					}
				}
				string temp ="";
				temp += buff;
				while(ss.peek() - '0' >= 0 && ss.peek() - '0' < 10){
					ss >> buff;
					temp += buff;
				}
				stack.push(stoi(temp));
			}
			else if(buff == ')'){
				close_paren = true;
				if(stack.empty()){
					malformed = true;
					break;
				}
				else {
					if(stack.top() < 0){
						malformed = true;
						break;
					}
				}
				while(stack.top() != OPEN_PAREN){
					if(stack.empty()){
						malformed = true;
						break;
					}
					if (!add && !multiply){
						if(stack.top() > OPEN_PAREN)
							int1 = stack.top();
						else{
							if(stack.top() == LEFT_SHIFT)
								int1 *= 2;
							else if(stack.top() == RIGHT_SHIFT)
								int1 /= 2;
							else if(stack.top() == ADD)
								add = true;
							else if (stack.top() == MULTIPLY)
								multiply = true;
						}
						stack.pop();
					}
					else {
						if((add && prevOperation == "m") || (multiply && prevOperation == "a")){
							malformed = true;
							break;
						}
						if(stack.top() > OPEN_PAREN)
							int2 = stack.top();
						else{
							if(stack.top() == LEFT_SHIFT)
								int2 *= 2;
							else if(stack.top() == RIGHT_SHIFT)
								int2 /= 2;
							else{
								add_or_multiply(add, multiply, int1, int2, final, prevOperation);
								stack.push(final);
								reset_vals(add, multiply, int1, int2);
							}
						}
						if(add || multiply)
							stack.pop();
					}
				}
				stack.pop();
				add_or_multiply(add, multiply, int1, int2, final, prevOperation);
				stack.push(final);
				if(!add && !multiply){
					malformed = true;
					break;
				}
				reset_vals(add, multiply, int1, int2);
			}
			else {
				malformed = true;
				break;
			}
		}
		if(!stack.empty()){
			while(!stack.empty()){
				if(final && stack.top() == LEFT_SHIFT)
					final *= 2;
				else if(final && stack.top() == RIGHT_SHIFT)
					final /= 2;
				else if(stack.top() == OPEN_PAREN)
					malformed = true;
				else if(!close_paren && stack.top() > OPEN_PAREN)
					final = stack.top();
				else if(!close_paren && stack.top() < 0)
					malformed = true;
				stack.pop();
			}			
		}
		if(!malformed){ 
			if(s != "")
				cout << final << endl;
		}
		else cout << "Malformed"<<endl;
	}
}

void reset_vals(bool &add, bool &multiply, int& int1, int& int2){
	add = false;
	multiply = false;
	int1 = 0;
	int2 = 0;
}

void add_or_multiply(bool &add, bool &multiply, int& int1, int& int2, int& final, string& prev){
	if(add){
		int1 += int2;
		prev = "a";
	}

	else if(multiply){
		int1 *= int2;
		prev = "m";
	}

	final = int1;
}