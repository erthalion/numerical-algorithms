#include "stdafx.h"

nonline::nonline(double b, double e, double eps, HMODULE m_dll)
{
	begin=b;
	end=e;
	epsilon=eps;
	dll=m_dll;
	function_max=(GetMaximumDiff)GetProcAddress(dll,"GetMaximumDiff");
	if(function_max==NULL)
	{
		MessageBoxA(NULL,"import function fail(GetMaximumDiff)","error",MB_OK);
		return ;
	}

	function_min=(GetMinimumDiff)GetProcAddress(dll,"GetMinimumDiff");
	if(function_min==NULL)
	{
		MessageBoxA(NULL,"import function fail(GetMinimumDiff)","error",MB_OK);
		return ;
	}

	function_x=(GetFunction)GetProcAddress(dll,"GetFunction");
	if(function_x==NULL)
	{
		MessageBoxA(NULL,"import function fail(GetFunction)","error",MB_OK);
		return ;
	}

	function_diff=(GetDiff)GetProcAddress(dll,"GetDiff");
	if(function_diff==NULL)
	{
		MessageBoxA(NULL,"import function fail(GetDiff)","error",MB_OK);
		return ;
	}

	function_solve=(GetSolve)GetProcAddress(dll,"GetSolve");
	if(function_solve==NULL)
	{
		MessageBoxA(NULL,"import function fail(GetSolve)","error",MB_OK);
		return ;
	}
	
	function_count=(GetCount)GetProcAddress(dll,"GetCount");
	if(function_count==NULL)
	{
		MessageBoxA(NULL,"import function fail(GetCount)","error",MB_OK);
		return ;
	}
	count=function_count();
	solve=new double[count-1];
	for(int i=0;i<count;i++)
	{
		solve[i]=(function_solve())[i];
	}

}


double nonline::div2(double b, double e)
{
	x_0=(b+e)/2;
//	if(function_diff(x_0)>0) k=-1;
//	else k=1;

//	if(function_diff(x_0)>0) k=-1*(1/function_max(b,e));
//	else k=-1*(1/function_min(b,e));
	k=-1*(1/function_max(b,e));

	if(function_max(b,e)*function_min(b,e)<0||abs(1+k*function_diff(x_0))>1)
	{
//	if(abs(1+k*function_diff(x_0))>1)
//	{
		if(function_x(b)==0)
		{
			x_0=b;
			return x_0;
		}
		if(function_x(e)==0)
		{
			x_0=e;
			return x_0;
		}
		if(function_x(b)*function_x((b+e)/2)<0)
		{
			e=(b+e)/2;
		}
		else
		{
			b=(b+e)/2;
		}
		div2(b,e);
	}
	begin1=b;end1=e;

return x_0;
}

double nonline::simple()
{
	double x_prev;
	double m=1;

	if(function_x(begin)*function_x(end)>0)
	{
		MessageBoxA(NULL,"insert correct [begin,end]","error",MB_ICONSTOP);
		return 0;
	}
	x_solve=div2(begin,end);

//	if(function_diff(x_solve)>0) k=-1*(1/function_max(begin,end));
//	else k=1*(1/function_max(begin,end));

	count_simple=0;
	while(m>epsilon)
	{
	x_prev=x_solve;
	x_solve=x_solve+k*function_x(x_solve);
	m=abs(x_solve-x_prev);
	//m=abs(function_x(x_solve));
	count_simple++;
	}
return x_solve;
}

double nonline::newton()
{
	double m=1;
	double x_prev;

	if(function_x(begin)*function_x(end)>0)
	{
		MessageBoxA(NULL,"insert correct [begin,end]","error",MB_ICONSTOP);
		return 0;
	}

	//if(function_diff(x_0)>0) k=-1*(1/function_max(begin,end));
	//else k=1*(1/function_max(begin,end));

	x_solve=div2(begin,end);

	count_newton=0;
	while(m>epsilon)
	{
		x_prev=x_solve;
		x_solve=x_solve-(function_x(x_solve)/function_diff(x_solve));
		m=abs(x_solve-x_prev);
		//m=abs(f(x_solve));
		count_newton++;
	}
return x_solve;
}

int nonline::get_simple_n()
{
	return count_simple;
}

int nonline::get_newton_n()
{
	return count_newton;
}

nonline::~nonline()
{
}

double nonline::get_err()
{
	double min=abs(solve[0]-x_solve);
	for(int i=0;i<count;i++)
	{
		if(min>abs(solve[i]-x_solve)) min=abs(solve[i]-x_solve);
	}
return min;
}

double nonline::get_begin1()
{
	return begin1;
}

double nonline::get_end1()
{
	return end1;
}
