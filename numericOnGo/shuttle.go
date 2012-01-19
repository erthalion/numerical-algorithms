// shuttle project main.go
package Shuttle

import (
	"fmt"
	//"./Interfaces"
)

func printVector(vector []float64, n int) {
	for i := 1; i < n; i++ {
		fmt.Printf("%f ", vector[i])
	}
	fmt.Printf("\n\n")

}

type ShuttleRight struct {
}

func (p *ShuttleRight) Shuttle(A, C, B, F []float64, u0 float64, n int) []float64 {
	C1 := make([]float64, n)
	F1 := make([]float64, n)
	x := make([]float64, n)

	C1[0] = 0
	F1[0] = u0

	for i := 1; i < n; i++ {
		C1[i] = -B[i-1] / (C[i-1] + A[i-1]*C1[i-1])
		F1[i] = (F[i-1] - A[i-1]*F1[i-1]) / (C[i-1] + A[i-1]*C1[i-1])
	}

	//x[n-1] = (C1[n-2]*F1[n-1] + F[n-1]) / (1 - C1[n-2]*C1[n-1])
	x[n-1] = (F[n-1] - A[n-1]*F1[n-1]) / (C[n-1] + A[n-1]*C1[n-1])

	for i := n - 2; i >= 0; i-- {
		x[i] = C1[i+1]*x[i+1] + F1[i+1]
	}

	return x
}
