#ifndef TAU_MIN_H
#define TAU_MIN_H

class tau_min:public iter
{
	double *r0;	//начальная невязка
public:
	tau_min(double **A,double *u,int n,double eps);
	double* get_solve_min();
	double* get_solve_min_n0();
	double get_rn();
	double get_r0();
//	double get_norm(double *r,int n);
	~tau_min();
};

#endif