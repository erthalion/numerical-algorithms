#include "stdafx.h"

typedef double (WINAPI *GetFunction)(double);
typedef double (WINAPI *GetMaximum)(double,double,int);
//typedef char* (WINAPI *GetFormula)();


Graph::Graph(double w,double h,double m,double n,double b,double e,HMODULE my_dll)
{
	o_x=w/2;o_y=h/2;width=w;height=h;mult_coeff=m;nodes=n*0.1;begin=b;end=e;delta=(e-b)/n;
	dll=my_dll;
	o_x1=o_x;o_y1=o_y;
	node=n;
	val_func=NULL;
}


double Graph::func_polynom(double x)
{
	double i,j;
	double mult;
	double sum=0;
	int n;
	GetFunction function;
	function=(GetFunction)GetProcAddress(dll,"GetFunction");

	if(function==NULL)
	{
		MessageBoxA(NULL,"import function fail(GetFunction)","error",MB_OK);
		return 0;
	}

	if(val_func==NULL)
	{
	val_func=new double[node];
	sum=0;
	for(i=begin,n=0;i<=end,n<=node;i=i+delta,n++)
	{
		mult=1;
		for(j=begin;j<end;j=j+delta)
		{
			if(j!=i)
			{
				//cout<<"x= "<<x<<" ";
				mult=mult*(x-j)/(i-j);
				//cout<<"mult "<<mult<<"\n";
			}
		}

		sum=sum+mult*function(i);
		val_func[n]=function(i);
		//cout<<"sum i "<<sum<<"\n";

//	char str[100];
//	MessageBoxA(NULL,itoa(val_func[n]*10,str,10),"",MB_OK);
	}

	}
	else
	{
	sum=0;
	for(i=begin,n=0;i<=end,n<=node;i=i+delta,n++)
	{
		mult=1;
		for(j=begin;j<end;j=j+delta)
		{
			if(j!=i)
			{
				//cout<<"x= "<<x<<" ";
				mult=mult*(x-j)/(i-j);
				//cout<<"mult "<<mult<<"\n";
			}
		}
		sum=sum+mult*val_func[n];
	//	val_func[n]=function(i);
	//cout<<"sum i "<<sum<<"\n";
	}

	}


return sum;
}


double Graph::get_o_x()
{
	return o_x;
}

double Graph::get_o_y()
{
	return o_y;
}

/////////////////////////
double Graph::get_o_x1()
{
	return o_x1;
}

double Graph::get_o_y1()
{
	return o_y1;
}
//////////////////////
double Graph::get_mult()
{
	return mult_coeff;
}

double Graph::get_begin()
{
	return begin;
}

double Graph::get_end()
{
	return end;
}

void Graph::paint_graph(CClientDC *dc,CRect *dlg_rc,double x,double y)
{
	CRect *rc;
	COLORREF backColor;
	backColor = RGB(255,255,255);  //replace with desired background color
	CBrush backBrush (backColor);
	CBrush* pOldBackBrush;
	double next_x=0,next_y=0,prev_x=0,prev_y=0;

	rc=dlg_rc;

	pOldBackBrush = dc->SelectObject(&backBrush);
	dc->Rectangle(40, 80, rc->Width()-40, rc->Height()-80);

	//Graph graph(rc.Width(),rc.Height(),20,40,-10,10);//width,height,mult_coeff,nodes,begin,end

	o_x1=o_x1+x;
	o_y1=o_y1+y;


	CPen pen1(PS_SOLID,1,RGB(0,0,0));
	CPen pen2(PS_SOLID,3,RGB(0,0,0));

////////show Ox,Oy////////
	dc->SelectObject(&pen1);
	
	if(	o_x1<rc->Width()-40&&
		o_x1>40)
	{
	dc->MoveTo(o_x1,80);
	dc->LineTo(o_x1,height-80);

	}
	if(	o_y1<rc->Height()-80&&
		o_y1>80)
	{
	dc->MoveTo(40,o_y1);
	dc->LineTo(width-40,o_y1);
	}
/////////////////////////
	//down - +y
	//right - +x

	dc->SelectObject(&pen2);
	prev_x=begin;
	prev_y=-func_polynom(begin);
	float i;
	double d=0.5/mult_coeff;
	if(d>1) d=1;
	if(d<0.0001) d=0.001;
	for(i=begin;i<end;i=i+d)//this delta must be <(end-begin)/nodes
	{
	//next_x=next_x+0.1;
	next_x=i;
	next_y=-func_polynom(next_x);
	if(	(o_x1+next_x*mult_coeff)<rc->Width()-45&&
		(o_x1+next_x*mult_coeff)>45&&
		(o_y1+next_y*mult_coeff)<rc->Height()-85&&
		(o_y1+next_y*mult_coeff)>85&&
		(o_x1+prev_x*mult_coeff)<rc->Width()-45&&
		(o_x1+prev_x*mult_coeff)>45&&
		(o_y1+prev_y*mult_coeff)<rc->Height()-85&&
		(o_y1+prev_y*mult_coeff)>85
		)
	{
	dc->MoveTo(o_x1+prev_x*mult_coeff,o_y1+prev_y*mult_coeff);
	dc->LineTo(o_x1+next_x*mult_coeff,o_y1+next_y*mult_coeff);
	}
	prev_x=next_x;
	prev_y=next_y;
	
	}

}


