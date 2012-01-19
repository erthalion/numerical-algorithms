#ifndef GRAPH_H
#define GRAPH_H

#include <math.h>
#include "afxwin.h"

class Graph
{
	double o_x,o_y,o_x1,o_y1;//coord O
	double width,height;
	double mult_coeff;
	double h;
	double begin,end;
	double delta;
	int node;
	HMODULE dll;
	double x_q;
	double* val_func;
public:
	Graph(double w,double h,double m,double* max,double b,double e,double x,HMODULE dll);
	double func_polynom_gauss(double x);
	void paint_graph(CClientDC *dc,CRect *rc,double x,double y);
	void paint_graph_orig(CClientDC *dc,CRect *rc,double x,double y);

	double get_x_q();
	double get_h();
	double get_o_x();
	double get_o_y();
	double get_o_x1();
	double get_o_y1();
	double get_delta();

	double get_mult();
	double get_begin();
	double get_end();
	void set_mult(double mult);
//	double theor(double x);
	double pract(double x);
};

#endif