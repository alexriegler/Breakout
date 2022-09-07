#include "SpriteRenderer.h"

namespace ar
{
    SpriteRenderer::SpriteRenderer(ShaderProgram&& shader_program)
        : m_shader_program{ std::move(shader_program) }
    {
        init_render_data();
    }

    SpriteRenderer::~SpriteRenderer()
    {
        glDeleteVertexArrays(1, &m_quad_vao);
    }

    SpriteRenderer::SpriteRenderer(SpriteRenderer&& other) noexcept
    {
        *this = std::move(other);
    }

    SpriteRenderer& SpriteRenderer::operator=(SpriteRenderer&& other) noexcept
    {
        if (this != &other)
        {
            m_shader_program = std::move(other.m_shader_program);
            m_quad_vao = std::exchange(other.m_quad_vao, 0);
        }
        return *this;
    }

    void SpriteRenderer::draw_sprite(Texture2D& texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
    {
        // Prepare transformations
        m_shader_program.use();
        glm::mat4 model = glm::mat4{ 1.0f };
        model = glm::translate(model, glm::vec3{ position, 0.0f });

        // We know from the transformations chapter that rotations always revolve around
        // the origin (0,0). Because we specified the quad's vertices with (0,0) as the
        // top-left coordinate, all rotations will rotate around this point of (0,0). The
        // origin of rotation is at the top-left of the quad, which produces undesirable
        // results. What we want to do is move the origin of rotation to the center of
        // the quad so the quad neatly rotates around this origin, instead of rotating
        // around the top-left of the quad. We solve this by translating the quad by half
        // its size first, so its center is at coordinate (0,0) before rotating.
        model = glm::translate(model, glm::vec3{ 0.5f * size.x, 0.5f * size.y, 0.0f });
        model = glm::rotate(model, glm::radians(rotate), glm::vec3{ 0.0f, 0.0f, 1.0f });
        model = glm::translate(model, glm::vec3{ -0.5f * size.x, -0.5f * size.y, 0.0f });

        model = glm::scale(model, glm::vec3{ size, 1.0f });

        // Transformation happen in right to left order (scale -> translate -> rotate -> translate)
        m_shader_program.set_mat4("model", model);
        m_shader_program.set_vec3("spriteColor", color);

        glActiveTexture(GL_TEXTURE0);
        texture.bind();

        glBindVertexArray(m_quad_vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

    void SpriteRenderer::init_render_data()
    {
        id_type vbo;
        const float vertices[] = {
            // pos      // tex
            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,

            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f
        };

        glGenVertexArrays(1, &m_quad_vao);
        glGenBuffers(1, &vbo);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindVertexArray(m_quad_vao);
        // "0" corresponds with "location = 0" in sprite.vert.
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid*)0);

        // Unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}
