#include "mesh.hpp"

#include "animations.hpp"
#include "assimp.hpp"
#include "constants.hpp"

Mesh mesh_for_type(MESH type, Animations &animations) {
    std::string file;
    switch (type) {
        case MESH::GOB:
            file = "assets/models/gob.fbx";
            break;
        default:
            log_dang("Unrecognized mesh type %d", type);
            break;
    }

    Assimp::Importer import;

    const aiScene* scene = import.ReadFile(
        file.c_str(), aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs |
                          aiProcess_ImproveCacheLocality | aiProcess_GlobalScale);

    if (!scene) {
        log_dang("Missing model file %s", file.c_str());
    }

    if (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        log_dang("%s: %s", file.c_str(), import.GetErrorString());
    }

    aiNode* node = scene->mRootNode;
    while (node->mNumMeshes == 0) {
        for (size i = 0; i < node->mNumChildren; i++) {
            aiNode* child = node->mChildren[i];
            if (child->mNumMeshes == 0) {
                continue;
            }
            node = child;
        }
    }

    if (!node) {
        log_dang("%s: no meshes found", file.c_str());
    }

    return mesh_from_scene_node(type, scene, animations, node);
}

void set_bone_vertex_data(BoneVertex &b, size bone_index, float weight);

Mesh mesh_from_scene_node(MESH type, const aiScene* scene, Animations &animations,
                          const aiNode* node) {
    u32 texture_id = 0;
    switch (type) {
        case MESH::GOB:
            texture_id = GOB_TEXTURE;
            break;
        default:
            log_dang("Unrecognized mesh type %d", type);
            break;
    }
    Mesh m = {};
    m.texture_id = texture_id;

    aiMesh* mesh = scene->mMeshes[node->mMeshes[(unsigned int)0]];

    for (size i = 0; i < mesh->mNumVertices; i++) {
        BoneVertex b = {};

        b.bone_indexes.fill(-1);
        b.bone_weights.fill(0.0f);

        vec3 temp;

        /* -------------------------------------------------------------------------- */
        /*                                     POS                                    */
        /* -------------------------------------------------------------------------- */
        temp[0] = mesh->mVertices[i].x;
        temp[1] = mesh->mVertices[i].y;
        temp[2] = mesh->mVertices[i].z;
        b.pos = temp;

        /* -------------------------------------------------------------------------- */
        /*                                   NORMAL                                   */
        /* -------------------------------------------------------------------------- */
        if (mesh->mNormals) {
            temp[0] = mesh->mNormals[i].x;
            temp[1] = mesh->mNormals[i].y;
            temp[2] = mesh->mNormals[i].z;
            b.norm = temp;
        }

        /* -------------------------------------------------------------------------- */
        /*                                     UVS                                    */
        /* -------------------------------------------------------------------------- */
        if (mesh->mTextureCoords[0]) {
            vec2 uv;
            uv[0] = mesh->mTextureCoords[0][i].x;
            uv[1] = mesh->mTextureCoords[0][i].y;
            b.uv = uv;
        } else {
            b.uv = { 0, 0 };
        }

        /* -------------------------------------------------------------------------- */
        /*                                    COLOR                                   */
        /* -------------------------------------------------------------------------- */
        b.color = 1.0f;

        m.verts.push_back(b);
    }

    /* -------------------------------------------------------------------------- */
    /*                                    BONES                                   */
    /* -------------------------------------------------------------------------- */
    for (size bidx = 0; bidx < mesh->mNumBones; bidx++) {
        size index = -1;
        std::string name = mesh->mBones[bidx]->mName.data;

        if (auto result = m.table.find(name); result != m.table.end()) {
            index = result->second.index;
        } else {
            aiMatrix4x4 mat = mesh->mBones[bidx]->mOffsetMatrix;
            BoneInfo info = { .index = (i16)m.table.size(), .offset = to_glm(mat) };
            index = info.index;
            m.table.insert({ name, info });
        }
        assert(index != -1);

        aiVertexWeight* weights = mesh->mBones[bidx]->mWeights;
        for (size widx = 0; widx < mesh->mBones[bidx]->mNumWeights; widx++) {
            u32 v_id = weights[widx].mVertexId;
            float weight = weights[widx].mWeight;
            set_bone_vertex_data(m.verts[v_id], index, weight);
        }
    }

    for (size i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (size j = 0; j < face.mNumIndices; j++) {
            m.indices.push_back((u16)face.mIndices[j]);
        }
    }

    assert(m.verts.size() > 0);
    assert(m.indices.size() > 0);

    /* -------------------------------------------------------------------------- */
    /*                                 ANIMATIONS                                 */
    /* -------------------------------------------------------------------------- */

    for (unsigned int i = 0; i < scene->mNumAnimations; i++) {
        aiAnimation* anim = scene->mAnimations[i];
        std::string name = anim->mName.C_Str();
        name = name.substr(name.find('|') + 1, name.size());
        std::transform(name.begin(), name.end(), name.begin(),
                       [](unsigned char c) { return std::toupper(c); });
        log_info("%s", name.data());

        animations.add(ANIMATION::IDLE, Animation(scene, anim, m));
    }

    return m;
}

void set_bone_vertex_data(BoneVertex &b, size bone_index, float weight) {
    for (size i = 0; i < BONE_MAX_PER_VERTEX; i++) {
        if (b.bone_indexes[i] < 0) {
            assert(bone_index < CHAR_MAX);
            b.bone_indexes[i] = (i8)bone_index;
            b.bone_weights[i] = weight;
            break;
        }
    }
}
