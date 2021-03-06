#ifndef ITER_H
#define ITER_H

class iter:public method
{
protected:
	double delta_min,delta_max;
	double tau;
	double epsilon;
	double step;
	double *r;
	double n0;	//���� ���������� ��������
	double *wn;
	double *w0;
	int val_step;
public:
	iter(double **A,double *u,int n,double eps);
	double* get_solve();
	void set_approx_null();	//��� ����������� �������
	void set_approx_near();	//��� ����������� ������� � ������� �������
	void set_approx_rand();	//��� ������� ������������
//	double* error(int n);	//���������� ������� ����������� n �������
	double get_delta_min();
	double get_delta_max();
	double get_norm(double *r,int n);
	double get_wn();
	double get_w0();
	double get_tau();
	int get_count_step();
	~iter();
};

#endif