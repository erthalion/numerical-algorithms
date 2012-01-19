#ifndef ITER_H
#define ITER_H

#define Pi 3.1415
#define f(i,j,h) 2*pow(Pi,2)*sin(Pi*(i*h))*sin(Pi*(j*h))
#define u_orig(i,j,h) sin(Pi*(i*h))*sin(Pi*(j*h))


class iter
{
int node;
double lenght;
double epsilon;
double tau;
double gamma1,gamma2;
double **u;
double **r;
double **differense;
double step;
double norm,norm_0;
int count_step;
public:
	iter(int n,double l,double eps);
	void print(double **matr,int n);
	double scal_prod(double **a,double **b,int n,double h);
	double solve();
	double solve_simple_tau();
	int get_count();
	int get_theor_count();

	void output_u();
	void output_n();
	~iter();
};


#endif 