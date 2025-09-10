#pragma once

#include "pch.hpp" // IWYU pragma: keep

mat4 to_glm(const aiMatrix4x4 &);
vec3 to_glm(const aiVector3D &v);
quat to_glm(const aiQuaternion &q);
