#include "stackint.h"
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;
int parseString(string str, stringstream &ss){
	char buff;
	int result = 0;
	while (ss >> buff) {
		//handle operators
		if(buff == '-'){
			if(ss.peek() == '2'){
				ss >> buff;
				cout << "left shift " << str.substr(2) << endl;
				result = parseString(str.substr(2), ss)*2;
				cout << "result * 2 "<< result <<endl;
			}
			if(ss.peek() == '3'){
				ss >> buff;
				cout << "right shift " << str.substr(2) << endl;
				result = parseString(str.substr(2), ss)/2;
				cout << "result / 2 "<< result << endl;
			}
			if(ss.peek() == '4'){
				ss >> buff;
				result += parseString(str.substr(2), ss);
				cout << "sum " << result << endl;
			}
			if(ss.peek() == '5'){
				ss >> buff;
				result *= parseString(str.substr(2), ss);
				cout << "product " << result << endl;
			}
		}
		else if(buff - '0' >= 0 && buff - '0' < 10){
			string temp ="";
			temp += buff;
			while(ss.peek() - '0' >= 0 && ss.peek() - '0' < 10){
				ss >> buff;
				temp += buff;
			}
			result = stoi(temp);
			cout << "num: "<<result << endl;
		}
	}
	// cout << "result: " << result << endl;
	return result;
}

int main(int argc, char const *argv[])
{
	if(argc < 1){
		cout << "Please choose an input file." << endl;
		return 1;
	}
	else if(argc > 2){
		cout << "Please choose one input file." << endl;
		return 1;
	}
	ifstream ifile(argv[1]);
	if(ifile.fail()){
		cout << "Couldn't open file." << endl;
		return 1;
	}
	const int OPEN_PAREN = -1;
	// const int CLOSE_PAREN = -2;
	const int LEFT_SHIFT = -2;
	const int RIGHT_SHIFT = -3;
	const int ADD = -4;
	const int MULTIPLY = -5;
	StackInt stack;
	//everything below should be bounded by another while loop that is dependent on getline
	string s;
	getline(ifile,s);
	stringstream ss(s);
	char buff;
	while(ss >> buff){
		if(buff == '('){
			stack.push(OPEN_PAREN);
		}
		else if(buff == '*'){
			stack.push(MULTIPLY);
		}
		else if(buff == '+'){
			stack.push(ADD);
		}
		else if(buff == '<'){
			stack.push(LEFT_SHIFT);
		}
		else if(buff == '>'){
			stack.push(RIGHT_SHIFT);
		}
		//Push the entire integer onto the stack
		else if(buff - '0' >= 0 && buff - '0' < 10){
			string temp ="";
			temp += buff;
			while(ss.peek() - '0' >= 0 && ss.peek() - '0' < 10){
				ss >> buff;
				temp += buff;
			}
			stack.push(stoi(temp));
		}
		else if(buff == ')'){
			string expression = "";
			int temp = 0;
			bool add = false;
			bool multiply = false;
			//pop expression within parentheses and calculate the value
			while(stack.top() != OPEN_PAREN && !stack.empty()){
				int copy = 0;
				if(stack.top() > -1){
					temp = stack.top();
				}
				else{
					if(stack.top() == -2){
						temp *= 2;
					}
					else if(stack.top() == -3){
						temp /= 2;
					}
					else if(stack.top() == -4){
						add = true;
						copy = temp;
					}
					else if (stack.top() == -5){
						multiply = true;
						copy = temp;
					}
				}
				stack.pop();
			}
			stack.pop();
			stack.push(temp);
			// stringstream ss3(expression);
			// int temp = parseString(expression, ss3);
			// cout << "expression: " << expression << endl;
			// stack.push(temp);
			// cout << "top: "<< stack.top() << endl;
		}
		cout << "top: "<< stack.top() << endl;
	}


	return 0;
}
