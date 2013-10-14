////////////////////////////////////////////////////////////////////////////////
///
///\file humanoid_lip_com_velocity_tracking_objective.h
///\brief Implement the LIP CoM velocity tracking objective
///\author de Gourcuff Martin
///\date 12/07/13
///
////////////////////////////////////////////////////////////////////////////////

#ifndef MPC_WALKGEN_HUMANOID_VELOCITY_TRACKING_OBJECTIVE_H
#define MPC_WALKGEN_HUMANOID_VELOCITY_TRACKING_OBJECTIVE_H

#include "../type.h"
#include "../model/lip_model.h"
#include "../humanoid_feet_supervisor.h"

namespace MPCWalkgen{
  class HumanoidLipComVelocityTrackingObjective
  {
    public:
      HumanoidLipComVelocityTrackingObjective(
          const LIPModel& lipModel,
          const HumanoidFeetSupervisor& feetSupervisor);
      ~HumanoidLipComVelocityTrackingObjective();

      const VectorX& getGradient(const VectorX& x0);
      const MatrixX& getHessian();

      /// \brief Set the torso velocity reference in the world frame
      ///        It is a vector of size 2*N, with N the number of samples
      ///        (refX, refY)
      void setVelRefInWorldFrame(const VectorX& velRefInWorldFrame);

    private:
      const LIPModel& lipModel_;
      const HumanoidFeetSupervisor& feetSupervisor_;

      VectorX velRefInWorldFrame_;

      VectorX gradient_;
      MatrixX hessian_;
  };
}
#endif // MPC_WALKGEN_HUMANOID_VELOCITY_TRACKING_OBJECTIVE_H
