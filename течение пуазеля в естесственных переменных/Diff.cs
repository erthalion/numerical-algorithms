using System;
using System.Collections.Generic;
using System.Text;
using System.Globalization;
using System.Threading;
using System.IO;

namespace WpfApplication1
{
    class Diff
    {
        int nx, ny;
        double tau;
        double hx, hy, lenght, hight;
        int iter;
        int T;
        FileStream f;
        string file_name;

        double vs, Re;
        double[,] C;
        double[,] u;
        double[,] v;


        public void Create(int in_nx, int in_ny, double in_l, double in_h, double in_vs, double in_D, double[,] in_u, double[,] in_v, int count, double t, string in_file_name)
        {
            tau = t;
            T = count;
            nx = in_nx;
            ny = in_ny;
            lenght = in_l;
            hight = in_h;
            vs = in_vs;
            Re = in_D;

            file_name = in_file_name;

            tau = 0.01;
            hx = lenght / (nx - 1);
            hy = hight / (ny - 1);

            u = new double[nx, ny];
            v = new double[nx, ny];
            C = new double[nx, ny];

            for (int i = 0; i < nx; i++)
            {
                for (int j = 0; j < ny; j++)
                {
                    u[i, j] = in_u[i, j];
                    v[i, j] = in_v[i, j];
                    C[i, j] = 0;
                }
            }

        }

