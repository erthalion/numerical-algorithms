// Grid.go
package Grid

import (
	//"./Interfaces"
)

type RectilinearGrid struct {
	nx,ny int
}

func (p *RectilinearGrid) Initialize(nx,ny int) {
	p.nx=nx
	p.ny=ny
}

func (p *RectilinearGrid) GetNodeType(i,j int) int {
	
	if i!=0&&j!=0 {
		return 0;
	}
	
	if i==0&&(j!=0||j!=p.ny-1) {
		return 1;
	}

	if j==0&&(i!=0||i!=p.nx-1) {
		return 2;
	}

	if i==p.nx-1&&(j!=0||j!=p.ny-1) {
		return 3;
	}

	if j==p.ny-1&&(i!=0||i!=p.nx-1) {
		return 4;
	}

	//else corner point
	return 5;
}