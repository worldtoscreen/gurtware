#pragma once

typedef unsigned long long offset_t;

// Represents the addresses used for reading.
namespace offsets
{
	/* User-Interface Stuff */
	namespace ui
	{
		// Type: __int32 (0x4)
		// Read From: executable
		constexpr offset_t WindowWidth = 0x43D788;
		// Type: __int32 (0x4)
		// Read From: executable
		constexpr offset_t WindowHeight = 0x43D78C;
		// Type: HWND (0x8)
		// Read From: engine
		constexpr offset_t hwnd = 0xC3E9A8;
	}

	/* Game Stuff */
	namespace game
	{
		// Type: uint64_t (0x8)
		// Read From: cgame
		constexpr offset_t CGameViewManager = 0x00261990;
		// Type: uint64_t (0x8)
		// Read From: game
		constexpr offset_t World = 0x14941F8;
	}

	/* View Stuff */
	namespace view
	{
		// Type: float (0x4)
		// Hierarchy: [CGameViewManager + 0x28]
		constexpr offset_t LocalFOV = 0x28;
	}

	/* Entity List Stuff */
	namespace entitylist
	{
		// Size of entities in the PlayerList.
		constexpr size_t EntitySize = 0x6C0;
		// Type: uint64_t (0x8)
		// Read From: World
		constexpr offset_t PlayerList = 0x4D0;
		// Type: uint64_t (0x8)
		// Read From: PlayerList
		constexpr offset_t LocalPlayer = 0x6C0;
	}


	/* Entity Stuff */
	namespace entity
	{
		// Type: vec3 (0xC)
		// Hierarchy: [Entity + 0x1C]
		constexpr offset_t Coordinates = 0x1C;
		// Type: vec3 (0xC)
		// Hierarchy: [Entity + 0x28]
		// Note: Engine inverts pitch
		constexpr offset_t Rotation = 0x28;
	}
}

// Represents the actual addresses of objects.
namespace instances
{
	uintptr_t engine = NULL;
	uintptr_t executable = NULL;
	uintptr_t cgame = NULL;
	uintptr_t game = NULL;

	uintptr_t CGameViewManager = NULL;
	uintptr_t World = NULL;
	uintptr_t PlayerList = NULL;
	uintptr_t LocalPlayer = NULL;

	float LocalFov;

	bool init( )
	{
		instances::engine = (uintptr_t)( GetModuleHandleA( "ref_gl_x64.dll" ) );
		instances::executable = (uintptr_t)( GetModuleHandleA( "warsow_x64.exe" ) );
		instances::cgame = (uintptr_t)( GetModuleHandleA( "cgame_x64.dll" ) );
		instances::game = (uintptr_t)( GetModuleHandleA( "game_x64.dll" ) );

		return instances::engine && instances::executable && instances::cgame && instances::game;
	}

	bool valid( )
	{
		return instances::engine && instances::executable && instances::cgame && instances::game;
	}

	bool read( )
	{
		instances::CGameViewManager = *(uintptr_t *)( instances::cgame + offsets::game::CGameViewManager );
		instances::World = *(uintptr_t *)( instances::game + offsets::game::World );
		if ( instances::World && instances::CGameViewManager )
		{
			instances::PlayerList = *(uintptr_t *)( instances::World + offsets::entitylist::PlayerList );
			instances::LocalPlayer = (uintptr_t)( instances::PlayerList + offsets::entitylist::LocalPlayer );
			instances::LocalFov = *(float *)( instances::CGameViewManager + offsets::view::LocalFOV );
			return true;
		}

		return false;
	}

	bool GetWindowDimensions( __int32 &WindowWidth, __int32 &WindowHeight )
	{
		if ( !instances::executable )
			return false;

		WindowWidth = *(__int32 *)( instances::executable + offsets::ui::WindowWidth );
		WindowHeight = *(__int32 *)( instances::executable + offsets::ui::WindowHeight );
		return true;
	}
}

struct vec3
{
	float x;
	float y;
	float z;

