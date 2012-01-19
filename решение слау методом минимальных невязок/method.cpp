#include "stdafx.h"

method::method(double **A_in,double *u_in,int n_in)
{
	double sum;
	n=n_in;
	A=A_in;
	u=u_in;
	f=new double[n];
	Af=new double*[n];
	u_solve=new double[n];
	mode=0;
	//заполняем расширенную матрицу из A
	for(int i=0;i<n;i++)
	{
		Af[i]=new double[n+1];
		for(int j=0;j<n;j++)
		{
			Af[i][j]=A_in[i][j];
		}
	}

	for(int i=0;i<n;i++)
	{
		sum=0;
		for(int j=0;j<n;j++)
		{
			sum=sum+A[i][j]*u[j];
		}
		f[i]=sum;
	}
	
	//заполняем расширенную матрицу правой частью уравн
	for(int i=0;i<n;i++)
	{
		Af[i][n]=f[i];
	}
}

method::method(double **A_in,double *f_in,int n_in,int mode)
{
	n=n_in;
	A=A_in;
	f=f_in;
	u=new double[n];
	Af=new double*[n];
	u_solve=new double[n];
	
	//заполняем расширенную матрицу из A
	for(int i=0;i<n;i++)
	{
		Af[i]=new double[n+1];
		for(int j=0;j<n;j++)
		{
			Af[i][j]=A_in[i][j];
		}
	}

/*	for(int i=0;i<n;i++)
	{
		sum=0;
		for(int j=0;j<n;j++)
		{
			sum=sum+A[i][j]*u[j];
		}
		f[i]=sum;
	}*/
	
	//заполняем расширенную матрицу правой частью уравн
	for(int i=0;i<n;i++)
	{
		Af[i][n]=f[i];
	}
}

double method::get_norm(double *r,int n)
{
	double max=abs(r[0]);
	for(int i=1;i<n;i++)
	{
		if(max<abs(r[i])) max=abs(r[i]);
	}
	return max;
}

double* method::get_difference(double *r1,double *r2,int n)
{
	double *result=new double[n];
	for(int i=0;i<n;i++)
	{
		result[i]=r1[i]-r2[i];
	}
	return result;
}

double* method::get_err()
{
//return get_norm(get_difference(u,u_solve,n),n);
	return get_difference(u,u_solve,n);
}

void method::reposition_column(int i, int j)
{
	double buff;
	for(int k=0;k<n;k++)
	{
		buff=A[k][i];
		A[k][i]=A[k][j];
		A[k][j]=buff;

		buff=Af[k][i];
		Af[k][i]=Af[k][j];
		Af[k][j]=buff;

	}
}

void method::reposition_line(int i, int j)
{
	double buff;
	for(int k=0;k<n;k++)
	{
		buff=A[i][k];
		A[i][k]=A[j][k];
		A[j][k]=buff;

		buff=Af[i][k];
		Af[i][k]=Af[j][k];
		Af[j][k]=buff;

	}

}

double* method::get_solve()
{
	return 0;
}

method::~method()
{
if(mode==0)
{
delete(f);
delete(u_solve);
	for(int i=0;i<n;i++)
	{
		delete(Af[i]);
	}
}
}

double method::get_err_revmatr(double **rev)
{
	double **e=new double*[n];
	for(int i=0;i<n;i++)
	{
		e[i]=new double[n];
	}

	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			if(i==j) e[i][j]=1;
			else e[i][j]=0;
		}
	}

	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			e[i][j]=abs(e[i][j]-rev[i][j]);
		}
	}

	double max=e[0][0];
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			if(max<e[i][j]) max=e[i][j];
		}
	}

	return max;
}

double** method::get_mult(double **A, double **B)
{
	double **buff=new double*[n];
	double sum;
	for(int i=0;i<n;i++)
	{
		buff[i]=new double[n];
	}

	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			sum=0;
			for(int k=0;k<n;k++)
			{
			sum=sum+A[i][k]*B[j][k];
			}
			buff[i][j]=sum;
		}
	}
return buff;
}

double** method::get_A()
{
	return A;
}

void method::print_f()
{
	FILE *g=fopen("output_f.txt","w");
	for(int i=0;i<n;i++)
	{
		fprintf(g,"%f ",f[i]);
	}
	fclose(g);
}

void method::print_u_solve()
{
	FILE *g=fopen("output_u_solve.txt","w");
	for(int i=0;i<n;i++)
	{
		fprintf(g,"%f ",u_solve[i]);
	}
	fclose(g);
}

void method::print_u()
{
	FILE *g=fopen("output_u.txt","w");
	for(int i=0;i<n;i++)
	{
		fprintf(g,"%f ",u[i]);
	}
	fclose(g);
}


void method::print_A()
{
	FILE *f=fopen("output_A.txt","w");
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
		fprintf(f,"%f ",A[i][j]);
		}
		fprintf(f,"\n");
	}
	fclose(f);
}