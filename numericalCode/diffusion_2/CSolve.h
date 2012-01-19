#ifndef CSOLVE_H
#define CSOLVE_H

#include <stdio.h>
#include <math.h>

class CSolve
{
        int nx, ny;      //���������� ����� �� �����������/���������
        double hx, hy;   //��� ����� �� �����������/���������
        double k;       //����� �������������
        double eps;
        double hight, lenght;	//������/������

        double **r;     //�������
        double **psi;   //�������� ������� psi
        double **f;    //������ �������� ������ ����� ���������

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