#include "stdafx.h"


integr::integr(double w, double h, double m, double b, double e, HMODULE dll, double my_eps,double my_eps2):Graph(w,h,m,1,b,e,dll)
{
eps=my_eps;
eps2=my_eps2;
	
	function_count=(GetCountPoint)GetProcAddress(dll,"GetCountPoint");
	if(function_count==NULL)
	{
		MessageBoxA(NULL,"import function fail(GetCountPoint)","error",MB_OK);
		return ;
	}

	function_point=(GetSingularPoint)GetProcAddress(dll,"GetSingularPoint");
	if(function_point==NULL)
	{
		MessageBoxA(NULL,"import function fail(GetSingularPoint)","error",MB_OK);
		return ;
	}

	point=function_point();

	function=(GetMaximum)GetProcAddress(dll,"GetMaximum");
	if(function==NULL)
	{
		MessageBoxA(NULL,"import function fail(GetMaximum)","error",MB_OK);
		return ;
	}

	function_x=(GetFunction)GetProcAddress(dll,"GetFunction");
	if(function_x==NULL)
	{
		MessageBoxA(NULL,"import function fail(GetFunction)","error",MB_OK);
		return ;
	}

	function_val=(GetValue)GetProcAddress(dll,"GetValue");
	if(function_val==NULL)
	{
		MessageBoxA(NULL,"import function fail(GetValue)","error",MB_OK);
		return ;
	}

	function_val_inf=(GetValueInf)GetProcAddress(dll,"GetValueInf");
	if(function_val_inf==NULL)
	{
		MessageBoxA(NULL,"import function fail(GetValueInf)","error",MB_OK);
		return ;
	}

	function_val_inf1=(GetValueInf1)GetProcAddress(dll,"GetValueInf1");
	if(function_val_inf1==NULL)
	{
		MessageBoxA(NULL,"import function fail(GetValueInf1)","error",MB_OK);
		return ;
	}

	function_val_inf2=(GetValueInf2)GetProcAddress(dll,"GetValueInf2");
	if(function_val_inf2==NULL)
	{
		MessageBoxA(NULL,"import function fail(GetValueInf2)","error",MB_OK);
		return ;
	}

	function_val_inf3=(GetValueInf3)GetProcAddress(dll,"GetValueInf3");
	if(function_val_inf3==NULL)
	{
		MessageBoxA(NULL,"import function fail(GetValueInf3)","error",MB_OK);
		return ;
	}

}

double* integr::first_formula()
{
	int n=0;
	double integr;
	double sum=0;
	double theor_err;
	double pract_err;
	double* ret;
	ret=new double[2];

	//find delta/////

	delta=pow(12.0*eps/((end-begin)*function(begin,end,2)),0.5);
	for(double i=begin;i<=end;i=i+delta,n++){}
	node=n;
	delta=(end-begin)/node;
	theor_err=pow(delta,2)*(end-begin)*function(begin,end,2)/12.0;
	
	//find integr////
	
	for(int i=1;i<node;i++)
	{
		sum=sum+function_x(begin+i*delta);
	}
	integr=delta*((function_x(begin)/2)+(function_x(end)/2)+sum);
	
	/////////////////
	
	pract_err=abs(integr-function_val(begin,end));

	ret[0]=delta;
	ret[1]=theor_err;
	ret[2]=pract_err;
	return ret;
}


double* integr::second_formula()
{
	int n=0;
	double integr=0;
	double sum_1=0;
	double sum_2=0;

	double theor_err;
	double pract_err;
	double* ret;
	ret=new double[2];

	//find delta

	delta=pow(180.0*eps/((end-begin)*function(begin,end,4)),0.25);
	for(double i=begin;i<=end;i=i+delta,n++){}	//проверить чтобы число отрезков 
												//при таком разбиении было кратно двум
	if(n%2!=0)	//проверяем на кратность N так как всего узлов N+1 отрезков--N
	{
		n++;
	}

	node=n;
	delta=(end-begin)/node;
	theor_err=pow(delta,4)*(end-begin)*function(begin,end,4)/180.0;

	//find integr
	
	for(int i=1;i<node;i=i+2)
	{
		sum_1=sum_1+function_x(begin+i*delta);
	}

	for(int i=2;i<node;i=i+2)
	{
		sum_2=sum_2+function_x(begin+i*delta);
	}

	integr=(delta/3.0)*(function_x(begin)+function_x(end)+4*sum_1+2*sum_2);

	
	////////////////

	pract_err=abs(integr-function_val(begin,end));

	ret[0]=delta;
	ret[1]=theor_err;
	ret[2]=pract_err;
	return ret;

}

