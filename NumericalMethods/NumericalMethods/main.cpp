#include <iostream>
#include <stdio.h>
#include <math.h>
#include <vector>
using namespace std;
int n, i, j, k;
double d, s;

vector<double> gauss(vector<vector<double>> A, vector<double> B) {
	vector<double> x(B.size());
	vector<double> tmp = A[0];
	for (int k = 0; k < tmp.size(); k++){ // прямой ход
		for (int j = k + 1; j < tmp.size(); j++){
			d = A[j][k] / A[k][k]; // формула (1)
			for (i = k; i < tmp.size(); i++){
				A[j][i] = A[j][i] - d * A[k][i]; // формула (2)
			}
			B[j] = B[j] - d*B[k]; // формула (3)
		}
	}

	for (k = tmp.size()-1; k >= 0; k--){ // обратный ход
		d = 0;
		for (j = k + 1; j <= tmp.size()-1; j++){
			s = A[k][j] * x[j]; // формула (4)
			d = d + s; // формула (4)
		}
		x[k] = (B[k] - d) / A[k][k]; // формула (4)
	}
	return x;

}

double determinant(vector<vector<double>>A) {
	double result=1;
	double d =0;
	vector<double> vect = A[0];
	for (int k = 0; k < vect.size(); k++) { // прямой ход
		for (int j = k + 1; j < vect.size(); j++) {
			d = A[j][k] / A[k][k]; // формула (1)
			for (i = k; i < n; i++) {
				A[j][i] = A[j][i] - d * A[k][i]; // формула (2)
			}
		}
	}
	for(int i = 0; i < n; i++) {
		double tmp = A[i][i];
		result *= tmp;
	}
	return result;
}

vector<vector<double>> createSpecialMatrix(vector<vector<double>> A, vector<double> B, int pos) {
	for (int i = 0; i < B.size(); i++) {
		A[i][pos] = B[i];
	}
	return A;
}

vector<double> cramer(vector<vector<double>> A, vector<double> B) {
	vector<double> result(B.size());
	vector<double> coefdet(B.size());
	double det = determinant(A);
	for (int i = 0; i < n; i++) {
		coefdet[i] = determinant(createSpecialMatrix(A, B, i));
		result[i] = coefdet[i] / det;
	}
	return result;
}

void printResult(vector<double> x) {
	//int n = sizeof(x) / sizeof(double);
	cout << "Result: " << endl;
	for (i = 0; i < x.size(); i++)
		cout << "x[" << i << "]=" << x[i] << " " << endl;
}

int main()
{
	cout << "Size: " << endl;
	cin >> n;
	size_t row=n;
	size_t col=n;
	vector<vector<double>> A (row,vector<double>(col));
	vector<double> B(row);
	vector<double> resultGauss(row);
	vector<double> resultCramer(row);
	//double **a = new double *[n];
	//for (i = 0; i <= n; i++)
	//	a[i] = new double[n];

	//double *b = new double[n];
	//double *resultGauss = new double[n];
	//double *resultCramer = new double[n];
	cout << "Enter coeffs: " << endl;
	for (i = 0; i < n; i++){
		for (j = 0; j < n; j++){
			cout << "A[ " << i << "," << j << "]= ";
			cin >> A[i][j];
			//a1[i][j] = a[i][j];
		}
		cout << "b[ " << i << "]= ";
		cin >> B[i];
	}

	//vector<double> vect = A[0];
	//cout << vect.size()<<endl;
	cout << "Determinant: " << determinant(A)<<endl;
	resultGauss = gauss(A,B);
	printResult(resultGauss);
	resultCramer = cramer(A,B);
	printResult(resultCramer);
	system("pause");
	return 0;
}