#include "stdafx.h"

iter::iter(int n,double l,double eps)
{
	node=n;
	lenght=l;
	epsilon=eps;
step=lenght/(n-1);
gamma1=8*pow(cos(Pi*step/2.0),2)/pow(step,2);
gamma2=8*pow(sin(Pi*step/2.0),2)/pow(step,2);
tau=2.0/(8*pow(sin(Pi*step/2.0),2)/pow(step,2)+8*pow(cos(Pi*step/2.0),2)/pow(step,2));
u=new double*[n];
r=new double*[n];
differense=new double*[n];
	for(int i=0;i<n;i++)
	{
		u[i]=new double[n];
		r[i]=new double[n];
		differense[i]=new double[n];
	}

	for(int i=0;i<n;i++)
	{
//заполнить u граничными значениями
		u[0][i]=0;
		u[n-1][i]=0;

		u[i][0]=0;
		u[i][n-1]=0;
//заполнить r граничными значениями
		r[0][i]=0;
		r[n-1][i]=0;

		r[i][0]=0;
		r[i][n-1]=0;
	}

//заполнить внутренние точки u
	for(int i=1;i<n-1;i++)
	{
		for(int j=1;j<n-1;j++)
		{
			u[i][j]=1;
			r[i][j]=0;
		}
	}

}

void iter::print(double **matr, int n)
{
	FILE *f;
	f=fopen("output.dat","w");
	
	fprintf(f,"TITLE=\"USERData\"\n");
	fprintf(f,"VARIABLES=x,y,u");
	fprintf(f,"\nZONE T=\"ZONE1\", i=%d j=%d f=Point", n,n );

	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			fprintf(f,"\n%2.8f %2.8f %2.8f", step*i, step*j, matr[i][j]);
		}
	}
	fclose(f);
}

double iter::scal_prod(double **a,double **b,int n,double h)
{
double sum=0;
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			sum=sum+a[i][j]*b[i][j]*pow(h,2);
		}
	}
return sum;
}

double iter::solve()
{
//double norm;
double Ar,q,p;
norm=1;
double omega=1.5;

count_step=0;
//find norm_0
double norm_0=0;

for(int i=1;i<node-1;i++)
	{
		for(int j=1;j<node-1;j++)
		{
			u[i][j]=(1-omega)*u[i][j]+omega*(u[i-1][j]+u[i][j-1])/4.0+omega*(u[i+1][j]+u[i][j+1])/4.0+omega*step*step*f(i,j,step)/4.0;
		}
	}


	for(int i=1;i<node-1;i++)
	{
		for(int j=1;j<node-1;j++)
		{
			r[i][j]=-((u[i+1][j]-2*u[i][j]+u[i-1][j])/(step*step)+(u[i][j+1]-2*u[i][j]+u[i][j-1])/(step*step))-f(i,j,step);
		}
	}

	norm_0=scal_prod(r,r,node,step);
	norm_0=sqrt(norm_0);

norm=norm_0;
while(norm/norm_0>=epsilon)
{
	count_step++;
	q=0;p=0;

	for(int i=1;i<node-1;i++)
	{
		for(int j=1;j<node-1;j++)
		{
			u[i][j]=(1-omega)*u[i][j]+omega*(u[i-1][j]+u[i][j-1])/4.0+omega*(u[i+1][j]+u[i][j+1])/4.0+omega*step*step*f(i,j,step)/4.0;
		}
	}


	for(int i=1;i<node-1;i++)
	{
		for(int j=1;j<node-1;j++)
		{
			r[i][j]=-((u[i+1][j]-2*u[i][j]+u[i-1][j])/(step*step)+(u[i][j+1]-2*u[i][j]+u[i][j-1])/(step*step))-f(i,j,step);
		}
	}

	norm=scal_prod(r,r,node,step);
	norm=sqrt(norm);
}

for(int i=0;i<node;i++)
{
	for(int j=0;j<node;j++)
	{
		differense[i][j]=u[i][j]-u_orig(i,j,step);
	}
}

//output_n();
//return sqrt(scal_prod(differense,differense,node,step));
return norm/norm_0;
}


double iter::solve_simple_tau()
{
//double norm1;
double Ar,q,p;
norm=1;
tau=2.0/(8*pow(sin(Pi*step/2.0),2)/pow(step,2)+8*pow(cos(Pi*step/2.0),2)/pow(step,2));

count_step=0;
//find norm_0
double norm_0=0;

	for(int i=1;i<node-1;i++)
	{
		for(int j=1;j<node-1;j++)
		{
			r[i][j]=-((u[i+1][j]-2*u[i][j]+u[i-1][j])/(step*step)+(u[i][j+1]-2*u[i][j]+u[i][j-1])/(step*step))-f(i,j,step);
		}
	}
	for(int i=1;i<node-1;i++)
	{
		for(int j=1;j<node-1;j++)
		{
			u[i][j]=u[i][j]-tau*r[i][j];
		}
	}

	norm_0=scal_prod(r,r,node,step);
	norm_0=sqrt(norm_0);

norm=norm_0;
while(norm/norm_0>=epsilon)
{
	count_step++;
	for(int i=1;i<node-1;i++)
	{
		for(int j=1;j<node-1;j++)
		{
			r[i][j]=-((u[i+1][j]-2*u[i][j]+u[i-1][j])/(step*step)+(u[i][j+1]-2*u[i][j]+u[i][j-1])/(step*step))-f(i,j,step);
		}
	}

	for(int i=1;i<node-1;i++)
	{
		for(int j=1;j<node-1;j++)
		{
			u[i][j]=u[i][j]-tau*r[i][j];
		}
	}

	norm=scal_prod(r,r,node,step);
	norm=sqrt(norm);
}

for(int i=0;i<node;i++)
{
	for(int j=0;j<node;j++)
	{
		differense[i][j]=u[i][j]-u_orig(i,j,step);
	}
}

//return sqrt(scal_prod(differense,differense,node,step));
return count_step;
}


iter::~iter()
{
	for(int i=0;i<node;i++)
	{
		delete(u[i]);
		delete(r[i]);
		delete(differense[i]);
	}

}

void iter::output_n()
{
FILE *out;
out=fopen("output_n.txt","a");
fprintf(out,"%2.8f %d \n", 0.01,count_step);
fclose(out);
}

void iter::output_u()
{
	print(u,node);
}

int iter::get_count()
{
	return count_step;
}

int iter::get_theor_count()
{
	return floor(log(1.0/epsilon)/log((1.0+gamma2/gamma1)/(1.0-gamma2/gamma1)));
	//gamma2/gamma1 ~ epsilon?
}