double* integr::second_formula(double b, double e, double epsilon)
{
	int n=0;
	double integr=0;
	double sum_1=0;
	double sum_2=0;

	double theor_err;
	double pract_err;
	double* ret;
	ret=new double[2];

	//find delta

	delta=pow(180.0*epsilon/((e-b)*function(b,e,4)),0.25);
	for(double i=b;i<=e;i=i+delta,n++){}	//проверить чтобы число отрезков 
												//при таком разбиении было кратно двум
	if(n%2!=0)	//проверяем на кратность N так как всего узлов N+1 отрезков--N
	{
		n++;
	}

	node=n;
	delta=(e-b)/node;
	theor_err=pow(delta,4)*(e-b)*function(b,e,4)/180.0;

	//find integr
	
	for(int i=1;i<node;i=i+2)
	{
		sum_1=sum_1+function_x(b+i*delta);
	}

	for(int i=2;i<node;i=i+2)
	{
		sum_2=sum_2+function_x(b+i*delta);
	}

	integr=(delta/3.0)*(function_x(b)+function_x(e)+4*sum_1+2*sum_2);

	
	////////////////

	pract_err=abs(integr-function_val(b,e));

	ret[0]=integr;
	ret[1]=theor_err;
	ret[2]=pract_err;
	return ret;

}

double* integr::second_formula1(double b, double e, double h)
{
	int n=0;
	double integr=0;
	double sum_1=0;
	double sum_2=0;

	double theor_err;
	double pract_err;
	double* ret;
	ret=new double[2];

	//find delta

//	delta=pow(180.0*epsilon/((e-b)*function(b,e,4)),0.25);
	delta=h;
	for(double i=b;i<=e;i=i+delta,n++){}	//проверить чтобы число отрезков 
												//при таком разбиении было кратно двум
	if(n%2!=0)	//проверяем на кратность N так как всего узлов N+1 отрезков--N
	{
		n++;
	}

	node=n;
	delta=(e-b)/node;
	theor_err=pow(delta,4)*(e-b)*function(b,e,4)/180.0;

	//find integr
	
	for(int i=1;i<node;i=i+2)
	{
		sum_1=sum_1+function_x(b+i*delta);
	}

	for(int i=2;i<node;i=i+2)
	{
		sum_2=sum_2+function_x(b+i*delta);
	}

	integr=(delta/3.0)*(function_x(b)+function_x(e)+4*sum_1+2*sum_2);

	
	////////////////

	pract_err=abs(integr-function_val(b,e));

	ret[0]=integr;
	ret[1]=theor_err;
	ret[2]=pract_err;
	return ret;

}

double* integr::third_formula()
{
	int n=0;
	double integr;
	double sum_1=0;
	double sum_2=0;

	double theor_err;
	double pract_err;
	double* ret;
	ret=new double[2];

	//find delta

	delta=pow(80.0*eps/((end-begin)*function(begin,end,4)),0.25);
	for(double i=begin;i<=end;i=i+delta,n++){}	//проверить чтобы число отрезков 
												//при таком разбиении было кратно трем
//	char str[100];
//	MessageBoxA(NULL,itoa(n,str,10),"n=",MB_OK);

	if(n%3!=0)
	{
		n=n+(3-n%3);
	}

//	MessageBoxA(NULL,itoa(n,str,10),"n=",MB_OK);

	node=n;
	delta=(end-begin)/n;
	theor_err=pow(delta,4)*(end-begin)*function(begin,end,4)/80.0;
	
	//find integr

	for(int i=1;i<node;i++)
	{
		if(i%3==0)
		{
			sum_2=sum_2+function_x(begin+i*delta);
		}
		else
		{
			sum_1=sum_1+function_x(begin+i*delta);
		}
	}

//	for(int i=3;i<node;i=i+3)
//	{
//		sum_2=sum_2+function_x(begin+i*delta);
//	}


	integr=delta*(3.0/8.0)*(function_x(begin)+function_x(end)+3*sum_1+2*sum_2);

	////////////////

	pract_err=abs(integr-function_val(begin,end));
//pract_err=(integr-function_val(begin,end));
	ret[0]=delta;
	ret[1]=theor_err;
	ret[2]=pract_err;
	return ret;


	
}

