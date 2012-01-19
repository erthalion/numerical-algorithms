#include <iostream>
#include <conio.h>
#include <math.h>
#include "alg_progon.h"
/* [0,2]  h=1  x*=1.5  needing spline-s2*/

using namespace std;

void main()
{
double *a,*b,*c,*d;
double h;
double s;	//s2=a2+b2(x2-x*)+(1/2)c2(x2-x*)^2+(1/6)d2(x2-x*)^3
//double *p,*q;
double *a1,*b1,*g1,*f1,*c1;


double m4=1;	//maximum 4diff for f
double eps;
double begin,end;
double x;
int n=0;
int k;

cout<<"insert begin\n";
cin>>begin;
cout<<"insert end\n";
cin>>end;
cout<<"insert epsilon\n";
cin>>eps;
cout<<"insert x*\n";
cin>>x;

h=pow(eps/m4,1/4);


for(double i=begin;i<=end;i=i+h,n++)
{
cout<<"x_"<<n<<"="<<i<<"\n";
cout<<"abs(i-x)/h="<<abs(i-x)/h<<"\n";
if(abs(i-x)/h<=0.5)
{
cout<<"k found\n";
k=n;
}
getch();
//n++;
}
cout<<"n="<<n<<"\n";
cout<<"x in "<<k<<"\n";
getch();

//p=new double[n+1];
//q=new double[n+1];
//c=new double[n+1];
d=new double[n+1];
b=new double[n+1];
a=new double[n+1];
int i;

c1=new double[n+1];
g1=new double[n+1];
b1=new double[n+1];
a1=new double[n+1];
f1=new double[n+1];

/*p[0]=0;
for(i=1;i<=n;i++)
{
	p[i]=-h/(4.0*h+h*p[i-1]);
}

q[0]=0;
for(i=1;i<=n;i++)
{
	q[i]=(sin(begin+(i-1)*h)-h*q[i-1])/(4.0*h+h*p[i-1]);
}

c[n]=0;
c[0]=0;
for(i=n-1;i>0;i--)
{
	c[i]=p[i+1]*c[i+1]+q[i+1];
}*/

f1[0]=NULL;
for(i=1;i<=n;i++)
{
	f1[i]=sin(begin+i*h);
}

a1[0]=NULL;
for(i=1;i<=n;i++)
{
	a1[i]=h;
}

b1[0]=NULL;
for(i=1;i<=n;i++)
{
	b1[i]=4.0*h;
}

g1[0]=NULL;
for(i=1;i<=n;i++)
{
	g1[i]=h;
}

c=alg_progon(a1,b1,g1,f1,c1,n);

/*for(i=1;i<=n;i++)
{
	cout<<"c="<<c[i]<<" c2="<<c2[i]<<"\n";
}
getch();*/

d[0]=NULL;
for(i=1;i<=n;i++)
{
	d[i]=(c[i-1]-c[i])/h;
}

b[0]=NULL;
for(i=1;i<=n;i++)
{
	b[i]=((sin(begin+(i-1)*h)-sin(begin+i*h))/h)-(1/2)*c[i]*h-(1/6)*(c[i-1]-c[i])*h;
}

a[0]=NULL;
for(i=1;i<=n;i++)
{
	a[i]=sin(begin+i*h);
}

double xk;
xk=begin+k*h;
s=a[k]+b[k]*(xk-x)+(1/2)*c[k]*pow((xk-x),2)+(1/6)*d[k]*pow((xk-x),3);

cout<<"sin(x)="<<sin(x)<<"\n";
cout<<"sk="<<s<<"\n";
cout<<"|f-s|="<<abs(sin(x)-s)<<"\n";
getch();

/*p=-h/(4.0*h);		//p2,,,p1=0
q=sin(2.0)/(4*h);	//q2,,,q1=0
c=0;			//cn=0
c1=sin(2.0)/4.0;	//c1,,,cn=0
d=c1/h;			//d2
b=((sin(1.0)-sin(2.0))/h)-0.5*c*h-(1/6.0)*c1*h;	//b2
a=sin(2.0);	//a2

s=a+b*(2.0-1.5)+0.5*c*pow((2.0-1.5),2)+(1/6.0)*d*pow((2.0-1.5),3);	//s2(x*)

cout<<"sin(1.5)="<<sin(1.5)<<"\n";
cout<<"s2(1.5)="<<s<<"\n";
cout<<"|f-s|="<<abs(sin(1.5)-s)<<"\n";*/
//getch();
}