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

        double vs, D;   //D=1/Re
        double[,] C;
        double[,] u;
        double[,] v;


        public void Create(int in_nx, int in_ny, double in_l, double in_h, double in_vs, double in_D, double[,] in_u, double[,] in_v, int count, double t,string in_file_name)
        {
            tau = t;
            T = count;
            nx = in_nx;
            ny = in_ny;
            lenght = in_l;
            hight = in_h;
            vs = in_vs;
            D = in_D;

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
            //for (int i = 0; i < nx; i++)
            //{
            //    C[i, 0] = (psi[i, 1] - psi[i, 0] - u[i, 0] * hy) * (2 / (hy * hy));
            //    C[i, ny-1] = (psi[i, ny-2] - psi[i, ny-1] - u[i, ny-1] * hy) * (2 / (hy * hy));
            //}

            //for (int j = 1; j < ny-1; j++)
            //{
            //    C[0, j] = -(psi[0, j] - psi[1, j] + v[0, j] * hx) * (2 / (hx * hx));
            //    C[nx-1, j] = -(psi[nx - 2, j] - psi[nx - 1, j] + v[nx - 1, j] * hx) * (2 / (hx * hx));
            //}
            for (int i = 1; i < nx - 1; i++)
            {
                C[i, 0] = (psi[i, 0] - u[i, 0] * hx) * (2 / (hx * hx));
                C[i, ny - 1] = (psi[i, ny - 1] + u[i, ny - 1] * hx) * (2 / (hx * hx));
            }
            for (int j = 0; j < ny; j++)
            {
                C[0, j] = - (psi[0, j] - v[0, j] * hy) * (2 / (hy * hy));
                C[nx - 1, j] = - (psi[nx - 1, j] + v[nx - 1 , j] * hy) * (2 / (hy * hy));
            }
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


            double[] AA = new double[nx];
            double[] CC = new double[nx];
            double[] FF = new double[nx];
            double[] BB = new double[nx];

            double[] temp_x = new double[nx];
            double[] temp_y = new double[ny];


            //задать начальные значения для вихря
            set_boundary(psi);

            //while (iter <= T)
            //{

                iter++;
                //printf("iter %d\n", iter);

                f_out.Write("\r\nZONE T=\"ZONE1\", i=" + nx + " j=" + ny + " f=Point");

                //i=0
                for (int j = 1; j < ny - 1; j++)
                {
                    AA[j] = (v[0, j] - vs) / (2 * hy) - D / (hy * hy);
                    CC[j] = 1 / (tau / 2) + 2 * D / (hy * hy);
                    BB[j] = -(v[0, j] - vs) / (2 * hy) - D / (hy * hy);
                    FF[j] = (-u[0, j] / (2 * hy) + D / (hy * hy)) * C[1, j] + (1 / (tau / 2) - 2 * D / (hy * hy)) * C[0, j] ;//+(u[i][j]/(2*hy)+D/(hy*hy))*C[i-1][j];
                }

                //на границе
                AA[0] = 0;
                CC[0] = 1 / hy;
                BB[0] = -1 / hy;
                FF[0] = 0;

                AA[ny - 1] = 1 / hy;
                CC[ny - 1] = -1 / hy;
                BB[ny - 1] = 0;
                FF[ny - 1] = 0;
                ///////////

                //C[0]=shuttle(AA,CC,BB,FF,ny);
                temp_y = shuttle(AA, CC, BB, FF, ny);

                for (int j = 0; j < ny; j++)
                {
                    C[0, j] = temp_y[j];
                }

                //внутри
                for (int i = 1; i < nx - 1; i++)
                {
                    for (int j = 1; j < ny - 1; j++)
                    {
                        AA[j] = (v[i, j] - vs) / (2 * hy) - D / (hy * hy);
                        CC[j] = 1 / (tau / 2) + 2 * D / (hy * hy);
                        BB[j] = -(v[i, j] - vs) / (2 * hy) - D / (hy * hy);
                        FF[j] = (-u[i, j] / (2 * hy) + D / (hy * hy)) * C[i + 1, j] + (1 / (tau / 2) - 2 * D / (hy * hy)) * C[i, j] + (u[i, j] / (2 * hy) + D / (hy * hy)) * C[i - 1, j];
                    }

                    //на границе
                    AA[0] = 0;
                    CC[0] = 1 / hy;
                    BB[0] = -1 / hy;
                    FF[0] = 0;

                    AA[ny - 1] = 1 / hy;
                    CC[ny - 1] = -1 / hy;
                    BB[ny - 1] = 0;
                    FF[ny - 1] = 0;
                    ///////////

                    //C[i]=shuttle(AA,CC,BB,FF,ny);
                    temp_y = shuttle(AA, CC, BB, FF, ny);

                    for (int j = 1; j < ny - 1; j++)
                    {
                        C[i, j] = temp_y[j];
                    }

                }

                //i=nx-1
                for (int j = 1; j < ny - 1; j++)
                {
                    AA[j] = (v[nx - 1, j] - vs) / (2 * hy) - D / (hy * hy);
                    CC[j] = 1 / (tau / 2) + 2 * D / (hy * hy);
                    BB[j] = -(v[nx - 1, j] - vs) / (2 * hy) - D / (hy * hy);
                    FF[j] =/*(-u[i][j]/(2*hy)+D/(hy*hy))*C[i+1][j]+*/ (1 / (tau / 2) - 2 * D / (hy * hy)) * C[nx - 1, j] + (u[nx - 1, j] / (2 * hy) + D / (hy * hy)) * C[nx - 2, j];
                }

                //на границе
                AA[0] = 0;
                CC[0] = 1 / hy;
                BB[0] = -1 / hy;
                FF[0] = 0;

                AA[ny - 1] = 1 / hy;
                CC[ny - 1] = -1 / hy;
                BB[ny - 1] = 0;
                FF[ny - 1] = 0;
                ///////////

                //C[nx-1]=shuttle(AA,CC,BB,FF,ny);
                temp_y = shuttle(AA, CC, BB, FF, ny);

                for (int j = 0; j < ny; j++)
                {
                    C[nx - 1, j] = temp_y[j];
                }


                //j=0
                for (int i = 1; i < nx - 1; i++)
                {
                    AA[i] = u[i, 0] / (2 * hx) - D / (hx * hx);
                    CC[i] = 1 / (tau / 2) + 2 * D / (hx * hx);
                    BB[i] = -u[i, 0] / (2 * hx) - D / (hx * hx);
                    FF[i] = (-(v[i, 0] - vs) / (2 * hy) + D / (hx * hx)) * C[i, 1] + (1 / (tau / 2) - 2 * D / (hx * hy)) * C[i, 0] ;//+((v[i][0]-vs)/(2*hx)+D/(hx*hx))*C[i][j-1];
                }

                //на границе
                AA[0] = 0;
                CC[0] = 1 / hx;
                BB[0] = -1 / hx;
                FF[0] = 0;

                AA[nx - 1] = 1 / hx;
                CC[nx - 1] = -1 / hx;
                BB[nx - 1] = 0;
                FF[nx - 1] = 0;
                ///////////

                //C[0]=shuttle(AA,CC,BB,FF,nx);
                temp_x = shuttle(AA, CC, BB, FF, nx);

                for (int i = 0; i < nx; i++)
                {
                    C[i, 0] = temp_x[i];
                }

                //внутри
                for (int j = 1; j < ny - 1; j++)
                {
                    for (int i = 1; i < nx - 1; i++)
                    {
                        AA[i] = u[i, j] / (2 * hx) - D / (hx * hx);
                        CC[i] = 1 / (tau / 2) + 2 * D / (hx * hx);
                        BB[i] = -u[i, j] / (2 * hx) - D / (hx * hx);
                        FF[i] = (-(v[i, j] - vs) / (2 * hy) + D / (hx * hx)) * C[i, j + 1] + (1 / (tau / 2) - 2 * D / (hx * hy)) * C[i, j] + ((v[i, j] - vs) / (2 * hx) + D / (hx * hx)) * C[i, j - 1];
                    }

                    //на границе
                    AA[0] = 0;
                    CC[0] = 1 / hx;
                    BB[0] = -1 / hx;
                    FF[0] = 0;

                    AA[nx - 1] = 1 / hx;
                    CC[nx - 1] = -1 / hx;
                    BB[nx - 1] = 0;
                    FF[nx - 1] = 0;
                    ///////////

                    //C[j]=shuttle(AA,CC,BB,FF,nx);
                    temp_x = shuttle(AA, CC, BB, FF, nx);

                    for (int i = 1; i < nx - 1; i++)
                    {
                        C[i, j] = temp_x[i];
                    }

                }

                //j=ny-1
                for (int i = 1; i < nx - 1; i++)
                {
                    AA[i] = u[i, ny - 1] / (2 * hx) - D / (hx * hx);
                    CC[i] = 1 / (tau / 2) + 2 * D / (hx * hx);
                    BB[i] = -u[i, ny - 1] / (2 * hx) - D / (hx * hx);
                    FF[i] =/*(-(v[i][ny-1]-vs)/(2*hy)+D/(hx*hx))*C[i][j+1]+*/(1 / (tau / 2) - 2 * D / (hx * hy)) * C[i, ny - 1] + ((v[i, ny - 1] - vs) / (2 * hx) + D / (hx * hx)) * C[i, ny - 2];
                }


                //на границе
                AA[0] = 0;
                CC[0] = 1 / hx;
                BB[0] = -1 / hx;
                FF[0] = 0;

                AA[nx - 1] = 1 / hx;
                CC[nx - 1] = -1 / hx;
                BB[nx - 1] = 0;
                FF[nx - 1] = 0;
                ///////////

                //C[ny-1]=shuttle(AA,CC,BB,FF,nx);
                temp_x = shuttle(AA, CC, BB, FF, nx);

                for (int i = 0; i < nx; i++)
                {
                    C[i, ny - 1] = temp_x[i];
                }


                //////////////////////////////////////////
                //задать начальные значения для вихря
                //set_boundary(psi);
                //////////////////////////////////////////

                //print("out.txt", nx, ny, C);

                for (int i = 0; i < nx; i++)
                {
                    for (int j = 0; j < ny; j++)
                    {
                        f_out.Write("\r\n" + hx * i + " " + hy * j + " " + C[i, j]);//psi[i, ny - j - 1]);
                    }
                }


            //}

            f_out.Close();
            f.Close();

            return C;
        }

    }
}
