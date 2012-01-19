#ifndef INTEGR_H
#define INTEGR_H

#include "graph.h"

typedef double (WINAPI *GetFunction)(double);
typedef double (WINAPI *GetMaximum)(double,double,int);
typedef double (WINAPI *GetValue)(double,double);
typedef double** (WINAPI *GetSingularPoint)();
typedef int (WINAPI *GetCountPoint)();
typedef double (WINAPI *GetValueInf)();
typedef double (WINAPI *GetValueInf1)(double);
typedef double (WINAPI *GetValueInf2)(double);
typedef double (WINAPI *GetValueInf3)(double,double);


class integr:public Graph
{
	double eps;
	double** point;
	double eps2;
	GetMaximum function;
	GetFunction function_x;
	GetValue function_val;
	GetSingularPoint function_point;
	GetCountPoint function_count;
	GetValueInf function_val_inf;
	GetValueInf1 function_val_inf1;
	GetValueInf2 function_val_inf2;
	GetValueInf3 function_val_inf3;
	double integr_val;
	double sum_err;
	double pract_err;
public:
	integr(double w,double h,double m,double b,double e,HMODULE dll,double eps,double eps2);

	//all funcitons return vector with value of integr+theor err(new eps)+pract err
	double* first_formula();
	double* second_formula();
	double* second_formula(double b, double e, double epsilon);
	double* second_formula1(double b, double e, double epsilon);

	double* third_formula();
	void flood_fill(CClientDC *dc,CRect *dlg_rc,double x,double y);
	double find_all_int(double b, double e, double epsilon2);	//[a,b]
	double find_all_int(char* b, double e, double epsilon2);	//[-infinity,b]
	double find_all_int(double b, char* e, double epsilon2);	//[a,infinity]
	double pract_error();
	double theor_err();
	double pract_error_inf();
	double pract_error_inf(char* b,double e);
	double pract_error_inf(double b,char* e);
	double pract_error_inf(double b,double e);


};


#endif