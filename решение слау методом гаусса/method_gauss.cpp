#include "stdafx.h"

method_gauss::method_gauss(double **A_in,double *f_in,int n_in):method(A_in,f_in,n_in)
{
}

method_gauss::method_gauss(double **A_in,double *f_in,int n_in,int mode):method(A_in,f_in,n_in,mode)
{
}

double* method_gauss::get_solve(int number)	//в отдельную функцию выделение переменной
{
//	double a,sum,max;
	double max;
	switch(number)
	{
	case 1:	//--основной гаусс
/////////////////////////////////
///////forward
				for(int k=0;k<n;k++)
				{
				//цикл по k--номер строки,которую отнимаем от остальных
				exclude(k);
				}
////////////////////////////////////////
//back
		return reverse();
		break;
////////////////////////////////////////
	case 2:	//--гаусс по столбцам
/////////////////////////////////
///////forward
			for(int k=0;k<n;k++)
			{
///////find max
				max=Af[k][k];	
				for(int i=k;i<n;i++)
				{
				if(max<abs(Af[k][i])) max=Af[k][i];
				}
//////reposition
				for(int i=k;i<n;i++)
				{
				if(max==Af[k][i]) reposition_column(k,i);
				}

				//цикл по k--номер строки,которую отнимаем от остальных
				exclude(k);
			}
////////////////////////////////////////
//back
		return reverse();
		break;
////////////////////////////////////////
	case 3:	//гаусс по строкам
/////////////////////////////////
///////forward
			for(int k=0;k<n;k++)
			{
///////find max
				max=Af[k][k];	
				for(int i=k;i<n;i++)
				{
				if(max<abs(Af[i][k])) max=Af[i][k];
				}
//////reposition
				for(int i=k;i<n;i++)
				{
				if(max==Af[i][k]) reposition_line(k,i);
				}

				//цикл по k--номер строки,которую отнимаем от остальных
				exclude(k);
			}
////////////////////////////////////////
//back
		return reverse();
		break;
////////////////////////////////////////
	case 4:	//--гаусс по всей матрице
/////////////////////////////////
///////forward
			for(int k=0;k<n;k++)
			{
///////find max for line
				max=Af[k][k];	
				for(int i=k;i<n;i++)
				{
				if(max<abs(Af[i][k])) max=Af[i][k];
				}
//////reposition
				for(int i=k;i<n;i++)
				{
				if(max==Af[i][k]) reposition_line(k,i);
				}
///////find max for column
				max=Af[k][k];	
				for(int i=k;i<n;i++)
				{
				if(max<abs(Af[k][i])) max=Af[k][i];
				}
//////reposition
				for(int i=k;i<n;i++)
				{
				if(max==Af[k][i]) reposition_column(k,i);
				}


				//цикл по k--номер строки,которую отнимаем от остальных
				exclude(k);
			}
////////////////////////////////////////
//back
		return reverse();
		break;
////////////////////////////////////////
	default:return 0;
	}
//return 0;
}

void method_gauss::exclude(int k)
{
	double a;
	for(int i=k+1;i<n;i++)
	{
		//запоминаем A[i][k] - первый элемент в изменяемой строке(поэтому и запоминаем)
		a=Af[i][k];
		if(a!=0)
		{
			//номер строк,от которых отнимаем k-ю
			for(int j=0;j<n+1;j++)	//release сделать j=i to...
			{
				//номер элемента,который считаем в i строке
				if(Af[k][k]!=0)
				{	
					//b=(Af[k][j]/Af[k][k])*a;
					Af[i][j]=Af[i][j]-(Af[k][j]/Af[k][k])*a;
				}
			}
		}
	}

	if(mode==0)
	{
	output();
	}
}

double* method_gauss::reverse()
{
	double sum;
		for(int i=n-1;i>=0;i--)
		{
			// i - номер строки,где считаем неизвестное
			sum=0;
			for(int j=n-1;j>i;j--)
			{
				sum=sum+Af[i][j]*u_solve[j];
			}
			u_solve[i]=(Af[i][n]-sum)/Af[i][i];

		}	
		if(mode==0)
		{
		out_vect();
		}
		return u_solve;
}

//method_gauss::~method_gauss()
//{
//delete(f);
//delete(u_solve);
//	for(int i=0;i<n;i++)
//	{
//		delete(Af[i]);
//	}
//
//}

void method_gauss::output()
{
	FILE *f;
	f=fopen("output.txt","w");
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n+1;j++)
		{
			fprintf(f,"%f ",Af[i][j]);
		}
		fprintf(f,"\n");
	}
	fclose(f);
		
}

void method_gauss::out_vect()
{
	FILE *f;
	f=fopen("output_vector.txt","w");
	for(int i=0;i<n;i++)
	{
		fprintf(f,"%f ",u_solve[i]);
	}
	fprintf(f,"\n\n");
	for(int i=0;i<n;i++)
	{
		fprintf(f,"%f ",u[i]);
	}

	fclose(f);
		
}

double** method_gauss::get_reverse_matr()
{
	method_gauss *buff;
	double **B=new double*[n];
	double *b=new double[n];
	double* f_new=new double[n];
	for(int i=0;i<n;i++)
	{
		B[i]=new double[n];
	}

	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			if(i==j) f_new[j]=1;
			else f_new[j]=0;
		}

		buff=new method_gauss(A,f_new,n,1);
		b=buff->get_solve(1);

		for(int j=0;j<n;j++)
		{
			B[j][i]=b[j];
		}
		delete(buff);

	}

	return B;
}