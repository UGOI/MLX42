/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   MLX42_Int.hpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: lde-la-h <main@w2wizard.dev>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/12 10:29:49 by lde-la-h      #+#    #+#                 */
/*   Updated: 2022/08/12 15:34:37 by lde-la-h      ########   odam.nl         */
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
# include <string_view> /* std::string_view */
# include <cctype> /* isspace, isprint, ... */
# include <assert.h> /* assert, static_assert, ... */

////////////////////////////////////////////////////////////////////////////////
// Macros
////////////////////////////////////////////////////////////////////////////////

# define MLX_ASSERT(cond, msg) assert(cond && msg);
# define MLX_NONNULL(var) MLX_ASSERT(var, "Value can't be null");
# define BPP 4

// Internal namespace
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
 */

// TODO: Use lambda instead maybe.

template<typename F>
struct callback
{
	void*	param;
	F       func;
};

// Callback structure used to handle mouse scrolling.
typedef callback<mlx_scrollfunc>	mlx_scroll;
// Callback structure used to handle mouse actions.
typedef callback<mlx_mousefunc>		mlx_mouse;
// Callback structure used to handle raw mouse movement.
typedef callback<mlx_cursorfunc>	mlx_cursor;
// Callback structure used to handle window closing
typedef callback<mlx_closefunc>		mlx_close;
// Callback structure used to handle window resizing.
typedef callback<mlx_resizefunc>	mlx_resize;
// Callback structure used to handle key presses.
typedef callback<mlx_keyfunc>		mlx_key;
// Callback structure used to handle loop interops
typedef callback<void(*)(void*)>	mlx_hook;

////////////////////////////////////////////////////////////////////////////////
// Types
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

bool error(mlx_errno_t val);

}
#endif
