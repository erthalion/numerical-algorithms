#ifndef INTEGR_H
#define INTEGR_H

#include "graph.h"

typedef double (WINAPI *GetFunction)(double);
typedef double (WINAPI *GetMaximum)(double,double,int);
typedef double (WINAPI *GetValue)(double,double);

class integr:public Graph
{
	double eps;
	GetMaximum function;
	GetFunction function_x;
	GetValue function_val;
public:
	integr(double w,double h,double m,double b,double e,HMODULE dll,double eps);
	//all funcitons return vector with value of integr+theor err(new eps)+pract err
	double* first_formula();
	double* second_formula();
	double* third_formula();

	
};


#endif