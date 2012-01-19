package SolverPPP

import (
	"./Interfaces"
	"./Shuttle"
)

//method
type Solver struct {
	owner Interfaces.Solve
	shuttle Interfaces.Shuttle
	grid Interfaces.Grid

	//dimension
	nx, ny int

	//steps
	tau, hx, hy float64

	a, k float64

	u [][]float64

	original func(int, int, int, Interfaces.Solve) float64
	//original func exp(k*t)*sin(Pi*x)*sin(Pi*y)

}

func (p *Solver) SetParameters(param []Interfaces.Param) {
}

func (p *Solver) Initialize(owner Interfaces.Solve, grid Interfaces.Grid, Original func(int, int, int, Interfaces.Solve) float64) {
	p.owner = owner
	p.shuttle = new(Shuttle.ShuttleRight)
	p.grid = grid

	p.nx = owner.GetNx()
	p.ny = owner.GetNy()

	p.hx = owner.GetHx()
	p.hy = owner.GetHy()
	p.tau = owner.GetTau()

	p.a = owner.GetA()
	p.u = owner.GetU()

	p.original = Original
}

func (p *Solver) SetBoundary(iter int) {
	//i=[0,nx] j=0
	for i := 0; i < p.nx; i++ {
		p.u[i][0] = p.original(iter, i, 0,p.owner) //math.Exp(float64(iter)*p.tau*p.k) * math.Sin(math.Pi*float64(i)*p.hx) * math.Sin(math.Pi*float64(0)*p.hy)
	}

	//i=[0,nx] j=ny-1
	for i := 0; i < p.nx; i++ {
		p.u[i][p.ny-1] = p.original(iter, i, p.ny-1,p.owner) //math.Exp(float64(iter)*p.tau*p.k) * math.Sin(math.Pi*float64(i)*p.hx) * math.Sin(math.Pi*float64(p.ny-1)*p.hy)
	}

	//i=0 j=[0,ny]
	for j := 0; j < p.ny; j++ {
		p.u[0][j] = p.original(iter, 0, j,p.owner) //math.Exp(float64(iter)*p.tau*p.k) * math.Sin(math.Pi*float64(0)*p.hx) * math.Sin(math.Pi*float64(j)*p.hy)
	}

	//i=nx-1 j=[0,ny]
	for j := 0; j < p.ny; j++ {
		p.u[p.nx-1][j] = p.original(iter, 0, p.nx-1,p.owner) //math.Exp(float64(iter)*p.tau*p.k) * math.Sin(math.Pi*float64(p.nx-1)*p.hx) * math.Sin(math.Pi*float64(j)*p.hy)
	}
}

func (p *Solver) SolveLayer(iter int) {
	var AA, CC, BB, FF []float64
	var temp [][]float64

	AA = make([]float64, p.nx-2)
	CC = make([]float64, p.nx-2)
	BB = make([]float64, p.nx-2)
	FF = make([]float64, p.nx-2)
	temp = make([][]float64, p.nx)
	var u_temp []float64

	p.SetBoundary(iter)

	for i := 0; i < p.nx; i++ {
		temp[i] = make([]float64, p.ny)
		for j := 0; j < p.ny; j++ {
			temp[i][j] = p.u[i][j]
		}
	}

	//first step
	for j := 1; j < p.ny-1; j++ {
		//fill coeff
		for i := 1; i < p.nx-1; i++ {
			FF[i-1] = -(p.a/p.hx)*(p.a/p.hx)*temp[i][j+1] + (2*(p.a*p.a/(p.hx*p.hx)-1/p.tau))*temp[i][j] - (p.a/p.hy)*(p.a/p.hx)*temp[i][j-1]
			AA[i-1] = (p.a / p.hx) * (p.a / p.hx)
			BB[i-1] = (p.a / p.hx) * (p.a / p.hx)
			CC[i-1] = -2 * (p.a*p.a/(p.hx*p.hx) + 1/p.tau)

			//fmt.Printf("%f %f %f %f\n",AA[i],CC[i],BB[i],FF[i])
		}

		//u_temp = Shuttle.Shuttle(AA, CC, BB, FF, p.u[0][j], p.ny-2)
		u_temp = p.shuttle.Shuttle(AA, CC, BB, FF, p.u[0][j], p.ny-2)
		for i := 1; i < p.nx-1; i++ {
			//in solving area
			if p.grid.GetNodeType(i,j)==0 {
				p.u[i][j] = u_temp[i-1]
			}
		}
	}

	AA = make([]float64, p.ny-2)
	CC = make([]float64, p.ny-2)
	BB = make([]float64, p.ny-2)
	FF = make([]float64, p.ny-2)

	for i := 0; i < p.nx; i++ {
		for j := 0; j < p.ny; j++ {
			temp[i][j] = p.u[i][j]
		}
	}

	//second step
	for i := 1; i < p.nx-1; i++ {
		//fill coeff
		for j := 1; j < p.ny-1; j++ {
			FF[j-1] = -(p.a/p.hy)*(p.a/p.hy)*temp[i+1][j] + (2*(p.a*p.a/(p.hy*p.hy)-1/p.tau))*temp[i][j] - (p.a/p.hy)*(p.a/p.hy)*temp[i-1][j]
			AA[j-1] = (p.a / p.hy) * (p.a / p.hy)
			BB[j-1] = (p.a / p.hy) * (p.a / p.hy)
			CC[j-1] = -2 * (p.a*p.a/(p.hy*p.hy) + 1/p.tau)

			//fmt.Printf("%f %f %f %f\n",AA[i],CC[i],BB[i],FF[i])

		}

		//u_temp = Shuttle.Shuttle(AA, CC, BB, FF, p.u[i][0], p.nx-2)
		u_temp = p.shuttle.Shuttle(AA, CC, BB, FF, p.u[i][0], p.nx-2)
		for j := 1; j < p.ny-1; j++ {
			if p.grid.GetNodeType(i,j)==0 {
				p.u[i][j] = u_temp[j-1]		
			}
		}

	}

}
