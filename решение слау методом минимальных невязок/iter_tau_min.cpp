#include "stdafx.h"

tau_min::tau_min(double **A, double *u, int n, double eps):iter(A,u,n,eps)
{
	r0=new double[n];
	for(int i=0;i<n;i++)
	{
		r0[i]=0;
	}
}

double* tau_min::get_solve_min()
{
	double Ar,q,p;

	for(int i=0;i<n;i++)
	{
		w0[i]=u[i]-u_solve[i];
	}


int count_step=0;

	q=0;p=0;
	for(int i=1;i<n-1;i++)
	{
		r0[i]=-((u_solve[i+1]-2*u_solve[i]+u_solve[i-1])/(step*step))-f[i];
		Ar=-((r0[i+1]-2*r0[i]+r0[i-1])/(step*step));
		q=q+Ar*r0[i]*step*step;	//скалярное произведение
		p=p+Ar*Ar*step*step;		//скалярное произведение
	}

	tau=q/p;
	for(int i=1;i<n-1;i++)
	{
			u_solve[i]=u_solve[i]-tau*r0[i];
	}

	*r=*r0;

	double norm_rn,norm_r0;
	norm_r0=get_norm(r0,n);
	norm_rn=norm_r0;

while(norm_rn/norm_r0>epsilon)
{
	q=0;p=0;
	for(int i=1;i<n-1;i++)
	{
		r[i]=-((u_solve[i+1]-2*u_solve[i]+u_solve[i-1])/(step*step))-f[i];
		Ar=-((r[i+1]-2*r[i]+r[i-1])/(step*step));
		q=q+Ar*r[i]*step*step;	//скалярное произведение
		p=p+Ar*Ar*step*step;		//скалярное произведение
	}

	tau=q/p;
	for(int i=1;i<n-1;i++)
	{
			u_solve[i]=u_solve[i]-tau*r[i];
	}
	count_step++;
	//print_u_solve();
	//MessageBoxA(NULL,"zxzxz","xzxzx",MB_OK);
	norm_rn=get_norm(r,n);
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


double* tau_min::get_solve_min_n0()
{
	double Ar,q,p;

	for(int i=0;i<n;i++)
	{
		w0[i]=u[i]-u_solve[i];
	}


int count_step=0;

	q=0;p=0;
	for(int i=1;i<n-1;i++)
	{
		r0[i]=-((u_solve[i+1]-2*u_solve[i]+u_solve[i-1])/(step*step))-f[i];
		Ar=-((r0[i+1]-2*r0[i]+r0[i-1])/(step*step));
		q=q+Ar*r0[i]*step*step;	//скалярное произведение
		p=p+Ar*Ar*step*step;		//скалярное произведение
	}

	tau=q/p;
	for(int i=1;i<n-1;i++)
	{
			u_solve[i]=u_solve[i]-tau*r0[i];
	}

	*r=*r0;

	double norm_rn,norm_r0;
	norm_r0=get_norm(r0,n);
	norm_rn=norm_r0;

while(count_step<=n0+1)
{
	q=0;p=0;
	for(int i=1;i<n-1;i++)
	{
		r[i]=-((u_solve[i+1]-2*u_solve[i]+u_solve[i-1])/(step*step))-f[i];
		Ar=-((r[i+1]-2*r[i]+r[i-1])/(step*step));
		q=q+Ar*r[i]*step*step;	//скалярное произведение
		p=p+Ar*Ar*step*step;		//скалярное произведение
	}

	tau=q/p;
	for(int i=1;i<n-1;i++)
	{
			u_solve[i]=u_solve[i]-tau*r[i];
	}
	count_step++;
	norm_rn=get_norm(r,n);
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



double tau_min::get_rn()
{
	return get_norm(r,n);
}

double tau_min::get_r0()
{
	return get_norm(r0,n);
}

/*double tau_min::get_norm(double *r, int n)
{
	double norm=0;
	for(int i=0;i<n;i++)
	{
		norm=norm+r[i]*r[i]*step*step;
	}
	return sqrt(norm);
}*/

tau_min::~tau_min()
{
	delete(r0);
}