void integr::flood_fill(CClientDC *dc,CRect *dlg_rc,double x,double y)
{
	CRect *rc;
	double next_x=0,next_y=0,prev_x=0,prev_y=0;

	o_x1=o_x1+x;
	o_y1=o_y1+y;

	rc=dlg_rc;
	CPen pen1(PS_SOLID,1,RGB(0,0,0));

	GetFunction function;
	function=(GetFunction)GetProcAddress(dll,"GetFunction");


	dc->SelectObject(&pen1);
	prev_x=begin;
	prev_y=-function(begin);
	double i;
	double d=0.2;

//	double d=5/mult_coeff;
//	if(d>1) d=1;
//	if(d<0.0001) d=0.001;

	for(i=begin;i<=end+d;i=i+d)//this delta must be <(end-begin)/nodes
	{
	//next_x=next_x+0.1;
	next_x=i;
	next_y=-function(next_x);
	if(	(o_x1+next_x*mult_coeff)<rc->Width()-45&&
		(o_x1+next_x*mult_coeff)>45&&
		(o_y1+next_y*mult_coeff)<rc->Height()-85&&
		(o_y1+next_y*mult_coeff)>85&&
		(o_x1+prev_x*mult_coeff)<rc->Width()-45&&
		(o_x1+prev_x*mult_coeff)>45&&
		(o_y1)<rc->Height()-85&&
		(o_y1)>85&&
		(o_y1+prev_y*mult_coeff)<rc->Height()-85&&
		(o_y1+prev_y*mult_coeff)>85
		)
	{
	dc->MoveTo(o_x1+prev_x*mult_coeff,o_y1+prev_y*mult_coeff);
	dc->LineTo(o_x1+prev_x*mult_coeff,o_y1);
	}
	prev_x=next_x;
	prev_y=next_y;
	
	}


/*	if(	(o_x1+next_x*mult_coeff)<rc->Width()-45&&
		(o_x1+next_x*mult_coeff)>45&&
		(o_y1+next_y*mult_coeff)<rc->Height()-85&&
		(o_y1+next_y*mult_coeff)>85&&
		(o_x1+prev_x*mult_coeff)<rc->Width()-45&&
		(o_x1+prev_x*mult_coeff)>45&&
		(o_y1)<rc->Height()-85&&
		(o_y1)>85&&
		(o_y1+prev_y*mult_coeff)<rc->Height()-85&&
		(o_y1+prev_y*mult_coeff)>85
		)
	{
	dc->MoveTo(o_x1+(end-d)*mult_coeff,o_y1+(-1)*function_x(end-d)*mult_coeff);
	dc->LineTo(o_x1+(end-d)*mult_coeff,o_y1);

	prev_x=next_x;
	prev_y=next_y;

	dc->MoveTo(o_x1+end*mult_coeff,o_y1+(-1)*function_x(end)*mult_coeff);
	dc->LineTo(o_x1+end*mult_coeff,o_y1);
	}*/

}


