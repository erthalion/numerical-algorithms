#include "stdafx.h"

diff::diff(double b,double e,int node,HMODULE m_dll)
{
	n=node;
	dll=m_dll;
	h=(e-b)/(n-1);
	begin=b;end=e;
	y=new double[n];

	p=new double[n];
	q=new double[n];
	f=new double[n];
	alfa=new double[1];
	beta=new double[1];
	gamma=new double[1];


	Get_y0 y0=(Get_y0)GetProcAddress(dll,"Get_y0");

	Get_p m_p=(Get_p)GetProcAddress(dll,"Get_p");
	Get_q m_q=(Get_q)GetProcAddress(dll,"Get_q");
	Get_f m_f=(Get_f)GetProcAddress(dll,"Get_f");
	
	Get_alfa m_alfa=(Get_alfa)GetProcAddress(dll,"Get_alfa");
	Get_beta m_beta=(Get_beta)GetProcAddress(dll,"Get_beta");
	Get_gamma m_gamma=(Get_gamma)GetProcAddress(dll,"Get_gamma");

	y[0]=y0(begin);

	for(int i=0;i<n;i++)
	{
		p[i]=m_p(i,i*h+begin);
		q[i]=m_q(i,i*h+begin);
		//f[i]=m_f(i,i*h+begin);
	}


	for(int i=0;i<2;i++)
	{
		alfa[i]=m_alfa()[i];
		beta[i]=m_beta()[i];
		//gamma[i]=m_gamma()[i];
	}

	gamma[0]=m_gamma(begin,end)[0];
	gamma[1]=m_gamma(begin,end)[1];

	for(int i=1;i<n-1;i++)
	{
		f[i]=m_f(i,i*h+begin);
	}

	f[0]=gamma[0];
	f[n-1]=gamma[1];

	F=(GetF)GetProcAddress(dll,"GetF");
	if(F==NULL)
	{
		MessageBoxA(NULL,"import function fail(GetF)","error",MB_OK);
		return ;
	}

}

double* diff::get_solve_euler()
{
	FILE *f=fopen("out.txt","w");
	for(int i=1;i<n;i++)
	{
	y[i]=y[i-1]+h*F(y[i-1],i*h+begin);
	fprintf(f,"%f %f\n",y[i],sin(i*h+begin));
	}
	fclose(f);

return y;
}

double* diff::get_solve_boundary()
{
	double *a=new double[n];
	double *b=new double[n];
	double *c=new double[n];

	double **A=new double*[n];
	for(int i=0;i<n;i++)
	{
	A[i]=new double[n];
	}

	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			A[i][j]=0;
		}	
	}


	a[0]=beta[0]-alfa[0]/h;
	a[n-1]=0;

	b[0]=alfa[0]/h;
	//b[n-1]=beta[1]+alfa[1]/h;
	c[n-1]=beta[1]+alfa[1]/h;

	c[0]=0;
	//c[n-1]=-alfa[1]/h;
	b[n-1]=-alfa[1]/h;

	for(int i=1;i<n-1;i++)
	{
		//a[i]=1.0/(h*h)+p[i]/(2*h);
		c[i]=1.0/(h*h)+p[i]/(2*h);
		b[i]=-2.0/(h*h)+q[i];
		//c[i]=1.0/(h*h)-1/(2*h);
		a[i]=1.0/(h*h)-p[i]/(2*h);
	}

	//y=alg_progon(a,b,c,f,y,n-1);
	for(int i=1;i<n-1;i++)
	{
	A[i][i]=b[i];
	A[i][i-1]=a[i];
	A[i][i+1]=c[i];
	}
	A[0][0]=a[0];
	A[0][1]=b[0];
	A[n-1][n-2]=b[n-1];
	A[n-1][n-1]=c[n-1];

method_gauss m(A,f,n,1);
y=m.get_solve(1);

//	for(int i=0;i<n;i++)
//	{
//	delete(A[i]);
//	}

	delete(a);
	delete(b);
	delete(c);

return y;
}

double diff::get_h()
{
	return h;
}

void diff::set_gamma(double g1,double g2)
{
	gamma[0]=g1;
	gamma[1]=g2;
	f[0]=g1;
	f[n-1]=g2;

}