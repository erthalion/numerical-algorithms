#include "stdafx.h"

native::native(int node,double epsilon,double l,double l_th)
{
n=node;eps=epsilon;lenght=l;l_theor=l_th;
h=1.0/(n+1);
lm=0;
//u=new double[n];
//e=new double[n];
//u_res=new double[n];
}

double native::A(double *f, int i, double h, int n)
{
	if(i==0) return (2*f[i]-f[i+1])/(h*h);
    if(i==n-1) return (-f[i-1]+2*f[i])/(h*h);
    return (-f[i-1]+2*f[i]-f[i+1])/(h*h);
}

double native::diff(double *f1, double *f2, double h, int n)
{
    int i;
    double b=0;
    for(i=0;i<n;i++)
	{
        b=b+(f1[i]-f2[i])*(f1[i]-f2[i])*h*h;
	}
    return sqrt(b);
}

double native::norm(double *f1, double h, int n)
{
    int i;
    double b;
	b=0;
    for(i=0;i<n;i++)
	{
       b=b+f1[i]*f1[i];
	}
    return sqrt(b*h*h);
}

double native::scal(double *f1, double *f2, int n)
{
    int i;
    double b=0;
    for(i=0;i<n;i++)
	{
       b=b+f1[i]*f2[i];
	}
    return b;
}

double native::find()
{
	double p,q,l_prev;
	double *u=new double[n];
	double *e=new double[n];
	double *u_res=new double[n];
	double *lx=new double[n];
	


	mod1=1;

	for(int i=0;i<n;i++)
	{
		u[i]=i+1;
	}


	for(int i=0;i<n;i++)
	{
		e[i]=u[i]/norm(u,h,n);
	}


	for(int i=0;i<n;i++)
    {
        u_res[i]=A(e,i,h,n);
    }



	mod1=diff(u_res,u,h,n);

	//while(mod1>eps)
	while(abs(l_theor-lm)>eps)
	{
		p=0;q=0;

		lm=scal(u_res,e,n)/scal(e,e,n);

		for(int i=0;i<n;i++)
		{
		u[i]=u_res[i];
		}

		for(int i=0;i<n;i++)
		{
			e[i]=u[i]/norm(u,h,n);
		}

		for(int i=0;i<n;i++)
          {
               u_res[i]=A(e,i,h,n);
          }

		for(int i=0;i<n;i++)
          {
               lx[i]=l_theor*e[i];
          }

	mod1=diff(u_res,u,h,n);
	mod2=diff(u_res,lx,h,n);
	}

delete(u);
delete(u_res);
delete(e);
delete(lx);

return lm;
}

double native::get_mod1()
{
	return abs(l_theor-lm);
}

double native::get_mod2()
{
	return mod2;
}