#include "CSolve.h"

CSolve::CSolve()
{
}

void CSolve::Create(int in_nx, int in_ny, double in_l, double in_h, double in_k, double in_eps)
{
        nx = in_nx; ny = in_ny;
        lenght = in_l;
        hight = in_h;
        hx = lenght / (nx - 1);
        hy = hight / (ny - 1);
        k = in_k;
        eps = in_eps;

        r = new double*[nx];
        f = new double*[nx];

        psi = new double*[nx];
        for (int i = 0; i < nx; i++)
        {
			r[i]=new double[ny];
			f[i]=new double[ny];
			psi[i]=new double[ny];

            for (int j = 0; j < ny; j++)
            {
                psi[i][j] = 0;
                r[i][j] = 0;
                f[i][j] = 0;
            }
        }

}

double** CSolve::Solve()
{
	double p, q, Ar, tau, mod = 1;
    double hxhx = 1.0 / (hx * hx);
    double hyhy = 1.0 / (hy * hy);
    double hxhy = hx * hy;

    while (mod > eps)
    {

        /////////////////////////////////////////
        //нахождение невязки
        ////////////////////////////////////////

        for (int i = 1; i < nx - 1; i++)
        {
            for (int j = 1; j < ny - 1; j++)
            {
                r[i][j] = (psi[i + 1][j] - 2.0 * psi[i][j] + psi[i - 1][j]) * hxhx + (psi[i][j + 1] - 2.0 * psi[i][j] + psi[i][j - 1]) * hyhy - f[i][j];
            }
        }

        ////////////////////////////////////////
        //нахождение tau
        ////////////////////////////////////////


        p = 0; q = 0;
        for (int i = 1; i < nx - 1; i++)
        {
            for (int j = 1; j < ny - 1; j++)
            {
                Ar = (r[i + 1][j] - 2.0 * r[i][j] + r[i - 1][j]) * hxhx + (r[i][j + 1] - 2.0 * r[i][j] + r[i][j - 1]) * hyhy;
                p += Ar * r[i][j] * hxhy;
                q += Ar * Ar * hxhy;
            }
        }

        tau = p / q;

        //////////////////////////////////////////////
        //пересчет psi
        //////////////////////////////////////////////

        for (int i = 0; i < nx; i++)
        {
            for (int j = 0; j < ny; j++)
            {
                psi[i][j] = psi[i][j] - tau * r[i][j];
            }
        }


        /////////////////////////////////////////////
        //нахождение модуля невязки
        /////////////////////////////////////////////
        mod = 0;

        for (int i = 0; i < ny; i++)
        {
            for (int j = 0; j < nx; j++)
            {
                mod = mod + r[i][j] * r[i][j] * hxhy;
            }
        }
        mod = sqrt(mod);
        //printf("mod %d\n",mod);

    }

    return psi;

}

double** CSolve::Get_u()
{
	double **u;
	u=new double*[nx];

    //i=0
    for (int j = 0; j < ny; j++)
    {
		u[j]=new double[ny];
        u[0][j] = (psi[0][j] - psi[1][j]) / hx;
    }

    for (int i = 1; i < nx-1; i++)
    {
        for (int j = 0; j < ny; j++)
        {
            u[i][j] = (psi[i - 1][j] - psi[i + 1][j]) / (2 * hx);
        }
    }

    //i=nx-1
    for (int j = 0; j < ny; j++)
    {
        u[nx - 1][j] = (psi[nx - 2][j] - psi[nx - 1][j]) / hx;
    }

    return u;
}

double** CSolve::Get_v()
{
    double **v;
	v = new double*[nx];

    //j=0
    for (int i = 0; i < nx; i++)
    {	
		v[i]=new double[ny];
        v[i][0] = (psi[i][0] - psi[0][1]) / hy;
    }

    for (int i = 0; i < nx; i++)
    {
        for (int j = 1; j < ny-1; j++)
        {
            v[i][j] = (psi[i][j-1] - psi[i][j+1]) / (2 * hy);
        }
    }

    //j=ny-1
    for (int i = 0; i < nx; i++)
    {
        v[i][ny-1] = (psi[i][ny-2] - psi[i][ny-1]) / hy;
    }

    return v;

}

void CSolve::set_f(double **in_f)
{
    for (int i = 0; i < nx; i++)
    {
        for (int j = 0; j < ny; j++)
        {
            f[i][j] = in_f[i][j];
        }
    }

}

double** CSolve::get_psi()
{
	return psi;
}
