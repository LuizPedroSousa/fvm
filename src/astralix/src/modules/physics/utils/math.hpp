#pragma once

#include "foundation/PxMat44.h"
#include "glm/common.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <ctype.h>

#include "foundation/PxQuat.h"
#include "foundation/PxSimpleTypes.h"
#include "foundation/PxVec3.h"

namespace astralix {
inline glm::mat4 PxMat44ToGlmMat4(physx::PxMat44 pxMatrix) {
  glm::mat4 matrix;
  for (int x = 0; x < 4; x++)
    for (int y = 0; y < 4; y++)
      matrix[x][y] = pxMatrix[x][y];
  return matrix;
}

inline physx::PxQuat GlmQuatToPxQuat(glm::quat quat) {
  return {quat.x, quat.y, quat.z, quat.w};
}

inline physx::PxMat44 GlmMat4ToPxMat44(glm::mat4 glmMatrix) {
  physx::PxMat44 matrix;
  for (int x = 0; x < 4; x++)
    for (int y = 0; y < 4; y++)
      matrix[x][y] = glmMatrix[x][y];
  return matrix;
}

inline glm::quat PxQuatToGlmQuat(const physx::PxQuat &pxQuat) {
  return glm::quat(pxQuat.w, pxQuat.x, pxQuat.y, pxQuat.z);
}

} // namespace astralix
