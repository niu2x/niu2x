#include <assimp/Importer.hpp> // C++ importer interface
#include <assimp/scene.h> // Output data structure
#include <assimp/postprocess.h> // Post processing flags

#include "niu2x/assert.h"
#include "niu2x/utils.h"
#include "niu2x/gfx.h"

namespace nx::gfx {

void mesh_init_from_file(mesh_t* mesh, const char* file, int idx)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(file,
        aiProcess_Triangulate | aiProcess_GenSmoothNormals
            | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
    NX_ASSERT(nullptr != scene, importer.GetErrorString())

    NX_ASSERT((uint32_t)idx < scene->mNumMeshes, "");
    auto* ai_mesh = scene->mMeshes[idx];
    NX_ASSERT(ai_mesh->mPrimitiveTypes == aiPrimitiveType_TRIANGLE, "");

    auto vertices_num = ai_mesh->mNumVertices;
    auto vertex_layout = vertex_layout_build(
        vertex_attr_type::position, vertex_attr_type::normal);

    struct vertex_t {
        float x, y, z;
        float nx, ny, nz;
    };

    vertex_t mid_point { 0, 0, 0, 0, 0, 0 };

    std::vector<vertex_t> vertices;
    vertices.resize(vertices_num);

    for (uint32_t i = 0; i < vertices_num; i++) {
        vertices[i].x = ai_mesh->mVertices[i].x;
        vertices[i].y = ai_mesh->mVertices[i].y;
        vertices[i].z = ai_mesh->mVertices[i].z;

        vertices[i].nx = ai_mesh->mNormals[i].x;
        vertices[i].ny = ai_mesh->mNormals[i].y;
        vertices[i].nz = ai_mesh->mNormals[i].z;

        mid_point.x += vertices[i].x;
        mid_point.y += vertices[i].y;
        mid_point.z += vertices[i].z;
    }

    mid_point.x /= vertices_num;
    mid_point.y /= vertices_num;
    mid_point.z /= vertices_num;

    for (uint32_t i = 0; i < vertices_num; i++) {
        vertices[i].x -= mid_point.x;
        vertices[i].y -= mid_point.y;
        vertices[i].z -= mid_point.z;
    }

    mesh->vb
        = create_vertex_buffer(vertex_layout, vertices_num, vertices.data());

    auto faces_num = ai_mesh->mNumFaces;

    std::vector<uint32_t> indices;
    indices.resize(faces_num * 3);

    for (uint32_t i = 0; i < faces_num; i++) {
        indices[i * 3 + 0] = ai_mesh->mFaces[i].mIndices[0];
        indices[i * 3 + 1] = ai_mesh->mFaces[i].mIndices[1];
        indices[i * 3 + 2] = ai_mesh->mFaces[i].mIndices[2];
    }

    mesh->ib = create_indice_buffer(faces_num * 3, indices.data());
}

} // namespace nx::gfx
