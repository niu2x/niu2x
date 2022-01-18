#include <assimp/Importer.hpp> // C++ importer interface
#include <assimp/scene.h> // Output data structure
#include <assimp/postprocess.h> // Post processing flags

#include "niu2x/assert.h"
#include "niu2x/utils.h"
#include "niu2x/gfx.h"
#include "niu2x/global.h"

namespace nx::gfx {

static void mesh_init(mesh_t* mesh, const aiMesh* ai_mesh)
{
    NX_ASSERT(ai_mesh->mPrimitiveTypes == aiPrimitiveType_TRIANGLE,
        "not TRIANGLE mesh: %d", ai_mesh->mPrimitiveTypes);

    auto vertices_num = ai_mesh->mNumVertices;
    auto vertex_layout = gfx::vertex_layout(
        vertex_attr_t::position, vertex_attr_t::normal, vertex_attr_t::uv);

    struct vertex_t {
        float x, y, z;
        float nx, ny, nz;
        float uvx, uvy, uvz;
    };

    std::vector<vertex_t> vertices;
    vertices.resize(vertices_num);

    for (int i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; i++) {
        auto texcoords = ai_mesh->mTextureCoords[i];
        if (texcoords) {
            for (uint32_t i = 0; i < vertices_num; i++) {
                vertices[i].uvx = texcoords[i].x;
                vertices[i].uvy = texcoords[i].y;
                vertices[i].uvz = texcoords[i].z;
            }
            break;
        }
    }

    for (uint32_t i = 0; i < vertices_num; i++) {
        vertices[i].x = ai_mesh->mVertices[i].x;
        vertices[i].y = ai_mesh->mVertices[i].y;
        vertices[i].z = ai_mesh->mVertices[i].z;

        vertices[i].nx = ai_mesh->mNormals[i].x;
        vertices[i].ny = ai_mesh->mNormals[i].y;
        vertices[i].nz = ai_mesh->mNormals[i].z;
        NX_LOG_D("vertex %d: %f %f %f", i, vertices[i].x, vertices[i].y,
            vertices[i].z);
    }

    mesh->vb
        = vertex_buffer_create(vertex_layout, vertices_num, vertices.data());
    auto faces_num = ai_mesh->mNumFaces;

    NX_LOG_D("mesh vertices_num %d", vertices_num);
    NX_LOG_D("mesh faces_num %d", faces_num);

    std::vector<uint32_t> indices;
    indices.resize(faces_num * 3);

    for (uint32_t i = 0; i < faces_num; i++) {
        indices[i * 3 + 0] = ai_mesh->mFaces[i].mIndices[0];
        indices[i * 3 + 1] = ai_mesh->mFaces[i].mIndices[1];
        indices[i * 3 + 2] = ai_mesh->mFaces[i].mIndices[2];

        NX_LOG_D("face %d: %d %d %d", i, indices[i * 3 + 0], indices[i * 3 + 1],
            indices[i * 3 + 2]);
    }

    mesh->ib = indice_buffer_create(faces_num * 3, indices.data());
}

static void mesh_group_init_node(
    mesh_group_t::node_t* node, const aiNode* ai_node)
{

    node->meshes_size = ai_node->mNumMeshes;
    node->meshes = NX_ALLOC(uint32_t, node->meshes_size);
    for (uint32_t i = 0; i < node->meshes_size; i++) {
        node->meshes[i] = ai_node->mMeshes[i];
    }

    node->transform[0][0] = ai_node->mTransformation.a1;
    node->transform[0][1] = ai_node->mTransformation.a2;
    node->transform[0][2] = ai_node->mTransformation.a3;
    node->transform[0][3] = ai_node->mTransformation.a4;

    node->transform[1][0] = ai_node->mTransformation.b1;
    node->transform[1][1] = ai_node->mTransformation.b2;
    node->transform[1][2] = ai_node->mTransformation.b3;
    node->transform[1][3] = ai_node->mTransformation.b4;

    node->transform[2][0] = ai_node->mTransformation.c1;
    node->transform[2][1] = ai_node->mTransformation.c2;
    node->transform[2][2] = ai_node->mTransformation.c3;
    node->transform[2][3] = ai_node->mTransformation.c4;

    node->transform[3][0] = ai_node->mTransformation.d1;
    node->transform[3][1] = ai_node->mTransformation.d2;
    node->transform[3][2] = ai_node->mTransformation.d3;
    node->transform[3][3] = ai_node->mTransformation.d4;

    math::mat4x4_transpose(node->transform);

    NX_LOG_D("meshes_size %d", node->meshes_size);
    gfx::mat4x4_dump(node->transform);

    node->children_size = ai_node->mNumChildren;
    node->children = NX_ALLOC(mesh_group_t::node_t, node->children_size);

    for (uint32_t i = 0; i < node->children_size; i++) {
        mesh_group_init_node(&(node->children[i]), ai_node->mChildren[i]);
    }
}

void mesh_group_init_from_file(
    mesh_group_t* mesh_group, const char* file, int flags)
{
    unused(flags);

    Assimp::Importer importer;

    auto import_flags = aiProcessPreset_TargetRealtime_Quality;
    import_flags |= aiProcess_FlipUVs;
    import_flags |= aiProcess_SortByPType;
    import_flags |= aiProcess_Triangulate;
    import_flags |= aiProcess_GenSmoothNormals;
    import_flags |= aiProcess_JoinIdenticalVertices;

    const aiScene* scene = importer.ReadFile(file, import_flags);
    NX_ASSERT(nullptr != scene, "%s", importer.GetErrorString());

    mesh_group->meshes = NX_ALLOC(mesh_t*, scene->mNumMeshes + 1);
    for (uint32_t i = 0; i < scene->mNumMeshes; i++) {
        mesh_group->meshes[i] = mesh_create();
        mesh_init(mesh_group->meshes[i], scene->mMeshes[i]);
    }
    mesh_group->meshes[scene->mNumMeshes] = nullptr;

    auto* ai_root = scene->mRootNode;
    mesh_group_init_node(&(mesh_group->root), ai_root);
}

void mesh_init_from_file(mesh_t* mesh, const char* file, int idx, int flags)
{
    unused(flags);

    Assimp::Importer importer;

    auto import_flags = aiProcessPreset_TargetRealtime_Quality;
    import_flags |= aiProcess_FlipUVs;
    import_flags |= aiProcess_SortByPType;
    import_flags |= aiProcess_Triangulate;
    import_flags |= aiProcess_GenSmoothNormals;
    import_flags |= aiProcess_JoinIdenticalVertices;

    const aiScene* scene = importer.ReadFile(file, import_flags);
    NX_ASSERT(nullptr != scene, "%s", importer.GetErrorString());

    NX_ASSERT((uint32_t)idx < scene->mNumMeshes, "");
    auto* ai_mesh = scene->mMeshes[idx];

    mesh_init(mesh, ai_mesh);
}

} // namespace nx::gfx
