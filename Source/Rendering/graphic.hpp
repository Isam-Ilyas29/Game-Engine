#pragma once

#include "Core/std_types.hpp"

#include <filesystem>


/*----------------------------------------------------------------------------------*/

namespace context {
	namespace graphics {
		bool initialiseGraphics();

	} // namespace graphics

} // namespace context

/*----------------------------------------------------------------------------------*/

// Error handling

#define GLAD_CHECK_ERROR(expression)			\
	do {										\
		expression;								\
		gladCheckError(__FILE__, __LINE__);		\
	} while (0);								


void gladCheckError(std::filesystem::path file, u32 line_number);

/*----------------------------------------------------------------------------------*/
