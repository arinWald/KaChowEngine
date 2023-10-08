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

void ModuleGeometry::ImportMesh(aiMesh* aiMesh)
{
    ourMesh = new Mesh();
    // copy vertices
    ourMesh->num_vertex = aiMesh->mNumVertices;
    ourMesh->vertex = new float[ourMesh->num_vertex * 3];
    memcpy(ourMesh->vertex, aiMesh->mVertices, sizeof(float) * ourMesh->num_vertex * 3);
    LOG("New mesh with %d vertices", ourMesh->num_vertex);

    // Pilota
    //// Same for index
    //ourMesh->num_indices = aiMesh->mNumindices;
    //ourMesh->indices = new float[ourMesh->num_indices];
    //memcpy(ourMesh->indices, aiMesh->mIndices, sizeof(float) * ourMesh->num_indices);
    //LOG("New mesh with %d indices", m->num_indices);

    // copy faces
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
    }
    else
    {
        delete ourMesh;
        /*return nullptr;*/
    }
}

void ModuleGeometry::BufferMesh(Mesh* mesh)
{
    //Fill buffers with vertex
    glEnableClientState(GL_VERTEX_ARRAY);
    glGenBuffers(1, (GLuint*)&(mesh->id_vertex));
    glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertex);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_vertex * VERTICES, mesh->vertex, GL_STATIC_DRAW);

    //Fill buffers with indices
    glGenBuffers(1, (GLuint*)&(mesh->id_index));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_index);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->num_index, mesh->index, GL_STATIC_DRAW);

    glDisableClientState(GL_VERTEX_ARRAY);

    //Add mesh to meshes vector
    meshes.push_back(mesh);
}


bool ModuleGeometry::CleanUp()
{


    // detach log stream
    aiDetachAllLogStreams();
    return true;
}