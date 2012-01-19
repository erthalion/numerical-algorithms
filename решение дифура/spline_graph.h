#ifndef SPLINE_GRAPH_H
#define SPLINE_GRAPH_H

typedef double (WINAPI *GetMaximum)(double,double,int);


class Spline
{
protected:
	double x_cur;
	double* coeff;
	double* func;
	int node;
	double begin,end;
	HMODULE dll;
	double delta;

public:
	Spline(double b,double e,HMODULE my_dll,double eps,double *f);
	double func_polynom(double x);
	double pract(double x);
	double theor(double x);
	double theor_dif(double x,int n);
	double pract_dif(double x,int n);
	~Spline();
};

#endif