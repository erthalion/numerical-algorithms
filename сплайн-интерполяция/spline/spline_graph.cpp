#include "stdafx.h"

typedef double (WINAPI *GetFunction)(double);
typedef double (WINAPI *GetMaximum)(double,double,int);
typedef double (WINAPI *GetDif)(double,int);

Spline::Spline(double w,double h,double m,double b,double e,HMODULE my_dll,double eps,double x):Graph(w,h,m,1,b,e,my_dll)
{
	int n=0;
	dll=my_dll;
	o_x1=o_x;o_y1=o_y;

	GetMaximum function;
	function=(GetMaximum)GetProcAddress(dll,"GetMaximum");

	delta=pow(eps/function(begin,end,4),0.25);
//	delta=delta-0.1;
	for(double i=begin;i<=end;i=i+delta,n++){}
	node=n;
	delta=(end-begin)/node;
	delta=delta-0.001;

	x_cur=x;
	coeff=NULL;
}

double Spline::func_polynom(double x)
{
	GetFunction function;
	function=(GetFunction)GetProcAddress(dll,"GetFunction");
	int n=0;
	int k=0;

	char str[100];
	//MessageBoxA(NULL,itoa(delta*100,str,10),"delta",MB_OK);
	for(double i=begin;i<=end;i=i+delta,n++)
	{
	if(abs(i-x)/delta<=1)
		{
		k=n;
		}
	}


double mi,mi1,xi,xi1;
double a,b,c,d;
double s;

	double *a1,*b1,*g1,*f,*f1,*c1;

	c1=new double[node+1];
	g1=new double[node+1];
	b1=new double[node+1];
	a1=new double[node+1];
	f1=new double[node+1];
	f=new double[node+1];

	a1[0]=NULL;
	b1[0]=NULL;
	g1[0]=NULL;
	f1[0]=NULL;
//	c1[0]=NULL;
//	f[0]=NULL;
int i;

for(i=0;i<=node;i++)
{
	f[i]=function(begin+i*delta);
}


for(i=1;i<=node;i++)
{
	a1[i]=1.0;
}

for(i=1;i<=node;i++)
{
	b1[i]=4.0;
}

for(i=1;i<=node;i++)
{
	g1[i]=1.0;
}

for(i=1;i<=node;i++)
{
	f1[i]=3*(f[i+1]-f[i-1])/delta;
}

//double *m;

alg_progon(a1,b1,g1,f1,c1,node);
c1[0]=-c1[1]/2.0+(3.0/2.0)*(f[1]-f[0])/delta;
c1[node]=-c1[node-1]/2.0+(3.0/2.0)*(f[node]-f[node-1])/delta;

if(coeff==NULL)
{
	coeff=new double[node];
	for(i=0;i<=node;i++)
	{
	coeff[i]=c1[i];
	}


}

	xi=begin+(k-1)*delta;
	xi1=begin+(k)*delta;


	a=(pow((xi1-x),2)*(2*(x-xi)+delta)/pow(delta,3));
	b=(pow((x-xi),2)*(2*(xi1-x)+delta)/pow(delta,3));
	c=pow((xi1-x),2)*(x-xi)/pow(delta,2);
	d=pow((x-xi),2)*(x-xi1)/pow(delta,2);
	s=a*function(xi)+b*function(xi1)+c*c1[k-1]+d*c1[k];

	delete a1;
	delete b1;
	delete c1;
	delete g1;
	delete f;
	delete f1;
	//delete m;

return s;
}

double Spline::pract(double x)
{
	GetFunction function;
	function=(GetFunction)GetProcAddress(dll,"GetFunction");

	if(function==NULL)
	{
		MessageBoxA(NULL,"import function fail(GetFunction)","error",MB_OK);
		return 0;
	}


	return abs(function(x)-func_polynom(x));

}

double Spline::theor(double x)
{
	GetMaximum function;
	function=(GetMaximum)GetProcAddress(dll,"GetMaximum");
	if(function==NULL)
	{
		MessageBoxA(NULL,"import function fail(GetMaximum)","error",MB_OK);
		return 0;
	}

	return function(begin,end,4)*pow(delta,4);

}

double Spline::theor_dif(double x,int n)
{
	GetMaximum function;
	function=(GetMaximum)GetProcAddress(dll,"GetMaximum");
	if(function==NULL)
	{
		MessageBoxA(NULL,"import function fail(GetMaximum)","error",MB_OK);
		return 0;
	}
	if(n>2) 
	{
		MessageBoxA(NULL,"dif degree n not exist","error",MB_OK);
		return 0;
	}

return function(begin,end,4)*pow(delta,4-n);
}

double Spline::pract_dif(double x, int degree)
{
	double a1,a2,a3,a4,s;
	double f1,f2,f3,f4,f5;
	double xi,xi1; 
	int k=0;
	int n=0;

	//search needing k
	for(double i=begin;i<=end;i=i+delta,n++)
	{
	if(abs(i-x)/delta<=1)
		{
		k=n;
		}
	}

	xi=begin+(k-1)*delta;
	xi1=begin+(k)*delta;

	GetDif function;
	function=(GetDif)GetProcAddress(dll,"GetDif");

	if(function==NULL)
	{
		MessageBoxA(NULL,"import function fail(GetDif)","error",MB_OK);
		return 0;
	}

	GetFunction function_x;
	function_x=(GetFunction)GetProcAddress(dll,"GetFunction");

	if(function_x==NULL)
	{
		MessageBoxA(NULL,"import function fail(GetFunction)","error",MB_OK);
		return 0;
	}
	
	f1=function_x(xi);
	f2=function_x(xi1);
	f3=coeff[k-1];
	f4=coeff[k];
	f5=function(x,n);

//	char str[100]
	switch(degree)
	{
		case 1:
			a1=(-2*(xi1-x)*(2*(x-xi)+delta)/pow(delta,3.0))+(pow(xi1-x,2.0)*2/pow(delta,3.0));
			a2=(2*(x-xi)*(2*(xi1-x)+delta)/pow(delta,3.0))+(pow(x-xi,2.0)*(-2)/pow(delta,3.0));
			a3=(-2*(xi1-x)*(x-xi)/pow(delta,2.0))+(pow(xi1-x,2.0)/pow(delta,2.0));
			a4=(2*(x-xi)*(x-xi1)/pow(delta,2.0))+(pow(x-xi,2.0)/pow(delta,3.0));
//			s=a1*function_x(xi)+a2*function_x(xi1)+a3*coeff[k-1]+a4*coeff[k];
			s=a1*f1+a2*f2+a3*f3+a4*f4;
			return abs(f5-s);//instead func_polunom solve real dif(s_k)!
		case 2:
			a1=(2*(2*(x-xi)+delta)/pow(delta,3.0))+(-2*(xi1-x)*2)*2/pow(delta,3.0);
			a2=(2*(2*(xi1-x)+delta)/pow(delta,3.0))+(-2*(x-xi)*2)*2/pow(delta,3.0);
			a3=(2*(x-xi)/pow(delta,2.0))+(-2*(xi1-x))*2/pow(delta,2.0);
			a4=(2*(x-xi1)/pow(delta,2.0))+(2*(x-xi))*2/pow(delta,2.0);
			s=a1*function_x(xi)+a2*function_x(xi1)+a3*coeff[k-1]+a4*coeff[k];
			return abs(function(x,n)-s);//instead func_polunom solve real dif(s_k)!
	}


}


Spline::~Spline()
{
}