#include "ModuleGeometry.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "GameObject.h"
#include "C_Material.h"
#include "C_Transform.h"

#include "Glew/include/glew.h"



#include <vector>

ModuleGeometry::ModuleGeometry(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleGeometry::~ModuleGeometry()
{

}

bool ModuleGeometry::Init()
{
    bool ret = true;

    // Stream log messages to Debug window
    struct aiLogStream stream;
    stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
    aiAttachLogStream(&stream);

    return ret;
}

bool ModuleGeometry::Start()
{
    bool ret = true;

    return ret;
}

GameObject* ModuleGeometry::LoadFile(std::string file_path)
{
    const aiScene* scene = aiImportFile(file_path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
    if (scene != nullptr && scene->HasMeshes())
    {
        // Use scene->mNumMeshes to iterate on scene->mMeshes array

        GameObject* finalObj = ProcessNode(scene, scene->mRootNode, App->scene->rootGameObject, file_path);

        aiReleaseImport(scene);

        return finalObj;
    }
    else {
        LOG("Error loading scene %s", file_path);
    }
}

Mesh* ModuleGeometry::ImportMesh(aiMesh* aiMesh)
{
    Mesh* mesh = new Mesh();
    // copy vertices
    mesh->num_vertex = aiMesh->mNumVertices;
    mesh->vertex = new float[mesh->num_vertex * VERTEX_ARGUMENTS];
    //memcpy(mesh->vertices, scene->mMeshes[i]->mVertices, sizeof(float) * mesh->num_vertices * 3);

    for (int k = 0; k < mesh->num_vertex; k++) {

        mesh->vertex[k * VERTEX_ARGUMENTS] = aiMesh->mVertices[k].x;
        mesh->vertex[k * VERTEX_ARGUMENTS + 1] = aiMesh->mVertices[k].y;
        mesh->vertex[k * VERTEX_ARGUMENTS + 2] = aiMesh->mVertices[k].z;

        if (aiMesh->mTextureCoords[0] == nullptr) continue;                 
        mesh->vertex[k * VERTEX_ARGUMENTS + 3] = aiMesh->mTextureCoords[0][k].x;
        // -1 to invert uv's
        mesh->vertex[k * VERTEX_ARGUMENTS + 4] = 1 - aiMesh->mTextureCoords[0][k].y;

    }

    LOG("New mesh with %d vertices", mesh->num_vertex);

    // copy faces
    if (aiMesh->HasFaces())
    {
        mesh->num_index = aiMesh->mNumFaces * 3;
        mesh->index = new uint[mesh->num_index]; // assume each face is a triangle

        for (uint j = 0; j < aiMesh->mNumFaces; j++)
        {
            if (aiMesh->mFaces[j].mNumIndices != 3) {
                LOG("WARNING, geometry face with != 3 indices!");
            }
            else
            {
                memcpy(&mesh->index[j * 3], aiMesh->mFaces[j].mIndices, 3 * sizeof(uint));
            }

        }

        mesh->InitAABB();

        //meshes.push_back(mesh);
        BufferMesh(mesh);

        return mesh;
    }
    else {

        delete mesh;

        return nullptr;
    }
}

GameObject* ModuleGeometry::ProcessNode(const aiScene* scene, aiNode* node, GameObject* parent, std::string Path)
{

    if (node->mNumMeshes == 0 && node->mNumChildren == 0) return nullptr;

    GameObject* gObj = new GameObject(parent);

    gObj->name = node->mName.C_Str();

    aiVector3D scale, position, rotation;
    aiQuaternion QuatRotation;

    node->mTransformation.Decompose(scale, QuatRotation, position);
    rotation = QuatRotation.GetEuler();

    gObj->mTransform->getScale() = float3(scale.x, scale.y, scale.z);
    gObj->mTransform->mPosition = float3(position.x, position.y, position.z);
    /*gObj->mTransform->mRotation = float3(rotation.x, rotation.y, rotation.z);*/
    gObj->mTransform->calculateMatrix();

    if (node->mNumMeshes != 0) {

        C_Mesh* component = new C_Mesh(gObj, UUIDGenerator::Generate());


        std::string texture_path = "";


        for (int i = 0; i < node->mNumMeshes; i++)
        {
            Mesh* mesh = ImportMesh(scene->mMeshes[node->mMeshes[i]]);

            if (mesh == nullptr) {
                LOG("Error loading scene %s", Path);
                continue;
            }

            mesh->owner = gObj;
            component->meshes.push_back(mesh);

            if (texture_path == "") texture_path = ImportTexture(scene, node->mMeshes[i], Path);

        }

        gObj->mComponents.push_back(component);

        if (texture_path != "") {
            C_Material* componentT = new C_Material(gObj, UUIDGenerator::Generate());
            gObj->mComponents.push_back(componentT);
            componentT->SetTexture(texture_path.c_str());
        }
    }

    for (int i = 0; i < node->mNumChildren; i++) {
        ProcessNode(scene, node->mChildren[i], gObj, Path);
    }

    return gObj;
}

std::string ModuleGeometry::ImportTexture(const aiScene* scene, int index, std::string path)
{

    if (scene->HasMaterials())
    {
        aiMaterial* MaterialIndex = scene->mMaterials[scene->mMeshes[index]->mMaterialIndex];
        if (MaterialIndex->GetTextureCount(aiTextureType_DIFFUSE) > 0) {

            aiString TextPath;
            MaterialIndex->GetTexture(aiTextureType_DIFFUSE, 0, &TextPath);

            for (int i = 0; i < path.size(); i++)
            {
                if (path[i] == '\\')
                {
                    path[i] = '/';
                }
            }

            std::string NormTextPath = TextPath.C_Str();

            for (int i = 0; i < NormTextPath.size(); i++)
            {
                if (NormTextPath[i] == '\\')
                {
                    NormTextPath[i] = '/';
                }
            }

            std::string AssetsPath = path;
            uint AssetsPos = AssetsPath.find("Assets/");

            AssetsPath = AssetsPath.substr(AssetsPos, AssetsPath.find_last_of("/") - AssetsPos);
            AssetsPath = AssetsPath.substr(AssetsPos, AssetsPath.find_last_of("/") - AssetsPos);
            AssetsPath.append("/Textures/").append(TextPath.C_Str());

            return AssetsPath;
        }
    }

    return "";
}

void Mesh::RenderFaceNormals()
{
    for (size_t i = 0; i < num_index; i += 3)
    {
        size_t index1 = index[i];
        size_t index2 = index[i + 1];
        size_t index3 = index[i + 2];

        double halfX = (vertexFaceNormals[index1 * 3] + vertexFaceNormals[index2 * 3] + vertexFaceNormals[index3 * 3]) / 3.0;
        double halfY = (vertexFaceNormals[index1 * 3 + 1] + vertexFaceNormals[index2 * 3 + 1] + vertexFaceNormals[index3 * 3 + 1]) / 3.0;
        double halfZ = (vertexFaceNormals[index1 * 3 + 2] + vertexFaceNormals[index2 * 3 + 2] + vertexFaceNormals[index3 * 3 + 2]) / 3.0;

        double edge1x = vertexFaceNormals[index2 * 3] - vertexFaceNormals[index1 * 3];
        double edge1y = vertexFaceNormals[index2 * 3 + 1] - vertexFaceNormals[index1 * 3 + 1];
        double edge1z = vertexFaceNormals[index2 * 3 + 2] - vertexFaceNormals[index1 * 3 + 2];

        double edge2x = vertexFaceNormals[index3 * 3] - vertexFaceNormals[index1 * 3];
        double edge2y = vertexFaceNormals[index3 * 3 + 1] - vertexFaceNormals[index1 * 3 + 1];
        double edge2z = vertexFaceNormals[index3 * 3 + 2] - vertexFaceNormals[index1 * 3 + 2];

        double normalx = edge1y * edge2z - edge1z * edge2y;
        double normaly = edge1z * edge2x - edge1x * edge2z;
        double normalz = edge1x * edge2y - edge1y * edge2x;

        double length = sqrt(normalx * normalx + normaly * normaly + normalz * normalz);
        normalx /= length;
        normaly /= length;
        normalz /= length;

        double debugLineLength = 0.5;

        double xFinal = halfX + normalx * debugLineLength;
        double yFinal = halfY + normaly * debugLineLength;
        double zFinal = halfZ + normalz * debugLineLength;

        glLineWidth(0.5f);
        glBegin(GL_LINES);
        glVertex3d(halfX, halfY, halfZ);
        glVertex3d(xFinal, yFinal, zFinal);
        glEnd();
        // Reset to default
        glLineWidth(1.0f);
    }

}

void Mesh::InitAABB()
{
    std::vector<float3> correctVertex;
    for (size_t i = 0; i < num_vertex * VERTEX_ARGUMENTS; i += VERTEX_ARGUMENTS)
    {
        correctVertex.emplace_back(vertex[i], vertex[i + 1], vertex[i + 2]);
    }
    AABB_box.SetFrom(&correctVertex[0], correctVertex.size());
}

void Mesh::RenderAABB()
{
    float3 corners1[8];

    OBB_box.GetCornerPoints(corners1);

    App->renderer3D->DrawBox(corners1, float3(255, 0, 0));

    float3 corners2[8];
    Global_AABB_box.GetCornerPoints(corners2);

    App->renderer3D->DrawBox(corners2, float3(0, 0, 255));
}

void Mesh::Render()
{
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, id_vertex);

    glVertexPointer(3, GL_FLOAT, sizeof(float) * VERTEX_ARGUMENTS, NULL);
    glTexCoordPointer(2, GL_FLOAT, sizeof(float) * VERTEX_ARGUMENTS, (void*)(sizeof(float) * 3));

    glBindTexture(GL_TEXTURE_2D, id_texture);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
    
    glPushMatrix();

    if (owner != nullptr) {

        glMultMatrixf(owner->mTransform->getGlobalMatrix().ptr());
    }

    glDrawElements(GL_TRIANGLES, num_index, GL_UNSIGNED_INT, NULL);

    glPopMatrix();

    glDisableClientState(GL_VERTEX_ARRAY);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_TEXTURE_COORD_ARRAY);
}

