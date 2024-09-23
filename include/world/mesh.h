#pragma once

#include <vector>
#include <rendering.h>
#include <glad/glad.h>

#include "glError.h"

class Mesh
{

public:
    Mesh()
    {
    }

    void init()
    {
        GLCall(glGenVertexArrays(1, &VAO));
        GLCall(glGenBuffers(1, &VBO));
        GLCall(glGenBuffers(1, &EBO));

        bind();

        GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0));
        GLCall(glEnableVertexAttribArray(0));

        GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)sizeof(glm::vec3)));
        GLCall(glEnableVertexAttribArray(1));

        unbind();
    }

    void draw()
    {
        if (indices.size() <= 0)
            return;
        bind();
        if (!depth_test)
        {
            glDisable(GL_DEPTH_TEST);
        }

        GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices.front(), GL_STATIC_DRAW));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices.front(), GL_STATIC_DRAW));
        GLCall(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void *)0));
        if (!depth_test)
        {
            glEnable(GL_DEPTH_TEST);
        }
        unbind();
    }

    void reset()
    {
        vertices = {};
        indices = {};
    }

    void setDepthTest(bool setter)
    {
        depth_test = setter;
    }

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

private:
    unsigned int VBO, EBO, VAO;
    bool depth_test = true;

    void bind()
    {
        GLCall(glBindVertexArray(VAO));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
    }

    void unbind()
    {
        GLCall(glBindVertexArray(0));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }
};