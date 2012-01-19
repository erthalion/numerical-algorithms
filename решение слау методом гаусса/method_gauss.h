#ifndef METHOD_GAUSS_H
#define MEHTOD_GAUSS_H

#include "stdafx.h"

class method_gauss:public method
{
public:
	method_gauss(double **A_in,double *u_in,int n_in);
	method_gauss(double **A_in,double *f_in,int n_in,int mode);
	double* get_solve(int number);	//1-основное решение,2-по столбцам,3-по строкам,4-по минорам?
	void exclude(int k);	//исключить переменную начиная с k+1
	double* reverse();	//обратный ход
	double** get_reverse_matr();
	void output();
	void out_vect();
//	~method_gauss();
};

#endif