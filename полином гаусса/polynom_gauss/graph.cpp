#include "stdafx.h"

typedef double (WINAPI *GetFunction)(double);
typedef double (WINAPI *GetMaximum)(double,double,int);
//typedef char* (WINAPI *GetFormula)();


Graph::Graph(double w,double h,double m,double* max,double b,double e,double x,HMODULE my_dll)
{
	o_x=w/2;o_y=h/2;width=w;height=h;mult_coeff=m;begin=b;end=e;
	dll=my_dll;
	o_x1=o_x;o_y1=o_y;
	val_func=NULL;

	char str[100];

	GetMaximum function;
	function=(GetMaximum)GetProcAddress(dll,"GetMaximum");

	if(function==NULL)
	{
		MessageBoxA(NULL,"import function fail(GetMaximum)","error",MB_OK);
		return;
	}

//	MessageBoxA(NULL,itoa(function(begin,end,3)*100,str,10),"max|sin|",MB_OK);
//	MessageBoxA(NULL,itoa(max*100,str,10),"max(R)",MB_OK);
	h=pow(16*(*max)/function(begin,end,3),0.33);
	int n=0;
	double xq=1;
	
	for(double i=begin;i<=end;i=i+h,n++)
	{
		if((abs(x-i)/h)<=1)
		{
			x_q=i;
		if((abs(x-i)/h)<=0.5) 
		{
			x_q=i;
			i=i+h;
			n++;
			//MessageBoxA(NULL,itoa((abs(x-i)/h)*100,str,10),"t",MB_OK);
			//MessageBoxA(NULL,itoa(xq*100,str,10),"xq",MB_OK);
		}
		}
	}
	delta=(end-begin)/n;

	//get three point
	if(x_q+delta<end&&n<3)
	{
		delta=(end-begin)/3;
	}

	if(x_q+delta>end&&n<4)
	{
		delta=(end-begin)/3;
	}
	
	*max=(pow(delta,3)*function(begin,end,3))/(16);

	// may optimize - not resolve x_q if delta - const
	n=0;
	for(double i=begin;i<=end;i=i+delta,n++)
	{
		if((abs(x-i)/delta)<=1)
		{
			x_q=i;
		if((abs(x-i)/delta)<=0.5) 
		{
			x_q=i;
			i=i+delta;
			n++;
			//MessageBoxA(NULL,itoa((abs(x-i)/h)*100,str,10),"t",MB_OK);
			//MessageBoxA(NULL,itoa(xq*100,str,10),"xq",MB_OK);
		}
		}
	}
	
	if(x_q+delta>end)
	{
		x_q=x_q-delta;
	}

	if(x_q-delta<begin)
	{
		x_q=x_q+delta;
	}

//	MessageBoxA(NULL,itoa(xq*100,str,10),"xq",MB_OK);
	/*if((x_q+delta)>end)
	{
		delta=delta-(x_q+delta-end)/(n-1);//-last point to devil
	}//--change delta
	
	*max=pow(delta,3)*function(begin,end,3)/16;
	n=0;
	for(double i=begin;i<=end;i=i+delta,n++)
	{
		if((abs(x-i)/delta)<=1)
		{
			x_q=i;
		if((abs(x-i)/delta)<=0.5) 
		{
			x_q=i;
			i=i+delta;
			n++;
			//MessageBoxA(NULL,itoa((abs(x-i)/h)*100,str,10),"t",MB_OK);
			//MessageBoxA(NULL,itoa(xq*100,str,10),"xq",MB_OK);
		}
		}
	}*/

//	MessageBoxA(NULL,itoa(h*100,str,10),"h",MB_OK);
//	MessageBoxA(NULL,itoa(x_q*100,str,10),"x_q",MB_OK);
//	MessageBoxA(NULL,itoa(abs(x-x_q)/h*100,str,10),"(x-x_q)/h",MB_OK);
}


double Graph::func_polynom_gauss(double x)
{
	double g;
	double f1,f2,f3;
	double t;
	GetFunction function;
	function=(GetFunction)GetProcAddress(dll,"GetFunction");

	if(function==NULL)
	{
		MessageBoxA(NULL,"import function fail(GetFunction)","error",MB_OK);
		return 0;
	}
	
	t=(x-x_q)/delta;
	f1=function(x_q);
	f2=function(x_q+delta)-function(x_q);
	f3=function(x_q+delta)-2*function(x_q)+function(x_q-delta);
	g=f1+f2*t+f3*t*(t-1)/2;

return g;
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

	double x_q_1,x_q_2;
	x_q_1=x_q-delta;
	x_q_2=x_q+delta;

	dc->SelectObject(&pen2);
	prev_x=x_q_1;
	prev_y=-func_polynom_gauss(x_q_1);
	float i;
	double d=0.5/mult_coeff;
	if(d>1) d=1;
	if(d<0.0001) d=0.001;

	double b,e;

	if(begin>x_q_1){b=begin;}
	else{b=x_q_1;}

	if(end<x_q_2){e=end;}
	else{e=x_q_2;}


	for(i=b;i<e;i=i+d)//this delta must be <(end-begin)/nodes
	{
	//next_x=next_x+0.1;
	next_x=i;
	next_y=-func_polynom_gauss(next_x);
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

/*
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
}*/

double Graph::pract(double x)
{
	GetFunction function;
	function=(GetFunction)GetProcAddress(dll,"GetFunction");

	if(function==NULL)
	{
		MessageBoxA(NULL,"import function fail(GetFunction)","error",MB_OK);
		return 0;
	}


	return abs(function(x)-func_polynom_gauss(x));
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
	if(function==NULL)
	{
		MessageBoxA(NULL,"import function fail(GetFunction)","error",MB_OK);
		return;
	}

	double x_q_1,x_q_2;
	x_q_1=x_q-delta;
	x_q_2=x_q+delta;


	dc->SelectObject(&pen2);
	prev_x=x_q_1;
	prev_y=-func_polynom_gauss(x_q_1);
	float i;
	double d=0.5/mult_coeff;
	if(d>1) d=1;
	if(d<0.0001) d=0.001;

	double b,e;

	if(begin>x_q_1){b=begin;}
	else{b=x_q_1;}

	if(end<x_q_2){e=end;}
	else{e=x_q_2;}


	for(i=b;i<e;i=i+d)//this delta must be <(end-begin)/nodes
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

double Graph::get_x_q()
{
	return x_q;
}


double Graph::get_h()
{
	return h;
}

double Graph::get_delta()
{
	return delta;
}
