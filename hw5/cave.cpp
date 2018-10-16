#include <iostream>
#include <stack>
#include <string>
#include <cstddef>

using namespace std;

int main(int argc, char const *argv[])
{
	cout << "Please enter an input to read" << endl;
	string input;
	getline(cin, input);
	if(input.find_first_not_of("nesw")!= string::npos){
		cout << "invalid" << endl;
		return 1;
	}
	stack<char> rope;
	for(int i = 0; i < input.length(); i++){
		if(rope.empty())
			rope.push(input[i]);
		else{
			if(input[i] == 'n'){
				if(rope.top() == 's')
					rope.pop();
				else
					rope.push('n');
			}
			else if(input[i] == 's'){
				if(rope.top() == 'n')
					rope.pop();
				else
					rope.push('s');
			}
			else if(input[i] == 'e'){
				if(rope.top() == 'w')
					rope.pop();
				else
					rope.push('e');
			}
			else if(input[i] == 'w'){
				if(rope.top() == 'e')
					rope.pop();
				else
					rope.push('w');
			}
		}
	}
	if(!rope.empty())
		cout << "not rolled up" << endl;
	else 
		cout << "rolled up" << endl;

	return 0;
}