#include "stdafx.h"
#include "shuttle.h"

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