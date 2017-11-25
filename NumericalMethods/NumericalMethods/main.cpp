#include <iostream>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <chrono>
#include <iostream>
#include "math.h"
#include "Functions.h"
#include "Gauss.h"
#include "Cramer.h"
#include "ProstIterations.h"
#include "Zeidel.h"
#include "Relax.h"

#define EPS 0.001

using namespace std;
int main()
{
	cout << "Size: " << endl;
	//int n = 3;
	int n;
	cin >> n;
	size_t row=n;
	size_t col=n;
	
	pair<vector<vector<double>>, vector<double>> pair;
	vector<vector<double>> A (row,vector<double>(col));
	vector<double> B(row);
	vector<double> resultGauss(row);
	vector<double> resultCramer(row);
	vector<double> resultIiter(row);
	vector<double> resultZeidel(row);
	vector<double> resultRelax(row);

	/*pair = Functions::generateRandomMatrix(n);
	A = pair.first;
	B = pair.second;*/

	//cout << A.size();
	int input;
	cout << "How you want to input matrix? (1 - manual || 0-random generate)" << endl;
	cin >> input;
	if (input == 1) {
		cout << "Enter coeffs: " << endl;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				cout << "A[ " << i << "," << j << "]= ";
				cin >> A[i][j];
			}
			cout << "b[ " << i << "]= ";
			cin >> B[i];
		}
	}else if (input == 0) {
		pair = Functions::generateRandomMatrix(n);
		A = pair.first;
		B = pair.second;
	}

	/*cout << "Enter coeffs: " << endl;
	for (i = 0; i < n; i++){
		for (j = 0; j < n; j++){
			cout << "A[ " << i << "," << j << "]= ";
			cin >> A[i][j];
		}
		cout << "b[ " << i << "]= ";
		cin >> B[i];
	}*/

	//A = { {5, -4, -1} ,
	//	    {4, 1, -2 }, 
	//	    {3, 1, -5}};
	//B = {-2, 8, 10};

	//A = { {4, 1, -2} ,
	//	  {1, -5, 1 }, 
	//	  {3, 1, -5}};
	//B = {8, -10, 10};

	//A = { {13.2, 1.9, 2.3} ,
	//	  {0.8, -7.3, -0.7}, 
	//	  {0.5, -1.4, -9.6}};
	//B = { 5.12, 5.2, 1.5};

	/*A = { {6.25, -1, 0.5} ,
		  {-1, 5, 2.12}, 
		  {0.5, 2.12, 3.6}};
	B = { 7.5, -8.68, -0.24};*/

	cout << "Determinant: " << Functions::determinant(A)<<endl;

	cout << "**********************************"<<endl;
	Gauss *gauss = new Gauss();
	auto start_time = std::chrono::steady_clock::now();
	resultGauss = gauss->gaussMethod(A,B);
	auto finish_time = std::chrono::steady_clock::now();
	Functions::printResult(resultGauss);
	auto result_time = std::chrono::duration_cast<std::chrono::microseconds>(finish_time - start_time);
	cout << endl <<"Gauss time: " << result_time.count()<<endl;
	
	cout << "*********************************"<<endl;

	Cramer *cramer = new Cramer;
	start_time = std::chrono::steady_clock::now();
	resultCramer = cramer->cramerMethod(A, B);
	finish_time = std::chrono::steady_clock::now();
	Functions::printResult(resultCramer);
	result_time = std::chrono::duration_cast<std::chrono::microseconds>(finish_time - start_time);
	cout << endl << "Cramer time: " << result_time.count() << endl;


	cout << "*********************************" << endl;

	ProstIteration *iter = new ProstIteration;
	start_time = std::chrono::steady_clock::now();
	resultIiter = iter->prostIterMethod(A, B);
	finish_time = std::chrono::steady_clock::now();
	Functions::printResult(resultIiter);
	result_time = std::chrono::duration_cast<std::chrono::microseconds>(finish_time - start_time);
	cout << endl << "Iter time: " << result_time.count() << endl;

	cout << "*********************************" << endl;

	Zeidel *zeidel = new Zeidel;
	start_time = std::chrono::steady_clock::now();
	resultZeidel = zeidel->zeidelMethod(A, B);
	finish_time = std::chrono::steady_clock::now();
	Functions::printResult(resultZeidel);
	result_time = std::chrono::duration_cast<std::chrono::microseconds>(finish_time - start_time);
	cout << endl << "Zeidel time: " << result_time.count() << endl;

	cout << "*********************************" << endl;

	double relax_coeff;
	cout << endl << "Enter RelaxCoeff: ";
	cin >> relax_coeff;
	cout << endl;
	Relax *relax = new Relax;
	start_time = std::chrono::steady_clock::now();
	resultRelax = relax->upRelaxMetod(A, B, relax_coeff);
	finish_time = std::chrono::steady_clock::now();
	Functions::printResult(resultRelax);
	result_time = std::chrono::duration_cast<std::chrono::microseconds>(finish_time - start_time);
	cout << endl << "Relax time: " << result_time.count() << endl;


	cout << "*********************************" << endl;

	system("pause");
	return 0;
}


