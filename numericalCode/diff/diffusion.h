#ifndef DIFFUSION_H
#define DIFFUSION_H

#include "shuttle.h"

class diffusion
{

	int nx,ny,T;
	double tau,vs,D;

	double hx,hy,lenght,hight;

	double **u;
	double **v;
	double **C;
	diffusion(double D,double tau,double vs,double lenght,double hight,double **u,double **v,int nx,int ny);
public:
	double** solve();
	~diffusion();

};

#endif