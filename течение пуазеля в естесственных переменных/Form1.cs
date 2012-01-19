using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using WpfApplication1;
using System.Diagnostics;
using System.Threading;
using System.Globalization;

namespace third_gui
{
    public partial class Form1 : Form
    {
        int nx;
        int ny;
        double l = 2;
        double h = 1;
        double vs = 0;
        double D;
        double t = 0.01;
        int count = 1;
        string file_name = "out.dat";


        int iter = 0;
        int max_iter;
        int k;    //коэфф стратификации
        double eps;

        int u_type=0;

        double[,] u;
        double[,] v;


        public Form1()
        {
            InitializeComponent();
        }

        public void run_solve()
        {

            double[,] press = new double[nx, ny];

            for (int i = 0; i < nx; i++)
            {
                for (int j = 0; j < ny; j++)
                {
                    u[i, j] = 0;
                    v[i, j] = 0;
                    press[i, j] = 0;
                }
            }

            //зависит от времени
            switch (u_type)
            {
                case (0): for (int j = 0; j < ny; j++)
                            {
                                //u[0, j] = 0.01 * (36 - (double)(j - (ny - 1) / 2) * (double)(j - (ny - 1) / 2) * 0.1);
                                //u[nx - 1, j] = 0.01 * (36 - (double)(j - (ny - 1) / 2) * (double)(j - (ny - 1) / 2) * 0.1);
                                
                                //u[0, j] = (2 - (double)(j - (ny - 1) / 2) * (double)(j - (ny - 1) / 2) * 0.1);
                                //u[nx - 1, j] = (2 - (double)(j - (ny - 1) / 2) * (double)(j - (ny - 1) / 2) * 0.1);

                                //u[0, j] = 0.5 * (6 - (double)(j - (ny - 1) / 2) * (double)(j - (ny - 1) / 2) * 0.1);
                                //u[nx - 1, j] = 0.5 * (6 - (double)(j - (ny - 1) / 2) * (double)(j - (ny - 1) / 2) * 0.1);

                                u[0, j] = (2 - (double)(j - (ny - 1) / 2) * (double)(j - (ny - 1) / 2) * 0.1);
                                u[nx - 1, j] = (2 - (double)(j - (ny - 1) / 2) * (double)(j - (ny - 1) / 2) * 0.1);
                                //u[0, j] = 0.25;
                                //u[nx-1,j]=0.25;
                            }
            //u[0, 0] = 0;
            //u[nx - 1, 0] = 0;
            //u[0, ny - 1] = 0;
            //u[nx - 1, ny - 1] = 0;
                            break;

                case (1): for (int i = 0; i < nx; i++)
                            {
                                //u[i, ny - 1] = Math.Sqrt(iter*0.1+0.1);
                            }
                            break;
                case (2): for (int i = 0; i < nx; i++)
                            {
                                //u[i, ny - 1] = Convert.ToDouble(iter+1)*10 / Convert.ToDouble(max_iter);//(iter * 0.1+0.1);
                            }
                            break;
            }

            Diff diffusion = new Diff();
            diffusion.Create(nx, ny, l, h, vs, D, u, v, count, t, file_name);


            CSolve laplas = new CSolve();
            laplas.Create(nx, ny, l, h, k, eps);
            laplas.setBound();
            laplas.setBoundary();
            //laplas.Debug();
            //Console.ReadKey();

            diffusion.begin_solve();
            laplas.Print_Begin("out_laplas.dat");

            while (iter < max_iter)
            {
                //diffusion.Debug();
                //laplas.Debug();
                //Console.ReadKey();

                iter++;
                Console.WriteLine("iter " + iter.ToString());
                diffusion.set_boundary(null);
                laplas.set_f(diffusion.Solve(null));
                laplas.Solve();
                laplas.Set_uv();
                u = laplas.Get_u();
                v = laplas.Get_v();
                laplas.GetPressure();
                //laplas.Debug();
                //Console.ReadKey();

                //зависит от времени
                //switch (u_type)
                //{
                //    case (0): for (int i = 0; i < nx; i++)
                //        {
                //            //u[i, ny - 1] = 2;
                //            laplas.Set_uv(2);
                //        }
                //        break;

                //    case (1): for (int i = 0; i < nx; i++)
                //        {
                //            u[i, ny - 1] = Math.Sqrt(iter * 0.1 + 0.1);
                //        }
                //        break;
                //    case (2): for (int i = 0; i < nx; i++)
                //        {
                //            u[i, ny - 1] = Convert.ToDouble(iter + 1) / Convert.ToDouble(max_iter);//(iter * 0.1+0.1);
                //        }
                //        break;
                //}

                diffusion.set_u_v(u, v);
                diffusion.set_boundary(laplas.get_psi());
                laplas.Print("out_laplas.dat");
            }
            laplas.Print("out_laplas.dat");

            //Process tecpot = Process.Start("Tecplot", "out.dat");
        }

        private void button1_Click(object sender, EventArgs e)
        {
            CultureInfo newCInfo = (CultureInfo)Thread.CurrentThread.CurrentCulture.Clone();
            newCInfo.NumberFormat.NumberDecimalSeparator = ".";
            Thread.CurrentThread.CurrentCulture = newCInfo;

            nx = Convert.ToInt32(textBox1.Text);
            ny = Convert.ToInt32(textBox2.Text);
            D = Convert.ToDouble(textBox3.Text);

            max_iter = Convert.ToInt32(textBox5.Text);
            k = Convert.ToInt32(textBox6.Text);    //коэфф стратификации
            eps = Convert.ToDouble(textBox4.Text);

            u = new double[nx, ny];
            v = new double[nx, ny];

            if (radioButton1.Checked) u_type = 0;
            if (radioButton2.Checked) u_type = 1;
            if (radioButton3.Checked) u_type = 2;


            Thread solve = new Thread(new ThreadStart(run_solve));
            solve.Start();

        }
    }
}
