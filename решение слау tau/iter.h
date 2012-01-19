#ifndef ITER_H
#define ITER_H

class iter:public method
{
	double delta_min,delta_max;
	double tau;
	double epsilon;
	double step;
	double *r;
	double n0;	//теор количество итераций
	double *wn;
	double *w0;
public:
	iter(double **A,double *u,int n,double eps);
	double* get_solve();
	void set_approx_null();	//нач приближение нулевое
	void set_approx_near();	//нач приближение близкое к точному решению
	void set_approx_rand();	//нач приближ произвольное
//	double* error(int n);	//нахождение вектора погрешности n степени
	double get_delta_min();
	double get_delta_max();
	double get_wn();
	double get_w0();
	double get_tau();
	~iter();
};

#endif