#ifndef CSOLVE_H
#define CSOLVE_H

#include <stdio.h>
#include <math.h>

class CSolve
{
        int nx, ny;      //количество узлов по горизонтали/вертикали
        double hx, hy;   //шаг сетки по горизонтали/вертикали
        double k;       //коэфф стратификации
        double eps;
        double hight, lenght;	//высота/длинна

        double **r;     //невязка
        double **psi;   //значения функции psi
        double **f;    //массив значений правой части уравнения

public:
		CSolve();

        void Create(int in_nx, int in_ny, double in_l, double in_h, double in_k, double in_eps);

        void CreateGrid(int n_left, double *line_left, int n_right, double *line_right, int n_top, double *line_top, int n_down, double *line_down);

        double **Solve();

        void Print(char *file_name);

		void Print_Grid(char *file_name);

        double** Get_u();

		double** Get_v();

        void set_f(double **in_f);

        double** get_psi();

};

#endif