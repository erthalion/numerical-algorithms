#ifndef METHOD_H
#define METHOD_H

#define Pi 3.1415

class method
{
protected:
	int n;	//размерность
	double **A;	//матрица значений
	double **Af;	//расширенная матрица
	double *f;	//вектор правой части уравн
	double *u;	//вектор точного решения
	double *u_solve;	//вектор полученного решения
	int mode;	//тест или счет
public:
	method(double **A,double *u,int n);
	method(double **A,double *f,int n,int mode);
	virtual double* get_solve();
	double* get_err();	//полученная погрешность
	double get_err_revmatr(double **rev);
//	double** get_inverse();	//найти обратну матрицу к A
	void reposition_column(int i,int j);	//перестановка местами столбцов
	void reposition_line(int i,int j);	//перестановка местами строк
	double get_norm(double *r,int n);	//посчитать норму вектора(n-размерность)
	double* get_difference(double *r1,double *r2,int n);	//разность векторов
	double** get_mult(double **A,double **B);
	double** get_A();
	void print_f();
	void print_A();
	void print_u_solve();
	void print_u();
	~method();
};

#endif 