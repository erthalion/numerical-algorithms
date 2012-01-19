#ifndef ITER_H
#define ITER_H

#define Pi 3.14159265358979323846
#define f(i,j,h) 2*Pi*Pi*cos(Pi*(i*h))*cos(Pi*(j*h))//2*Pi*Pi*sin(Pi*(i*h))*sin(Pi*(j*h))//
#define u_orig(i,j,h) sin(Pi*(i*h))*sin(Pi*(j*h))
#define free_f(i,j,h) i*i*h*h+j*j*h*h//cos(Pi*(i*h))*cos(Pi*(j*h))//i*h+j*h//cos(Pi*(i*h))*cos(Pi*(j*h))//sin(Pi*(i*h))*sin(Pi*(j*h))//



class iter
{
int n;
double lenght;
double eps;
double tau;
double gamma1,gamma2;
double **u;
double **r;
double **differense;
double step;
double mod;
double norm,norm_0;
int count_step;
public:
	iter(int n,double l,double eps);
	void print(double **matr,int n);
	double scal_prod(double **a,double **b,int n,double h);
	double solve();
	int get_count();
	int get_theor_count();
	double A(double** u,int i,int j);

	void output_u();
	void output_n();
	double get_mod();
	double get_n();
	double get_n0();
	~iter();
};


#endif 