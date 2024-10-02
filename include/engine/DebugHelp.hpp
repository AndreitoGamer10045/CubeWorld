#pragma once

#include <iostream>
#include <cstdlib>

#include "glad/glad.h"

// CRITICAL_ASSERT: If it fails, it shows an error and closes the program
#define CRITICAL_ASSERT(condition, message)                              \
    do {                                                                 \
        if (!(condition)) {                                              \
            std::cerr << "CRITICAL ASSERT failed: (" #condition ") in "   \
                      << __FILE__ << " line " << __LINE__ << ": "        \
                      << message << std::endl;                           \
            std::abort();                                                \
        }                                                                \
    } while (false)

// NON_CRITICAL_ASSERT: If it fails, it only shows an error
#define NON_CRITICAL_ASSERT(condition, message)                          \
    do {                                                                 \
        if (!(condition)) {                                              \
            std::cerr << "NON-CRITICAL ASSERT failed: (" #condition ") in "\
                      << __FILE__ << " line " << __LINE__ << ": "        \
                      << message << std::endl;                           \
        }                                                                \
    } while (false)


//https://learnopengl.com/In-Practice/Debugging
void GLAPIENTRY glDebugOutput(GLenum source,
	GLenum type,
	unsigned int id,
	GLenum severity,
	GLsizei length,
	const char* message,
	const void* userParam);

void enableReportGlErrors();