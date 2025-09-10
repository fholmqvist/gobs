#include "bone.hpp"
#include "assimp.hpp"
#include <glm/matrix.hpp>

Bone::Bone(std::string _name, size index, aiNodeAnim &channel) {
    (void)channel;

    bone_index = index;
    name = _name;
    local_transform = mat4();

    for (size i = 0; i < channel.mNumPositionKeys; i++) {
        KeyPos p;
        p.pos = to_glm(channel.mPositionKeys[i].mValue);
        p.timestamp = (float)channel.mPositionKeys[i].mTime;
        pos[i] = p;
    }

    for (size i = 0; i < channel.mNumRotationKeys; i++) {
        KeyRot r;
        r.orientation = to_glm(channel.mRotationKeys[i].mValue);
        r.timestamp = (float)channel.mRotationKeys[i].mTime;
        rot[i] = r;
    }

    for (size i = 0; i < channel.mNumScalingKeys; i++) {
        KeyScale s;
        s.scale = to_glm(channel.mScalingKeys[i].mValue);
        s.timestamp = (float)channel.mScalingKeys[i].mTime;
        scale[i] = s;
    }
}

void interpolate_pos(Bone &b, float time, mat4 out);
void interpolate_rot(Bone &b, float time, mat4 out);
void interpolate_scale(Bone &b, float time, mat4 out);

size pos_index(const Bone &b, float time);
size rot_index(const Bone &b, float time);
size scale_index(const Bone &b, float time);

float scale_factor(float last_time, float next_time, float time);

mat4 interpolate_pos(Bone &b, float time) {
    if (b.pos.size() == 1) {
        return translate(mat4(1), b.pos[0].pos);
    }

    size i0 = pos_index(b, time);
    size i1 = i0 + 1;

    auto p0 = b.pos[i0];
    auto p1 = b.pos[i1];

    float factor = scale_factor(p0.timestamp, p1.timestamp, time);

    return translate(mat4(1), mix(p0.pos, p1.pos, factor));
}

mat4 interpolate_rot(Bone &b, float time) {
    if (b.rot.size() == 1) {
        quat q = b.rot[0].orientation;
        q = normalize(q);
        return mat4(q);
    }

    size_t i0 = rot_index(b, time);
    size_t i1 = i0 + 1;

    float factor = scale_factor(b.rot[i0].timestamp, b.rot[i1].timestamp, time);

    quat q0 = normalize(b.rot[i0].orientation);
    quat q1 = normalize(b.rot[i1].orientation);

    return mat4(normalize(slerp(q0, q1, factor)));
}

mat4 interpolate_scale(Bone &b, float time) {
    if (b.scale.size() == 1) {
        return scale(mat4(1), b.scale[0].scale);
    }

    size_t i0 = scale_index(b, time);
    size_t i1 = i0 + 1;

    float factor = scale_factor(b.scale[i0].timestamp, b.scale[i1].timestamp, time);

    auto s0 = b.scale[i0].scale;
    auto s1 = b.scale[i1].scale;

    return glm::scale(mat4(1), mix(s0, s1, factor));
}

size pos_index(Bone &b, float time) {
    for (size i = 0; i < (size)b.pos.size() - 1; ++i) {
        if (time < b.pos[i + 1].timestamp) {
            return (int)i;
        }
    }
    log_dang("Missing bone %d pos index", b.bone_index);
    return 0;
}

size rot_index(Bone &b, float time) {
    for (size i = 0; i < (size)b.rot.size() - 1; ++i) {
        if (time < b.rot[i + 1].timestamp) {
            return (int)i;
        }
    }
    log_dang("Missing bone %d rot index", b.bone_index);
    return 0;
}

size scale_index(Bone &b, float time) {
    for (size i = 0; i < (size)b.scale.size() - 1; ++i) {
        if (time < b.scale[i + 1].timestamp) {
            return (int)i;
        }
    }
    log_dang("Missing bone %d scale index", b.bone_index);
    return 0;
}

float scale_factor(float last_time, float next_time, float time) {
    float mid = time - last_time;
    float diff = next_time - last_time;
    return diff != 0.0f ? (mid / diff) : 0.0f;
}
