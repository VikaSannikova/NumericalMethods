#include "ProstIterations.h"
#include "Functions.h"
//#include <vector>

vector<double> ProstIteration::prostIterMethod(vector<vector<double>> A, vector<double> B) { //будут нужны условия ждля проверки сходимости
	pair<vector<vector<double>>, vector<double>> pair;
	vector<double> result(A.size());
	result.assign(result.size(), 0);

	vector<vector<double>> _A = A;
	vector<double> _B = B;

	for (int i = 0; i < A.size(); i++) {
		if (A[i][i] != 0) { //вот здесь проверка на 0 и умножение на -1
			for (int j = 0; j < A.size(); j++) {
				if (i != j) {
					A[i][j] /= -A[i][i];
				}
			}
			B[i] /= A[i][i];
			A[i][i] = 0;
		}
	}

	vector<double> cur_iter(A.size());
	vector<double> prev_iter(A.size());
	if (Functions::checkForDiagonal(A)) {
		prev_iter.assign(prev_iter.size(), 0);
		for (int i = 0; i < A.size(); i++) {
			for (int j = 0; j < A.size(); j++) {
				cur_iter[i] = +A[i][j] * prev_iter[i];
			}
			cur_iter[i] += B[i];
		}
		while (Functions::stopIter(cur_iter, prev_iter)) {
			prev_iter = cur_iter;
			cur_iter.assign(A.size(), 0);
			for (int i = 0; i < A.size(); i++) {
				for (int j = 0; j < A.size(); j++) {
					cur_iter[i] += A[i][j] * prev_iter[j];
				}
				cur_iter[i] += B[i];
			}

		}
		result = cur_iter;
	}
	else {
		cout << "this matrix is not diagonal predominance" << endl;
		pair = Functions::makeDiagonal(_A, _B);
		for (int i = 0; i < pair.first.size(); i++) {
			result[i] = pair.second[i] / pair.first[i][i];
		}
	}
	//result = cur_iter;
	return result;
}