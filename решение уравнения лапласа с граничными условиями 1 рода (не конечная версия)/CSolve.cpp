#include "CSolve.h"
#include <math.h>
#include <stdio.h>


CSolve::CSolve(int in_nx,int in_ny,double in_l,double in_h,double in_k,double in_eps)
{
    nx=in_nx;ny=in_ny;
	lenght=in_l;
	hight=in_h;
	hx=lenght/(nx-1);
	hy=hight/(ny-1);
    k=in_k;
    eps=in_eps;

    r=new double*[nx];

    psi=new double*[nx];
    for(int i=0;i<nx;i++)
    {
        psi[i]=new double[ny];
        r[i]=new double[ny];

        for(int j=0;j<ny;j++)
        {
            psi[i][j]=0;
            r[i][j]=0;
       }
    }

}


CSolve::~CSolve()
{
    for(int i=0;i<nx;i++)
    {
        delete(psi[i]);
        delete(r[i]);
    }

}


double** CSolve::Solve(int mode)	//mode==1|2 - вариант задачи
{

double p,q,Ar,tau,mod=1;
double hxhx=1.0/(hx*hx);
double hyhy=1.0/(hy*hy);
double hxhy=hx*hy;

if(mode==1)
{

while(mod>eps)
{

/////////////////////////////////////////
//нахождение невязки
////////////////////////////////////////

    for(int i=1;i<nx-1;i++)
    {
        for(int j=1;j<ny-1;j++)
        {
            r[i][j]=(psi[i+1][j]-2.0*psi[i][j]+psi[i-1][j])*hxhx + (psi[i][j+1]-2.0*psi[i][j]+psi[i][j-1])*hyhy + k*k*psi[i][j] - f(i,j);
        }
    }

////////////////////////////////////////
//нахождение tau
////////////////////////////////////////


    p=0;q=0;
    for(int i=1;i<nx-1;i++)
    {
        for(int j=1;j<ny-1;j++)
        {
            Ar=(r[i+1][j]-2.0*r[i][j]+r[i-1][j])*hxhx + (r[i][j+1]-2.0*r[i][j]+r[i][j-1])*hyhy + k*k*r[i][j];
            p+=Ar*r[i][j]*hxhy;
            q+=Ar*Ar*hxhy;
        }
    }

    tau=p/q;

//////////////////////////////////////////////
//пересчет psi
//////////////////////////////////////////////

    for(int i=0;i<nx;i++)
    {
        for(int j=0;j<ny;j++)
        {
            psi[i][j]=psi[i][j]-tau*r[i][j];
        }
    }


/////////////////////////////////////////////
//нахождение модуля невязки
/////////////////////////////////////////////
    mod=0;

    for(int i=0;i<ny;i++)
    {
        for(int j=0;j<nx;j++)
        {
            mod=mod+r[i][j]*r[i][j]*hxhy;
        }
    }
    mod=sqrt(mod);

    printf("mod %f\n",mod);

}

}

if(mode==2)
{
while(mod>eps)
{

/////////////////////////////////////////
//нахождение невязки
////////////////////////////////////////

    for(int i=1;i<nx-1;i++)
    {
        for(int j=1;j<ny-1;j++)
        {
            r[i][j]=(psi[i+1][j]-2.0*psi[i][j]+psi[i-1][j])*hxhx + (psi[i][j+1]-2.0*psi[i][j]+psi[i][j-1])*hyhy + k*k*psi[i][j] - g(i,j);
        }
    }

////////////////////////////////////////
//нахождение tau
////////////////////////////////////////


    p=0;q=0;
    for(int i=1;i<nx-1;i++)
    {
        for(int j=1;j<ny-1;j++)
        {
            Ar=(r[i+1][j]-2.0*r[i][j]+r[i-1][j])*hxhx + (r[i][j+1]-2.0*r[i][j]+r[i][j-1])*hyhy + k*k*r[i][j];
            p+=Ar*r[i][j]*hxhy;
            q+=Ar*Ar*hxhy;
        }
    }

    tau=p/q;

//////////////////////////////////////////////
//пересчет psi
//////////////////////////////////////////////

    for(int i=0;i<nx;i++)
    {
        for(int j=0;j<ny;j++)
        {
            psi[i][j]=psi[i][j]-tau*r[i][j];
        }
    }


/////////////////////////////////////////////
//нахождение модуля невязки
/////////////////////////////////////////////
    mod=0;

    for(int i=0;i<ny;i++)
    {
        for(int j=0;j<nx;j++)
        {
            mod=mod+r[i][j]*r[i][j]*hxhy;
        }
    }
    mod=sqrt(mod);

    printf("mod %f\n",mod);

}

}


return psi;

}


