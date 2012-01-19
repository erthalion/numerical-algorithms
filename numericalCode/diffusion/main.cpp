#include <stdio.h>
#include <math.h>
#include "CSolve.h"

//first_eq C(n+1/2)[i][j]/(tau/2) + u[i][j] * (C(n+1/2)[i+1][j] - C(n+1/2)[i-1][j]) / (2*hx) - D * (C(n+1/2)[i+1][j] - 2*C(n+1/2)[i][j] + C(n+1/2)[i-1][j])/ (hx*hx) = -(v[i][j]-vs)*(C(n)[i][j+1]-C(n)[i][j-1])/(2*hy) +D*(C(n)[i][j+1] - 2*C(n)[i][j] + C(n)[i][j-1])/ (hy*hy) + C(n)[i][j]/(tau/2)

//second_eq C(n+1)[i][j]/(tau/2) + (v[i][j]-vs)*(C(n+1)[i][j+1]-C(n+1)[i][j-1])/(2*hy) -D*(C(n+1)[i][j+1] - 2*C(n+1)[i][j] + C(n+1)[i][j-1])/ (hy*hy) = -(u[i][j] * (C(n+1/2)[i+1][j] - C(n+1/2)[i-1][j]) / (2*hx)) + D*(C(n+1/2)[i+1][j] - 2*C(n+1/2)[i][j] + C(n+1/2)[i-1][j])/ (hx*hx)) + C(n+1/2)[i][j]/(tau/2)


//алгоритм прогонки
double* shuttle(double *A,double *C,double *B,double *F,int n)
{
double *C1=new double[n];
double *F1=new double[n];
double *x=new double[n];


	C1[0]=C[0];
	for(int i=1;i<n;i++)
	{
	C1[i]=C[i]-A[i]*B[i-1]/C1[i-1];
	}

	F1[0]=F[0];
	for(int i=1;i<n;i++)
	{
	F1[i]=F[i]-A[i]*F1[i-1]/C1[i-1];
	}

	x[n-1]=F1[n-1]/C1[n-1];
	for(int i=n-2;i>=0;i--)
	{
	x[i]=(F1[i]-B[i]*x[i+1])/C1[i];
	}
	
	return x;

	delete(C1);
	delete(F1);
	delete(x);
}


void print(char *file,int nx,int ny,double **C)
{
	FILE *f=fopen(file,"w");

    for(int i=0;i<nx;i++)
    {
        for(int j=0;j<ny;j++)
        {
            fprintf(f,"%2.2f ", C[i][j]);
        }
		fprintf(f,"\n");
    }
	fclose(f);
}

