#include "stdafx.h"

//typedef double (WINAPI *GetMaximum)(double,double,int);


Spline::Spline(double b,double e,HMODULE my_dll,double eps,double *f)
{
	int n=0;
	begin=b;end=e;
	dll=my_dll;
	
	x_cur=0;

	//GetMaximum function;
	//function=(GetMaximum)GetProcAddress(dll,"GetMaximum");

	//delta=pow(eps/function(begin,end,4),0.25);
	delta=eps;	//eps - step!!!
//	delta=delta-0.1;
	for(double i=begin;i<end;i=i+delta)
	{
		n++;
	}
	node=n;

	func=new double[node];
	for(int i=0;i<node;i++)
	{
		func[i]=f[i];
	}

	//delta=(end-begin)/node;
	//delta=delta-0.001;


	//x_cur=x;
	coeff=NULL;
}

double Spline::func_polynom(double x)
{
	int n=0;
	int k=0;

	//char str[100];
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

	c1=new double[node];
	g1=new double[node];
	b1=new double[node];
	a1=new double[node];
	f1=new double[node];
	f=new double[node];

	a1[0]=NULL;
	b1[0]=NULL;
	g1[0]=NULL;
	f1[0]=NULL;
//	c1[0]=NULL;
//	f[0]=NULL;
int i;
double f0;

for(int i=0;i<node;i++)
{
	f0=func[i];
	f[i]=func[i];
}


for(i=1;i<node;i++)
{
	a1[i]=1.0;
}

for(i=1;i<node;i++)
{
	b1[i]=4.0;
}

for(i=1;i<node;i++)
{
	g1[i]=1.0;
}

for(i=1;i<node-1;i++)
{
	f1[i]=3*(f[i+1]-f[i-1])/delta;
}

//double *m;

alg_progon(a1,b1,g1,f1,c1,node-1);
c1[0]=-c1[1]/2.0+(3.0/2.0)*(f[1]-f[0])/delta;
c1[node-1]=-c1[node-2]/2.0+(3.0/2.0)*(f[node-1]-f[node-2])/delta;

if(coeff==NULL)
{
	coeff=new double[node];
	for(i=0;i<node;i++)
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
	//s=a*function(xi)+b*function(xi1)+c*c1[k-1]+d*c1[k];
	s=a*func[k-1]+b*func[k]+c*c1[k-1]+d*c1[k];

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
	return 0;
}

double Spline::theor(double x)
{
	return 0;
}

double Spline::theor_dif(double x,int n)
{
	return 0;
}

double Spline::pract_dif(double x, int degree)
{
	return 0;
}


Spline::~Spline()
{
	delete(func);
}