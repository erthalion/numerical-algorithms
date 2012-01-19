#ifndef DIFF_H
#define DIFF_H

typedef double (WINAPI *GetF)(double,double);
typedef double (WINAPI *Get_p)(int,double);
typedef double (WINAPI *Get_q)(int,double);
typedef double (WINAPI *Get_f)(int,double);

typedef double* (WINAPI *Get_alfa)();
typedef double* (WINAPI *Get_beta)();
typedef double* (WINAPI *Get_gamma)(double x1,double x2);

typedef double (WINAPI *Get_y0)(double);


class diff
{
	int n;
	double *y;
	double h;
	double begin;
	double end;

	double *p;
	double *q;
	double *f;
	double *alfa;
	double *beta;
	double *gamma;

	HMODULE dll;
	GetF F;
public:
	diff(double b,double e,int n,HMODULE dll);
	double* get_solve_euler();
	double* get_solve_boundary();
	double get_h();
	void set_gamma(double gamma1,double gamma2);
};

#endif