#include <assimp/Importer.hpp> // C++ importer interface
#include <assimp/scene.h> // Output data structure
#include <assimp/postprocess.h> // Post processing flags

#include "niu2x/assert.h"
#include "niu2x/utils.h"
#include "niu2x/gfx.h"

namespace nx::gfx {

void mesh_init_from_file(mesh_t* mesh, const char* file, int idx, int flags)
{
    Assimp::Importer importer;

    auto import_flags = aiProcessPreset_TargetRealtime_Quality;
    import_flags |= aiProcess_FlipUVs;

    const aiScene* scene = importer.ReadFile(file, import_flags);
    NX_ASSERT(nullptr != scene, importer.GetErrorString())

    NX_ASSERT((uint32_t)idx < scene->mNumMeshes, "");
    auto* ai_mesh = scene->mMeshes[idx];
    NX_ASSERT(ai_mesh->mPrimitiveTypes == aiPrimitiveType_TRIANGLE, "");

    auto vertices_num = ai_mesh->mNumVertices;
    auto vertex_layout = vertex_layout_build(vertex_attr_type::position,
        vertex_attr_type::normal, vertex_attr_type::uv);

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
    }

    if (flags & MESH_AUTO_CENTER) {
        vertex_t mid_point { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

        for (uint32_t i = 0; i < vertices_num; i++) {
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
    }

    mesh->vb
        = create_vertex_buffer(vertex_layout, vertices_num, vertices.data());
    auto faces_num = ai_mesh->mNumFaces;

    NX_LOG_D("mesh %s", file);
    NX_LOG_D("mesh vertices_num %d", vertices_num);
    NX_LOG_D("mesh faces_num %d", faces_num);

    std::vector<uint32_t> indices;
    indices.resize(faces_num * 3);

    uint32_t max_indice = 0, min_indice = 999999;

    for (uint32_t i = 0; i < faces_num; i++) {
        indices[i * 3 + 0] = ai_mesh->mFaces[i].mIndices[0];
        indices[i * 3 + 1] = ai_mesh->mFaces[i].mIndices[1];
        indices[i * 3 + 2] = ai_mesh->mFaces[i].mIndices[2];

        max_indice = std::max(max_indice, indices[i * 3 + 0]);
        max_indice = std::max(max_indice, indices[i * 3 + 1]);
        max_indice = std::max(max_indice, indices[i * 3 + 2]);

        min_indice = std::min(min_indice, indices[i * 3 + 0]);
        min_indice = std::min(min_indice, indices[i * 3 + 1]);
        min_indice = std::min(min_indice, indices[i * 3 + 2]);
    }

    mesh->ib = create_indice_buffer(faces_num * 3, indices.data());
}

} // namespace nx::gfx
