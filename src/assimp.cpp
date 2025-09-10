#include "assimp.hpp"

mat4 to_glm(const aiMatrix4x4 &m) {
    return mat4(m.a1, m.b1, m.c1, m.d1, m.a2, m.b2, m.c2, m.d2, m.a3, m.b3, m.c3, m.d3, m.a4, m.b4,
                m.c4, m.d4);
}

vec3 to_glm(const aiVector3D &v) {
    return vec3(v.x, v.y, v.z);
}

quat to_glm(const aiQuaternion &q) {
    return quat(q.w, q.x, q.y, q.z);
}
