#include "stdafx.h"


Graph::Graph(double w,double h,double m,double b,double e,HMODULE my_dll,double *f,double eps)
{
	sp=new Spline(b,e,my_dll,eps,f);
	o_x=w/2;o_y=h/2;width=w;height=h;mult_coeff=m;begin=b;end=e;
	dll=my_dll;
	o_x1=o_x;o_y1=o_y;
	function_x=(GetFunction)GetProcAddress(dll,"GetFunction");
	if(function_x==NULL)
	{
		MessageBoxA(NULL,"import function fail(GetFunction)","error",MB_OK);
		return ;
	}

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

double Graph::get_delta()
{
	return delta;
}

void Graph::paint_orig(CClientDC *dc, CRect *dlg_rc, double x, double y)
{
	CRect *rc;
	COLORREF backColor;
	backColor = RGB(255,255,255);  //replace with desired background color
	CBrush backBrush (backColor);
	CBrush* pOldBackBrush;
	double next_x=0,next_y=0,prev_x=0,prev_y=0;

	rc=dlg_rc;

	pOldBackBrush = dc->SelectObject(&backBrush);
//	dc->Rectangle(40, 80, rc->Width()-40, rc->Height()-80);

	//Graph graph(rc.Width(),rc.Height(),20,40,-10,10);//width,height,mult_coeff,nodes,begin,end

	o_x1=o_x1+x;
	o_y1=o_y1+y;


	CPen pen1(PS_SOLID,1,RGB(0,0,0));
	CPen pen2(PS_SOLID,3,RGB(255,0,0));

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
	prev_y=-function_x(begin);
	double i;
	double d=0.5/mult_coeff;
	if(d>1) d=1;
	if(d<0.0001) d=0.001;
	for(i=begin;i<end;i=i+d)//this delta must be <(end-begin)/nodes
	{
	//next_x=next_x+0.1;
	next_x=i;
	next_y=-function_x(next_x);
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
//	dc->Rectangle(40, 80, rc->Width()-40, rc->Height()-80);

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
	prev_y=-sp->func_polynom(begin);
	double i;
	double d=0.5/mult_coeff;
	if(d>1) d=1;
	if(d<0.0001) d=0.001;
	for(i=begin;i<end;i=i+d)//this delta must be <(end-begin)/nodes
	{
	//next_x=next_x+0.1;
	next_x=i;
	next_y=-sp->func_polynom(next_x);
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

void Graph::paint_graph(CClientDC *dc,CRect *dlg_rc,double x,double y,int ms)
{
	CRect *rc;
	COLORREF backColor;
	backColor = RGB(255,255,255);  //replace with desired background color
	CBrush backBrush (backColor);
	CBrush* pOldBackBrush;
	double next_x=0,next_y=0,prev_x=0,prev_y=0,next_t=0,prev_t=0;

	rc=dlg_rc;

	pOldBackBrush = dc->SelectObject(&backBrush);
//	dc->Rectangle(40, 80, rc->Width()-40, rc->Height()-80);

	//Graph graph(rc.Width(),rc.Height(),20,40,-10,10);//width,height,mult_coeff,nodes,begin,end

	o_x1=o_x1+x;
	o_y1=o_y1+y;


	CPen pen1(PS_SOLID,1,RGB(0,0,0));
	CPen pen2(PS_SOLID,5,RGB(0,0,0));
	CPen pen3(PS_SOLID,5,RGB(255,255,255));
	CPen pen4(PS_SOLID,5,RGB(255,0,0));

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
	prev_y=-sp->func_polynom(begin);
	double i;
	double d=0.5/mult_coeff;
	if(d>1) d=1;
	if(d<0.0001) d=0.001;
	for(i=begin;i<end;i=i+d)//this delta must be <(end-begin)/nodes
	{
	//Sleep(ms);
	//next_x=next_x+0.1;
	//next_t=i;
	next_x=i;
	next_y=-sp->func_polynom(i);
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
	//dc->MoveTo(o_x1+prev_x*mult_coeff,o_y1+prev_y*mult_coeff);
	//dc->LineTo(o_x1+next_x*mult_coeff,o_y1+next_y*mult_coeff);
	dc->SelectObject(&pen4);
	dc->MoveTo(o_x1+50,o_y1+prev_y*mult_coeff);
	dc->LineTo(o_x1+50,o_y1+next_y*mult_coeff);

	//dc->MoveTo(o_x1,o_y1+prev_y*mult_coeff);
	//dc->LineTo(o_x1,o_y1+next_y*mult_coeff);
	Sleep(ms);
	dc->SelectObject(&pen3);
	dc->MoveTo(o_x1+50,o_y1+prev_y*mult_coeff);
	dc->LineTo(o_x1+50,o_y1+next_y*mult_coeff);

	//dc->MoveTo(o_x1,o_y1+prev_y*mult_coeff);
	//dc->LineTo(o_x1,o_y1+next_y*mult_coeff);
	

	}
	prev_x=next_x;
	//prev_t=next_t;
	prev_y=next_y;
	
	}

	dc->SelectObject(&pen4);
	dc->MoveTo(o_x1+50,o_y1+prev_y*mult_coeff);
	dc->LineTo(o_x1+50,o_y1+next_y*mult_coeff);

}


void Graph::set_mult(double mult)
{
	mult_coeff=mult;
}

void Graph::rectangle(CClientDC *dc,CRect *dlg_rc,double x,double y)
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


}


void Graph::line(CClientDC *dc, CRect *dlg_rc, double x, double y, double begin, double end)
{
	CRect *rc;

	rc=dlg_rc;

//	o_x1=o_x1+x;
//	o_y1=o_y1+y;

	CPen pen1(PS_SOLID,2,RGB(255,0,0));

	dc->SelectObject(&pen1);
	if(	(o_x1+begin*mult_coeff)<rc->Width()-45&&
		(o_x1+begin*mult_coeff)>45&&
		(o_x1+end*mult_coeff)<rc->Width()-45&&
		(o_x1+end*mult_coeff)>45&&
		(o_y1)<rc->Height()-85&&
		(o_y1)>85
		)
	{
	dc->MoveTo(begin*mult_coeff+o_x1,o_y1);
	dc->LineTo(end*mult_coeff+o_x1,o_y1);
	}

}

double Graph::get_err(double x)
{
	return abs(sp->func_polynom(x)-function_x(x));
}

Graph::~Graph()
{
	sp->~Spline();
}

