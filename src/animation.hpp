#include "bone.hpp"
#include "mesh.hpp"

struct AssimpNodeData {
    std::string name;
    mat4 transform;
    std::vector<AssimpNodeData> children;
};

struct Animation {
    std::vector<Bone> bones;

    float tps;
    float duration;

    mat4 global_inverse;

    AssimpNodeData root_node;

    std::unordered_map<std::string, BoneInfo> table;

    Animation(std::string path, Mesh m);
    Bone* find_bone(std::string);
    void read_missing_bones(aiAnimation* node_anim, Mesh* m);
};
