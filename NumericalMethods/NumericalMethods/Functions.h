#pragma once
#include <vector>
#include <iostream>
#include <random>
#include <chrono>
using namespace std;
//namespace Functions{
	class Functions {
	public:
		static double determinant(vector<vector<double>>A);
		static double vectorNorm(vector<double> a);
		static vector<vector<double>> createSpecialMatrix(vector<vector<double>> A, vector<double> B, int pos);
		static bool checkForDiagonal(vector<vector<double>> A);
		static pair<vector<vector<double>>, vector<double>> makeDiagonal(vector<vector<double>> A, vector<double> B);
		static bool stopIter(vector<double> a, vector<double>b);
		static void printResult(vector<double> x);
		static pair<vector<vector<double>>, vector<double>> generateRandomMatrix(int n);
		};
//}
