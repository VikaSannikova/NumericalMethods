#pragma once
#include <vector>
#include <iostream>
#include "Functions.h"
using namespace std;

class Zeidel {
public:
	static vector<double> zeidelMethod(vector<vector<double>> A, vector<double> B);
};