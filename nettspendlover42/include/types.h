#pragma once

struct vec3
{
	float x;
	float y;
	float z;

	vec3( ) : x( 0.f ), y( 0.f ), z( 0.f ) { };
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

	vec2 operator /( vec2 other )
	{
		return vec2( x / other.x, y / other.y );
	};

	vec2 operator *( vec2 other )
	{
		return vec2( x * other.x, y * other.y );
	};

	bool operator ==( vec2 other )
	{
		return x == other.x && y == other.y;
	};

};