void ModuleGeometry::BufferMesh(Mesh* mesh)
{

    glEnableClientState(GL_VERTEX_ARRAY);

    glGenBuffers(1, (GLuint*)&(mesh->id_vertex));
    glGenBuffers(1, (GLuint*)&(mesh->id_index));

    glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertex);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_vertex * VERTEX_ARGUMENTS, mesh->vertex, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_index);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->num_index, mesh->index, GL_STATIC_DRAW);
    glDisableClientState(GL_VERTEX_ARRAY);
    meshes.push_back(mesh);

    //glDisableClientState(GL_VERTEX_ARRAY);
}

void ModuleGeometry::RenderScene()
{
    meshesRendered = 0;
    //Render the scene
    for (int i = 0; i < meshes.size(); i++) {
        // Render if object inside frustrum
        if (App->renderer3D->GetMainCamera()->ObjectInsideFrustrum(meshes[i]))
        {
            // Reset to default
            glColor3f(1.0f, 1.0f, 1.0f);
            meshes[i]->Render();

            if (App->editor->isAABB)
            {
                meshes[i]->RenderAABB(); //AABBs

                //Raycast line
                App->renderer3D->DrawLine(App->renderer3D->ls.a, App->renderer3D->ls.b);

                //Frustum debug
                C_Camera* pilota = App->renderer3D->GetMainCamera();
                if (pilota != nullptr) {
                    float3 corners[8];
                    pilota->frustum.GetCornerPoints(corners);
                    App->renderer3D->DrawBox(corners, float3(1, .5, .9));
                }

            }

            meshesRendered++;

            glColor3f(1.0f, 0.0f, 0.0f);
            if (meshes[i]->owner->GetMeshComponent()->showNormals) {
                meshes[i]->RenderFaceNormals();
            }
        }

        
    }
}

