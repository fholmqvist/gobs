#include "animation.hpp"

#include "assimp.hpp"
#include <assimp/anim.h>

void read_hierarchy_data(aiNode* src, AssimpNodeData &dst);

Animation::Animation(std::string path, Mesh m) {
    Assimp::Importer import;

    const aiScene* scene = import.ReadFile(
        path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GlobalScale);

    if (!scene) {
        log_dang("Missing model file %s", path.c_str());
    } else if (!scene->mRootNode) {
        log_dang("%s: No root node (file may contain only animations or skeletons)", path.c_str());
    } else if (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
        log_dang("%s: Incomplete scene (meshes: %u, materials: %u, animations: %u)", path.c_str(),
                 scene->mNumMeshes, scene->mNumMaterials, scene->mNumAnimations);
    } else if (scene->mNumAnimations == 0) {
        log_dang("%s: no animations", path.c_str());
    }

    // TODO:
    size anim_number = scene->mNumAnimations == 1 ? 0 : scene->mNumAnimations - 1;
    aiAnimation* node_anim = scene->mAnimations[anim_number];

    duration = (float)node_anim->mDuration;
    tps = (float)node_anim->mTicksPerSecond;

    global_inverse = aiMatrix4x4ToGlm(scene->mRootNode->mTransformation);
    global_inverse = glm::inverse(global_inverse);

    root_node.transform = mat4(1);

    read_hierarchy_data(scene->mRootNode, root_node);
    read_missing_bones(node_anim, &m);
}

Bone* Animation::find_bone(std::string name) {
    for (auto &bone : bones) {
        if (bone.name == name) {
            return &bone;
        }
    }
    return nullptr;
}

void Animation::read_missing_bones(aiAnimation* node_anim, Mesh* m) {
    auto mtable = m->table;

    for (size i = 0; i < node_anim->mNumChannels; i++) {
        aiNodeAnim* channel = node_anim->mChannels[i];
        std::string name = channel->mNodeName.data;
        BoneInfo info = { .index = (size)mtable.size(), .offset = mat4(1) };
        mtable.insert_or_assign(name, info);
        bones.push_back(Bone(name, info.index, *channel));
    }

    table = mtable;
}

void read_hierarchy_data(aiNode* src, AssimpNodeData &dst) {
    assert(src && src->mName.length > 0);
    dst.name = src->mName.data;
    dst.children.reserve(src->mNumChildren);

    dst.transform = aiMatrix4x4ToGlm(src->mTransformation);

    for (size i = 0; i < src->mNumChildren; i++) {
        AssimpNodeData child = {};
        read_hierarchy_data(src->mChildren[i], child);
        dst.children.push_back(child);
    }
}