double integr::find_all_int(char *b, double e, double epsilon2)
{
////////////////////////////////////////////
	double* integr;
	double* prev=0;
	double d;	//подобрать
	double sum=0,sum1=0;
	double a;
	double round,step;	//подобрать
	int i=0;
	int n=0;
	n=function_count();
	while(i<=n&&point[i][0]<=e)
	{
		if(point[i][3]==0)
		{
			MessageBoxA(NULL,"integr not exist","error",MB_OK);
			return 0;
		}
		i++;
	}
	i=0;
	sum_err=0;
////////до какой точки идти от -бесконечности////////////

	round=1;
	step=1;

	if(n==0||point[0][0]>=e)
	{
		a=e;
	}
	else
	{
		a=point[0][0]-round;
	}
////////////считаем от -бесконечности//////////////

	d=epsilon2+1;
//	prev=second_formula1(a,a+step,eps);
	prev=second_formula1(a-step,a,eps);
	a=a-step;
	sum1=sum1+*prev;

	while(d>epsilon2)
	{
		integr=second_formula1(a-step,a,eps);
		sum_err=sum_err+integr[1];
		d=abs(integr[0]-prev[0]);
		sum1=sum1+*integr;
		*prev=*integr;
		a=a-step;
	}
	
	sum=sum1;
////////////////////////////////////////////////////

	i=0;
	while(i<=n&&point[i][0]+round<=e)
	{
/////////befor point[i] on [point-round,point]///////
		a=point[i][0]-round;
		step=round/2;
		sum1=0;

		prev=second_formula1(a,a+step,eps);
		sum1=sum1+*prev;
		a=a+step;
		step=step/2;

		d=epsilon2+1;
		while(d>epsilon2)
		{
			integr=second_formula1(a,a+step,eps);
			sum_err=sum_err+integr[1];
			d=abs(integr[0]-prev[0]);
			sum1=sum1+*integr;
			*prev=*integr;
			a=a+step;
			step=step/2;
		}
		sum=sum+sum1;

///////correction round////////////////////////////
	if((i+1)<=n&&point[i+1][0]<=e)
	{
		round=(point[i+1][0]-point[i][0])/2;
	}
	else
	{
		round=e-point[i][0];
	}

////////after point[i] on [point,point+round]///////
		a=point[i][0]+round;
		step=round/2;
		sum1=0;

//		prev=second_formula(a,a+step,eps);
		prev=second_formula1(a-step,a,eps);
		a=a-step;
		step=step/2;
		sum1=sum1+*prev;
		sum_err=sum_err+prev[1];

		d=epsilon2+1;
		while(d>epsilon2)
		{
			integr=second_formula1(a-step,a,eps);
			sum_err=sum_err+integr[1];
			d=abs(integr[0]-prev[0]);
			sum1=sum1+*integr;
			*prev=*integr;
			a=a-step;
			step=step/2;
		}
		sum=sum+sum1;
		i++;
	}
integr_val=sum;

////////////////////////////////////////////////////
pract_err=abs(integr_val-function_val_inf1(end));
return sum;
}

double integr::find_all_int(double b, char* e, double epsilon2)
{
////////////////////////////////////////////
	double* integr;
	double* prev=0;
	double d;	//подобрать
	double sum=0,sum1=0;
	double a;
	double round,step;	//подобрать
	int i=0;
	int n=0;	//count ingular point

	n=function_count();
	while(i<=n&&point[i][0]>=b)
	{
		if(point[i][3]==0)
		{
			MessageBoxA(NULL,"integr not exist","error",MB_OK);
			return 0;
		}
		i++;
	}
	i=0;

	sum_err=0;
////////////////////////////////////////////////////
//if(point[0]!=NULL)
//{
//	round=point[0]-b;
//}
	while(i<n&&point[i][0]<b){i++;}
	round=abs(point[i-1][0]-b);

	if(round==0&&i>0){round=abs((point[i-1][0]-point[i][0])/2.0);}
//	if(round==0&&i==0){round=1;}
	if(round==0&&i==0){point[0][2]=0,round=1;}


//	i=0;
	while(i<n)
	{
		if(point[i][2]==1)
		{
/////////befor point[i] on [point-round,point]///////
		a=point[i][0]-round;
		step=round/2;
		sum1=0;

//		prev=second_formula(a-step,a,eps);
		prev=second_formula1(a,a+step,eps);
		sum_err=sum_err+prev[1];
		sum1=sum1+*prev;
		a=a+step;
		step=step/2;

		d=epsilon2+1;
		while(d>epsilon2)
		{
			integr=second_formula1(a,a+step,eps);
			sum_err=sum_err+integr[1];
			d=abs(integr[0]-prev[0]);
			sum1=sum1+*integr;
			*prev=*integr;
			a=a+step;
			step=step/2;
		}
		sum=sum+sum1;
		}
///////correction round////////////////////////////
	if((i+1)<n)
	{
		round=(point[i+1][0]-point[i][0])/2;
	}

		if(point[i][1]==1)
		{
////////after point[i] on [point,point+round]///////
		a=point[i][0]+round;
		step=round/2;
		sum1=0;

		prev=second_formula1(a-step,a,eps);
		sum_err=sum_err+prev[1];
		a=a-step;
		step=step/2;
		sum1=sum1+*prev;
		d=epsilon2+1;
		while(d>epsilon2)
		{
			integr=second_formula1(a-step,a,eps);
			sum_err=sum_err+integr[1];
			d=abs(integr[0]-prev[0]);
			sum1=sum1+*integr;
			*prev=*integr;
			a=a-step;
			step=step/2;
		}

		sum=sum+sum1;
		}
		i++;
	}

////////от какой точки идти до бесконечности////////////

	if(n==0)
	{
		a=b;
	}
	else
	{
		a=point[n-1][0]+round;
	}
////////////считаем до бесконечности//////////////

	step=10;
	sum1=0;
	prev=second_formula1(a,a+step,eps);
		sum_err=sum_err+prev[1];
		sum1=sum1+*prev;
		a=a+step;

	d=epsilon2+1;
	while(d>epsilon2)
	{
		integr=second_formula1(a,a+step,eps);
		sum_err=sum_err+integr[1];
		d=abs(integr[0]-prev[0]);
		sum1=sum1+*integr;
		*prev=*integr;
		a=a+step;
	}
	
	sum=sum+sum1;
	integr_val=sum;

pract_err=abs(integr_val-function_val_inf2(begin));

return sum;
}