	vec3( ) : x( 0.f ), y( 0.f ), z( 0.f ) { };
	vec3( uintptr_t start, bool inverse = false )
	{
		if ( inverse )
		{
			x = *(float *)( start );
			z = *(float *)( start + 0x4 );
			y = *(float *)( start + 0x8 );
		}
		else
		{
			x = *(float *)( start );
			y = *(float *)( start + 0x4 );
			z = *(float *)( start + 0x8 );
		}
	}
	vec3( float x, float y, float z ) : x( x ), y( y ), z( z ) { };

	vec3 operator +( vec3 other )
	{
		return vec3(
			x + other.x,
			y + other.y,
			z + other.z
		);
	};

	vec3 operator -( vec3 other )
	{
		return vec3(
			x - other.x,
			y - other.y,
			z - other.z
		);
	};

	vec3 operator /( vec3 other )
	{
		return vec3(
			x / other.x,
			y / other.y,
			z / other.z
		);
	};

	vec3 operator *( vec3 other )
	{
		return vec3(
			x * other.x,
			y * other.y,
			z * other.z
		);
	};

	bool operator ==( vec3 other )
	{
		return x == other.x && y == other.y && z == other.z;
	};
};

struct vec2
{
	float x;
	float y;

	vec2( ) : x( 0.f ), y( 0.f ) { };
	vec2( float x, float y ) : x( x ), y( y ) { };

	vec2 operator +( vec2 other )
	{
		return vec2( x + other.x, y + other.y );
	};

	vec2 operator -( vec2 other )
	{
		return vec2( x - other.x, y - other.y );
	};

	vec2 operator /( float other )
	{
		return vec2( x / other, y / other );
	};

	vec2 operator /( vec2 other )
	{
		return vec2( x / other.x, y / other.y );
	};

	vec2 operator *( vec2 other )
	{
		return vec2( x * other.x, y * other.y );
	};

	ImVec2 ToImVec2( )
	{
		return ImVec2( x, y );
	}

	bool operator ==( vec2 other )
	{
		return x == other.x && y == other.y;
	};

};

#include <math.h>
#define M_PI 3.141592653

// Matrix structure (4x4 for projection)
struct mat4 {
	float m[4][4];
};

// Function to create a perspective projection matrix
mat4 CreatePerspectiveMatrix( float fov, float aspect, float zNear, float zFar ) {
	mat4 proj = { 0 };
	float f = 1.0f / tanf( fov * 0.5f * ( M_PI / 180.0f ) );

	proj.m[0][0] = f / aspect;
	proj.m[1][1] = f;
	proj.m[2][2] = ( zFar + zNear ) / ( zNear - zFar );
	proj.m[2][3] = ( 2.0f * zFar * zNear ) / ( zNear - zFar );
	proj.m[3][2] = -1.0f;

	return proj;
}

// Function to create a rotation matrix from pitch, yaw, roll (inverted pitch for Qfusion)
mat4 CreateRotationMatrix( vec3 rotation ) {
	mat4 rot = { 0 };
	float pitch = -rotation.x * ( M_PI / 180.0f ); // Inverted pitch
	float yaw = rotation.y * ( M_PI / 180.0f );
	float roll = rotation.z * ( M_PI / 180.0f );

	float cp = cosf( pitch );
	float sp = sinf( pitch );
	float cy = cosf( yaw );
	float sy = sinf( yaw );
	float cr = cosf( roll );
	float sr = sinf( roll );

	rot.m[0][0] = cy * cr + sy * sp * sr;
	rot.m[0][1] = sr * cp;
	rot.m[0][2] = cy * sp * sr - sy * cr;

	rot.m[1][0] = sy * sp * cr - cy * sr;
	rot.m[1][1] = cr * cp;
	rot.m[1][2] = sy * sr + cy * sp * cr;

	rot.m[2][0] = sy * cp;
	rot.m[2][1] = -sp;
	rot.m[2][2] = cy * cp;

	rot.m[3][3] = 1.0f;

	return rot;
}

