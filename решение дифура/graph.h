#ifndef GRAPH_H
#define GRAPH_H

#include <math.h>
#include "afxwin.h"
#include "spline_graph.h"

typedef double (WINAPI *GetFunction)(double);


class Graph
{
protected:
	double o_x,o_y,o_x1,o_y1;//coord O
	double width,height;
	double mult_coeff;
	double nodes;
	double begin,end;
	HMODULE dll;
	double* val_func;
	double delta;
	int node;
	GetFunction function_x;
	Spline *sp;


public:
	Graph(double w,double h,double m,double b,double e,HMODULE dll,double *f,double eps);
	void paint_graph(CClientDC *dc,CRect *rc,double x,double y);
	void paint_graph(CClientDC *dc,CRect *rc,double x,double y,int ms);

	void paint_orig(CClientDC *dc,CRect *rc,double x,double y);
	void rectangle(CClientDC *dc,CRect *dlg_rc,double x,double y);
	void line(CClientDC *dc,CRect *dlg_rc,double x,double y,double begin,double end);

	double get_o_x();
	double get_o_y();
	double get_o_x1();
	double get_o_y1();	
	double get_delta();

	double get_mult();
	double get_begin();
	double get_end();
	void set_mult(double mult);
	double get_err(double x);
	~Graph();
};

#endif