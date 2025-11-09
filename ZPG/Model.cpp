#include "Model.h"
#include "tiny_obj_loader.h"

Model::Model(const float* vertices, size_t vertexCount, int format)
    : count(static_cast<GLsizei>(vertexCount))
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    size_t strideFloats = 3; 
    if (format == 1) strideFloats = 6;      
    else if (format == 2) strideFloats = 8;  

    glBufferData(GL_ARRAY_BUFFER, vertexCount * strideFloats * sizeof(float), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, strideFloats * sizeof(float), (void*)0);

    if (format >= 1) {
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, strideFloats * sizeof(float), (void*)(3 * sizeof(float)));
    }

    if (format >= 2) {
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, strideFloats * sizeof(float), (void*)(6 * sizeof(float)));
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Model::Model(const char* name)
{
    std::string inputfile = std::string("../assets/") + name;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile.c_str(), "../assets/");
    if (!warn.empty()) std::cout << "Warn: " << warn << std::endl; 
    if (!err.empty()) std::cerr << "Err: " << err << std::endl;
    if (!ret) throw std::runtime_error("Failed to load OBJ file!");

    std::vector<float> vertices;

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            // Position
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 0]);
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 1]);
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 2]);

            // Normals (if exist)
            if (index.normal_index >= 0) {
                vertices.push_back(attrib.normals[3 * index.normal_index + 0]);
                vertices.push_back(attrib.normals[3 * index.normal_index + 1]);
                vertices.push_back(attrib.normals[3 * index.normal_index + 2]);
            }
            else {
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
            }

            //  UV coordinates (if exist)
            if (index.texcoord_index >= 0) {
                vertices.push_back(attrib.texcoords[2 * index.texcoord_index + 0]);
                vertices.push_back(attrib.texcoords[2 * index.texcoord_index + 1]);
            }
            else {
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
            }
        }
    }

    GLuint VBO = 0;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);


    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    count = (int)vertices.size() / 3;


    glGenVertexArrays(1, &VAO); 
    glBindVertexArray(VAO); 
    glEnableVertexAttribArray(0); 
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1); 
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2); 
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
}

Model::~Model() {
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

void Model::draw() const {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, count);
    glBindVertexArray(0);
}
