#pragma once

namespace instances
{
	uintptr_t engine = NULL;
	uintptr_t executable = NULL;
	uintptr_t cgame = NULL;
	uintptr_t game = NULL;

	bool init( )
	{
		instances::engine = (uintptr_t)( GetModuleHandleA( "ref_gl_x64.dll" ) );
		instances::executable = (uintptr_t)( GetModuleHandleA( "warsow_x64.exe" ) );
		instances::cgame = (uintptr_t)( GetModuleHandleA( "cgame_x64.dll" ) );
		instances::game = (uintptr_t)( GetModuleHandleA( "game_x64.dll" ) );

		return instances::engine && instances::executable && instances::cgame && instances::game;
	}
}