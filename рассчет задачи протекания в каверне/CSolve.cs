﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Globalization;
using System.Threading;


namespace WpfApplication1
{
    class CSolve
    {
        int nx, ny;      //количество узлов по горизонтали/вертикали
        double hx, hy;   //шаг сетки по горизонтали/вертикали
        double k;       //коэфф стратификации
        double eps;
        double hight, lenght;	//высота/длинна
        FileStream file;

        double[,] r;     //невязка
        double[,] psi;   //значения функции psi
        double[,] psi_next;   //значения функции psi
        double[,] f;    //массив значений правой части уравнения
        double[,] u;
        double[,] v;
    
        public void Create(int in_nx, int in_ny, double in_l, double in_h, double in_k, double in_eps)
        {
            nx = in_nx; ny = in_ny;
            lenght = in_l;
            hight = in_h;
            hx = lenght / (nx - 1);
            hy = hight / (ny - 1);
            k = in_k;
            eps = in_eps;

            r = new double[nx, ny];
            f = new double[nx, ny];

            u = new double[nx, ny];
            v = new double[nx, ny];


            psi = new double[nx, ny];
            psi_next = new double[nx, ny];
            for (int i = 0; i < nx; i++)
            {
                for (int j = 0; j < ny; j++)
                {
                    psi[i, j] = 0;
                    psi_next[i, j] = 0; 
                    r[i, j] = 0;
                    f[i, j] = 0;
                    u[i, j] = 0;
                    v[i, j] = 0;
                }
            }

        }

        public void setBoundary()
        {
            double h = 0;
            double sum = 0;
            double temp = 0;

            //left boundary
            for (int j = 0; j < ny; j++)
            {
                sum = u[0, 0] / 2;
                for (int i = 1; i < j; i++)
                {
                    sum += u[0, i];
                }

                sum += u[0, j] / 2;
                h = hy;
                psi[0, j] = sum *h;
                temp = psi[0, j];
            }

            //top boundary
            for (int i = 0; i < nx; i++)
            {
                psi[i, ny - 1] = temp;
            }

            //right boundary
            for (int j = ny - 1; j > 0; j--)
            {
                sum = u[nx - 1, ny - 1] / 2;
                for (int i = ny - 2; i > j; i--)
                {
                    sum += u[nx - 1, i];
                }

                sum += u[nx - 1, j] / 2;
                h = hy;
                psi[nx - 1, j] = temp - sum * h;
            }

            //for (int j = 0; j < ny; j++)
            //{
            //    sum = u[nx-1, 0] / 2;
            //    for (int i = 1; i < j; i++)
            //    {
            //        sum += u[nx - 1, i];
            //    }

            //    sum += u[nx - 1, j] / 2;
            //    h = hy;
            //    psi[nx-1, j] = sum * h;
            //}

        }
                
        public void CreateGrid(int n_left, double[] line_left, int n_right, double[] line_right, int n_top, double[] line_top, int n_down, double[] line_down)
        {
	        //line_... - для каждого отв три аргумента в массиве
	        //первый - начальная координата
	        //второй - длинна
	        //третий - скорость
	        double coord=0,lenght=0,temp=0;
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
		        psi[0,j]+=line_left[3*n+2]*(j-num)*hy;
		        //temp=psi[0,j];
		        lenght=(j-num)*hy;
		        j++;
		        }
		        //j--;
                psi[0, j] += line_left[3 * n + 2] * line_left[3 * n + 1];
                temp = psi[0, j];
                psi[0, j - 1] = temp;
		        for(int k=j;k<ny;k++)
		        {
			        psi[0,k]=temp;
		        }
	        }

	        //для верхней границы
	        for(int k=1;k<nx;k++)
	        {
		        psi[k,ny-1]=temp;
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
		        psi[i,ny-1]+=line_top[3*n+2]*(i-num)*hx;
		        //temp=psi[i,ny-1];
		        lenght=(i-num)*hx;
		        i++;
		        }

