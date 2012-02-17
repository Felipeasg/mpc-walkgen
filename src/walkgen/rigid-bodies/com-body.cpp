#include <mpc-walkgen/rigid-bodies/com-body.h>
#include <mpc-walkgen/tools.h>

using namespace MPCWalkgen;
using namespace Eigen;


CoMBody::CoMBody(const MPCData * generalData,
		const RobotData * robotData,
		const Interpolation * interpolation)
	:RigidBody(generalData, robotData, interpolation)
{}

CoMBody::~CoMBody(){}

void CoMBody::interpolate(MPCSolution & solution, double currentTime, const VelReference & velRef){
	interpolation_->computeInterpolationByJerk(solution.state_vec[0].CoMTrajX_, solution.state_vec[0].CoMTrajY_, state_,
			dynamic(interpolationPos), solution.solution(0),
			solution.solution(generalData_->QPNbSamplings));

	interpolation_->computeInterpolationByJerk(solution.state_vec[1].CoMTrajX_, solution.state_vec[1].CoMTrajY_, state_,
			dynamic(interpolationVel), solution.solution(0),
			solution.solution(generalData_->QPNbSamplings));

	interpolation_->computeInterpolationByJerk(solution.state_vec[2].CoMTrajX_, solution.state_vec[2].CoMTrajY_, state_,
			dynamic(interpolationAcc), solution.solution(0),
			solution.solution(generalData_->QPNbSamplings));

	interpolation_->computeInterpolationByJerk(solution.CoPTrajX, solution.CoPTrajY, state_,
			dynamic(interpolationCoP), solution.solution(0),
			solution.solution(generalData_->QPNbSamplings));

	interpolateTrunkOrientation(solution, currentTime, velRef);
}

void CoMBody::computeOneDynamicMatrices(DynamicMatrix & dyn,
double S, double T, int N, DynamicMatrixType type){
	dyn.S.setZero(N,3);
	dyn.U.setZero(N,N);
	dyn.UT.setZero(N,N);
	dyn.UInv.setZero(N,N);
	dyn.UInvT.setZero(N,N);


	switch (type){
		case posDynamic:
			for(int i=0;i<N;++i){
				dyn.S(i,0) = 1;
				dyn.S(i,1) =i*T + S;
				dyn.S(i,2) = S*S/2 + i*T*S + i*i*T*T/2;

				dyn.U(i,0) = dyn.UT(0,i) =S*S*S/6 + i*T*S*S/2 + S*(i*i*T*T/2 );
				for(int j=1;j<N;j++){
					if (j<=i){
						dyn.U(i,j) = dyn.UT(j,i) =T*T*T/6 + 3*(i-j)*T*T*T/6 + 3*(i-j)*(i-j)*T*T*T/6;
					}
				}
			}
		break;

		case velDynamic:
			for(int i=0;i<N;i++){
				dyn.S(i,0) = 0.0;
				dyn.S(i,1) = 1.0;
				dyn.S(i,2) = i*T + S;

				dyn.U(i,0) = dyn.UT(0,i) = S*S/2 + i*T*S;
				for(int j=1;j<N;j++){
					if (j<=i){
						dyn.U(i,j) = dyn.UT(j,i) = T*T/2 + (i-j)*T*T;
					}
				}
			}
		break;

		case accDynamic:
			for(int i=0;i<N;i++){
				dyn.S(i,2) = 1.0;

				dyn.U(i,0) = dyn.UT(0,i) = S;
				for(int j=1;j<N;j++){
					if (j<=i){
						dyn.U(i,j) = dyn.UT(j,i) = T;
					}
				}
			}
		break;

		case copDynamic:
			for(int i=0;i<N;i++){
				dyn.S(i,0) = 1;
				dyn.S(i,1) =i*T + S;
				dyn.S(i,2) = S*S/2 + i*T*S + i*i*T*T/2-robotData_->CoMHeight/9.81;

				dyn.U(i,0) = dyn.UT(0,i) =S*S*S/6 + i*T*S*S/2 + S*(i*i*T*T/2 - robotData_->CoMHeight/9.81);
				for(int j=1;j<N;j++){
					if (j<=i){
						dyn.U(i,j) = dyn.UT(j,i) =T*T*T/6 + 3*(i-j)*T*T*T/6 + 3*(i-j)*(i-j)*T*T*T/6- T*robotData_->CoMHeight/9.81;
					}
				}

			}
			inverse(dyn.U,dyn.UInv);
			dyn.UInvT=dyn.UInv.transpose();
		break;

		default:
			dyn.U.setIdentity();
			dyn.UT.setIdentity();
		break;
	}
}

void CoMBody::interpolateTrunkOrientation(MPCSolution & result,
		double /*currentTime*/, const VelReference & velRef){

	Eigen::Matrix<double,6,1> factor;


	double T = generalData_->stepPeriod;/*
	if (result.supportState_vec[0].phase == ss){
		T = result.supportState_vec[0].startTime+generalData_->stepPeriod-currentTime;;
	}else{
		T = generalData_->stepPeriod;
	}*/


	int nbSampling = generalData_->nbIterationSimulation();

	if (result.state_vec[0].trunkYaw_.rows() != nbSampling){
		for(int i=0;i<3;++i){
			result.state_vec[i].trunkYaw_.resize(nbSampling);
		}
	}

	double orientation0 = result.supportTrunkOrientation_vec[0];
	double orientation1;
	double orientation2;

	int size = result.supportTrunkOrientation_vec.size();


	if (size>=2){
		orientation1 = result.supportTrunkOrientation_vec[1];
	}else{
		orientation1 = orientation0;
	}
	if (size>=3){
		orientation2 = result.supportTrunkOrientation_vec[2];
	}else{
		orientation2 = orientation1;
	}

	Vector3d nextTrunkState(orientation1, -velRef.local.yaw, 0);


	interpolation_->computePolynomialNormalisedFactors(factor, state().yaw, nextTrunkState, T);
	for(int i=0;i<nbSampling;++i){
		double ti = (i+1)*generalData_->simSamplingPeriod;

		result.state_vec[0].trunkYaw_(i) = interpolation_->p(factor, ti/T);
		result.state_vec[1].trunkYaw_(i) = interpolation_->dp(factor, ti/T)/T;
		result.state_vec[2].trunkYaw_(i) = interpolation_->ddp(factor, ti/T)/(T*T);

	}


}