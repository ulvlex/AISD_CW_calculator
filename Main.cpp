#include <iostream>
#include <string>
#include "Tokens.h"

using namespace std;

string expr;

int main() {
	cout << "Enter a mathematical expression" << endl;
	cin >> expr;
	token token;
	if (token.create_tokens(expr) == true) {
		if (token.balancing_brackets(expr) == true) {
			cout << "Prefix form of the record: " << endl;
			token.prefix_form();
			cout << token.out_res();
			cout << " = " << token.answer() << endl;
		}
		else {
			cout << "Check the placement of brackets" << endl;
		}
	}
	else cout << "Error";

	return 0;
}