		        //i--;
                psi[i, ny - 1] += line_top[3 * n + 2] * line_top[3 * n + 1];
                temp = psi[i, ny - 1];
                psi[i - 1,ny - 1] = temp;
		        for(int k=i;k<nx;k++)
		        {
			        psi[k,ny-1]=temp;
		        }

	        }

	        //для правой границы
	        for(int k=0;k<ny;k++)
	        {
                psi[nx - 1, k] = temp;
            }

	        for(int n=0;n<n_right;n++)
	        {
		        coord=1;lenght=0;
		        j=ny-1;
		        while(coord>=line_right[3*n])	//поиск начальной точки
		        {
		        coord=(j)*hy;
		        j--;
		        }
		        //после нахождения
		        j++;	//лишнее прохождение предыдущего цикла
		        num=j;
		        while(lenght<line_right[3*n+1]&&j>=0)	//пока длинна отрезка не достигнута
		        {
		        psi[nx-1,j]+=line_right[3*n+2]*(num-j)*hy;
		        //temp=psi[ny-1,j];
		        lenght=(num-j)*hy;
		        j--;

		        }

		        //j++;
                psi[ny-1,j]+=line_right[3 * n + 2] * line_right[3 * n + 1];
                temp=psi[ny-1,j];
                psi[ny - 1, j +1] = temp;
		        for(int k=j;k>=0;k--)
		        {
			        psi[ny-1,k]=temp;
		        }

	        }

            //для нижней границы
            for (int k = 0; k < nx; k++)
            {
                psi[k, 0] = temp;
            }

            for (int n = 0; n < n_down; n++)
            {
                coord = 1; lenght = 0;
                i = nx-1;
                while (coord >= line_down[3 * n])	//поиск начальной точки
                {
                    coord = i * hx;
                    i--;
                }
                //после нахождения
                i++;	//лишнее прохождение предыдущего цикла
                num = i;
                while (lenght <= line_down[3*n+1]&&i<nx)	//пока длинна отрезка не достигнута
                {
                    psi[i, 0] += line_down[3*n+2]*(num-i)*hx;
                    temp = psi[i, 0];
                    lenght = (num-i) * hx;
                    i--;
                }

                //i++;
                psi[i, 0] += line_down[3 * n + 2] * line_down[3 * n + 1];
                temp = psi[i, 0];
                psi[i + 1, 0] = temp;
                for (int k = i; k >=0; k--)
                {
                    psi[k, 0]= temp;
                }

            }



        }

        public double[,] Solve()
        {
            double p, q, Ar, tau, mod = 0;
            double hxhx = 1.0 / (hx * hx);
            double hyhy = 1.0 / (hy * hy);
            double hxhy = hx * hy;

            for (int i = 0; i < nx; i++)
            {
                for (int j = 0; j < ny; j++)
                {
                    r[i, j] = 0;
                    psi_next[i, j] = psi[i, j];
                }
            }

            for (int i = 1; i < nx - 1; i++)
            {
                for (int j = 1; j < ny - 1; j++)
                {
                    r[i, j] = (psi[i + 1, j] - 2.0 * psi[i, j] + psi[i - 1, j]) * hxhx + (psi[i, j + 1] - 2.0 * psi[i, j] + psi[i, j - 1]) * hyhy - f[i, j];
                }
            }

            for (int i = 0; i < nx; i++)
            {
                for (int j = 0; j < ny; j++)
                {
                    mod = mod + r[i, j] * r[i, j] * hxhy;
                }
            }
            mod = Math.Sqrt(mod);

            while (mod > eps)
            {

                ////////////////////////////////////////
                //нахождение tau
                ////////////////////////////////////////


                p = 0; q = 0;
                for (int i = 1; i < nx - 1; i++)
                {
                    for (int j = 1; j < ny - 1; j++)
                    {
                        Ar = (r[i + 1, j] - 2.0 * r[i, j] + r[i - 1, j]) * hxhx + (r[i, j + 1] - 2.0 * r[i, j] + r[i, j - 1]) * hyhy;
                        p += Ar * r[i, j] * hxhy;
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
                        psi_next[i, j] = psi[i, j] - tau * r[i, j];
                    }
                }

                for (int i = 1; i < nx-1; i++)
                {
                    for (int j = 1; j < ny-1; j++)
                    {
                        psi[i, j] = psi_next[i, j];
                    }
                }

                /////////////////////////////////////////
                //нахождение невязки
                ////////////////////////////////////////

                for (int i = 1; i < nx - 1; i++)
                {
                    for (int j = 1; j < ny - 1; j++)
                    {
                        r[i, j] = (psi[i + 1, j] - 2.0 * psi[i, j] + psi[i - 1, j]) * hxhx + (psi[i, j + 1] - 2.0 * psi[i, j] + psi[i, j - 1]) * hyhy - f[i, j];
                    }
                }


                /////////////////////////////////////////////
                //нахождение модуля невязки
                /////////////////////////////////////////////
                mod = 0;

                for (int i = 0; i < nx; i++)
                {
                    for (int j = 0; j < ny; j++)
                    {
                        mod = mod + r[i, j] * r[i, j] * hxhy;
                    }
                }
                mod = Math.Sqrt(mod);
                //Console.WriteLine("mod "+mod.ToString());

            }

            return psi;
        }

        public void Print_Begin(string file_name)
        {
            CultureInfo newCInfo = (CultureInfo)Thread.CurrentThread.CurrentCulture.Clone();
            newCInfo.NumberFormat.NumberDecimalSeparator = ".";
            Thread.CurrentThread.CurrentCulture = newCInfo;
            file = new FileStream(file_name, FileMode.Create);

            StreamWriter f_out = new StreamWriter(file);

            f_out.Write("TITLE=\"USERData\"\r\n");
            f_out.Write("VARIABLES=x,y,u,v,psi");
            //f_out.Write("\r\nZONE T=\"ZONE1\", i=" + nx + " j=" + ny + " f=Point");

            f_out.Close();
            file.Close();
        }

        public void Print(string file_name)
        {
            CultureInfo newCInfo = (CultureInfo)Thread.CurrentThread.CurrentCulture.Clone();
            newCInfo.NumberFormat.NumberDecimalSeparator = ".";
            Thread.CurrentThread.CurrentCulture = newCInfo;
            FileStream f=new FileStream(file_name,FileMode.Append);

            StreamWriter f_out = new StreamWriter(f);

            //f_out.Write("TITLE=\"USERData\"\r\n");
            //f_out.Write("VARIABLES=x,y,psi");
            f_out.Write("\r\nZONE T=\"ZONE\", i="+nx+" j="+ny+" f=Point");

            double[,] u = Get_u();
            double[,] v = Get_v();


            for (int i = 0; i < nx; i++)
            {
                for (int j = 0; j < ny; j++)
                {
                    f_out.Write("\r\n" + hx * i + " " + hy * j + " " + " " + u[i,j] + " " + v[i,j] + " " + psi[i, j]);//psi[i, ny - j - 1]);
                }
            }
            f_out.Close();
            f.Close();
        }

        public void Print_Grid(string file_name)
        {
            FileStream f = new FileStream(file_name, FileMode.Create);
            StreamWriter f_out = new StreamWriter(f);

            for (int i = 0; i < nx; i++)
            {
                for (int j = 0; j < ny; j++)
                {
                    f_out.Write(psi[j, nx - i - 1].ToString()+" ");
                }
                f_out.Write("\r\n");
            }

            f_out.Close();
            f.Close();
        }

        public void Debug()
        {
            Console.Write("\npsi\n");
            for (int i = 0; i < nx; i++)
            {
                for (int j = 0; j < ny; j++)
                {
                    Console.Write(psi[i, j] + " ");
                }
                Console.Write("\n");
            }

            Console.Write("\nu\n");
            for (int i = 0; i < nx; i++)
            {
                for (int j = 0; j < ny; j++)
                {
                    Console.Write(u[i, j] + " ");
                }
                Console.Write("\n");
            }

            //Console.ReadKey();

        }

        public void setBound()
        {
            for (int j = 0; j < ny; j++)
            {
                u[0, j] = (36 - (double)(j - (ny - 1) / 2) * (double)(j - (ny - 1) / 2) * 0.1);
                u[nx - 1, j] = (36 - (double)(j - (ny - 1) / 2) * (double)(j - (ny - 1) / 2) * 0.1);
                //u[0, j] = 0.25;
                //u[nx - 1, j] = 0.25;
            }

            //u[0, 0] = 0;
            //u[nx - 1, 0] = 0;
            //u[0, ny - 1] = 0;
            //u[nx - 1, ny - 1] = 0;

            for (int j = 0; j < ny; j++)
            {
                //u[0, j] = (double)(j - ny / 2) * (double)(j - ny / 2);
                //u[nx - 1, j] = (double)(j - ny / 2) * (double)(j - ny / 2);
                v[nx - 1, j] = 0;
                v[0, j] = 0;
            }

            for (int i = 0; i < nx; i++)
            {
                v[i, ny - 1] = 0;
                u[i, ny - 1] = 0;
                v[i, 0] = 0;
                u[i, 0] = 0;
            }
        }

        public void Set_uv()
        {
            for (int j = 0; j < ny; j++)
            {
                u[0, j] = (36 - (double)(j - (ny - 1) / 2) * (double)(j - (ny - 1) / 2) * 0.1);
                u[nx - 1, j] = (36 - (double)(j - (ny - 1) / 2) * (double)(j - (ny - 1) / 2) * 0.1);
                //u[0, j] = (j / ny - 0.5) * (j / ny - 0.5);
                //u[nx - 1, j] = (j / ny - 0.5) * (j / ny - 0.5);
                //u[0, j] = 0.25;
                //u[nx - 1, j] = 0.25;
                v[nx - 1, j] = 0;
                v[0, j] = 0;
            }

            for (int i = 0; i < nx; i++)
            {
                v[i, ny - 1] = 0;
                u[i, ny - 1] = 0;
                v[i, 0] = 0;
                u[i, 0] = 0;
            }

            for (int j = 1; j < ny - 1; j++)
            {
                for (int i = 1; i < nx - 1; i++)
                {
                    u[i, j] = (psi[i, j + 1] - psi[i, j - 1]) / (2 * hy);
                    v[i, j] = -(psi[i + 1, j] - psi[i - 1, j]) / (2 * hx);
                }
            }
        }

        public double[,] Get_u()
        {
            ////du/dx=(psi[i+1]-psi[i-1])/2h
            //double[,] u=new double[nx,ny];

            ////i=0
            //for (int j = 0; j < ny; j++)
            //{
            //    //u[0, j] = (psi[0, j] - psi[1, j]) / hx;
            //    u[0, j] = 0;
            //}

            //for (int i = 0; i < nx; i++)
            //{
            //    for (int j = 1; j < ny-1; j++)
            //    {
            //        //u[i, j] = (psi[i - 1, j] - psi[i + 1, j]) / (2 * hx);
            //        u[i,j] = (psi[i,j + 1] - psi[i,j - 1]) / (2 * hy);
            //    }
            //}

            ////i=nx-1
            //for (int j = 0; j < ny; j++)
            //{
            //    //u[nx - 1, j] = (psi[nx - 2, j] - psi[nx - 1, j]) / hx;
            //    u[nx - 1, j] = 0;
            //}

            return u;
        }

        public double[,] Get_v()
        {
            ////dv/dy=-(psi[i+1]-psi[i-1])/2h
            //double[,] v = new double[nx, ny];

            ////j=0
            //for (int i = 0; i < nx; i++)
            //{
            //    //v[i, 0] = - (psi[i, 0] - psi[0, 1]) / hy;
            //    v[i, 0] = 0;
            //}

            //for (int i = 1; i < nx-1; i++)
            //{
            //    for (int j = 0; j < ny; j++)
            //    {
            //        //v[i, j] = - (psi[i , j-1] - psi[i , j+1]) / (2 * hy);
            //        v[i,j] = -(psi[i + 1,j] - psi[i - 1,j]) / (2 * hx);
            //    }
            //}

            ////j=ny-1
            //for (int i = 0; i < nx; i++)
            //{
            //    //v[i, ny-1] = - (psi[i, ny-2] - psi[i, ny-1]) / hy;
            //    v[i, ny - 1] = 0;
            //}

            return v;
        }

        public void set_f(double[,] in_f)
        {

            for (int i = 0; i < nx; i++)
            {
                for (int j = 0; j < ny; j++)
                {
                    f[i, j] = in_f[i, j];
                }
            }
        }

        public double[,] get_psi()
        {
            return psi;
        }
        

    }
}
