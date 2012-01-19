#include <iostream>
#include "CSolve.h"


int main()
{
    CSolve *S;
    S=new CSolve(40,40,1,1,1,0.0001);
    //S->Solve();
    //S->Print("psi.dat");
	int n_left=2,n_right=1,n_top=0,n_down=0;
	double *left,*right,*top=NULL,*down=NULL;

	left=new double[5];
	right=new double[2];

	left[0]=0.0;
	left[1]=0.3;
	left[2]=5;

	left[3]=0.5;
	left[4]=0.3;
	left[5]=5;

	right[0]=0.2;
	right[1]=0.3;
	right[2]=10;

	S->CreateGrid(n_left,left,n_right,right,n_top,top,n_down,down);
	S->Print_Grid("grid.txt");
	S->Solve(2);
	S->Print("psi.dat");

    return 0;
}
