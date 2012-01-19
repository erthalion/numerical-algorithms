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
        double l = 1;
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

            for (int i = 0; i < nx; i++)
            {
                for (int j = 0; j < ny; j++)
                {
                    u[i, j] = 0;
                    v[i, j] = 0;
                }
            }

            //зависит от времени
            switch (u_type)
            {
                case (0): for (int i = 0; i < nx; i++)
                            {
                                u[i, ny - 1] = -0.1;
                            }
                            break;

                case (1): for (int i = 0; i < nx; i++)
                            {
                                u[i, ny - 1] = -Math.Sqrt(iter*0.1+0.1);
                            }
                            break;
                case (2): for (int i = 0; i < nx; i++)
                            {
                                u[i, ny - 1] = -(iter * 0.1+0.1);
                            }
                            break;
            }

            Diff diffusion = new Diff();
            diffusion.Create(nx, ny, l, h, vs, D, u, v, count, t, file_name);

            CSolve laplas = new CSolve();
            laplas.Create(nx, ny, l, h, k, eps);


            diffusion.begin_solve();
            while (iter < max_iter)
            {
                iter++;
                Console.WriteLine("iter " + iter.ToString());
                laplas.set_f(diffusion.Solve(laplas.get_psi()));
                laplas.Solve();
                u = laplas.Get_u();
                v = laplas.Get_v();

                    //зависит от времени
                    switch (u_type)
                    {
                        case (0): for (int i = 0; i < nx; i++)
                            {
                                u[i, ny - 1] = -0.1;
                            }
                            break;

                        case (1): for (int i = 0; i < nx; i++)
                            {
                                u[i, ny - 1] = -Math.Sqrt(iter * 0.1 + 0.1);
                            }
                            break;
                        case (2): for (int i = 0; i < nx; i++)
                            {
                                u[i, ny - 1] = -(iter * 0.1 + 0.1);
                            }
                            break;
                    }

                diffusion.set_u_v(u, v);

            }

            laplas.Print("out_laplas.dat");

            Process tecpot = Process.Start("Tecplot", "out.dat");

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
