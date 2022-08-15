#pragma once
#include <glad/glad.h>
#include<glm/glm.hpp>
#include"shader.h"
class light2d
{
public:
    light2d(Shader shader, glm::vec2 size, glm::vec2 position, glm::vec3 color = glm::vec3(255.0f, 255.0f, 255.0f), float rotate = 0.0f)
        : shader(shader), size(size), position(position), color(color), rotate(rotate)
    {

        initRenderData();
    }
	~light2d();
    unsigned int VAO;
    float rotate;
    Shader shader;

    glm::vec2 size;
	glm::vec2 position;
    glm::vec3 color;
    // render all particles
    void Draw()
    {
        // use additive blending to give it a 'glow' effect
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        // prepare transformations
        this->shader.Use();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(position, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)
        model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to center of quad
        model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
        model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back

        model = glm::scale(model, glm::vec3(size, 1.0f)); // last scale

        this->shader.SetMatrix4("model", model);

        // render textured quad
        this->shader.SetVector3f("spriteColor", color);

        glBindVertexArray(this->VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        // don't forget to reset to default blending mode
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

private:
    void initRenderData()
    {
        // configure VAO/VBO
        unsigned int VBO;
        float vertices[] = {
            // pos      // tex
            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,

            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f
        };
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindVertexArray(this->VAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
};

