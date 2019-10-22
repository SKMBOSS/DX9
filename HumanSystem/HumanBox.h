#pragma once
#include <d3d9.h>
#include <d3dx9.h>

class HumanBox
{
public :
	HumanBox* parent;

	D3DXMATRIXA16	m_matWorld;
	D3DXMATRIXA16	m_matTM;
	D3DXMATRIXA16	m_matR;
public :
	void TRANSLATION(float x, float y, float z);
	void ROTATIONX(float speed);
	void ROTATIONZ(float speed);
	D3DXMATRIXA16* RENDER();
	void SET_PARENT(HumanBox &_parent);
public:
	HumanBox();
	~HumanBox();
};

