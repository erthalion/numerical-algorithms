#include "CSolve.h"

CSolve::CSolve()
{
}

void CSolve::Print_Grid(char *file_name)
{
	FILE *f=fopen(file_name,"w");
	for(int i=0;i<nx;i++)
	{
		for(int j=0;j<ny;j++)
		{
			fprintf(f,"%f ",psi[i][j]);
		}
		fprintf(f,"\n");
	}
	fclose(f);
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
        //���������� �������
        ////////////////////////////////////////

        for (int i = 1; i < nx - 1; i++)
        {
            for (int j = 1; j < ny - 1; j++)
            {
                r[i][j] = (psi[i + 1][j] - 2.0 * psi[i][j] + psi[i - 1][j]) * hxhx + (psi[i][j + 1] - 2.0 * psi[i][j] + psi[i][j - 1]) * hyhy - f[i][j];
            }
        }

        ////////////////////////////////////////
        //���������� tau
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
        //�������� psi
        //////////////////////////////////////////////

        for (int i = 0; i < nx; i++)
        {
            for (int j = 0; j < ny; j++)
            {
                psi[i][j] = psi[i][j] - tau * r[i][j];
            }
        }


        /////////////////////////////////////////////
        //���������� ������ �������
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
	//double **u;
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
            u[i][j] = (psi[i-1][j] - psi[i + 1][j]) / (2*hx);
        }
    }

    //i=nx-1
    for (int j = 0; j < ny; j++)
    {
        u[nx-1][j] = (psi[nx - 2][j] - psi[nx - 1][j]) / hx;
    }

    return u;
}

