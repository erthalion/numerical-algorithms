#ifndef METHOD_H
#define METHOD_H

#include <math.h>
#include <stdio.h>

class method
{
protected:
	int n;	//�����������
	double **A;	//������� ��������
	double **Af;	//����������� �������
	double *f;	//������ ������ ����� �����
	double *u;	//������ ������� �������
	double *u_solve;	//������ ����������� �������
	int mode;	//���� ��� ����
public:
	method(double **A,double *u,int n);
	method(double **A,double *f,int n,int mode);
	virtual double* get_solve();
	double get_err();	//���������� �����������
	double get_err_revmatr(double **rev);
//	double** get_inverse();	//����� ������� ������� � A
	void reposition_column(int i,int j);	//������������ ������� ��������
	void reposition_line(int i,int j);	//������������ ������� �����
	double get_norm(double *r,int n);	//��������� ����� �������(n-�����������)
	double* get_difference(double *r1,double *r2,int n);	//�������� ��������
	double** get_mult(double **A,double **B);
	double** get_A();
	~method();
};

#endif 