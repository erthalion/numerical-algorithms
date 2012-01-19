#ifndef DIFF_H
#define DIFF_H

#include <stdio.h>

class diff
{
        int nx, ny;
        double tau;
        double hx, hy, lenght, hight;
        int iter;
        int T;
        FILE *f;
        char *file_name;

        double vs, D;   //D=1/Re
        double **C;
        double **u;
        double **v;

public:
		diff();

		void Create(int in_nx, int in_ny, double in_l, double in_h, double in_vs, double in_D, double **in_u, double **in_v, int count, double t,char *in_file_name);

        double* shuttle(double* A, double* C, double* B, double* F, int n);

        void Print(char *file_name);

        void set_boundary(double **psi);

        void set_u_v(double **in_u, double **in_v);

        void begin_solve(char *file_name);

        double** Solve(double **psi, char *file_name);

};

#endif