#include "Equation.h"

Equation::Equation() { }

Equation::Equation(string equation) : infix(equation), foundSolution(false) {
	ParseInfix();
	GetPrefix();
	Bisection();
	GetFraction();
}

Equation::Equation(double co[]) : foundSolution(false) {
	solutionSet.clear();
	for(int i = 0; i < 7; i++) coefficients[i] = co[i];
	if(!coefficients[0]) solutionSet.push_back(0);
	FirstDerivative();
	for(int i = -10000; i <= 10000; i++) NewtonMethod(i, 100);
	//GetFraction();
	Unique();
	cout << "------> x = ";
	for(int i = 0; i < solutionSet.size(); i++) cout << solutionSet[i] << ", ";
}

Equation::~Equation() { }

void Equation::FirstDerivative() {
	for(int i = 1; i < 7; i++) firstDerivativeCoefficients[i - 1] = coefficients[i] * i;
}

void Equation::PrintResult() {
	cout << result;
	if(denominator != 1) cout << ' ' << numerator << '/' << denominator;
	cout << endl;
}

long long Equation::GCD(long long a, long long b) {
	if(!b) return a;
	return GCD(b, a % b);
}

void Equation::GetFraction() {
	double error = 1e-6;
	int n1 = 1; int n2 = 0;
	int d1 = 0; int d2 = 1;
	double b = result;

	do {
		double a = floor(b);
		int temp = n1; 
		n1 = a*n1 + n2; 
		n2 = temp;
		temp = d1; 
		d1 = a*d1 + d2;
		d2 = temp;
		b = 1 / (b - a);
	} while(abs(result - n1*1.0 / d1) > error);
	
	numerator = n1;
	denominator = d1;
}


void Equation::ParseInfix() {
	// Remove all spaces
	string tempInfix;
	for(int i = 0; i < infix.size(); i++) {
		if(infix[i] != ' ') tempInfix += infix[i];
	}
	infix = tempInfix;

	// Converting -x -> (0-1)x
	if(infix[0] == '-') {
		if(isdigit(infix[1])) infix.insert(0, 1, '0');
		else {
			infix.insert(1, "1)");
			infix.insert(0, "(0");
		}
	}

	// Converting 2x -> 2*x or 2(x+2) -> 2*(x+2) or (2+5)x -> (2+5)*x
	for(int i = 0; i < infix.size() - 1; i++) {
		if(infix[i] == '=' && infix[i + 1] == '-') {
			if(isdigit(infix[i + 2])) infix.insert(i + 1, 1, '0');
			else {
				infix.insert(i + 2, "1)");
				infix.insert(i + 1, "(0");
			}
		}

		if(((isdigit(infix[i]) || infix[i] == ')') && isalpha(infix[i + 1])) || (isdigit(infix[i]) && infix[i + 1] == '(')) {
			infix.insert(i + 1, 1, '*');
		}
	}

	// Parsing
	string token;
	for(int i = 0; i < infix.size(); i++) {
		if(!isdigit(infix[i])) token += infix[i];
		while(i < infix.size() && (isdigit(infix[i]) || infix[i]=='.')) {
			token += infix[i];
			i++;
		}

		parsedInfix.push_back(token);
		if(isdigit(token[0])) i--;
		token = "";
	}

	if(!token.empty()) parsedInfix.push_back(token);
}

int Equation::Precedence(string Operator) {
	if(Operator == "=") return 0;
	if(Operator == "(") return 1;
	if(Operator == "+" || Operator == "-") return 2;
	if(Operator == "*" || Operator == "/") return 3;
}

void Equation::GetPrefix() {
	stack<string> tokens, operators;
	for(int i = 0; i < parsedInfix.size(); i++) {
		if(isdigit(parsedInfix[i][0]) || isalpha(parsedInfix[i][0])) {
			tokens.push(parsedInfix[i]);
		}
		else if(parsedInfix[i][0] == '(') {
			operators.push(parsedInfix[i]);
		}
		else if(parsedInfix[i][0] == ')') {
			while(!operators.empty() && operators.top() != "(") {
				tokens.push(operators.top());
				operators.pop();
			}
			operators.pop();
		}
		else {
			while(!operators.empty() && Precedence(operators.top()) >= Precedence(parsedInfix[i])) {
				tokens.push(operators.top());
				operators.pop();
			}
			operators.push(parsedInfix[i]);
		}
	}

	while(!operators.empty()) {
		tokens.push(operators.top());
		operators.pop();
	}

	while(!tokens.empty()) {
		prefix.push_back(tokens.top());
		tokens.pop();
	}
}

void Equation::Bisection() {
	PrefixTree prefixTree;
	prefixTree.BuildTree(&prefix);
	double low = -1e17, high = 1e17, mid;
	
	while(high > low + BSeps) {
		mid = (high + low) / 2;
		prefixTree.SetUnknownValue(mid);
		Relation relationForMid = prefixTree.SolveTree();
		prefixTree.SetUnknownValue(high);
		Relation relationForHigh = prefixTree.SolveTree();

		if(relationForMid == Equal) {
			result = mid;
			return;
		}

		if(relationForMid == relationForHigh) high = mid;
		else low = mid;
	}
}

double Equation::substituteInEquation(double X) {
	double value = 0;
	for(int i = 0; i < 7; i++) value += coefficients[i] * pow(X, i);
	return value;
}

double Equation::substituteInDerivative(double X) {
	double value = 0;
	for(int i = 0; i < 7; i++) value += firstDerivativeCoefficients[i] * pow(X, i);
	return value;
}

void Equation::NewtonMethod(double X, int iterations) {
	if(iterations==0) return;
	double y = substituteInEquation(X), yDash = substituteInDerivative(X);
	if(abs(yDash) < tolerance) return;
	double newX = X - (y / yDash);
	if(abs(newX - X) / abs(newX) < tolerance) {
		solutionSet.push_back(newX);
		return;
	}
	NewtonMethod(newX, iterations-1);
}

void Equation::Unique() {
	sort(solutionSet.begin(), solutionSet.end());
	for(int i = 0; i < solutionSet.size(); i++) {
		for(int j = 0; j < i; j++) {
			if(abs(solutionSet[i] - solutionSet[j]) < 1e-9) {
				solutionSet.erase(solutionSet.begin() + i, solutionSet.begin() + i + 1);
				i--;
				break;
			}
		}
	}

}