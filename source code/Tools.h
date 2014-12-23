#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <float.h>
#include <cmath>
#include <algorithm>

using namespace std;

const double eps = 1e-14;
const double tolerance = 1e-12;
const double error = 1e-7;
//Bisection range
const double iterations = 100;
const double high = 1e18;
const double low = -1e18;

class Tools {
public:
	static int Precedence(string);
	static long long GCD(long long, long long);
	static pair<long long, long long> GetFraction(double);
	static void Unique(vector<double>*);
};
