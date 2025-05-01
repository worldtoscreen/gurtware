#pragma once

#include <Windows.h>
#include <iostream>
#include <GL/gl.h>
#include <stdio.h>
#include <thread>
#include <chrono>
#include <cmath>

#include "MinHook.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_opengl3.h"

#include "globals.h"
#include "offsets.h"

#include <stdio.h>
#include <stdarg.h>

void FormatToBuffer( char *buffer, size_t bufferSize, const char *fmt, ... ) {
    va_list args;
    va_start( args, fmt );
    vsnprintf_s( buffer, bufferSize, _TRUNCATE, fmt, args );
    va_end( args );
}

#pragma comment(lib, "opengl32.lib")