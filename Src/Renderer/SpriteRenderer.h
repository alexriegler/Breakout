#pragma once

#include "ShaderProgram.h"
#include "Texture2D.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ar
{
    /// <summary>
    /// This class hosts a shader object, a single vertex array object,
    /// and a render and initialization function. Its constructor takes a
    /// shader object that it uses for all future rendering.
    /// </summary>
    class SpriteRenderer
    {
    public:
        // Aliases
        using id_type = GLuint;

        // Constructor
        /// <summary>
        /// Inits shaders and shapes.
        /// </summary>
        /// <param name="shader_program">The shader program to use when rendering the sprite</param>
        SpriteRenderer(ShaderProgram&& shader_program);

        // Destructor
        ~SpriteRenderer();

        // Copy constructor & assignment operator
        SpriteRenderer(const SpriteRenderer& other) = delete;
        SpriteRenderer& operator=(const SpriteRenderer& other) = delete;

        // Move constructor & assignment operator
        SpriteRenderer(SpriteRenderer&& other) noexcept;
        SpriteRenderer& operator=(SpriteRenderer&& other) noexcept;

        // Functions
        /// <summary>
        /// Renders a defined quad textured with given sprite.
        /// </summary>
        /// <param name="texture">The texture to render</param>
        /// <param name="position">The position of the sprite</param>
        /// <param name="size">The size of the sprite</param>
        /// <param name="rotate">The rotation of the sprite, in degrees</param>
        /// <param name="color">The color for the sprite</param>
        void draw_sprite(Texture2D& texture,
                         glm::vec2 position,
                         glm::vec2 size = glm::vec2{ 10.0f, 10.0f },
                         float rotate = 0.0f,
                         glm::vec3 color = glm::vec3{ 1.0f });

    private:
        /// <summary>
        /// Initializes and configures the quad's buffer and vertex attributes.
        /// </summary>
        void init_render_data();

        // Data
        ShaderProgram m_shader_program;
        id_type m_quad_vao{};
    };
}
