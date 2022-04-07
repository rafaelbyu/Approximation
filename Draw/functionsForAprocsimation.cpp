#include"functionsForAprocsimation.h"
#include "stdafx.h"
#include <math.h>

double f(double x)
{
	return  x * exp(-x*x);
}

double P(double x, double* C, int n)
{
	double P = 0;
	for (int i = 0; i <= n; i++)
	{
		P += C[i] * pow(x, i);
	}
	return P;
}

void Sort(double** A, double* B, int col, int n)
{
	if (col == 2)
	{
		col = col;
	}
	int swap = col;
	double A_;
	for (int i = col; i < n; i++)
	{
		if (abs(A[swap][col]) < abs(A[i + 1][col])) swap = i + 1;
	}
	for (int i = 0; i <= n; i++)
	{
		A_ = A[col][i];
		A[col][i] = A[swap][i];
		A[swap][i] = A_;
	}
	A_ = B[col];
	B[col] = B[swap];
	B[swap] = A_;
}

void Gauss(double** A, double* C, double* B, int n, int m)
{
	int n1 = n;
	double A_;
	for (int k = 0; k < n1; k++)
	{
		Sort(A, B, k, n1);
		//	Print(A, B, n1);
		for (int i = n1 - n; i < n1; i++)
		{
			if (A[i + 1][k] != 0)
			{
				A_ = A[i + 1][k];
				for (int j = n1 - n; j <= n1; j++)
				{
					//if (A[i+1][k]!=0)A[i][j] = A[i][j] - A[i + 1][j] * A[i][k] / A[i + 1][k]	
					A[i + 1][j] -= A[k][j] * A_ / A[k][k];
				}
				B[i + 1] -= B[k] * A_ / A[k][k];
			}
		}
		//	Print(A, B, n1);
		n--;
	}
	for (int i = n1; i >= 0; i--)
	{
		C[i] = B[i];
		for (int j = i + 1; j <= n1; j++)
		{
			C[i] -= A[i][j] * C[j];
		}
		C[i] /= A[i][i];
	}
}

double S_Calc(int n, int m, double* x, double* y, double* C)
{
	double S = 0;
	for (int i = 0; i <= m; i++)
	{
		S += (P(x[i], C, n) - y[i]) * (P(x[i], C, n) - y[i]);
	}
	return S;
}
