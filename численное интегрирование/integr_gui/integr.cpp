#include "stdafx.h"


integr::integr(double w, double h, double m, double b, double e, HMODULE dll, double my_eps):Graph(w,h,m,1,b,e,dll)
{
eps=my_eps;

	function=(GetMaximum)GetProcAddress(dll,"GetMaximum");
	if(function==NULL)
	{
		MessageBoxA(NULL,"import function fail(GetMaximum)","error",MB_OK);
		return ;
	}

	function_x=(GetFunction)GetProcAddress(dll,"GetFunction");
	if(function_x==NULL)
	{
		MessageBoxA(NULL,"import function fail(GetFunction)","error",MB_OK);
		return ;
	}

	function_val=(GetValue)GetProcAddress(dll,"GetValue");
	if(function_val==NULL)
	{
		MessageBoxA(NULL,"import function fail(GetValue)","error",MB_OK);
		return ;
	}

}

double* integr::first_formula()
{
	int n=0;
	double integr;
	double sum=0;
	double theor_err;
	double pract_err;
	double* ret;
	ret=new double[2];

	//find delta/////

	delta=pow(12.0*eps/((end-begin)*function(begin,end,2)),0.5);
	for(double i=begin;i<=end;i=i+delta,n++){}
	node=n;
	delta=(end-begin)/node;
	theor_err=pow(delta,2)*(end-begin)*function(begin,end,2)/12.0;
	
	//find integr////
	
	for(int i=1;i<node;i++)
	{
		sum=sum+function_x(begin+i*delta);
	}
	integr=delta*((function_x(begin)/2)+(function_x(end)/2)+sum);
	
	/////////////////
	
	pract_err=abs(integr-function_val(begin,end));

	ret[0]=integr;
	ret[1]=theor_err;
	ret[2]=pract_err;
	return ret;
}


double* integr::second_formula()
{
	int n=0;
	double integr=0;
	double sum_1=0;
	double sum_2=0;

	double theor_err;
	double pract_err;
	double* ret;
	ret=new double[2];

	//find delta

	delta=pow(180.0*eps/((end-begin)*function(begin,end,4)),0.25);
	for(double i=begin;i<=end;i=i+delta,n++){}	//проверить чтобы число отрезков 
												//при таком разбиении было кратно двум
	if(n%2!=0)	//проверяем на кратность N так как всего узлов N+1 отрезков--N
	{
		n++;
	}

	node=n;
	delta=(end-begin)/node;
	theor_err=pow(delta,4)*(end-begin)*function(begin,end,4)/180.0;

	//find integr
	
	for(int i=1;i<node;i=i+2)
	{
		sum_1=sum_1+function_x(begin+i*delta);
	}

	for(int i=2;i<node;i=i+2)
	{
		sum_2=sum_2+function_x(begin+i*delta);
	}

	integr=(delta/3.0)*(function_x(begin)+function_x(end)+4*sum_1+2*sum_2);

	
	////////////////

	pract_err=abs(integr-function_val(begin,end));

	ret[0]=integr;
	ret[1]=theor_err;
	ret[2]=pract_err;
	return ret;

}

double* integr::third_formula()
{
	int n=0;
	double integr;
	double sum_1=0;
	double sum_2=0;

	double theor_err;
	double pract_err;
	double* ret;
	ret=new double[2];

	//find delta

	delta=pow(80.0*eps/((end-begin)*function(begin,end,4)),0.25);
	for(double i=begin;i<=end;i=i+delta,n++){}	//проверить чтобы число отрезков 
												//при таком разбиении было кратно трем
	if(n%3!=0)
	{
		n=n+n%3;
	}

	node=n;
	delta=(end-begin)/node;
	theor_err=pow(delta,4)*(end-begin)*function(begin,end,4)/80.0;
	
	//find integr

	for(int i=1;i<node;i++)
	{
		sum_1=sum_1+function_x(begin+i*delta);
	}

	for(int i=3;i<node;i=i+3)
	{
		sum_2=sum_2+function_x(begin+i*delta);
	}

	integr=delta*(3.0/8.0)*(function_x(begin)+function_x(end)+3*sum_1+2*sum_2);

	////////////////

	pract_err=abs(integr-function_val(begin,end));

	ret[0]=integr;
	ret[1]=theor_err;
	ret[2]=pract_err;
	return ret;


	
}