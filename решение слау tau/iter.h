#ifndef ITER_H
#define ITER_H

class iter:public method
{
	double delta_min,delta_max;
	double tau;
	double epsilon;
	double step;
	double *r;
	double n0;	//���� ���������� ��������
	double *wn;
	double *w0;
public:
	iter(double **A,double *u,int n,double eps);
	double* get_solve();
	void set_approx_null();	//��� ����������� �������
	void set_approx_near();	//��� ����������� ������� � ������� �������
	void set_approx_rand();	//��� ������� ������������
//	double* error(int n);	//���������� ������� ����������� n �������
	double get_delta_min();
	double get_delta_max();
	double get_wn();
	double get_w0();
	double get_tau();
	~iter();
};

#endif