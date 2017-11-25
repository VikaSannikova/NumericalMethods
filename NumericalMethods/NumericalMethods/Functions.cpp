#include "Functions.h"
#define EPS 0.00001
//using namespace Functions;

	double Functions::determinant(vector<vector<double>>A) {
		double result = 1;
		double d = 0;
		vector<double> vect = A[0];
		for (int k = 0; k < vect.size(); k++) { // прямой ход
			for (int j = k + 1; j < vect.size(); j++) {
				d = A[j][k] / A[k][k]; // формула (1)
				for (int i = k; i < A.size(); i++) {
					A[j][i] = A[j][i] - d * A[k][i]; // формула (2)
				}
			}
		}
		for (int i = 0; i < A.size(); i++) {
			double tmp = A[i][i];
			result *= tmp;
		}
		return result;
	}

	double Functions::vectorNorm(vector<double> a) {
		double norm = 0;
		for (int i = 0; i < a.size(); i++) {
			double tmp = abs(a[i]);
			if (norm < tmp) {
				norm = tmp;
			}
		}
		return norm;
	}

	vector<vector<double>> Functions::createSpecialMatrix(vector<vector<double>> A, vector<double> B, int pos) {
		for (int i = 0; i < B.size(); i++) {
			A[i][pos] = B[i];
		}
		return A;
	}

	bool Functions::checkForDiagonal(vector<vector<double>> A) { //возможна хуета
		double norm = 0;
		for (int i = 0; i < A.size(); i++) {
			for (int j = 0; j < A[0].size(); j++) {
				norm += A[i][j] * A[i][j];
			}
		}
		if (sqrt(norm) < 1)
			return true;
		else
			return false;
	}

	pair<vector<vector<double>>, vector<double>> Functions::makeDiagonal(vector<vector<double>> A, vector<double> B) {
		//забить и сделать в итерациях?
		double d;
		pair<vector<vector<double>>, vector<double>> result;
		for (int k = 0; k < A.size(); k++) {
			for (int j = k + 1; j < A[0].size(); j++) {
				d = A[j][k] / A[k][k];
				for (int i = k; i < A.size(); i++) {
					A[j][i] = A[j][i] - d * A[k][i];
				}
				B[j] = B[j] - d*B[k];
			}
		}
		for (int k = A.size() - 1; k >= 0; k--) { // need >=    ?
			for (int j = k - 1; j >= 0; j--) {
				d = A[j][k] / A[k][k];
				for (int i = k; i >= 0; i--) {
					A[j][i] = A[j][i] - d*A[k][i];
				}
				B[j] = B[j] - d*B[k];
			}
		}
		result.first = A;
		result.second = B;
		return result;
	}

	bool Functions::stopIter(vector<double> a, vector<double>b) {
		vector<double>c = a;
		for (int i = 0; i < a.size(); i++) {
			c[i] -= b[i];
		}
		if (Functions::vectorNorm(c) < EPS) {
			return false;
		}
		else {
			return true;
		}
	}

	void Functions::printResult(vector<double> x) {
		cout << "Result: " << endl;
		for (int i = 0; i < x.size(); i++)
			cout << "x[" << i << "]=" << x[i] << " " << endl;
	}

	pair<vector<vector<double>>, vector<double>> Functions::generateRandomMatrix(int n) {

		pair<vector<vector<double>>, vector<double>> pair;
		default_random_engine generator(time(0));
		uniform_real_distribution <double> distribution(-10, 10);
		vector<vector<double>> A(n,vector<double>(n));
		vector<double> B(n);
		double tmp;
		int k=0;
		for (int i = 0; i < n; i++) {
			double max = A[i][0];
			for (int j = 0; j < n; j++) {
				tmp = distribution(generator);
				if (j < i) {
					A[i][j] = tmp;
					if (abs(tmp) > abs(max)) {
						max = tmp;
						k = j;
					}
				}
				else if(i==j){
					if (abs(tmp) < abs(max)) {
						A[i][k] = tmp;
						//max = tmp;
						A[i][i] = max;						
					}
					else {
						max = tmp;
						A[i][i] = tmp; //max
					}
				}
				else {
					if (abs(tmp) > abs(max)) {
						A[i][j] = A[i][i];
						max = tmp;	
						A[i][i] = max;
					}
					else {
						A[i][j] = tmp;
					}
				}
			}
			tmp = distribution(generator);
			B[i] = tmp;
		}
		pair.first = A;
		pair.second = B;
		return pair;
	}