        public void Debug()
        {
            Console.Write("\nC\n");
            for (int i = 0; i < nx; i++)
            {
                for (int j = 0; j < ny; j++)
                {
                    Console.Write(C[i, j]+" ");
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

        private double[] shuttle(double[] A, double[] C, double[] B, double[] F, int n)
        {
            double[] C1 = new double[n];
            double[] F1 = new double[n];
            double[] x = new double[n];


            C1[0] = C[0];
            for (int i = 1; i < n; i++)
            {
                C1[i] = C[i] - A[i] * B[i - 1] / C1[i - 1];
            }

            F1[0] = F[0];
            for (int i = 1; i < n; i++)
            {
                F1[i] = F[i] - A[i] * F1[i - 1] / C1[i - 1];
            }

            x[n - 1] = F1[n - 1] / C1[n - 1];
            for (int i = n - 2; i >= 0; i--)
            {
                x[i] = (F1[i] - B[i] * x[i + 1]) / C1[i];
            }

            return x;

        }
        /*private double[] shuttle(double[] A, double[] C, double[] B, double[] F, int n)
        {
            double[] x = new double[n];
            for (int i = 1; i < n; i++)
            {
                double m = C[i] / A[i - 1];
                A[i] = A[i] - m * B[i - 1];
                F[i] = F[i] - m * F[i - 1];
            }

            x[n - 1] = F[n - 1] / A[n - 1];

            for (int i = n - 2; i >= 0; i--)
            {
                x[i] = (F[i] - C[i] * x[i + 1]) / A[i];
            }

            return x;
        }*/


        public void Print(string file_name)
        {
            CultureInfo newCInfo = (CultureInfo)Thread.CurrentThread.CurrentCulture.Clone();
            newCInfo.NumberFormat.NumberDecimalSeparator = ".";
            Thread.CurrentThread.CurrentCulture = newCInfo;
            FileStream f = new FileStream(file_name, FileMode.Create);

            StreamWriter f_out = new StreamWriter(f);

            f_out.Write("TITLE=\"USERData\"\r\n");
            f_out.Write("VARIABLES=x,y,u");
            f_out.Write("\r\nZONE T=\"ZONE1\", i=" + nx + " j=" + ny + " f=Point");

            for (int i = 0; i < nx; i++)
            {
                for (int j = 0; j < ny; j++)
                {
                    f_out.Write("\r\n" + hx * i + " " + hy * j + " " + C[i, j]);//psi[i, ny - j - 1]);
                }
            }
            f_out.Close();
            f.Close();
        }

        public void set_boundary(double[,] psi)
        {
            //через производные по скоростям
            for (int j = 1; j < ny - 1; j++)
            {
                C[0,j] = (u[1,j] - u[0,j]) / hy - (v[1,j] - v[0,j]) / hx;
                C[nx - 1,j] = (u[nx - 1,j] - u[nx - 2,j]) / hy - (v[nx - 1,j] - v[nx - 2,j]) / hx;
            }
            for (int i = 0; i < nx; i++)
            {
                C[i,0] = (u[i,1] - u[i,0]) / hy - (v[i,1] - v[i,0]) / hx;
                C[i,ny - 1] = (u[i,ny - 1] - u[i,ny - 2]) / hy - (v[i,ny - 1] - v[i,ny - 2]) / hx;
            }

            //Debug();
            //Console.ReadKey();
            /*for (int i = 0; i < nx; i++)
            {
                for (int j = 0; j < ny; j++)
                {
                    Console.Write(String.Format("u[,]={0}\n", u[i, j]));
                    Console.ReadKey();
                }
            }*/

            //for (int i = 0; i < nx; i++)
            //{
            //    C[i, 0] = (u[i, 0] - u[i, 1]) / hx - (v[i, 0] - v[i, 1]) / hy;
            //    C[i, ny - 1] = (u[i, nx - 2] - u[i, nx - 1]) / hx - (v[i, nx - 2] - v[i, nx - 1]) / hy;
            //}
            //for (int j = 1; j < ny - 1; j++)
            //{
            //    C[0, j] = (u[0, j] - u[1, j]) / hx - (v[0, j] - v[1, j]) / hy;
            //    C[nx - 1, j] = (u[nx - 2, j] - u[nx - 1, j]) / hx - (v[nx - 2, j] - v[nx - 1, j]) / hy;
            //}

            //условие тома
            //for (int i = 0; i < nx; i++)
            //{
            //    //C[i, 0] = 2 * (psi[i, 1] - u[i, 0] * hy) / (hy * hy);
            //    //C[i, ny - 1] = 2*(psi[i, ny - 2] - u[i, ny - 1] * hx) / (hx * hx);
            //    C[0, i] = 2 * (psi[1, i] - v[0, i] * hy) / (hy * hy);
            //    C[ny - 1, i] = 2 * (psi[ny - 2, i] - v[ny - 1, i] * hx) / (hx * hx);
            //}

            //for (int j = 1; j < ny - 1; j++)
            //{
            //    //C[0,j]=-2*(psi[1,j]+v[1,j]*hx) / (hx*hx);
            //    //C[nx-1,j]=-2*(psi[nx-2,j]+v[nx-1,j]*hx) / (hx*hx);
            //    C[j, 0] = -2 * (psi[j, 1] + u[j, 1] * hx) / (hx * hx);
            //    C[j, nx - 1] = -2 * (psi[j, nx - 2] + u[j, nx - 1] * hx) / (hx * hx);
            //}
        }

        public void set_u_v(double[,] in_u, double[,] in_v)
        {
            for (int i = 0; i < nx; i++)
            {
                for (int j = 0; j < ny; j++)
                {
                    u[i, j] = in_u[i, j];
                    v[i, j] = in_v[i, j];
                }
            }

        }

        public void begin_solve()
        {
            CultureInfo newCInfo = (CultureInfo)Thread.CurrentThread.CurrentCulture.Clone();
            newCInfo.NumberFormat.NumberDecimalSeparator = ".";
            Thread.CurrentThread.CurrentCulture = newCInfo;
            f = new FileStream(file_name, FileMode.Create);

            StreamWriter f_out = new StreamWriter(f);
            f_out.Write("TITLE=\"USERData\"\r\n");
            f_out.Write("VARIABLES=x,y,u");

            f_out.Close();
            f.Close();
        }

        public double[,] Solve(double[,] psi)
        {
            CultureInfo newCInfo = (CultureInfo)Thread.CurrentThread.CurrentCulture.Clone();
            newCInfo.NumberFormat.NumberDecimalSeparator = ".";
            Thread.CurrentThread.CurrentCulture = newCInfo;
            f = new FileStream(file_name, FileMode.Append);

            StreamWriter f_out = new StreamWriter(f);

            //f_out.Write("TITLE=\"USERData\"\r\n");
            //f_out.Write("VARIABLES=x,y,u");

            double[,] C1 = new double[nx, ny];
            for (int i = 0; i < nx; i++)
            {
                for (int j = 0; j < ny; j++)
                {
                    C1[i, j] = C[i, j];
                }
            }

            double[] x = new double[nx];
            double[] d = new double[nx];
            double[] x1 = new double[ny];
            double[] d1 = new double[ny];

            double AA, BB, CC, FF;

            for (int j = 0; j < ny; j++)
            {
                for (int i = 0; i < nx; i++)
                {
                    C1[i, j] = C[i, j];
                }
            }

            //по j
            for (int j = 1; j < ny - 1; j++)
            {
                x[0] = 0;
                d[0] = C1[0, j];
                x[nx - 1] = 0;
                d[nx - 1] = C1[nx - 1, j];

                for (int i = 1; i < nx - 1; i++)
                {
                    AA = u[i, j] / (2 * hx) - 1.0 / (Re * hx * hx);
                    BB = 2 / tau + 2.0 / (Re * hx * hx);
                    CC = -u[i, j] / (2 * hx) - 1.0 / (Re * hx * hx);
                    FF = 2 * C1[i, j] / tau - v[i, j] * (C1[i, j + 1] - C1[i, j - 1]) / (2 * hy) + (C1[i, j + 1] - 2 * C1[i, j] + C1[i, j - 1]) / (Re * hy * hy);

                    //Console.Write(String.Format("A[]={0}\nC[]={1}\nB[]={2}\nF[]={3}\n", A , B , C, F));
                    //Console.ReadKey();

                    x[i] = -AA / (BB + CC * x[i - 1]);
                    d[i] = (FF - CC * d[i - 1]) / (BB + CC * x[i - 1]);
                }

                C[nx - 1, j] = (x[nx - 1] * d[nx - 2] + d[nx - 1]) / (1 - x[nx - 2] * x[nx - 1]);

                for (int i = nx - 2; i >= 0; i--)
                {
                    C[i, j] = x[i] * C[i + 1, j] + d[i];
                }
            }

            //Console.Write("\n after cycle on j C[n/2,2]=" + W[n / 2, m-2]);
            //Console.ReadKey();


            for (int j = 0; j < ny; j++)
            {
                for (int i = 0; i < nx; i++)
                {
                    C1[i, j] = C[i, j];
                }
            }


            //по i
            for (int i = 1; i < nx - 1; i++)
            {
                x1[0] = 0;
                d1[0] = C1[i, 0];
                x1[ny - 1] = 0;
                d1[ny - 1] = C1[i, ny - 1];

                for (int j = 1; j < ny - 1; j++)
                {
                    AA = v[i, j] / (2 * hy) - 1.0 / (Re * hy * hy);
                    BB = 2 / tau + 2.0 / (Re * hy * hy);
                    CC = -v[i, j] / (2 * hy) - 1.0 / (Re * hy * hy);
                    FF = 2 * C1[i, j] / tau - u[i, j] * (C1[i + 1, j] - C1[i - 1, j]) / (2 * hx) + (C1[i + 1, j] - 2 * C1[i, j] + C1[i - 1, j]) / (Re * hx * hx);

                    //Console.Write(String.Format("A[]={0}\nC[]={1}\nB[]={2}\nF[]={3}\n", A, B, C, F));
                    //Console.ReadKey();

                    x1[j] = -AA / (BB + CC * x1[j - 1]);
                    d1[j] = (FF - CC * d1[j - 1]) / (BB + CC * x1[j - 1]);
                }

                C[i, ny - 1] = (x1[ny - 1] * d1[ny - 2] + d1[ny - 1]) / (1 - x1[ny - 2] * x1[ny - 1]);

                for (int j = ny - 2; j >= 0; j--)
                {
                    C[i, j] = x1[j] * C[i, j + 1] + d1[j];
                }
            }

            f_out.Close();
            f.Close();

            return C;
        }

    }
}
