// numeric project main.go
package main

import (
	"fmt"
	"math"
	"syscall"
	"strconv"
	"./Interfaces"
	"./SolverPPP"
	"./Grid"
	//"./SolverSR"
)

//main type
type solve struct {
	//dimension
	nx, ny int
	iter   int

	//steps
	tau, hx, hy float64

	length, height, a, k float64

	outputFileName string
	file           int

	solver Interfaces.Solver

	u [][]float64
	//original func exp(k*t)*sin(Pi*x)*sin(Pi*y)
}

func Original(iter,i, j int, p Interfaces.Solve) float64 {
	return math.Exp(p.GetTau()*p.GetK()*float64(iter)) * math.Sin(math.Pi*float64(i)*p.GetHx()) * math.Sin(math.Pi*float64(j)*p.GetHy())
}

func (p *solve) Initialize(nx, ny, iter int, length, height, tau, a, k float64, outputFileName string, param []Interfaces.Param, solv Interfaces.Solver, grid Interfaces.Grid) {
	p.iter = iter
	p.nx = nx
	p.ny = ny
	p.length = length
	p.height = height
	p.tau = tau
	p.hx = length / float64(nx-1)
	p.hy = height / float64(ny-1)
	p.outputFileName = outputFileName
	p.a = a
	p.k = k
	p.u = make([][]float64, nx)
	for i := 0; i < nx; i++ {
		p.u[i] = make([]float64, ny)
	}

	//initial value t=0
	for i := 0; i < nx; i++ {
		for j := 0; j < ny; j++ {
			p.u[i][j] = math.Sin(math.Pi*float64(i)*p.hx) * math.Sin(math.Pi*float64(j)*p.hy)
		}
	}
	
	
	//p.solver = new(SolverPPP.Solver)
	p.solver = solv
	p.solver.Initialize(p,grid , Original)
	p.solver.SetParameters(param)
}

func (p *solve) GetNx() int {
	return p.nx
}

func (p *solve) GetNy() int {
	return p.ny
}

func (p *solve) GetHx() float64 {
	return p.hx
}

func (p *solve) GetHy() float64 {
	return p.hy
}

func (p *solve) GetA() float64 {
	return p.a
}

func (p *solve) GetTau() float64 {
	return p.tau
}

func (p *solve) GetK() float64 {
	return p.k
}

func (p *solve) GetIter() int {
	return p.iter
}

func (p *solve) GetU() [][]float64 {
	return p.u
}

func (p *solve) OutputLayer(iter int) {
	if iter == 0 {
		p.file, _ = syscall.Open(p.outputFileName, syscall.O_RDWR|syscall.O_CREAT, 0666)
	}

	for i := 0; i < p.nx; i++ {
		for j := 0; j < p.ny; j++ {
			syscall.Write(p.file, []byte(strconv.Ftoa64(float64(i)*p.hx, 'f', 10)))
			syscall.Write(p.file, []byte(" "))
			syscall.Write(p.file, []byte(strconv.Ftoa64(float64(j)*p.hy, 'f', 10)))
			syscall.Write(p.file, []byte(" "))
			syscall.Write(p.file, []byte(strconv.Ftoa64(p.u[i][j], 'f', 10)))
			syscall.Write(p.file, []byte("\n"))
		}
		syscall.Write(p.file, []byte("\n"))
	}
	syscall.Write(p.file, []byte("\n"))
	
	if iter==p.iter {
		syscall.Close(p.file)
	}

}

func (p *solve) RunSolve() {
	var max float64
	var file, _ = syscall.Open("Norm", syscall.O_RDWR|syscall.O_CREAT, 0666)

	for i := 0; i < p.iter; i++ {
		p.solver.SolveLayer(i)
		p.OutputLayer(i)

		max = math.Fabs(p.u[0][0] - math.Exp(p.tau*p.k*float64(p.iter))*math.Sin(math.Pi*float64(0)*p.hx)*math.Sin(math.Pi*float64(0)*p.hy))
		for s := 0; s < p.nx; s++ {
			for j := 0; j < p.ny; j++ {
				//max = math.Fabs(math.Exp(p.tau*p.k*float64(i))*math.Sin(math.Pi*float64(s)*p.hx)*math.Sin(math.Pi*float64(j)*p.hy))
				//syscall.Write(file, []byte(strconv.Ftoa64(max, 'f', 10)))
				//syscall.Write(file, []byte(" "))

				if math.Fabs(p.u[s][j]-math.Exp(p.tau*p.k*float64(i))*math.Sin(math.Pi*float64(s)*p.hx)*math.Sin(math.Pi*float64(j)*p.hy)) > max {
					max = math.Fabs(p.u[s][j] - math.Exp(p.tau*p.k*float64(i))*math.Sin(math.Pi*float64(s)*p.hx)*math.Sin(math.Pi*float64(j)*p.hy))
				}
			}
			//syscall.Write(file, []byte("\n"))
		}
		//syscall.Write(file, []byte("\n"))

		syscall.Write(file, []byte(strconv.Ftoa64(max, 'f', 10)))
		syscall.Write(file, []byte("\n"))

		fmt.Printf("norm %f\n", max)
	}

	syscall.Close(file)
}

func main() {
	fmt.Println("begin solve")
	var Solve solve
	var Solver = new(SolverPPP.Solver)
	var Grid = new(Grid.RectilinearGrid)
	var Parameters=make([]Interfaces.Param, 1)
	Parameters[0].SetParam(1)
	var alfa=0.5
	k := -2 * math.Pi * math.Pi*alfa*alfa
	Solve.Initialize(50, 50, 50, 1, 1, 0.01, alfa, k, "output.dat",Parameters,Solver,Grid)
	Solve.RunSolve()
}
