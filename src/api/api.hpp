/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   api.hpp                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: lde-la-h <main@w2wizard.dev>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/12 10:35:14 by lde-la-h      #+#    #+#                 */
/*   Updated: 2022/08/12 14:08:56 by lde-la-h      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef API_HPP
# define API_HPP
# include "MLX42_Int.hpp"
namespace MLX {

/**
 * @brief Base class for an API implementation.
 * 
 * Base class implementation for the common actions used
 * in mlx such as creating an image.
 */
class API
{
public: // Initilization

	/**
	 * @brief Initiliaze the API.
	 */
	virtual bool init(void) = 0;

	/**
	 * @brief Loop function of the API, this will execute
	 * in the main loop of the program and does everything
	 * the API needs to do during that time.
	 */
	virtual void loop(void) = 0;

	/**
	 * @brief De-initiliaze the API.
	 */
	virtual void shutdown(void) = 0;

public: // Window Functions

	/**
	 * @brief Creates a new window according to the API.
	 * @param[in] width The width of the window.
	 * @param[in] height The height of the window.
	 * @param[in] title The title of the window.
	 * @param[in] resize Enable window resizing.
	 * @return The MLX handle or nullptr on failure.
	 */
	virtual mlx_t* newWindow(int32_t width, int32_t height, const std::string_view title, bool resize) = 0;

public: // Image Functions

	/**
	 * Sets / puts a pixel onto an image.
	 * 
	 * NOTE: It is considered undefined behaviour when putting a pixel 
	 * beyond the bounds of an image.
	 * 
	 * @param[in] image The MLX instance handle.
	 * @param[in] x The X coordinate position.
	 * @param[in] y The Y coordinate position.
	 * @param[in] color The color value to put.
	 */
	virtual void putPixel(mlx_image_t* image, uint32_t x, uint32_t y, uint32_t color) = 0;

	/**
	 * Creates and allocates a new image buffer.
	 * 
	 * @param[in] width The desired width of the image.
	 * @param[in] height The desired height of the image.
	 * @return Pointer to the image buffer, if it failed to allocate then NULL.
	 */
	virtual mlx_image_t* newImage(uint32_t width, uint32_t height) = 0;

	/**
	 * Draws a new instance of an image, it will then share the same
	 * pixel buffer as the image.
	 * 
	 * NOTE: Keep in mind that the instance array gets reallocated, try to
	 * to store the return value to the instance! 
	 * NOT the pointer! It will become invalid!
	 * 
	 * WARNING: Try to display as few images onto the window as possible,
	 * drawing too many images will cause a loss in peformance!
	 * 
	 * @param[in] img The image to draw onto the screen.
	 * @param[in] x The X position.
	 * @param[in] y The Y position.
	 * @return Index to the instance, or -1 on failure.
	 */
	virtual int32_t imageToWindow(mlx_image_t* img, int32_t x, int32_t y) = 0;

	/**
	 * Deleting an image will remove it from the render queue as well as any and all
	 * instances it might have. Additionally, just as extra measures sets all the
	 * data to NULL.
	 * 
	 * If you simply wish to stop rendering an image without de-allocation
	 * set the 'enabled' boolean in the image struct.
	 * 
	 * @param[in] image The image to delete.
	 */
	virtual void deleteImage(mlx_image_t* image) = 0;

	/**
	 * Allows you to resize an image to a new size, the pixel buffer is re-allocated
	 * to fit & the previous data is copied over. New pixels are zeroed.
	 * 
	 * @param[in] img The image to resize.
	 * @param[in] nwidth The new width.
	 * @param[in] nheight The new height.
	 * @return True if image was resize or false on error.
	 */
	virtual bool resizeImage(mlx_image_t* img, uint32_t nwidth, uint32_t nheight) = 0;
};

} // namespace MLX
#endif
