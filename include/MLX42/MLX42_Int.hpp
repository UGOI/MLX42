/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   MLX42_Int.hpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: W2Wizard <w2.wizzard@gmail.com>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/27 23:55:34 by W2Wizard      #+#    #+#                 */
/*   Updated: 2022/08/12 10:21:45 by lde-la-h      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MLX42_INT_H
# define MLX42_INT_H
# define LODEPNG_NO_COMPILE_ENCODER
# define LODEPNG_NO_COMPILE_ALLOCATORS
# define LODEPNG_NO_COMPILE_ANCILLARY_CHUNKS
# if defined(__APPLE__)
#  define GL_SILENCE_DEPRECATION
# endif
# include "MLX42/MLX42.h"
# include "lodepng/lodepng.h"
# include "glad/glad.h"
# include "KHR/khrplatform.h"
# include <GLFW/glfw3.h>
# if defined(_WIN32)
#  include <windows.h>
# else
#  include <err.h>
# endif
# include <limits>
# include <memory>
# include <iostream>
# include <string> /* std::string */
# include <cctype> /* isspace, isprint, ... */
# include <assert.h> /* assert, static_assert, ... */

// Macros
////////////////////////////////////////////////////////////////////////////////

# define MLX_ASSERT(cond, msg) assert(cond && msg);
# define MLX_NONNULL(var) MLX_ASSERT(var, "Value can't be null");
# define BPP 4

// Globals
////////////////////////////////////////////////////////////////////////////////

// Settings array, use the enum 'key' to get the value.
extern int32_t mlx_settings[MLX_SETTINGS_MAX];
extern const char* vert_shader;
extern const char* frag_shader;
#endif
