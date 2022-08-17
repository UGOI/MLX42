/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   MLX42_Int.hpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: lde-la-h <main@w2wizard.dev>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/12 10:29:49 by lde-la-h      #+#    #+#                 */
/*   Updated: 2022/08/15 09:51:13 by lde-la-h      ########   odam.nl         */
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
# include <limits>
# include <memory>
# include <iostream>
# include <string> /* std::string */
# include <string_view> /* std::string_view */
# include <cctype> /* isspace, isprint, ... */
# include <assert.h> /* assert, static_assert, ... */
# include <exception>

////////////////////////////////////////////////////////////////////////////////
// Macros
////////////////////////////////////////////////////////////////////////////////

# define MLX_ASSERT(cond, msg) assert(cond && msg);
# define MLX_NONNULL(var) MLX_ASSERT(var, "Value can't be null");
# define BPP 4

// Internal namespace, only for use inside the library.
namespace MLX::Int {

////////////////////////////////////////////////////////////////////////////////
// Globals
////////////////////////////////////////////////////////////////////////////////

// Vertex shader code string literal.
extern const char* vertShader;
// Fragment shader code string literal.
extern const char* fragShader;
// Settings array, use the enum 'key' to get the value.
extern int32_t settings[MLX_SETTINGS_MAX];

////////////////////////////////////////////////////////////////////////////////
// Hooks
////////////////////////////////////////////////////////////////////////////////

/**
 * There are 2 types of hooks, special and generics.
 *
 * Specials: Are specific callback functions to a specific action
 * such as window resizing or key presses. These are attached to the
 * callbacks of glfw. In case MLX itself needs the callback we call
 * the specials in that callback since there can only ever be a single
 * callback.
 *
 * Generics: Generics are MLX42 specific hooks and can have multiple
 * hooks at the same time, these are executed every frame and can be
 * used as an alternative for key presses or animations for instance.
 *
 * NOTE: Hooks could be achieved with va_args to have any amount
 * of args sized functor but we can't/don't want to let the user
 * deal with va_args and having to look up what args are what, etc...
 *
 * We want to keep it straight forward with functors already describing
 * what params they have.
 * 
 * NOTE: Hooks are dealt with lambdas!
 */

////////////////////////////////////////////////////////////////////////////////
// Types
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

void setError(mlx_errno_t val);

}
#endif
