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
#include "instances.h"

#pragma comment(lib, "opengl32.lib")