#include "stdafx.h"

iter::iter(int node,double l,double epsilon)
{
n=node;
lenght=l;
eps=epsilon;
step=lenght/(n-1);
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
			sum=sum+a[i][j]*b[i][j]*h*h;
		}
	}
return sum;
}

double iter::A(double** u,int i,int j)
{
double ret;
	if(i==0)
	{
		ret=-(u[1][j]-u[0][j])/step;
	}
	if(i==n-1)
	{
		ret=(u[n-1][j]-u[n-2][j])/step;
	}
	if(j==0)
	{
		ret=-(u[i][1]-u[i][0])/step;
	}
	if(j==n-1)
	{
		ret=(u[i][n-1]-u[i][n-2])/step;
	}
	if(i!=0&&i!=n-1&&j!=0&&j!=n-1)
	{
		ret=-(u[i+1][j]-2*u[i][j]+u[i-1][j])/(step*step)-(u[i][j+1]-2*u[i][j]+u[i][j-1])/(step*step);
	}
return ret;

}

double iter::get_mod()
{
	return mod;
}

double iter::solve()
{
	int n0=50;
	//double norm,norm0;
	double p=0,q=0,tau_sum;
	double Ar;
	int count;

	mod=1;

	double **u=new double*[n];
	double **phi=new double*[n];

	double **r=new double*[n];

	for(int i=0;i<n;i++)
	{
		phi[i]=new double[n];
		u[i]=new double[n];
		r[i]=new double[n];
	}


	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			phi[i][j]=free_f(i,j,step);
			r[i][j]=0;
		}
	}

//print(phi,n);
//действуем на phi оператором А--u=A(phi)


	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			u[i][j]=A(phi,i,j);
		}
	}

	u[0][0]=(u[1][0]+u[0][1])/2;
	u[n-1][0]=(u[n-2][0]+u[n-1][1])/2;
	u[0][n-1]=(u[0][n-2]+u[1][n-1])/2;
	u[n-1][n-1]=(u[n-1][n-2]+u[n-2][n-1])/2;

print(u,n);

	tau_sum=0;

////первый шаг//////////

	for(int i=1;i<n-1;i++)
	{
		for(int j=1;j<n-1;j++)
		{
			r[i][j]=A(u,i,j)-f(i,j,step);
		}
	}

	for(int i=1;i<n-1;i++)
	{
		r[i][0]=A(u,i,0);
	}

	for(int i=1;i<n-1;i++)
	{
		r[i][n-1]=A(u,i,n-1);
	}

	for(int i=1;i<n-1;i++)
	{
		r[0][i]=A(u,0,i);
	}

	for(int i=1;i<n-1;i++)
	{
		r[n-1][i]=A(u,n-1,i);
	}

	r[0][0]=(r[1][0]+r[0][1])/2;
	r[n-1][0]=(r[n-2][0]+r[n-1][1])/2;
	r[0][n-1]=(r[0][n-2]+r[1][n-1])/2;
	r[n-1][n-1]=(r[n-1][n-2]+r[n-2][n-1])/2;

	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			//////////////
			Ar=A(r,i,j);
			q=q+Ar*r[i][j]*step*step;	//скалярное произведение
			p=p+Ar*Ar*step*step;		//скалярное произведение
			
			////////////////////////
		}
	}
		
	tau=q/p;
	tau_sum+=tau;

	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			u[i][j]=u[i][j]-tau*r[i][j];
		}
	}

	norm_0=scal_prod(r,r,n,step);
	norm_0=sqrt(norm_0);

	double norm1;
	double norm2;
double c;
	count=1;
	while(mod>eps)//&&count<10000)
	{
		if(count>1)
		{
		tau_sum=0;
		}
		count++;
		//find n0-1 tau
		for(int k=0;k<n0-1;k++)
		{
				for(int i=1;i<n-1;i++)
				{
					for(int j=1;j<n-1;j++)
					{
						r[i][j]=A(u,i,j)-f(i,j,step);
					}
				}

	for(int i=1;i<n-1;i++)
	{
		r[i][0]=A(u,i,0);
	}

	for(int i=1;i<n-1;i++)
	{
		r[i][n-1]=A(u,i,n-1);
	}

	for(int i=1;i<n-1;i++)
	{
		r[0][i]=A(u,0,i);
	}

	for(int i=1;i<n-1;i++)
	{
		r[n-1][i]=A(u,n-1,i);
	}
			r[0][0]=(r[1][0]+r[0][1])/2;
			r[n-1][0]=(r[n-2][0]+r[n-1][1])/2;
			r[0][n-1]=(r[0][n-2]+r[1][n-1])/2;
			r[n-1][n-1]=(r[n-1][n-2]+r[n-2][n-1])/2;

				for(int i=0;i<n;i++)
				{
				for(int j=0;j<n;j++)
				{
					//////////////
					Ar=A(r,i,j);
					q=q+Ar*r[i][j]*step*step;	//скалярное произведение
					p=p+Ar*Ar*step*step;		//скалярное произведение
					/////////////////////////
				}
				}

				tau=q/p;
				tau_sum+=tau;

				for(int i=0;i<n;i++)
				{
					for(int j=0;j<n;j++)
					{
						u[i][j]=u[i][j]-tau*r[i][j];
					}
				}

		}
		//find n0 tau

				for(int i=1;i<n-1;i++)
				{
					for(int j=1;j<n-1;j++)
					{
						r[i][j]=A(u,i,j)-f(i,j,step);
					}
				}

	for(int i=1;i<n-1;i++)
	{
		r[i][0]=A(u,i,0);
	}

	for(int i=1;i<n-1;i++)
	{
		r[i][n-1]=A(u,i,n-1);
	}

	for(int i=1;i<n-1;i++)
	{
		r[0][i]=A(u,0,i);
	}

	for(int i=1;i<n-1;i++)
	{
		r[n-1][i]=A(u,n-1,i);
	}

			r[0][0]=(r[1][0]+r[0][1])/2;
			r[n-1][0]=(r[n-2][0]+r[n-1][1])/2;
			r[0][n-1]=(r[0][n-2]+r[1][n-1])/2;
			r[n-1][n-1]=(r[n-1][n-2]+r[n-2][n-1])/2;

				for(int i=0;i<n;i++)
				{
					for(int j=0;j<n;j++)
					{
						u[i][j]=u[i][j]+tau_sum*r[i][j];
					}
				}

			norm=scal_prod(r,r,n,step);
			norm=sqrt(norm);
			mod=norm;//norm_0;
			norm1=scal_prod(u,u,n,step);
			norm1=sqrt(norm1);

			
			c=0;
				for(int i=0;i<n;i++)
				{
					for(int j=0;j<n;j++)
					{
						c=c+cos(Pi*i*step)*cos(Pi*j*step)*cos(Pi*i*step)*cos(Pi*j*step)*step*step;
					}
				}
				c=sqrt(c);

			//getch();
	}
	print(u,n);
	delete(u);
	delete(phi);
	delete(r);
	return mod;

}

double iter::get_n()
{
	return norm;
}

double iter::get_n0()
{
	return norm_0;
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
	print(u,n);
}

int iter::get_count()
{
	return count_step;
}

int iter::get_theor_count()
{
	return floor(log(1.0/eps)/log((1.0+gamma2/gamma1)/(1.0-gamma2/gamma1)));
	//gamma2/gamma1 ~ epsilon?
}