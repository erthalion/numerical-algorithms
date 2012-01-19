#include "stdafx.h"

diffusion::diffusion(double in_D,double in_tau,double in_vs,double in_lenght,double in_hight,double **in_u,double **in_v,int in_nx,int in_ny)
{
	nx=in_nx;ny=in_ny;D=in_D;
	vs=in_vs;tau=in_tau;
	lenght=in_lenght;hight=in_hight;

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
			u[i][j]=in_u[i][j];
			v[i][j]=in_v[i][j];
		}
	}



}

double** diffusion::solve()
{
	return 0;
}