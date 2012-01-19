#ifndef NATIVE_H
#define NATIVE_H

#define Pi 3.14159265358979323846

class native
{
	int n;
	double lenght;
	double h;
	double eps;
	double lm;
	double l_theor;
	double *u,*e,u_res;
	double mod1;
	double mod2;
public:
	native(int n,double eps,double lenght,double l_theor);
	double scal(double *f1,double *f2,int n);
	double A(double *f,int i,double h,int n);
	double diff(double *f1,double *f2,double h,int n);
	double norm(double *f1,double h,int n);
	double find();
	double get_mod1();
	double get_mod2();
	~native();
};

#endif