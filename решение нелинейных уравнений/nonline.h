#ifndef _NONLINE_H
#define _NONLINE_H

#include "afxwin.h"

typedef double (WINAPI *GetFunction)(double);
typedef double (WINAPI *GetMaximumDiff)(double,double);
typedef double (WINAPI *GetMinimumDiff)(double,double);
typedef double (WINAPI *GetDiff)(double);
typedef double* (WINAPI *GetSolve)();
typedef int (WINAPI *GetCount)();




class nonline
{
	double begin,end;
	double begin1,end1;
	double epsilon;
	double x_solve,x_0;
	double k;
	int count_simple;
	int count_newton;
	double *solve;
	int count;
	HMODULE dll;
	GetMaximumDiff function_max;
	GetMinimumDiff function_min;
	GetFunction function_x;
	GetDiff function_diff;
	GetSolve function_solve;
	GetCount function_count;

public:
	nonline(double b,double e,double eps,HMODULE m_dll);
	double div2(double begin,double end);
	double simple();
	double newton();
	int get_simple_n();
	int get_newton_n();
	double get_err();
	double get_begin1();
	double get_end1();
	//double get_err_newton();

	~nonline();

};

#endif
