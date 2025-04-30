#pragma once
#include "types.h"

typedef unsigned long long offset_t;

namespace offsets
{
	/* User-Interface Stuff */

	// Type: __int32 (0x4)
	// Read From: executable
	constexpr offset_t WindowWidth = 0x43D788;
	// Type: __int32 (0x4)
	// Read From: executable
	constexpr offset_t WindowHeight = 0x43D78C;
	// Type: HWND (0x8)
	// Read From: engine
	constexpr offset_t hwnd = 0xC3E9A8;

	/* Game Stuff */
	// Type: uint64_t (0x8)
	// Read From: cgame
	constexpr offset_t CGameViewManager = 0x00261990;
	// Type: uint64_t (0x8)
	// Read From: game
	constexpr offset_t World = 0x14941F8;
	// Type: uint64_t (0x8)
	// Read From: World
	constexpr offset_t PlayerList = 0x4D0;
	// Type: uint64_t (0x8)
	// Read From: PlayerList
	constexpr offset_t FirstEntry = 0x4D0;


	/* View Stuff */
	// Type: float (0x4)
	// Hierarchy: [CGameViewManager + 0x28]
	constexpr offset_t LocalFOV = 0x28;

	/* Entity List Stuff */
	// Size of entities in the PlayerList.
	constexpr size_t EntitySize = 0x6C0;
	
	// Type: vec3 (0x12)
	// Hierarchy: [FirstEntry + 0x1C]
	constexpr offset_t Coordinates = 0x1C;
}