double** CSolve::Get_v()
{
    //double **v;
	v = new double*[nx];

    //j=0
    for (int i = 0; i < nx; i++)
    {	
		v[i]=new double[ny];
        v[i][0] = -(psi[i][0] - psi[0][1]) / hy;
    }

    for (int i = 0; i < nx; i++)
    {
        for (int j = 1; j < ny-1; j++)
        {
            v[i][j] = -(psi[i][j-1] - psi[i][j+1]) / (2*hy);
        }
    }

    //j=ny-1
    for (int i = 0; i < nx; i++)
    {
        v[i][ny-1] = -(psi[i][ny-2] - psi[i][ny-1]) / hy;
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


void CSolve::CreateGrid(int n_left, double *line_left, int n_right, double *line_right, int n_top, double *line_top, int n_down, double *line_down)
{
	//line_... - ��� ������� ��� ��� ��������� � �������
	//������ - ��������� ����������
	//������ - ������
	//������ - ��������
	double coord=0,lenght=0,temp=0;
	int i,j,num;

	//��� ����� �������
	for(int n=0;n<n_left;n++)
	{
		coord=0;lenght=0;
		j=0;
		while(coord<=line_left[3*n])	//����� ��������� �����
		{
		coord=j*hy;
		j++;
		}
		//����� ����������
		j--;	//������ ����������� ����������� �����
		num=j;
		while(lenght<line_left[3*n+1]&&j<ny)	//���� ������ ������� �� ����������
		{
		psi[0][j]+=line_left[3*n+2]*(j-num)*hy;
		//temp=psi[0,j];
		lenght=(j-num)*hy;
		j++;
		}
		//j--;
        psi[0][j] += line_left[3 * n + 2] * line_left[3 * n + 1];
        temp = psi[0][j];
        psi[0][j - 1] = temp;
		for(int k=j;k<ny;k++)
		{
			psi[0][k]=temp;
		}
	}

	//��� ������� �������
	for(int k=1;k<nx;k++)
	{
		psi[k][ny-1]=temp;
	}


	for(int n=0;n<n_top;n++)
	{	
		coord=0;lenght=0;
		i=0;
		while(coord<=line_top[3*n])	//����� ��������� �����
		{
		coord=i*hx;
		i++;
		}
		//����� ����������
		i--;	//������ ����������� ����������� �����
		num=i;
		while(lenght<line_top[3*n+1]&&i<nx)	//���� ������ ������� �� ����������
		{
		psi[i][ny-1]+=line_top[3*n+2]*(i-num)*hx;
		//temp=psi[i,ny-1];
		lenght=(i-num)*hx;
		i++;
		}

		//i--;
        psi[i][ny - 1] += line_top[3 * n + 2] * line_top[3 * n + 1];
        temp = psi[i][ny - 1];
        psi[i - 1][ny - 1] = temp;
		for(int k=i;k<nx;k++)
		{
			psi[k][ny-1]=temp;
		}

	}

	//��� ������ �������
	for(int k=0;k<ny;k++)
	{
        psi[nx - 1][k] = temp;
    }

	for(int n=0;n<n_right;n++)
	{
		coord=1;lenght=0;
		j=ny-1;
		while(coord>=line_right[3*n])	//����� ��������� �����
		{
		coord=(j)*hy;
		j--;
		}
		//����� ����������
		j++;	//������ ����������� ����������� �����
		num=j;
		while(lenght<line_right[3*n+1]&&j>=0)	//���� ������ ������� �� ����������
		{
		psi[nx-1][j]+=line_right[3*n+2]*(num-j)*hy;
		//temp=psi[ny-1,j];
		lenght=(num-j)*hy;
		j--;

		}

		//j++;
        psi[ny-1][j]+=line_right[3 * n + 2] * line_right[3 * n + 1];
        temp=psi[ny-1][j];
        psi[ny - 1][j +1] = temp;
		for(int k=j;k>=0;k--)
		{
			psi[ny-1][k]=temp;
		}

	}

    //��� ������ �������
    for (int k = 0; k < nx; k++)
    {
        psi[k][0] = temp;
    }

    for (int n = 0; n < n_down; n++)
    {
        coord = 1; lenght = 0;
        i = nx-1;
        while (coord >= line_down[3 * n])	//����� ��������� �����
        {
            coord = i * hx;
            i--;
        }
        //����� ����������
        i++;	//������ ����������� ����������� �����
        num = i;
        while (lenght <= line_down[3*n+1]&&i<nx)	//���� ������ ������� �� ����������
        {
            psi[i][0] += line_down[3*n+2]*(num-i)*hx;
            temp = psi[i][0];
            lenght = (num-i) * hx;
            i--;
        }

        //i++;
        psi[i][0] += line_down[3 * n + 2] * line_down[3 * n + 1];
        temp = psi[i][0];
        psi[i + 1][0] = temp;
        for (int k = i; k >=0; k--)
        {
            psi[k][0]= temp;
        }

    }

}

void CSolve::Print(char *file_name)
{
	FILE *f;
	f=fopen(file_name,"w");
	fprintf(f,"TITLE=\"USERData\"\n");
	fprintf(f,"VARIABLES=x,y,u");
	fprintf(f,"\nZONE T=\"ZONE1\", i=%d j=%d f=Point", nx,ny );

	for(int i=0;i<nx;i++)
	{
		for(int j=0;j<ny;j++)
		{
			fprintf(f,"\n%2.8f %2.8f %2.8f", hx*i, hy*j, psi[i][j]);
		}
	}
	fclose(f);

}

void CSolve::Print_uv(char *file_name)
{
	FILE *f;
	f=fopen(file_name,"w");
	fprintf(f,"TITLE=\"USERData\"\n");
	fprintf(f,"VARIABLES=x,y,u,v");
	fprintf(f,"\nZONE T=\"ZONE1\", i=%d j=%d f=Point", nx,ny );

	for(int i=0;i<nx;i++)
	{
		for(int j=0;j<ny;j++)
		{
			fprintf(f,"\n%2.8f %2.8f %2.8f %2.8f", hx*i, hy*j, Get_v()[i][j],Get_u()[i][j]);
		}
	}
	fclose(f);

}
