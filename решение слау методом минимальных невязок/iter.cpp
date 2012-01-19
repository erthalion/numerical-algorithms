#include "stdafx.h"

iter::iter(double **A,double *u,int n,double eps):method(A,u,n)
{
		epsilon=eps;
		step=1.0/(n-1);
		//step=1.0/n;
		delta_min=pow(sin(Pi*step/2),2)*4.0/(step*step);
		delta_max=pow(sin(Pi*(n-1)*step/2),2)*4.0/(step*step);
		tau=2.0/(delta_min+delta_max);
		r=new double[n];
		w0=new double[n];
		wn=new double[n];
		r[0]=0;	//граничныей значения известны
		r[n-1]=0;	//граничные значения известны

		u_solve[0]=0;	//граничныей значения известны
		u_solve[n-1]=0;	//граничныей значения известны

		double s=(delta_max-delta_min)/(delta_max+delta_min);
		n0=floor(log(1.0/epsilon)/log(1.0/s));	//находим теор количество итераций

/////////////////////////////
//задаем правую часть
		double sum;
		for(int i=0;i<n;i++)
		{
			sum=0;
			for(int j=0;j<n;j++)
			{
				sum=sum+A[i][j]*u[j]*1.0/(step*step);
			}
			f[i]=sum;
		}
print_A();
print_f();
}

double* iter::get_solve()
{
	for(int i=0;i<n;i++)
	{
		w0[i]=u[i]-u_solve[i];
	}

int count_step=0;
while(count_step<=n0+1)
{
	for(int i=1;i<n-1;i++)
	{
		r[i]=-((u_solve[i+1]-2*u_solve[i]+u_solve[i-1])/(step*step))-f[i];
	}

	for(int i=1;i<n-1;i++)
	{
			u_solve[i]=u_solve[i]-tau*r[i];
	}
	count_step++;
	//print_u_solve();
	//MessageBoxA(NULL,"zxzxz","xzxzx",MB_OK);
}

val_step=count_step;

	for(int i=0;i<n;i++)
	{
		wn[i]=u[i]-u_solve[i];
	}

	print_u();
	print_u_solve();
return u_solve;
}

void iter::set_approx_null()
{
	for(int i=1;i<n-1;i++)
	{
		u_solve[i]=0;
	}
	u_solve[0]=0;
	u_solve[n-1]=0;
}

void iter::set_approx_near()
{
	for(int i=1;i<n-1;i++)
	{
		u_solve[i]=u[i]+rand()*0.0000001;
	}
	u_solve[0]=0;
	u_solve[n-1]=0;
}

void iter::set_approx_rand()
{
	for(int i=1;i<n-1;i++)
	{
		u_solve[i]=rand();
	}
	u_solve[0]=0;
	u_solve[n-1]=0;

}

double iter::get_delta_max()
{
	return delta_max;
}

double iter::get_delta_min()
{
	return delta_min;
}

double iter::get_wn()
{
	return get_norm(wn,n);
}

double iter::get_w0()
{
	return get_norm(w0,n);
}

double iter::get_tau()
{
	return tau;
}

int iter::get_count_step()
{
	return val_step;
}

iter::~iter()
{
	delete(r);
	delete(w0);
	delete(wn);
	//method::~method();

}

double iter::get_norm(double *r, int n)
{
	double norm=0;
	for(int i=0;i<n;i++)
	{
		norm=norm+r[i]*r[i]*step*step;
	}
	return sqrt(norm);
}