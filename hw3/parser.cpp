#include "stackint.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

void parser(ifstream &ifile);

void reset_vals(bool &add, bool &multiply, int& int1, int& int2);

void add_or_multiply(bool &add, bool &multiply, int& int1, 
	int& int2, int& final, string& prev);

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
	//set characters to negative values to pass into stack
	const int OPEN_PAREN = -1, LEFT_SHIFT = -2, RIGHT_SHIFT = -3,
	 ADD = -4, MULTIPLY = -5;
	StackInt stack;
	string s;
	//loop through file and evaluate each line
	while(getline(ifile,s)){
		char buff;
		//int1 and int2 will hold values that will be added and multiplied. 
		//final holds the final val
		int final = 0, int1 = 0, int2 = 0;
		//checks to see if operations are used, if the string is malformed, 
		//or if a close paren was seen
		bool add = false, multiply = false, malformed = false, close_paren = false;
		stringstream ss(s);
		while(ss >> buff){
			//holds prev operation if avaliable
			string prevOperation = "";
			//push '('' if the previous character is not a number
			if(buff == '('){
				if(!stack.empty()){
					if(stack.top() > -1){
						malformed = true;
						break;
					}
				}
				stack.push(OPEN_PAREN);
			}
			//push '*' or '+' when stack is not empty and prev char is a number
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
			//push '<' or '>' if prev character is not '(' of a number
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
			//push a number if the stack isn't empty
			else if(buff - '0' >= 0 && buff - '0' < 10){
				if (!stack.empty()) {
					if(stack.top() > -1){
						malformed = true;
						break;
					}
				}
				string temp ="";
				temp += buff;
				//push the entire number not each digit
				while(ss.peek() - '0' >= 0 && ss.peek() - '0' < 10){
					ss >> buff;
					temp += buff;
				}
				stack.push(stoi(temp));
			}
			else if(buff == ')'){
				close_paren = true;
				//checks to see malformed cases. if empty stack or if a non-number
				//is before the ')'
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
				//opop from stack until you hit '('
				while(stack.top() != OPEN_PAREN){
					//if you pop the whole stack without stopping, its malformed
					if(stack.empty()){
						malformed = true;
						break;
					}
					//check to see if operations are present and set add and multiply to
					//desired values
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
						//check to see if there is a mix of add and multiply in the
						//same expression
						if((add && prevOperation == "m") || 
							(multiply && prevOperation == "a")){
							malformed = true;
							break;
						}
						//add or multiply int1 and int2 
						if(stack.top() > OPEN_PAREN)
							int2 = stack.top();
						else{
							if(stack.top() == LEFT_SHIFT)
								int2 *= 2;
							else if(stack.top() == RIGHT_SHIFT)
								int2 /= 2;
							else{
								add_or_multiply(add, multiply, int1, 
									int2, final, prevOperation);
								stack.push(final);
								reset_vals(add, multiply, int1, int2);
							}
						}
						if(add || multiply)
							stack.pop();
					}
				}
				//stack top is '('. pop it and if need be, add or multiply int1 and int2
				//set final and push it to the stack. If no operations were present, string is malformed
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
		//if there is leftover characters or if no ')' was present, evaluate the expression
		if(!stack.empty()){
			while(!stack.empty()){
				if(final && stack.top() == LEFT_SHIFT)
					final *= 2;
				else if(final && stack.top() == RIGHT_SHIFT)
					final /= 2;
				else if(stack.top() == OPEN_PAREN)
					malformed = true;
				else if(!close_paren && stack.top() > OPEN_PAREN){
					final = stack.top();
					close_paren = true;
				}
				else if(!close_paren && stack.top() < 0 && !final)
					malformed = true;
				else if (stack.top() == ADD || stack.top() == MULTIPLY)
					malformed = true;
				stack.pop();
			}			
		}
		//print value of expression if not malformed and if line wasn't whitespace
		if(!malformed){ 
			if(s.find_first_not_of(' ') != std::string::npos && 
				s.find_first_not_of('\t') != std::string::npos)
				cout << final << endl;
		}
		else cout << "Malformed"<<endl;
	}
}
//resets the bools and ints used to evaluate the expression
void reset_vals(bool &add, bool &multiply, int& int1, int& int2){
	add = false;
	multiply = false;
	int1 = 0;
	int2 = 0;
}
//adds or multiplies int 1 and int2 then sets final to their sum/product
void add_or_multiply(bool &add, bool &multiply, int& int1, 
	int& int2, int& final, string& prev){
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