#include "ModuleGeometry.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "GameObject.h"
#include "C_Material.h"

#include "Glew/include/glew.h"

ModuleGeometry::ModuleGeometry(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleGeometry::~ModuleGeometry()
{

}

bool ModuleGeometry::Init()
{
    return true;
}

bool ModuleGeometry::Start()
{
    bool ret = true;

    // Stream log messages to Debug window
    struct aiLogStream stream;
    stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
    aiAttachLogStream(&stream);

    return ret;
}

GameObject* ModuleGeometry::LoadFile(const char* file_path)
{
    const aiScene* scene = aiImportFile(file_path, aiProcess_Triangulate | aiProcess_FlipUVs);

    // Si la escena té meshes
    if (scene != nullptr && scene->HasMeshes())
    {
        GameObject* parentGameObject = new GameObject(App->scene->rootGameObject);

        // Set the gameobject the name of the original file
        parentGameObject->name = std::string(file_path).substr(std::string(file_path).find_last_of(char(92)) + 1);
        parentGameObject->name = parentGameObject->name.substr(std::string(file_path).find_last_of("/") + 1);
        
        // ProcessNode here?
        for (int i = 0; i < scene->mNumMeshes; i++)
        {
            GameObject* childGameObject = new GameObject();
            parentGameObject->AddThisChild(childGameObject);
            childGameObject->name = "Mesh_" + std::to_string(i);

            ImportMesh(scene->mMeshes[i], parentGameObject, childGameObject, scene, i);
            childGameObject->GetMaterialComponent()->texture_path = file_path;

        }

        // Use scene->mNumMeshes to iterate on scene->mMeshes array
        aiReleaseImport(scene);

        return parentGameObject;
    }
    else
    {
        LOG("Error loading scene: %s", file_path);
    }
}

void ModuleGeometry::ImportMesh(aiMesh* aiMesh, GameObject* PgameObject, GameObject* CgameObject, const aiScene* scene, int index)
{

    Mesh* ourMesh = new Mesh();

    //TEST
    ourMesh->num_vertex = aiMesh->mNumVertices;
    ourMesh->vertex = new float[ourMesh->num_vertex * VERTEX_ARGUMENTS];
    ourMesh->vertexFaceNormals = new float[ourMesh->num_vertex * 3];
    memcpy(ourMesh->vertexFaceNormals, aiMesh->mVertices, sizeof(float) * ourMesh->num_vertex * 3);
    

    // Pilla les dades del aiMesh i les posa al ourMesh (les x, y i z)
    // Quan fem UV's, tambe caldra les x i y de les UV
    for (int v = 0; v < ourMesh->num_vertex; v++) {
        //vertices
        ourMesh->vertex[v * VERTEX_ARGUMENTS] = aiMesh->mVertices[v].x;
        ourMesh->vertex[v * VERTEX_ARGUMENTS + 1] = aiMesh->mVertices[v].y;
        ourMesh->vertex[v * VERTEX_ARGUMENTS + 2] = aiMesh->mVertices[v].z;

        //uvs
        if (aiMesh->mTextureCoords[0] == nullptr) continue;
        ourMesh->vertex[v * VERTEX_ARGUMENTS + 3] = aiMesh->mTextureCoords[0][v].x;
        ourMesh->vertex[v * VERTEX_ARGUMENTS + 4] = aiMesh->mTextureCoords[0][v].y;
    }

    if (aiMesh->HasFaces())
    {
        ourMesh->num_index = aiMesh->mNumFaces * 3;
        ourMesh->index = new uint[ourMesh->num_index]; // assume each face is a triangle

        for (uint i = 0; i < aiMesh->mNumFaces; ++i)
        {
            if (aiMesh->mFaces[i].mNumIndices != 3)
            {
                LOG("WARNING, geometry face with != 3 indices!");
            }
            else
            {
                memcpy(&ourMesh->index[i * 3], aiMesh->mFaces[i].mIndices, 3 * sizeof(uint));
            }
        }
        

        BufferMesh(ourMesh);

        C_Mesh* meshComp = new C_Mesh(CgameObject);
        ourMesh->owner = CgameObject;
        meshComp->mesh = ourMesh;
        CgameObject->AddComponent(meshComp);

        ourMesh->id_texture = App->texture2D->checkerID;



        if (scene->HasMaterials()) {
            if (scene->mMaterials[scene->mMeshes[index]->mMaterialIndex]->GetTextureCount(aiTextureType_DIFFUSE) > 0) {

                aiString texture_path;
                scene->mMaterials[scene->mMeshes[index]->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &texture_path);
                aiString new_path;
                new_path.Set("Assets/Textures/");
                new_path.Append(texture_path.C_Str());


                C_Material* matComp = new C_Material();
                matComp->mParent = CgameObject;
                matComp->SetTexture(new_path.C_Str());
                CgameObject->AddComponent(matComp);
            }
            else
            {
                // For primitives only (not empty primitive)
                C_Material* matComp = new C_Material();
                matComp->mParent = CgameObject;
                CgameObject->AddComponent(matComp);
            }
        }
        ourMesh->texture_height = App->texture2D->textureHeight;
        ourMesh->texture_width = App->texture2D->textureWidth;
        
    }
    else
    {
        delete ourMesh;
    }
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
    glDrawElements(GL_TRIANGLES, num_index, GL_UNSIGNED_INT, NULL);


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
    //Render the scene
    for (int i = 0; i < meshes.size(); i++) {
        // Reset to default
        glColor3f(1.0f, 1.0f, 1.0f);
        meshes[i]->Render();    
        glColor3f(1.0f, 0.0f, 0.0f);
        if (meshes[i]->owner->GetMeshComponent()->showNormals) {
            meshes[i]->RenderFaceNormals();
        }
    }
}


bool ModuleGeometry::CleanUp()
{

    aiDetachAllLogStreams();
    return true;
}

vec3 Mesh::GetVectorFromIndex(float* startValue)
{
    float x = *startValue;
    ++startValue;
    float y = *startValue;
    ++startValue;
    float z = *startValue;

    return vec3(x, y, z);
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
            vec3 A = GetVectorFromIndex(&vertex[index[i] * 3]);
            vec3 B = GetVectorFromIndex(&vertex[index[i + 1] * 3]);
            vec3 C = GetVectorFromIndex(&vertex[index[i + 2] * 3]);

            vec3 middle((A.x + B.x + C.x) / 3.f, (A.y + B.y + C.y) / 3.f, (A.z + B.z + C.z) / 3.f);

            vec3 crossVec = cross((B - A), (C - A));
            vec3 normalDirection = normalize(crossVec);

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