//void ModuleGeometry::RenderGameScene()
//{
//    meshesRendered = 0;
//    for (int i = 0; i < meshes.size(); i++) {
//
//        if (App->renderer3D->GetMainCamera()->ObjectInsideFrustrum(meshes[i]))
//        {
//            glColor3f(1.0f, 1.0f, 1.0f);
//            meshes[i]->Render();
//            glColor3f(1.0f, 0.0f, 0.0f);
//            meshesRendered++;
//        }
//    }
//}


bool ModuleGeometry::CleanUp()
{

    aiDetachAllLogStreams();
    return true;
}

float3 Mesh::GetVectorFromIndex(float* startValue)
{
    float x = *startValue;
    ++startValue;
    float y = *startValue;
    ++startValue;
    float z = *startValue;

    return float3(x, y, z);
}

void Mesh::RenderMeshDebug(/*bool* vertexNormals,*/ bool* faceNormals)
{

    if (*faceNormals == true)
    {
        float normalLenght = 0.5f;
        //Face normals
        glColor3f(0, 1, 0);
        glBegin(GL_LINES);
        for (int i = 0; i < num_index; i += 3)
        {
            float3 A = GetVectorFromIndex(&vertex[index[i] * 3]);
            float3 B = GetVectorFromIndex(&vertex[index[i + 1] * 3]);
            float3 C = GetVectorFromIndex(&vertex[index[i + 2] * 3]);

            float3 middle((A.x + B.x + C.x) / 3.f, (A.y + B.y + C.y) / 3.f, (A.z + B.z + C.z) / 3.f);

            float3 crossVec = Cross((B - A), (C - A));
            float3 normalDirection = crossVec.Normalized();

            glVertex3f(middle.x, middle.y, middle.z);
            glVertex3f(middle.x + normalDirection.x * normalLenght, middle.y + normalDirection.y * normalLenght, middle.z + normalDirection.z * normalLenght);
        }
        glEnd();
        glPointSize(1.f);
        glColor3f(1, 1, 1);
    }
}

void ModuleGeometry::DestroyMesh(Mesh* mesh)
{
    for (size_t i = 0; i < meshes.size(); i++)
    {
        if (meshes[i] == mesh) {
            meshes.erase(meshes.begin() + i);
            delete mesh;
            mesh = nullptr;
            return;
        }
    }
}

