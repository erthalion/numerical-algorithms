#ifndef METHOD_GAUSS_H
#define MEHTOD_GAUSS_H

#include "stdafx.h"

class method_gauss:public method
{
public:
	method_gauss(double **A_in,double *u_in,int n_in);
	method_gauss(double **A_in,double *f_in,int n_in,int mode);
	double* get_solve(int number);	//1-�������� �������,2-�� ��������,3-�� �������,4-�� �������?
	void exclude(int k);	//��������� ���������� ������� � k+1
	double* reverse();	//�������� ���
	double** get_reverse_matr();
	void output();
	void out_vect();
//	~method_gauss();
};

#endif