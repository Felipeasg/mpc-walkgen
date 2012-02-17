#ifndef TOOLS
#define TOOLS

////////////////////////////////////////////////////////////////////////////////
///
///\file	tools.h
///\brief	A class to compute bodies interpolation
///\author	Herdt Andrei
///\author	Lafaye Jory
///\author	Keith François
///\version	1.0
///\date	05/01/12
///
////////////////////////////////////////////////////////////////////////////////

#include <Eigen/Dense>

namespace MPCWalkgen{

	void pseudoInverse(const Eigen::MatrixXd & X, Eigen::MatrixXd & Xinv, double eps=1e-8);

	void inverse(const Eigen::MatrixXd & X, Eigen::MatrixXd & Xinv, double eps=1e-8);

	inline double pow2(double v);

	inline double pow3(double v);

	inline double pow4(double v);

	inline double pow5(double v);

	// \brief Return true if the matrix given is triangular.
	// \param [in] m
	bool isUpperTriangular(const Eigen::MatrixXd & m);

	// \brief Return true if the matrix given is triangular.
	// \param [in] m
	bool hasCholeskyForm(const Eigen::MatrixXd & m);

	// \brief Return true if the matrix given is block diagonal
	// \param [in] R the rotation matrix block diagonal
	bool isSparseRotationMatrix(const Eigen::MatrixXd & rot);

	// \brief Compute the multiplication of the rotation matrix R by M
	// \param [in out] mInOut = R mInOut R^T
	// \param [in] R the rotation matrix block diagonal
	void computeRM(Eigen::MatrixXd & mInOut, const Eigen::MatrixXd & rot);


	// \brief Compute the multiplication of the rotation matrix M by the R^t
	// \param [in out] mInOut = R mInOut R^T
	// \param [in] R the rotation matrix block diagonal
	void computeMRt(Eigen::MatrixXd & mInOut, const Eigen::MatrixXd & rot);

	// \brief This method considers only blocks of matrices 2.2, so as to reduce the
	// number of multiplications.
	// \param [in out] mInOut = R mInOut R^T
	// \param [in] R the rotation matrix block diagonal
	void rotateCholeskyMatrix(Eigen::MatrixXd & mInOut, const Eigen::MatrixXd & rot);
}

#include "tools.hxx"

/*! \fn void MPCWalkgen::pseudoInverse(const Eigen::MatrixXd & X, Eigen::MatrixXd & Xinv, double eps=1e-8)
* \brief Compute the pseudo inverse Xinv of the matrix X.
* \param X    [in] the matrix X
* \param Xinv [out] the pseudo inverse of X
* \param eps if a value is lower (in absolute) than eps, it replaced by 0
*/

/*! \fn void MPCWalkgen::inverse(const Eigen::MatrixXd & X, Eigen::MatrixXd & Xinv, double eps=1e-8)
* \brief Compute the inverse Xinv of the invertible matrix X.
* \param X    [in] the matrix X
* \param Xinv [out] the pseudo inverse of X
* \param eps if a value is lower (in absolute) than eps, it replaced by 0
*/

/*! \fn double MPCWalkgen::pow2(double v)
* \brief Return v^2
*/

/*! \fn double MPCWalkgen::pow3(double v)
* \brief Return v^3
*/

/*! \fn double MPCWalkgen::pow4(double v)
* \brief Return v^4
*/

/*! \fn double MPCWalkgen::pow5(double v)
* \brief Return v^5
*/

#endif //TOOLS