double integr::find_all_int(double b, double e, double epsilon2)
{
////////////////////////////////////////////
	double* integr;
	double* prev=0;
	double d;	//подобрать
	double sum=0,sum1=0;
	double a;
	double round,step;	//подобрать
	int i=0;
	int n=0;	//count ingular point
	n=function_count();
	while(i<=n&&point[i][0]<=e&&point[i][0]>=b)
	{
		if(point[i][3]==0)
		{
			MessageBoxA(NULL,"integr not exist","error",MB_OK);
			return 0;
		}
		i++;
	}
	i=0;

	sum_err=0;

if(n==0)
{
	integr=second_formula1(b,e,eps);
	return integr[0];
}

	
	while(i<=n&&point[i][0]<b){i++;}
	round=abs(point[i][0]-b);

	if(round==0&&i>0){round=abs(point[i-1]-point[i])/2;}
	if(round==0&&i==0){point[0][2]=0,round=1;}

//}
	
	i=0;
	while(i<=n)
	{
		if(point[i][2]==1)
		{
/////////befor point[i] on [point-round,point]///////
		a=point[i][0]-round;
		step=round/2;
		sum1=0;

//		prev=second_formula(a-step,a,eps);
		prev=second_formula1(a,a+step,eps);
		sum1=sum1+*prev;
		sum_err=sum_err+prev[1];
		a=a+step;
		step=step/2;

		d=epsilon2+1;
		while(d>epsilon2)
		{
			integr=second_formula1(a,a+step,eps);
			sum_err=sum_err+integr[1];
			d=abs(integr[0]-prev[0]);
			sum1=sum1+*integr;
			*prev=*integr;
			a=a+step;
			step=step/2;
		}
		sum=sum+sum1;
		}
///////correction round////////////////////////////
	if((i+1)<=n&&point[i+1][0]<=e)
	{
		round=(point[i+1][0]-point[i][0])/2;
	}
	else
	{
		round=e-point[i][0];
	}
		if(point[i][1]==1)
		{
////////after point[i] on [point,point+round]///////
		a=point[i][0]+round;
		step=round/2;
		sum1=0;

		prev=second_formula1(a-step,a,eps);
		a=a-step;
		step=step/2;
		sum1=sum1+*prev;
		sum_err=sum_err+prev[1];
		d=epsilon2+1;
		while(d>epsilon2)
		{
			integr=second_formula1(a-step,a,eps);
			sum_err=sum_err+integr[1];
			d=abs(integr[0]-prev[0]);
			sum1=sum1+*integr;
			*prev=*integr;
			a=a-step;
			step=step/2;
		}

		sum=sum+sum1;
		}

		i++;
	}

integr_val=sum;
pract_err=abs(integr_val-function_val_inf3(begin,end));

return sum;
}

double integr::pract_error()
{
	return pract_err;
}

double integr::theor_err()
{
	return sum_err;
}

double integr::pract_error_inf()
{
	return function_val_inf();
}

double integr::pract_error_inf(char* b,double e)
{
	return function_val_inf1(e);
}
double integr::pract_error_inf(double b,char* e)
{
	return function_val_inf2(b);
}
double integr::pract_error_inf(double b,double e)
{
	return function_val_inf3(b,e);
}

/*if(a-step>a)
	{
	beg=a;en=a-step
	}
if(a>a+step)
	{
	beg=a;en=a+step
	}
*/