#include "Gauss.h"

vector<double> Gauss::gaussMethod(vector<vector<double>> A, vector<double> B) {
	double d,s;
	vector<double> x(B.size());
	vector<double> tmp = A[0];
	int n = A.size();
	for (int k = 0; k < tmp.size(); k++) { // прямой ход
		for (int j = k + 1; j < tmp.size(); j++) {
			d = A[j][k] / A[k][k];
			for (int i = k; i < tmp.size(); i++) {
				A[j][i] = A[j][i] - d * A[k][i];
			}
			B[j] = B[j] - d*B[k];
		}
	}
	for (int k = tmp.size() - 1; k >= 0; k--) { // обратный ход
		d = 0;
		for (int j = k + 1; j <= tmp.size() - 1; j++) {
			s = A[k][j] * x[j];
			d = d + s;
		}
		x[k] = (B[k] - d) / A[k][k];
	}
	return x;
}