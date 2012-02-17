/*
*  The purpose of this test is to simply check the installation 
*  of lssol. Especially, we want to make sure that the link was succesful
*  The problem solved is hence quite simple:
*   min    || ( -1   -2) (x0)  - (-1) ||
*   x0,x1  || ( -2   -1) (x1)    ( 1) ||
*  The solution is (-1, 1)
*/

#include <cmath>
#include <cstdio>
#include <iostream>
#include <cstring>

#include <mpc-walkgen/qp-solvers/lssol-solver.h>

using namespace Eigen;
using namespace MPCWalkgen;

/* Solve a simple quadratic programming problem: find values of x that minimize
f(x) = 1/2 x1^2 + x2^2 -x1.x2 - 2x1 - 6x2

subject to
 x1 +  x2 ≤ 2
–x1 + 2x2 ≤ 2
2x1 +  x2 ≤ 3
0 ≤ x1, 0 ≤ x2.

Expected result:
x   = 0.6667, 1.3333
obj = -8.2222
*/
int main ()
{
	LSSOLSolver qp(2,3);
	qp.reset();
	qp.nbVar(2);
	qp.nbCtr(3);


	// create the qp problem
	Matrix2d Q;
	Q << 1, -1, -1, 2;
	Q *= 0.5;
	qp.matrix(matrixQ).addTerm(Q);

	Vector2d P;
	P << -2, -6;
	qp.matrix(vectorP).addTerm(P);

	MatrixXd A(3,2);
	A << 1,  1,
	  -1,  2,
	   2,  1;
	qp.matrix(matrixA).addTerm(A);

	Vector3d bl;
	bl << -1e10, -1e10, -1e10;
	qp.matrix(vectorBL).addTerm(bl);


	Vector3d bu;
	bu << 2,2,3;
	qp.matrix(vectorBU).addTerm(bu);

	Vector2d xl;
	xl.setZero();
	qp.matrix(vectorXL).addTerm(xl);

	Vector2d xu;
	xu.fill(1e10);
	qp.matrix(vectorXU).addTerm(xu);

	MPCSolution result;
	result.reset();
	result.useWarmStart=false;
	result.initialSolution.resize(2);
	result.initialConstraints.resize(2+3);

	// solution supposee
	//  initial << 0.666, 1.33333;
	//  std::cout << (A * initial) << std::endl;
	//  std::cout << (initial.transpose() * Q * initial + P.transpose() * initial ) << std::endl;

	qp.solve(result);

	Vector2d expectedResult;
	expectedResult << 2./3., 4./3.;

	bool success = ((result.solution - expectedResult).norm() < 1e-9);
	return (success ? 0 : 1);
}