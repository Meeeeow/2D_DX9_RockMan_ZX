#include "stdafx.h"
#include "ObjectBase.h"


void CObjectBase::LineCheck(CObjectBase* _pObejct)
{
	
}

void CObjectBase::Hurricane_Hit(float fY, int iDmg, RECT HitBox)
{
	Default_Hit(iDmg,HitBox);
	RECT rc = {};
	if (IntersectRect(&rc, &m_tHitBox, &HitBox))
	{
		m_tInfo.vPos.y -= fY;
		if (m_tInfo.vPos.y < 0)
			m_bDead = true;
	}
}

void CObjectBase::FX_Hit(float fX, int iDmg, RECT HitBox)
{
	Default_Hit(iDmg, HitBox);
	RECT rc = {};
	if (IntersectRect(&rc, &m_tHitBox, &HitBox))
	{
		m_tInfo.vPos.x += m_iDirection * fX;
	}
}

CObjectBase::CObjectBase()
	:m_bDead(false), m_iDirection(OBJ_RIGHT), m_bCheckLine(true)
{
	ZeroMemory(&m_tHitBox, sizeof(m_tHitBox));
}


CObjectBase::~CObjectBase()
{
}