int main()
{
	int nx=40,ny=40,T;
	double tau=0.01;
	double hx,hy,lenght=1,hight=1;
	hx=(double)1/(nx-1);
	hy=(double)1/(ny-1);
	int iter=0;

	FILE *f;
	f=fopen("out.dat","w");
	fprintf(f,"TITLE=\"USERData\"\n");
	fprintf(f,"VARIABLES=x,y,n");

	
	double vs=0,D=0.05;
	int k=0;
	double eps=0.001;

	double **C=new double *[nx];
	double **u=new double *[nx];
	double **v=new double *[nx];
	
	double *AA=new double[nx];
	double *CC=new double[nx];
	double *FF=new double[nx];
	double *BB=new double[nx];

	double *temp_x=new double[nx];
	double *temp_y=new double[ny];
	
	for(int i=0;i<nx;i++)
	{
	C[i]=new double[ny];
	u[i]=new double[ny];
	v[i]=new double[ny];

	for(int j=0;j<ny;j++)
	{
		C[i][j]=0;
		u[i][j]=0;//0.01;
		v[i][j]=0;//0.01;
	}

	}
	
	double *left=new double[3];
	double *right=new double[3];
	double *down=new double[3];

	//левая граница
	left[0]=0.1;
	left[1]=0.4;
	
	//скорость на левой границе
	left[2]=0;

	//правая граница
	right[0]=0.7;
	right[1]=0.4;
	
	//скорость на правой границе
	right[2]=-5;

	//нижняя граница
	down[0]=0.7;
	down[1]=0.4;
	
	//скорость на нижней границе
	down[2]=5;



	//решить первую задачу
	CSolve *laplas=new CSolve();
	laplas->Create(nx,ny,1,1,k,eps);
	laplas->CreateGrid(1,left,1,right,0,NULL,1,down);
	laplas->Print_Grid("out_grid.txt");
	laplas->Solve();
	laplas->Print("laplas.dat");
	
	//найти скорости, описывающие нужную конфигурацию течения (входы-выходы)
	//u=laplas->Get_u();
	//v=laplas->Get_v();


	for(int i=0;i<nx;i++)
	{
		for(int j=0;j<ny;j++)
		{
			u[i][j]=laplas->Get_v()[nx-1-i][j];
			v[i][j]=laplas->Get_u()[nx-1-i][ny-1-j];
		}
	}

	print("out_u.txt",nx,ny,u);
	print("out_v.txt",nx,ny,v);

	laplas->Print_uv("out_uv.dat");
		
	while(iter<=500)
	{
	
	iter++;
	printf("iter %d\n",iter);
	
	fprintf(f,"\nZONE T=\"ZONE %d\", i=%d j=%d f=Point",iter, nx, ny);

	////////////////////////////////////////////
	//начальное значение концентрации
	for(int i=nx*0.3;i<nx*0.7;i++)
	{
		C[0][i]=1;
	}
	////////////////////////////////////////////

	//зафиксировать i
	
	//i=0
	for(int j=1;j<ny-1;j++)
	{
	AA[j]=(v[0][j]-vs)/(2*hy)-D/(hy*hy);
	CC[j]=1/(tau/2)+2*D/(hy*hy);
	BB[j]=-(v[0][j]-vs)/(2*hy)-D/(hy*hy);
	FF[j]=(-u[0][j]/(2*hy)+D/(hy*hy))*C[1][j]+(1/(tau/2)-2*D/(hy*hy))*C[0][j];//+(u[i][j]/(2*hy)+D/(hy*hy))*C[i-1][j];
	}

	////на границе
	AA[0]=0;
	CC[0]=1/hy;
	BB[0]=-1/hy;
	FF[0]=0;

	AA[ny-1]=1/hy;
	CC[ny-1]=-1/hy;
	BB[ny-1]=0;
	FF[ny-1]=0;
	/////////////

	//C[0]=shuttle(AA,CC,BB,FF,ny);
	temp_y=shuttle(AA,CC,BB,FF,ny);

	for(int j=0;j<ny;j++)
	{
		C[0][j]=temp_y[j];
	}

	//внутри
	for(int i=1;i<nx-1;i++)
	{
		for(int j=1;j<ny-1;j++)
		{
		AA[j]=(v[i][j]-vs)/(2*hy)-D/(hy*hy);
		CC[j]=1/(tau/2)+2*D/(hy*hy);
		BB[j]=-(v[i][j]-vs)/(2*hy)-D/(hy*hy);
		FF[j]=(-u[i][j]/(2*hy)+D/(hy*hy))*C[i+1][j]+(1/(tau/2)-2*D/(hy*hy))*C[i][j]+(u[i][j]/(2*hy)+D/(hy*hy))*C[i-1][j];
		}

		//на границе
		AA[0]=0;
		CC[0]=1/hy;
		BB[0]=-1/hy;
		FF[0]=0;

		AA[ny-1]=1/hy;
		CC[ny-1]=-1/hy;
		BB[ny-1]=0;
		FF[ny-1]=0;
		///////////

		//C[i]=shuttle(AA,CC,BB,FF,ny);
		temp_y=shuttle(AA,CC,BB,FF,ny);

		for(int j=1;j<ny-1;j++)
		{
			C[i][j]=temp_y[j];
		}

	}

	//i=nx-1
	for(int j=1;j<ny-1;j++)
	{
	AA[j]=(v[nx-1][j]-vs)/(2*hy)-D/(hy*hy);
	CC[j]=1/(tau/2)+2*D/(hy*hy);
	BB[j]=-(v[nx-1][j]-vs)/(2*hy)-D/(hy*hy);
	FF[j]=/*(-u[i][j]/(2*hy)+D/(hy*hy))*C[i+1][j]+*/(1/(tau/2)-2*D/(hy*hy))*C[nx-1][j]+(u[nx-1][j]/(2*hy)+D/(hy*hy))*C[nx-2][j];
	}

	////на границе
	AA[0]=0;
	CC[0]=1/hy;
	BB[0]=-1/hy;
	FF[0]=0;

	AA[ny-1]=1/hy;
	CC[ny-1]=-1/hy;
	BB[ny-1]=0;
	FF[ny-1]=0;
	/////////////

	//C[nx-1]=shuttle(AA,CC,BB,FF,ny);
	temp_y=shuttle(AA,CC,BB,FF,ny);

	for(int j=0;j<ny;j++)
	{
		C[nx-1][j]=temp_y[j];
	}


	//зафиксировать j
	
	
	//j=0
	for(int i=1;i<nx-1;i++)
	{
	AA[i]=u[i][0]/(2*hx)-D/(hx*hx);
	CC[i]=1/(tau/2)+2*D/(hx*hx);
	BB[i]=-u[i][0]/(2*hx)-D/(hx*hx);
	FF[i]=(-(v[i][0]-vs)/(2*hy)+D/(hx*hx))*C[i][1]+(1/(tau/2)-2*D/(hx*hy))*C[i][0];//+((v[i][0]-vs)/(2*hx)+D/(hx*hx))*C[i][j-1];
	}

	////на границе
	AA[0]=0;
	CC[0]=1/hx;
	BB[0]=-1/hx;
	FF[0]=0;

	AA[nx-1]=1/hx;
	CC[nx-1]=-1/hx;
	BB[nx-1]=0;
	FF[nx-1]=0;
	/////////////

	//C[0]=shuttle(AA,CC,BB,FF,nx);
	temp_x=shuttle(AA,CC,BB,FF,nx);

	for(int i=0;i<nx;i++)
	{
		C[i][0]=temp_x[i];
	}

	//внутри
	for(int j=1;j<ny-1;j++)
	{
		for(int i=1;i<nx-1;i++)
		{
		AA[i]=u[i][j]/(2*hx)-D/(hx*hx);
		CC[i]=1/(tau/2)+2*D/(hx*hx);
		BB[i]=-u[i][j]/(2*hx)-D/(hx*hx);
		FF[i]=(-(v[i][j]-vs)/(2*hy)+D/(hx*hx))*C[i][j+1]+(1/(tau/2)-2*D/(hx*hy))*C[i][j]+((v[i][j]-vs)/(2*hx)+D/(hx*hx))*C[i][j-1];
		}

		//на границе
		AA[0]=0;
		CC[0]=1/hx;
		BB[0]=-1/hx;
		FF[0]=0;

		AA[nx-1]=1/hx;
		CC[nx-1]=-1/hx;
		BB[nx-1]=0;
		FF[nx-1]=0;
		/////////////

		//C[j]=shuttle(AA,CC,BB,FF,nx);
		temp_x=shuttle(AA,CC,BB,FF,nx);

		for(int i=1;i<nx-1;i++)
		{
			C[i][j]=temp_x[i];
		}

	}
	
	//j=ny-1
	for(int i=1;i<nx-1;i++)
	{
	AA[i]=u[i][ny-1]/(2*hx)-D/(hx*hx);
	CC[i]=1/(tau/2)+2*D/(hx*hx);
	BB[i]=-u[i][ny-1]/(2*hx)-D/(hx*hx);
	FF[i]=/*(-(v[i][ny-1]-vs)/(2*hy)+D/(hx*hx))*C[i][j+1]+*/(1/(tau/2)-2*D/(hx*hy))*C[i][ny-1]+((v[i][ny-1]-vs)/(2*hx)+D/(hx*hx))*C[i][ny-2];
	}


	////на границе
	AA[0]=0;
	CC[0]=1/hx;
	BB[0]=-1/hx;
	FF[0]=0.1;

	AA[nx-1]=1/hx;
	CC[nx-1]=-1/hx;
	BB[nx-1]=0;
	FF[nx-1]=0;
	/////////////


	//C[ny-1]=shuttle(AA,CC,BB,FF,nx);
	temp_x=shuttle(AA,CC,BB,FF,nx);

	for(int i=0;i<nx;i++)
	{
		C[i][ny-1]=temp_x[i];
	}



	////////////////////////////////////////////
	//начальное значение концентрации
	//for(int i=nx*0.3;i<nx*0.7;i++)
	//{
	//	C[0][i]=1;
	//}
	////////////////////////////////////////////

	print("out.txt",nx,ny,C);

    for(int i=0;i<nx;i++)
    {
        for(int j=0;j<ny;j++)
        {
            fprintf(f,"\n%2.8f %2.8f %2.8f", hx*i, hy*j, C[j][i]);
        }
    }

	
	}
	
	return 0;
}
