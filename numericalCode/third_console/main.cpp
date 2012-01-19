#include <stdio.h>
#include "CSolve.h"
#include "diff.h"

int main()
{
    int nx = 50;
    int ny = 50;
    double l = 1;
    double h = 1;
    double vs = 0;
    double D = 0.01;
    double t=0.01;
    int count=1;
    char file_name[]="out.dat";


    int iter=0;
    int max_iter=200;
    int k=0;    //коэфф стратификации
    double eps=0.001;

    double **u;
    double **v;
	v=new double*[nx];
	u=new double*[nx];


    for(int i=0;i<nx;i++)
    {
		u[i]=new double[ny];
		v[i]=new double[ny];
        for(int j=0;j<ny;j++)
        {
            u[i][j]=0;
            v[i][j]=0;
        }
    }

    //зависит от времени
    for(int i=0;i<nx;i++)
    {
        u[i][ny-1]=1;
    }

    diff *diffusion=new diff();
    diffusion->Create(nx, ny, l, h, vs, D, u, v, count, t, file_name);

    CSolve *laplas = new CSolve();
    laplas->Create(nx, ny, l, h, k, eps);


    diffusion->begin_solve(file_name);
    while (iter < max_iter)
    {
        iter++;
		printf("iter %d\n",iter);
        laplas->set_f(diffusion->Solve(laplas->get_psi(),file_name));
        laplas->Solve();
        u = laplas->Get_u();
        v = laplas->Get_v();
              
        //зависит от времени
        for (int i = 0; i < nx; i++)
        {
            u[i][ny-1] = 1;
        }

    }

	return 0;
}