double Graph::theor(double x)
{
	int i=0;
	double mult=1,fact=1;
	double j;
	GetMaximum function;
	function=(GetMaximum)GetProcAddress(dll,"GetMaximum");
	
	if(function==NULL)
	{
		MessageBoxA(NULL,"import function fail(GetMaximum)","error",MB_OK);
		return 0;
	}
	

	for(j=begin;j<end;j=j+delta)
	{
			mult=mult*(x-j);//*function(begin,end,(end-begin)/delta+1);
			fact=fact*(i+1);
	i++;
	}


	return abs(mult*function(begin,end,(end-begin)/delta+1)/fact);
}

double Graph::pract(double x)
{
	GetFunction function;
	function=(GetFunction)GetProcAddress(dll,"GetFunction");

	if(function==NULL)
	{
		MessageBoxA(NULL,"import function fail(GetFunction)","error",MB_OK);
		return 0;
	}


	return abs(function(x)-func_polynom(x));
}


void Graph::paint_graph_orig(CClientDC *dc,CRect *dlg_rc,double x,double y)
{
	CRect *rc;
//	COLORREF backColor;
//	backColor = RGB(255,255,255);  //replace with desired background color
//	CBrush backBrush (backColor);
//	CBrush* pOldBackBrush;
	double next_x=0,next_y=0,prev_x=0,prev_y=0;

	rc=dlg_rc;

//	pOldBackBrush = dc->SelectObject(&backBrush);
//	dc->Rectangle(40, 80, rc->Width()-40, rc->Height()-80);

	//Graph graph(rc.Width(),rc.Height(),20,40,-10,10);//width,height,mult_coeff,nodes,begin,end

	o_x1=o_x1+x;
	o_y1=o_y1+y;


	CPen pen1(PS_SOLID,1,RGB(0,0,0));
	CPen pen2(PS_SOLID,3,RGB(200,0,0));

////////show Ox,Oy////////
/*	dc->SelectObject(&pen1);
	
	if(	o_x1<rc->Width()-40&&
		o_x1>40)
	{
	dc->MoveTo(o_x1,80);
	dc->LineTo(o_x1,height-80);

	}
	if(	o_y1<rc->Height()-80&&
		o_y1>80)
	{
	dc->MoveTo(40,o_y1);
	dc->LineTo(width-40,o_y1);
	}
	*/
/////////////////////////
	//down - +y
	//right - +x
	GetFunction function;
	function=(GetFunction)GetProcAddress(dll,"GetFunction");


	dc->SelectObject(&pen2);
	prev_x=begin;
	prev_y=-func_polynom(begin);
	float i;
	double d=0.5/mult_coeff;
	if(d>1) d=1;
	if(d<0.0001) d=0.001;

	for(i=begin;i<end;i=i+d)//this delta must be <(end-begin)/nodes
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
		(o_y1+prev_y*mult_coeff)<rc->Height()-85&&
		(o_y1+prev_y*mult_coeff)>85
		)
	{
	dc->MoveTo(o_x1+prev_x*mult_coeff,o_y1+prev_y*mult_coeff);
	dc->LineTo(o_x1+next_x*mult_coeff,o_y1+next_y*mult_coeff);
	}
	prev_x=next_x;
	prev_y=next_y;
	
	}

}


void Graph::set_mult(double mult)
{
	mult_coeff=mult;
}