void CSolve::Print(char* file_name)
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

void CSolve::Print_Grid(char* file_name)
{
	FILE *f;
	f=fopen(file_name,"w");

	for(int i=0;i<nx;i++)
	{
		for(int j=0;j<ny;j++)
		{
			fprintf(f," %f",psi[j][nx-i-1]);
		}
	fprintf(f,"\n");
	}

	fclose(f);
}

void CSolve::CreateGrid(int n_left,double *line_left,int n_right,double *line_right,int n_top,double *line_top,int n_down,double *line_down)
{
	//line_... - для каждого отв три аргумента в массиве
	//первый - начальная координата
	//второй - длинна
	//третий - скорость
	double coord=0,lenght=0,temp;
	int i,j,num;

	//для левой границы
	for(int n=0;n<n_left;n++)
	{
		coord=0;lenght=0;
		j=0;
		while(coord<=line_left[3*n])	//поиск начальной точки
		{
		coord=j*hy;
		j++;
		}
		//после нахождения
		j--;	//лишнее прохождение предыдущего цикла
		num=j;
		while(lenght<line_left[3*n+1]&&j<ny)	//пока длинна отрезка не достигнута
		{
		psi[0][j]+=line_left[3*n+2]*(j-num)*hy;
		temp=psi[0][j];
		lenght=(j-num)*hy;
		j++;
		}
		j--;
		for(int k=j;k<ny;k++)
		{
			psi[0][k]=temp;
		}
	}

	//для верхней границы
	for(int k=0;k<nx;k++)
	{
		psi[k][ny-1]=temp;
	}


	for(int n=0;n<n_top;n++)
	{	
		coord=0;lenght=0;
		i=0;
		while(coord<=line_top[3*n])	//поиск начальной точки
		{
		coord=i*hx;
		i++;
		}
		//после нахождения
		i--;	//лишнее прохождение предыдущего цикла
		num=i;
		while(lenght<line_top[3*n+1]&&i<nx)	//пока длинна отрезка не достигнута
		{
		psi[i][ny-1]+=line_top[3*n+2]*(i-num)*hx;
		temp=psi[i][ny-1];
		lenght=(i-num)*hx;
		i++;
		}

		i--;
		for(int k=i;k<nx;k++)
		{
			psi[k][ny-1]=temp;
		}

	}

	//для нижней границы
	temp=0;
	for(int n=0;n<n_down;n++)
	{
		coord=0;lenght=0;
		i=0;
		while(coord<=line_down[3*n])	//поиск начальной точки
		{
		coord=i*hx;
		i++;
		}
		//после нахождения
		i--;	//лишнее прохождение предыдущего цикла
		num=i;
		while(lenght<line_down[3*n+1]&&i<nx)	//пока длинна отрезка не достигнута
		{
		psi[i][0]+=line_down[3*n+2]*(i-num)*hx;
		temp=psi[i][0];
		lenght=(i-num)*hx;
		i++;
		}

		i--;
		for(int k=i;k<nx;k++)
		{
			psi[k][0]=temp;
		}

	}

	//для правой границы
	for(int k=0;k<ny;k++)
	{
		psi[nx-1][k]=temp;
	}

	for(int n=0;n<n_right;n++)
	{
		coord=0;lenght=0;
		j=0;
		while(coord<=line_right[3*n])	//поиск начальной точки
		{
		coord=j*hy;
		j++;
		}
		//после нахождения
		j--;	//лишнее прохождение предыдущего цикла
		num=j;
		while(lenght<line_right[3*n+1]&&j<ny)	//пока длинна отрезка не достигнута
		{
		psi[nx-1][j]+=line_right[3*n+2]*(j-num)*hy;
		temp=psi[ny-1][j];
		lenght=(j-num)*hy;
		j++;
		}

		j--;
		for(int k=j;k<ny;k++)
		{
			psi[ny-1][k]=temp;
		}

	}

}