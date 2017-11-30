#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define log_error(M, ...) printf("[ERROR] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define log_warning(M, ...) printf("[WARN] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define log_info(M, ...) printf("[INFO] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#define log_error_exit(M, ...) if (true) { log_error(M, ##__VA_ARGS__); exit(EXIT_FAILURE); }

#define unimplemented() if(true) { log_error("Hit unimplemented code block"); exit(EXIT_FAILURE); }

#define allocation_failure() if (true) { log_error("Out of memory"); exit(EXIT_FAILURE); }
#define check_allocation(A) if (A == NULL) { log_error("Memory allocation failed."); exit(EXIT_FAILURE); }

#define check(A, M, ...) if (!(A)) { log_warning(M, ##__VA_ARGS__); }
#define check_null(A) check(A, #A " is NULL")
#define check_exit(A, M, ...) if (!(A)) { log_error(M, ##__VA_ARGS__); getchar(); exit(EXIT_FAILURE); }
#define check_exit_if_null(A) check_exit(A, #A " is NULL")
#define memcheck(A) check_exit(A, "Out Of Memory: " #A)

#ifndef NODEBUG
#define dcheck(A, M, ...) check(A, M, ##__VA_ARGS__)
#define dcheck_null(A) check_null(A)
#define dcheck_exit(A, M, ...) check_exit(A, M, ##__VA_ARGS__)
#define dcheck_exit_if_null(A) check_exit_if_null(A)
#else
#define dcheck(A, M, ...)
#define dcheck_null(A)
#define dcheck_exit(A, M, ...)
#define dcheck_exit_if_null(A)
#endif

#ifdef UTILITY_OPENGL
#include <GL/glew.h>
#define log_opengl_errors() if (true) { GLenum error; while (error = glGetError(), error != GL_NO_ERROR) { log_error("OpenGL Error: 0x%08x", error); } }
#endif
