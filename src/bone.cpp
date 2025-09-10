#include "bone.hpp"

Bone::Bone(std::string _name, size index, aiNodeAnim &channel) {
    (void)channel;

    bone_index = index;
    name = _name;
    local_transform = mat4();

    // for (size i = 0; i < npos; i++) {
    //     KeyPos pos;
    //     a_vec3_to_cglm(&channel->mPositionKeys[i].mValue, pos.pos);
    //     pos.timestamp = (float)channel->mPositionKeys[i].mTime;
    //     pos[i] = pos;
    // }

    // for (size i = 0; i < nrot; i++) {
    //     KeyRot rot;
    //     a_quat_to_cglm(&channel->mRotationKeys[i].mValue, rot.orientation);
    //     rot.timestamp = (float)channel->mRotationKeys[i].mTime;
    //     rot[i] = rot;
    // }

    // for (size i = 0; i < nscale; i++) {
    //     KeyScale scale;
    //     a_vec3_to_cglm(&channel->mScalingKeys[i].mValue, scale.scale);
    //     scale.timestamp = (float)channel->mScalingKeys[i].mTime;
    //     b->scale[i] = scale;
    // }
}

// void interpolate_pos(Bone* b, float time, mat4 out);
// void interpolate_rot(Bone* b, float time, mat4 out);
// void interpolate_scale(Bone* b, float time, mat4 out);

// size pos_index(const Bone* b, float time);
// size rot_index(const Bone* b, float time);
// size scale_index(const Bone* b, float time);

// float scale_factor(float last_time, float next_time, float time);

// void bone_update(Bone* b, float time) {
//     mat4 translation;
//     interpolate_pos(b, time, translation);

//     mat4 rotation;
//     interpolate_rot(b, time, rotation);

//     mat4 scale;
//     interpolate_scale(b, time, scale);

//     mat4 tmp;
//     glm_mat4_mul(rotation, scale, tmp);
//     glm_mat4_mul(translation, tmp, b->local_transform);
// }

// void interpolate_pos(Bone* b, float time, mat4 out) {
//     if (b->npos == 1) {
//         glm_translate_make(out, b->pos[0].pos);
//         return;
//     }

//     size i0 = pos_index(b, time);
//     size i1 = i0 + 1;

//     float factor = scale_factor(b->pos[i0].timestamp, b->pos[i1].timestamp, time);
//     vec3 interp;
//     glm_vec3_lerp(b->pos[i0].pos, b->pos[i1].pos, factor, interp);

//     glm_translate_make(out, interp);
// }

// void interpolate_rot(Bone* b, float time, mat4 out) {
//     if (b->nrot == 1) {
//         versor q;
//         glm_quat_normalize_to(b->rot[0].orientation, q);
//         glm_quat_mat4(q, out);
//         return;
//     }

//     size i0 = rot_index(b, time);
//     size i1 = i0 + 1;

//     float factor = scale_factor(b->rot[i0].timestamp, b->rot[i1].timestamp, time);
//     versor q;
//     glm_quat_slerp(b->rot[i0].orientation, b->rot[i1].orientation, factor, q);
//     glm_quat_normalize(q);

//     glm_quat_mat4(q, out);
// }

// void interpolate_scale(Bone* b, float time, mat4 out) {
//     if (b->nscale == 1) {
//         glm_scale_make(out, b->scale[0].scale);
//         return;
//     }

//     size i0 = scale_index(b, time);
//     size i1 = i0 + 1;

//     float factor = scale_factor(b->scale[i0].timestamp, b->scale[i1].timestamp, time);
//     vec3 interp;
//     glm_vec3_lerp(b->scale[i0].scale, b->scale[i1].scale, factor, interp);

//     glm_scale_make(out, interp);
// }

// inline size pos_index(const Bone* b, float time) {
//     for (size i = 0; i < b->npos - 1; ++i) {
//         if (time < b->pos[i + 1].timestamp) {
//             return (int)i;
//         }
//     }
//     log_dang("Missing bone %d pos index", b->bone_index);
//     return 0;
// }

// inline size rot_index(const Bone* b, float time) {
//     for (size i = 0; i < b->nrot - 1; ++i) {
//         if (time < b->rot[i + 1].timestamp) {
//             return (int)i;
//         }
//     }
//     log_dang("Missing bone %d rot index", b->bone_index);
//     return 0;
// }

// inline size scale_index(const Bone* b, float time) {
//     for (size i = 0; i < b->nscale - 1; ++i) {
//         if (time < b->scale[i + 1].timestamp) {
//             return (int)i;
//         }
//     }
//     log_dang("Missing bone %d scale index", b->bone_index);
//     return 0;
// }

// inline float scale_factor(float last_time, float next_time, float time) {
//     float mid = time - last_time;
//     float diff = next_time - last_time;
//     return diff != 0.0f ? (mid / diff) : 0.0f;
// }
