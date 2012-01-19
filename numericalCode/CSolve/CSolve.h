#ifndef CSOLVE_H_INCLUDED
#define CSOLVE_H_INCLUDED

#define f(i,j) (k*k-2*M_PI*M_PI)*sin(M_PI*i*hx)*sin(M_PI*j*hy)
#define g(i,j) (k*k*j*hy)
#define M_PI		3.14159265358979323846

class CSolve
{
    int nx,ny;      //количество узлов по горизонтали/вертикали
    double hx,hy;   //шаг сетки по горизонтали/вертикали
    double k;       //коэфф стратификации
    double eps;
	double hight,lenght;	//высота/длинна

    double **r;     //невязка
    double **psi;   //значения функции psi

public:
    CSolve(int in_nx,int in_ny,double in_hx,double in_hy,double in_k,double in_eps);


    void CreateGrid(int n_left,double *line_left,int n_right,double *line_right,int n_top,double *line_top,int n_down,double *line_down);
    double** Solve(int mode);
    void Print(char* file_name);
	void Print_Grid(char* file_name);

	~CSolve();
};

#endif // CSOLVE_H_INCLUDED
