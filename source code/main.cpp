#include "LinearPolynomialEquation.h"
#include "NonLinearPolynomialEquation.h"


int main() {
	// testing 7aga
	while(true) {
		cout << "(1) Linear polynomial equation\n(2) Non-linear polynomial equation\n";
		int choice;
		cin >> choice;
		Equation *E;
		if(choice == 1) {
			cout << "Insert the equation in any form: ";
			string eq;
			cin.ignore();
			getline(cin, eq);
			E = new LinearPolynomialEquation(eq);
			E->PrintResult();
		}
		else {
			cout << "Insert the coefficients in order:\n" << "x^0   x^1   x^2   x^3   x^4\n";
			double arr[5];
			for(int i = 0; i < 5; i++) cin >> arr[i];
			E = new NonLinearPolynomialEquation(arr);
			E->PrintResult();
		}
		cout << endl << endl;
	}
	return 0;
}