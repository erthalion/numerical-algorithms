//#include "stdafx.h"


double* alg_progon(double* a,double* b,double* g,double* f,double* c,int n)
{
	double *p,*q;
	p=new double[n+1];
	q=new double[n+1];
	
	int i;

p[0]=0;
for(i=1;i<=n;i++)
{
	p[i]=-g[i]/(b[i]+a[i]*p[i-1]);
}

q[0]=0;
for(i=1;i<=n;i++)
{
	q[i]=(f[i-1]-a[i]*q[i-1])/(b[i]+a[i]*p[i-1]);
}

c[n]=0;
c[0]=0;
for(i=n-1;i>0;i--)
{
	c[i]=p[i+1]*c[i+1]+q[i+1];
}

delete p;
delete q;

return c;

}