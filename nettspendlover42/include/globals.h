#pragma once

namespace globals
{
	bool imgui_init = false;
	HWND game_hwnd = nullptr;
	uintptr_t engine = NULL;
	uintptr_t executable = NULL;
	bool uninjecting = false;

	bool menu_open = true;
}