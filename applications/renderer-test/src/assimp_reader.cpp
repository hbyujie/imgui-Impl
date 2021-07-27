#include "assimp_reader.h"

#include <iostream>

namespace Assimp
{

std::string GetFileName(const char *path)
{
    std::string str(path);
    char ch1 = (char)'//';
    size_t pos1 = str.find_last_of(ch1);

    char ch2 = (char)'.';
    size_t pos2 = str.find_last_of(ch2);

    return str.substr(pos1 + 1, pos2 - pos1 - 1);
}

std::string ProcessPath(aiString *aPath, const std::string directory)
{
    std::string path = std::string(aPath->C_Str());
    // parse path directly if path contains "/" indicating it is an absolute path;  otherwise
    // parse as relative.
    if (path.find(":/") == std::string::npos || path.find(":\\") == std::string::npos)
        path = directory + "/" + path;
    return path;
}

/*  Functions   */
// constructor, expects a path to a 3D model.
AssimpReader::AssimpReader(const std::string &filename, const std::string &dir)
{
    std::cout << "obj path: " << filename << std::endl;

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(filename, aiProcessPreset_TargetRealtime_Quality);

    if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "Assimp failed to load model at path: " << filename << std::endl;
        return;
    }

    m_model_name = GetFileName(filename.c_str());

    CruiseNode(scene->mRootNode, scene, dir);
}

/*  Private   */
void AssimpReader::CruiseNode(const aiNode *node, const aiScene *scene, const std::string directory)
{
    // process each mesh located at the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *assimpMesh = scene->mMeshes[node->mMeshes[i]];
        aiMaterial *assimpMat = scene->mMaterials[assimpMesh->mMaterialIndex];

        std::string model_name = assimpMesh->mName.C_Str();
        auto &model = m_models[model_name];

        ParseMesh(assimpMesh, &model);
        ParseMaterial(assimpMat, directory, &model.usemtl);
    }

    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        CruiseNode(node->mChildren[i], scene, directory);
    }
}

void AssimpReader::ParseMesh(const aiMesh *aMesh, Model *model)
{
    for (unsigned int i = 0; i < aMesh->mNumFaces; i++)
    {
        aiFace face = aMesh->mFaces[i];

        // triangle mesh
        assert(face.mNumIndices == 3);
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            model->usemtl.indices.push_back(face.mIndices[j]);
        }
    }

    for (unsigned int i = 0; i < aMesh->mNumVertices; i++)
    {
        model->positions.push_back(glm::vec3(aMesh->mVertices[i].x, aMesh->mVertices[i].y, aMesh->mVertices[i].z));

        if (aMesh->HasNormals())
        {
            model->normals.push_back(glm::vec3(aMesh->mNormals[i].x, aMesh->mNormals[i].y, aMesh->mNormals[i].z));
        }

        if (aMesh->HasTextureCoords(0))
        {
            model->texcoords.push_back(glm::vec2(aMesh->mTextureCoords[0][i].x, aMesh->mTextureCoords[0][i].y));
        }

        if (aMesh->HasVertexColors(0))
        {
            model->colors.push_back(glm::vec4(aMesh->mColors[0][i].r, aMesh->mColors[0][i].g, aMesh->mColors[0][i].b,
                                              aMesh->mColors[0][i].a));
        }

        if (aMesh->HasTangentsAndBitangents())
        {
            model->tangents.push_back(glm::vec3(aMesh->mTangents[i].x, aMesh->mTangents[i].y, aMesh->mTangents[i].z));
            model->bitangents.push_back(
                glm::vec3(aMesh->mBitangents[i].x, aMesh->mBitangents[i].y, aMesh->mBitangents[i].z));
        }
    }
}

void AssimpReader::ParseMaterial(const aiMaterial *aMaterial, const std::string directory, Usemtl *usemtle)
{
    /* NOTE:

    About texture types:

    We use a PBR metallic/roughness workflow so the loaded models are expected to have
    textures conform the workflow: albedo, (normal), metallic, roughness, (ao). Since Assimp
    made certain assumptions regarding possible types of loaded textures it doesn't directly
    translate to our model thus we make some assumptions as well which the 3D author has to
    comply with if he wants the mesh(es) to directly render with its specified textures:

    - aiTextureType_DIFFUSE:   Albedo
    - aiTextureType_NORMALS:   Normal		// aiTextureType_HEIGHT replace aiTextureType_NORMALS
    - aiTextureType_SPECULAR:  metallic
    - aiTextureType_SHININESS: roughness
    - aiTextureType_AMBIENT:   AO (ambient occlusion)
    - aiTextureType_EMISSIVE:  Emissive

    */

    if (aMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
    {
        // we only load the first of the list of diffuse textures, we don't really care about
        // meshes with multiple diffuse layers; same holds for other texture types.
        aiString file;
        aMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file);
        std::string fileName = ProcessPath(&file, directory);
        usemtle->map_kd = fileName;
    }
    if (aMaterial->GetTextureCount(aiTextureType_HEIGHT) > 0)
    {
        aiString file;
        aMaterial->GetTexture(aiTextureType_HEIGHT, 0, &file);
        std::string fileName = ProcessPath(&file, directory);
        usemtle->map_bump = fileName;
    }
    if (aMaterial->GetTextureCount(aiTextureType_SPECULAR) > 0)
    {
        aiString file;
        aMaterial->GetTexture(aiTextureType_SPECULAR, 0, &file);
        std::string fileName = ProcessPath(&file, directory);
        usemtle->map_ks = fileName;
    }
    // if (aMaterial->GetTextureCount(aiTextureType_SHININESS) > 0)
    //{
    //    aiString file;
    //    aMaterial->GetTexture(aiTextureType_SHININESS, 0, &file);
    //    std::string fileName = ProcessPath(&file, directory);
    //    material->TexRoughness.fileName = fileName;
    //}
    // if (aMaterial->GetTextureCount(aiTextureType_AMBIENT) > 0)
    //{
    //    aiString file;
    //    aMaterial->GetTexture(aiTextureType_AMBIENT, 0, &file);
    //    std::string fileName = ProcessPath(&file, directory);
    //    material->TexAO.fileName = fileName;
    //}
}
} // namespace Assimp