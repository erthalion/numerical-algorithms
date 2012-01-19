#ifndef SPLINE_GRAPH_H
#define SPLINE_GRAPH_H

#include "graph.h"

typedef double (WINAPI *GetMaximum)(double,double,int);


class Spline:public Graph
{
protected:
	double x_cur;
	double* coeff;
public:
	Spline(double w,double h,double m,double b,double e,HMODULE my_dll,double eps,double x);
	double func_polynom(double x);
	double pract(double x);
	double theor(double x);
	double theor_dif(double x,int n);
	double pract_dif(double x,int n);
	~Spline();
};

#endif