/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   opengl.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: lde-la-h <main@w2wizard.dev>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/12 10:35:14 by lde-la-h      #+#    #+#                 */
/*   Updated: 2022/08/15 09:49:13 by lde-la-h      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPENGL_HPP
# define OPENGL_HPP
# include <list>
# include <algorithm>
# include "api.hpp"
# define MLX_BATCH_SIZE 12000
namespace MLX::API {

/**
 * @brief API implementation of MLX42 in OpenGL.
 */
class OpenGL final : public MLX::GFXAPI
{
public: // Ctor ~ Dtor
    OpenGL();

public: // Initilization

    /** @inerhit */
	bool init(void) override;
	void loop(void) override;
	void shutdown(void) override;

public: // Window Functions

	mlx_t* newWindow(int32_t width, int32_t height, const std::string_view title, bool resize) override;

public: // Image Functions

	void putPixel(mlx_image_t* image, uint32_t x, uint32_t y, uint32_t color) override;
	mlx_image_t* newImage(uint32_t width, uint32_t height) override;
	int32_t imageToWindow(mlx_image_t* img, int32_t x, int32_t y) override;
	void deleteImage(mlx_image_t* image) override;
	bool resizeImage(mlx_image_t* img, uint32_t nwidth, uint32_t nheight) override;

private: // Misc Functions

	/**
	 * Compiles the given shader source code, of a given shader type.
	 * 
	 * @param[in] code The shader source code.
	 * @param[in] Type GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_GEOMETRY_SHADER, ...
	 * @return Non-zero on success, the shader object else 0.
	 */
	uint32_t compileShader(const std::string_view code, int32_t type);

    /**
     * Render all instances that exist in the render queue.
     */
    void renderImages(void);

    /**
     * Recalculate the view projection matrix, used by images for screen pos
     * Reference: https://bit.ly/3KuHOu1 (Matrix View Projection)
     * @param[in] mlx The MLX instance.
     * @param[in] width The window width.
     * @param[in] height The window height.
     */
    void updateMatrix(int32_t width, int32_t height);

    /**
     * @brief Flush the batched vertices.
     */
    void flushBatch(void);

    /**
     * @brief Binds the given image onto an available texture.
     * 
     * @param img The image to bind.
     * @return The texture index.
     */
    int8_t bindTexture(mlx_image_t* img);

public: // Types

	// OpenGL layout for a vertex in MLX.
	struct vertex
	{
		float	x;
		float	y;
		float	z;
		float	u;
		float	v;
		int8_t	tex;
	};

	// Draw call queue entry.
	struct drawQueue
	{
		mlx_image_t*	image;
		int32_t			instanceid;
	};

    // Image context.
    struct imageCTX
    {
        GLuint	texture;
        size_t	instancesCapacity;
    };

private: // Attributes

    // State
    bool sortQueue = false;

	// Shader
	GLuint vao; // Vertex array object.
	GLuint vbo; // Vertex buffer object.
	GLuint shaderProgram; // The shader program runing the vert and frag shader.
	
	// Rendering
	std::list<mlx_image_t> images; // Current existing images.
    std::vector<drawQueue> renderQueue; // Instances and their order of being rendered.

    // Window
    int32_t initialWidth;
	int32_t initialHeight;

	// Batching
	int32_t zdepth; // Current Z depth for instances.
	int32_t batchSize; // The batching size.
	int32_t boundTextures[16]; // Array of textures.
	vertex batchVertices[MLX_BATCH_SIZE]; // The array of batched vertices.
};

} // namespace MLX
#endif
