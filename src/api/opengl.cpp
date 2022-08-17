/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   opengl.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: lde-la-h <main@w2wizard.dev>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/12 13:04:19 by lde-la-h      #+#    #+#                 */
/*   Updated: 2022/08/15 09:50:41 by lde-la-h      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "opengl.hpp"

MLX::API::OpenGL::OpenGL() : vao(0), vbo(0), shaderProgram(0), zdepth(0)
{
    memset(this->boundTextures, 0, sizeof(this->boundTextures));
}

////////////////////////////////////////////////////////////////////////////////

bool MLX::API::OpenGL::init(void)
{
    int32_t success;
	uint32_t vshader = 0;
	uint32_t fshader = 0;
	char infolog[512] = {0};

	// Load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		return (MLX::Int::setError(MLX_GLADFAIL), false);

    // Compile shaders
	if (!(vshader = this->compileShader(MLX::Int::vertShader, GL_VERTEX_SHADER)))
		return (MLX::Int::setError(MLX_VERTFAIL), false);
	if (!(fshader = this->compileShader(MLX::Int::fragShader, GL_FRAGMENT_SHADER)))
		return (MLX::Int::setError(MLX_FRAGFAIL), false);
	if (!(this->shaderProgram = glCreateProgram()))
		return (MLX::Int::setError(MLX_SHDRFAIL), false);

    // Attach, link and check the status.
	glAttachShader(this->shaderProgram, vshader);
	glAttachShader(this->shaderProgram, fshader);
	glLinkProgram(this->shaderProgram);
	glGetProgramiv(this->shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->shaderProgram, sizeof(infolog), NULL, infolog);
        std::cerr << infolog << std::endl;
		return (MLX::Int::setError(MLX_SHDRFAIL), false);
	}
	glDeleteShader(vshader);
	glDeleteShader(fshader);

	glUseProgram(this->shaderProgram);

    // Generate the buffers //
    glActiveTexture(GL_TEXTURE0);
	glGenVertexArrays(1, &this->vao);
	glGenBuffers(1, &this->vbo);
	glBindVertexArray(this->vao);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

	// Vertex XYZ coordinates
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), NULL);
	glEnableVertexAttribArray(0);

	// UV Coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	// Texture index
	glVertexAttribIPointer(2, 1, GL_BYTE, sizeof(vertex), (void *)(sizeof(float) * 5));
	glEnableVertexAttribArray(2);

    glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (uint32_t i = 0; i < 16; i++)
    {
        std::string texture("Texture");
        texture += std::to_string(i);

        glUniform1i(glGetUniformLocation(this->shaderProgram, texture.data()), i);
    }
    return (true);
}

void MLX::API::OpenGL::loop(void)
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MLX::API::OpenGL::shutdown(void)
{

}

////////////////////////////////////////////////////////////////////////////////

mlx_t* MLX::API::OpenGL::newWindow(int32_t width, int32_t height, const std::string_view title, bool resize)
{
    return (nullptr);
}

////////////////////////////////////////////////////////////////////////////////

void MLX::API::OpenGL::putPixel(mlx_image_t* image, uint32_t x, uint32_t y, uint32_t color)
{
	MLX_NONNULL(image);
	MLX_ASSERT(x < image->width, "Pixel is out of bounds");
	MLX_ASSERT(y < image->height, "Pixel is out of bounds");

	uint8_t* pixel = &image->pixels[(y * image->width + x) * BPP];

	*(pixel++) = (uint8_t)(color >> 24);
	*(pixel++) = (uint8_t)(color >> 16);
	*(pixel++) = (uint8_t)(color >> 8);
	*(pixel++) = (uint8_t)(color & 0xFF);
}

mlx_image_t* MLX::API::OpenGL::newImage(uint32_t width, uint32_t height)
{
    return (nullptr);
}

int32_t MLX::API::OpenGL::imageToWindow(mlx_image_t* img, int32_t x, int32_t y)
{
    return (0);
}

void MLX::API::OpenGL::deleteImage(mlx_image_t* image)
{

}

bool MLX::API::OpenGL::resizeImage(mlx_image_t* img, uint32_t nwidth, uint32_t nheight)
{
    return (false);
}

////////////////////////////////////////////////////////////////////////////////

uint32_t MLX::API::OpenGL::compileShader(const std::string_view code, int32_t type)
{
	GLuint shader;
	int32_t success;
	char infolog[512] = {0};

	if (code.empty() || (shader = glCreateShader(type)) == 0) 
		return (GL_NONE);

	GLint len = code.length();
    const char* data = code.data();

	glShaderSource(shader, 1, &data, &len);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, sizeof(infolog), NULL, infolog);
        std::cerr << infolog << std::endl;
		return (GL_NONE);
	}
	return (shader);
}

void MLX::API::OpenGL::renderImages(void)
{
    // Sort to fix transparency
    if (this->sortQueue)
    {
        this->sortQueue = false;
        std::sort(this->renderQueue.begin(), this->renderQueue.end(), [&](drawQueue& A, drawQueue& B) {
            return (A.image->instances[A.instanceid].z < B.image->instances[B.instanceid].z);
        });
    }

    // Upload textures
    for (auto& image : this->images)
    {
        const GLint handle = static_cast<GLint>(static_cast<imageCTX*>(image.context)->texture);

        glBindTexture(GL_TEXTURE_2D, handle);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.pixels);
    }

    // Draw instance
    for (auto& entry : this->renderQueue)
    {
        mlx_instance_t& instance = entry.image->instances[entry.instanceid];
        
        if (!entry.image->enabled || !instance.enabled)
            continue;
        
        float w = static_cast<float>(entry.image->width);
        float h = static_cast<float>(entry.image->height);
        float x = static_cast<float>(instance.x);
        float y = static_cast<float>(instance.y);
        float z = static_cast<float>(instance.z);
        int8_t tex = bindTexture(entry.image);

        vertex vertices[6] = {
            (vertex){x, y, z, 0.f, 0.f, tex},
            (vertex){x + w, y + h, z, 1.f, 1.f, tex},
            (vertex){x + w, y, z, 1.f, 0.f, tex},
            (vertex){x, y, z, 0.f, 0.f, tex},
            (vertex){x, y + h, z, 0.f, 1.f, tex},
            (vertex){x + w, y + h, z, 1.f, 1.f, tex},
        };
        memmove(this->batchVertices + this->batchSize, vertices, sizeof(vertices));
        this->batchSize += 6; // 2 * 3 Tris

        if (this->batchSize >= MLX_BATCH_SIZE)
            flushBatch();
    }
}

void MLX::API::OpenGL::updateMatrix(int32_t width, int32_t height)
{
	/**
	 * Incase the setting to stretch the image is set, we maintain the width and height but not
	 * the depth.
	 */
	const float fwidth = MLX::Int::settings[MLX_STRETCH_IMAGE] ? this->initialWidth : width;
	const float fheight = MLX::Int::settings[MLX_STRETCH_IMAGE] ? this->initialHeight : height;
	const float depth = this->zdepth;

	const float matrix[16] = {
		2.f / fwidth, 0, 0, 0,
		0, 2.f / -(fheight), 0, 0,
		0, 0, -2.f / (depth - -depth), 0,
		-1, -(fheight / -fheight),
		-((depth + -depth) / (depth - -depth)), 1
	};

	glUniformMatrix4fv(glGetUniformLocation(this->shaderProgram, "ProjMatrix"), 1, GL_FALSE, matrix);
}

void MLX::API::OpenGL::flushBatch(void)
{
	if (this->batchSize <= 0)
		return;

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, this->batchSize * sizeof(vertex), this->batchVertices, GL_STATIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, this->batchSize);

	this->batchSize = 0;
	memset(this->boundTextures, 0, sizeof(this->boundTextures));
}

int8_t MLX::API::OpenGL::bindTexture(mlx_image_t* img)
{
    const GLint handle = static_cast<GLint>(static_cast<imageCTX*>(img->context)->texture);

	// Attempt to bind the texture, or obtain the index if it is already bound.
	for (int8_t i = 0; i < 16; i++)
	{
		if (this->boundTextures[i] == handle)
			return (i);

		if (this->boundTextures[i] == GL_NONE)
		{
			this->boundTextures[i] = handle;

			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, handle);
			return (i);
		}
	}

	// If no free slot was found, flush the batch and assign the texture to the first available slot
    this->flushBatch();

	this->boundTextures[0] = handle;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, handle);
	return (0);
}