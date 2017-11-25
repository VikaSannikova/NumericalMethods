#include "Cramer.h"
#include "Functions.h"

vector<double> Cramer::cramerMethod(vector<vector<double>> A, vector<double> B) {
	vector<double> result(B.size());
	vector<double> coefdet(B.size());
	double det = Functions::determinant(A);
	for (int i = 0; i < A.size(); i++) {
		coefdet[i] = Functions::determinant(Functions::createSpecialMatrix(A, B, i));
		result[i] = coefdet[i] / det;
	}
	return result;
}