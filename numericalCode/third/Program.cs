using System;
using System.Collections.Generic;
using System.Text;
using WpfApplication1;

namespace third
{
    class Program
    {
        static void Main(string[] args)
        {
            int nx = 50;
            int ny = 50;
            double l = 1;
            double h = 1;
            double vs = 0;
            double D = 0.01;
            double t=0.01;
            int count=1;
            string file_name="out.dat";


            int iter=0;
            int max_iter=200;
            int k=0;    //коэфф стратификации
            double eps=0.001;

            double[,] u=new double[nx,ny];
            double[,] v=new double[nx,ny];

            for(int i=0;i<nx;i++)
            {
                for(int j=0;j<ny;j++)
                {
                    u[i,j]=0;
                    v[i,j]=0;
                }
            }

            //зависит от времени
            for(int i=0;i<nx;i++)
            {
                u[i,ny-1]=1;
            }

            Diff diffusion=new Diff();
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
                for (int i = 0; i < nx; i++)
                {
                    u[i, ny-1] = 1;
                }

            }


        }
    }
}
