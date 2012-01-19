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

double method::get_err()
{
return get_norm(get_difference(u,u_solve,n),n);
}

/*double** method::get_inverse()
{
	double sum,a;
	double **B=new double*[n];
	double **A1=new double*[n];
	for(int i=0;i<n;i++)
	{
		B[i]=new double[n];
		A1[i]=new double[n+1];
	}

	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			A1[i][j]=Af[i][j];
		}
	}

for(int number=0;number<n;number++)
{
//начало счета [number]-го вектора из B
//заполняем правую часть векторами с единицей на месте [number]
	for(int i=0;i<n;i++)
	{
		if(number==i) A1[i][n]=1;
		else A1[i][n]=0;

	}

	for(int k=0;k<n;k++)
	{
		//цикл по k--номер строки,которую отнимаем от остальных
	for(int i=k+1;i<n;i++)
	{
		//запоминаем A[i][k] - первый элемент в изменяемой строке(поэтому и запоминаем)
		a=A1[i][k];
		if(a!=0)
		{
			//номер строк,от которых отнимаем k-ю
			for(int j=0;j<n+1;j++)	//release сделать j=i to...
			{
				//номер элемента,который считаем в i строке
				if(A1[k][k]!=0)
				{	
					//b=(Af[k][j]/Af[k][k])*a;
					A1[i][j]=A1[i][j]-(A1[k][j]/A1[k][k])*a;
				}
			}
		}
	}
	}
////////////////////////////////////////
//back
		for(int i=n-1;i>=0;i--)
		{
			// i - номер строки,где считаем неизвестное
			sum=0;
			for(int j=n-1;j>i;j--)
			{
				sum=sum+A1[i][j]*B[j][number];
			}
			B[i][number]=(A1[i][4]-sum)/A1[i][i];

		}
//конец цикла по векторам
}

return B;
}*/

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