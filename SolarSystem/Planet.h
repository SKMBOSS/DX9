#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "SAFE_DELETE.h"
#include "CUSTOM_VERTEX.h"

class Planet
{
public:
	CUSTOMVERTEX vertices[4] =
	{
		{-1, 1, 1, 0xffff0000},
		{  1, 1, 1, 0xff00ff00 },
		{  1, 1, -1, 0xff0000ff },
		{ -1, 1, -1, 0xffffff00 },
	};

	D3DXMATRIXA16	g_matTMParent;
	D3DXMATRIXA16	g_matRParent;
public:
	Planet();
	~Planet();
};

