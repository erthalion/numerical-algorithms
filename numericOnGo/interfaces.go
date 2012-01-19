package Interfaces

type Param struct {
	value float64
}

func (p *Param) GetParam() float64 {
	return p.value
}

func (p *Param) SetParam(value float64) {
	p.value=value
}

//method
type Solver interface {
	Initialize(owner Solve, grid Grid, Original func(int,int,int,Solve) float64)
	SolveLayer(iter int)
	SetBoundary(iter int)
	SetParameters(p []Param)
}

//shuttle
type Shuttle interface {
	Shuttle(A, C, B, F []float64, u0 float64, n int) []float64
}

//grid mask
type Grid interface {
	Initialize(nx,ny int)
	GetNodeType(i,j int) int
}

//main
type Solve interface {
	Initialize(nx, ny, iter int, length, height, tau, a, k float64, outputFileName string, param []Param,solv Solver, grid Grid)
	OutputLayer(iter int)
	RunSolve()
	GetNx() int
	GetNy() int
	GetHx() float64
	GetHy() float64
	GetA() float64
	GetTau() float64
	GetK() float64
	GetIter() int
	GetU() [][]float64
}
