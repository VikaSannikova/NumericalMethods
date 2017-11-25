#pragma once
#include "Functions.h"
#include <iostream>
#include <vector>
using namespace std;

class Relax {
public:
	static vector<double> upRelaxMetod(vector<vector<double>> A, vector <double> B, double relaxCoeff);
};
