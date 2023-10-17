#include "ModuleGeometry.h"
#include "Application.h"
#include "ModuleTextures.h"

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

void ModuleGeometry::LoadFile(const char* file_path)
{
    const aiScene* scene = aiImportFile(file_path, aiProcess_Triangulate | aiProcess_FlipUVs);

    // Si la escena té meshes
    if (scene != nullptr && scene->HasMeshes())
    {

        // ProcessNode here?
        for (int i = 0; i < scene->mNumMeshes; i++)
        {
            ImportMesh(scene->mMeshes[i]);
        }

        // Use scene->mNumMeshes to iterate on scene->mMeshes array
        aiReleaseImport(scene);
    }
    else
    {
        LOG("Error loading scene: %s", file_path);
    }
}

void ModuleGeometry::ImportMesh(aiMesh* aiMesh)
{
    Mesh* ourMesh = new Mesh();
    // copy vertices
    //ourMesh->num_vertex = aiMesh->mNumVertices;
    // *3 pk ara el vertex té només xyz. Pilota
    //ourMesh->vertex = new float[ourMesh->num_vertex * VERTEX_ARGUMENTS];
    
    /*memcpy(ourMesh->vertex, aiMesh->mVertices, sizeof(float) * ourMesh->num_vertex * 3);
    LOG("New mesh with %d vertices", ourMesh->num_vertex);*/

    //TEST
    ourMesh->num_vertex = aiMesh->mNumVertices;
    ourMesh->vertex = new float[ourMesh->num_vertex * VERTEX_ARGUMENTS];
    //memcpy(ourMesh->vertex, aiMesh->mVertices, sizeof(float) * ourMesh->num_vertex * VERTEX_ARGUMENTS);

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

    // Load faces
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
                //memcpy(ourMesh->vertex, aiMesh->mVertices, sizeof(float) * ourMesh->num_vertex * VERTEX_ARGUMENTS);
            }
        }
        ourMesh->id_texture = App->texture2D->textureID;
        ourMesh->texture_height = App->texture2D->textureHeight;
        ourMesh->texture_width = App->texture2D->textureWidth;

        BufferMesh(ourMesh);

        //ourMesh->VBO = 0;
        //ourMesh->EBO = 0;

        //// Dos buffers, vertex i index
        //glGenBuffers(1, (GLuint*)&(ourMesh->VBO));
        //glGenBuffers(1, (GLuint*)&(ourMesh->EBO));

        //// Bind and fill buffers
        //glBindBuffer(GL_ARRAY_BUFFER, ourMesh->VBO);
        //glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ourMesh->num_vertex * VERTEX_ARGUMENTS, ourMesh->vertex, GL_STATIC_DRAW);
        //glBindBuffer(GL_ARRAY_BUFFER, 0);

        ////Fill buffers with indices
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ourMesh->EBO);
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * ourMesh->num_index, ourMesh->index, GL_STATIC_DRAW);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        ////BufferMesh(ourMesh);

        ////Add mesh to meshes vector
        //meshes.push_back(ourMesh);

        //return(ourMesh);
    }
    else
    {
        delete ourMesh;
        //return nullptr;
    }
}

void Mesh::Render()
{
    //glEnable(GL_TEXTURE_2D);
    //glEnableClientState(GL_VERTEX_ARRAY);

    //// Binding buffers
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    //// Draw
    //glVertexPointer(3, GL_FLOAT, 0, NULL);
    //glDrawElements(GL_TRIANGLES, num_index, GL_UNSIGNED_INT, NULL);

    //// Unbind buffers
    //glDisableClientState(GL_VERTEX_ARRAY);




    glEnable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, id_vertex);

    glVertexPointer(3, GL_FLOAT, sizeof(float) * VERTEX_ARGUMENTS, NULL);
    glTexCoordPointer(2, GL_FLOAT, sizeof(float) * VERTEX_ARGUMENTS, (void*)(sizeof(float) * 3));
    // … bind and use other buffers

    glBindTexture(GL_TEXTURE_2D, id_texture);

    //Indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
    glDrawElements(GL_TRIANGLES, num_index, GL_UNSIGNED_INT, NULL);


    glDisableClientState(GL_VERTEX_ARRAY);
    //cleaning texture
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_TEXTURE_COORD_ARRAY);
}

void ModuleGeometry::BufferMesh(Mesh* mesh)
{
    //Fill buffers with vertex
    glEnableClientState(GL_VERTEX_ARRAY);

    // Dos buffers, vertex i index
    glGenBuffers(1, (GLuint*)&(mesh->id_vertex));
    glGenBuffers(1, (GLuint*)&(mesh->id_index));

    // Bind and fill buffers
    glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertex);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_vertex * VERTEX_ARGUMENTS, mesh->vertex, GL_STATIC_DRAW);

    //Fill buffers with indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_index);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->num_index, mesh->index, GL_STATIC_DRAW);
    glDisableClientState(GL_VERTEX_ARRAY);
    meshes.push_back(mesh);

    //glDisableClientState(GL_VERTEX_ARRAY);
}

void ModuleGeometry::RenderScene()
{
    bool trueE = true;
    //Render the scene
    for (int i = 0; i < meshes.size(); i++) {
        meshes[i]->Render();
        // Print normals (not working)
        //meshes[i]->RenderMeshDebug(&trueE);
    }
}


bool ModuleGeometry::CleanUp()
{

    // detach log stream
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
    //if (*vertexNormals == true)
    //{
    //    float normalLenght = 0.05f;
    //    glPointSize(3.0f);
    //    glColor3f(1, 0, 0);
    //    glBegin(GL_POINTS);
    //    for (unsigned int i = 0; i < vertices_count * 3; i += 3)
    //    {
    //        glVertex3f(vertices[i], vertices[i + 1], vertices[i + 2]);
    //    }
    //    glEnd();
    //    glColor3f(0, 1, 0);
    //    glPointSize(1.0f);

    //    //Vertex normals
    //    glColor3f(0, 1, 0);
    //    glBegin(GL_LINES);
    //    for (unsigned int i = 0; i < normals_count * 3; i += 3)
    //    {
    //        glVertex3f(vertices[i], vertices[i + 1], vertices[i + 2]);
    //        glVertex3f(vertices[i] + normals[i] * normalLenght, vertices[i + 1] + normals[i + 1] * normalLenght, vertices[i + 2] + normals[i + 2] * normalLenght);
    //    }
    //    glEnd();
    //    glColor3f(1, 1, 1);
    //}

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

