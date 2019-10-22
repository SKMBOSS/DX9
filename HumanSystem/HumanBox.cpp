#include "HumanBox.h"

HumanBox::HumanBox()
{
	m_matR =
	{
	1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	0,0,0,1,
	};
}


HumanBox::~HumanBox()
{
}


void HumanBox::TRANSLATION(float x, float y, float z)
{
	D3DXMatrixTranslation(&m_matTM, x, y, z);
}

void HumanBox::ROTATIONZ(float speed)
{
	D3DXMatrixRotationZ(&m_matR, GetTickCount() / speed);
}

void HumanBox::ROTATIONX(float speed)
{
	D3DXMatrixRotationX(&m_matR, GetTickCount() / speed);
}

D3DXMATRIXA16* HumanBox::RENDER()
{
	if (parent == nullptr)
	{
		m_matWorld = m_matR * m_matTM;
	}
	else
	{
		m_matWorld = m_matR * m_matTM * parent->m_matWorld;
	}
	

	return &m_matWorld;
}

void HumanBox::SET_PARENT(HumanBox &_parent)
{
	parent = &_parent;
}