// Function to multiply a 4x4 matrix by a vector (homogeneous coordinates)
vec3 MatrixTransform( mat4 matrix, vec3 vector ) {
	vec3 result;
	float w;

	result.x = matrix.m[0][0] * vector.x + matrix.m[0][1] * vector.y + matrix.m[0][2] * vector.z + matrix.m[0][3];
	result.y = matrix.m[1][0] * vector.x + matrix.m[1][1] * vector.y + matrix.m[1][2] * vector.z + matrix.m[1][3];
	result.z = matrix.m[2][0] * vector.x + matrix.m[2][1] * vector.y + matrix.m[2][2] * vector.z + matrix.m[2][3];
	w = matrix.m[3][0] * vector.x + matrix.m[3][1] * vector.y + matrix.m[3][2] * vector.z + matrix.m[3][3];

	if ( w != 0.0f ) {
		result.x /= w;
		result.y /= w;
		result.z /= w;
	}

	return result;
}

struct camera
{
	vec3 coords;
	vec3 rotation;
	float fov;
	vec2 dimensions;
	uintptr_t owner;

	bool valid = false;

	camera( bool debug = false )
	{
		if ( !instances::valid( ) )
		{
			if ( debug ) printf( "\t[-] camera::camera invalid instance\n" );
			return;
		}

		if ( !instances::LocalPlayer )
		{
			if ( debug ) printf( "\t[-] camera::camera invalid LocalPlayer\n" );
			return;
		}

		fov = instances::LocalFov;
		coords = vec3( instances::LocalPlayer + offsets::entity::Coordinates, true );
		rotation = vec3( instances::LocalPlayer + offsets::entity::Rotation );
		rotation.x /= -1.f;

		int x, y;
		if ( !instances::GetWindowDimensions( x, y ) )
		{
			if ( debug ) printf( "\t[-] camera::camera invalid window dimensions\n" );
			return;
		}

		dimensions = { (float)x, (float)y };
		owner = (uintptr_t)instances::LocalPlayer;
		valid = true;
	}

	bool Project( vec3 target, vec2 &projected )
	{
		if ( !valid )
			return false;

		// Define near and far planes (common values for games)
		float zNear = 0.1f;
		float zFar = 1000.0f; /// TWEAK THIS

		// Calculate aspect ratio
		float aspect = dimensions.x / dimensions.y;

		// Create perspective projection matrix
		mat4 projMatrix = CreatePerspectiveMatrix( fov, aspect, zNear, zFar );

		// Create rotation matrix (view matrix component)
		mat4 rotMatrix = CreateRotationMatrix( rotation );

		// Transform target point by rotation matrix
		vec3 transformed = MatrixTransform( rotMatrix, target );

		// Apply perspective projection
		transformed = MatrixTransform( projMatrix, transformed );

		// Check if point is in front of camera (z > 0)
		if ( transformed.z < 0.0f ) {
			return false;
		}

		// Normalize to NDC (Normalized Device Coordinates) and map to screen space
		projected.x = ( transformed.x + 1.0f ) * 0.5f * dimensions.x;
		projected.y = ( 1.0f - transformed.y ) * 0.5f * dimensions.y; // Flip Y for OpenGL

		// Check if point is within screen bounds
		if ( projected.x < 0.0f || projected.x > dimensions.x ||
			projected.y < 0.0f || projected.y > dimensions.y ) {
			return false;
		}

		return true;
	}

	void Debug( )
	{
		if ( !valid )
		{
			printf( "[camera::debug] attempted to debug an invalid camera\n" );
		}
		else
		{
			printf( "[camera::debug] owner %llX, \n\tcoords: %f, %f, %f\n\trotation: %f, %f, %f\n\tfov: %f\n\twindow dimensions: %f, %f\n", owner, coords.x, coords.y, coords.z, rotation.x, rotation.y, rotation.z, fov, dimensions.x, dimensions.y );
		}

		printf( "\n\n\n" );
	}

};