#include "ModuleGeometry.h"
#include "Application.h"



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

    file_path = "D:/warrior.FBX";

    return ret;
}

void ModuleGeometry::LoadFile()
{
    const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);

    // Si la escena té meshes
    if (scene != nullptr && scene->HasMeshes())
    {
        // Use scene->mNumMeshes to iterate on scene->mMeshes array
        aiReleaseImport(scene);
    }
    else
        LOG("Error loading scene % s", file_path);
}

Mesh* ModuleGeometry::ImportMesh(aiMesh* aiMesh)
{
    Mesh* ourMesh = new Mesh();
    // copy vertices
    ourMesh->num_vertex = aiMesh->mNumVertices;
    // *3 pk ara el vertex té només xyz. Pilota
    ourMesh->vertex = new float[ourMesh->num_vertex * VERTEX_ARGUMENTS];
    
    memcpy(ourMesh->vertex, aiMesh->mVertices, sizeof(float) * ourMesh->num_vertex * 3);
    LOG("New mesh with %d vertices", ourMesh->num_vertex);

    // Pilota
    //// Same for index
    //ourMesh->num_indices = aiMesh->mNumindices;
    //ourMesh->indices = new float[ourMesh->num_indices];
    //memcpy(ourMesh->indices, aiMesh->mIndices, sizeof(float) * ourMesh->num_indices);
    //LOG("New mesh with %d indices", m->num_indices);

    // Pilla les dades del aiMesh i les posa al ourMesh (les x, y i z)
    // Quan fem UV's, tambe caldra les x i y de les UV
    for (int v = 0; v < ourMesh->num_vertex; v++) {
        //vertices
        ourMesh->vertex[v * VERTEX_ARGUMENTS] = aiMesh->mVertices[v].x;
        ourMesh->vertex[v * VERTEX_ARGUMENTS + 1] = aiMesh->mVertices[v].y;
        ourMesh->vertex[v * VERTEX_ARGUMENTS + 2] = aiMesh->mVertices[v].z;

        ////uvs
        //if (aiMesh->mTextureCoords[0] == nullptr) continue;
        //ourMesh->vertex[v * VERTEX_ARGUMENTS + 3] = aiMesh->mTextureCoords[0][v].x;
        //ourMesh->vertex[v * VERTEX_ARGUMENTS + 4] = aiMesh->mTextureCoords[0][v].y;
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
            }
        }

        BufferMesh(ourMesh);

        return(ourMesh);
    }
    else
    {
        delete ourMesh;
        return nullptr;
    }
}

void Mesh::Render()
{
    glEnableClientState(GL_VERTEX_ARRAY);

    // Binding buffers
    glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);

    // Draw
    glDrawElements(GL_TRIANGLES, num_index, GL_UNSIGNED_INT, NULL);

    // Unbind buffers
    glDisableClientState(GL_VERTEX_ARRAY);
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

    //Add mesh to meshes vector
    meshes.push_back(mesh);
}

void ModuleGeometry::RenderScene()
{
    //Render the scene
    for (int i = 0; i < meshes.size(); i++) {
        meshes[i]->Render();
    }
}


bool ModuleGeometry::CleanUp()
{

    // detach log stream
    aiDetachAllLogStreams();
    return true;
}