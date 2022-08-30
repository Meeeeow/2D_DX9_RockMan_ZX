#include "stdafx.h"
#include "ScrollMgr.h"


void CScrollMgr::Set_StartQuake()
{
	m_vQuakeScroll.x += CRandomMgr::Random();
	m_vQuakeScroll.x -= CRandomMgr::Random();
	m_vQuakeScroll.y += CRandomMgr::Random();
	m_vQuakeScroll.y -= CRandomMgr::Random();
}

void CScrollMgr::Set_ScrollLock(int _maxxSize, int _maxysize)
{
	if (0 < m_vScroll.x)
		m_vScroll.x = 0;
	if (WINCX - _maxxSize > m_vScroll.x)
		m_vScroll.x = WINCX - _maxxSize;
}

CScrollMgr::CScrollMgr()
{
	ZeroMemory(&m_vScroll, sizeof(_vec2));
	ZeroMemory(&m_vShowScroll, sizeof(_vec2));
	ZeroMemory(&m_vQuakeScroll, sizeof(_vec2));
}


CScrollMgr::~CScrollMgr()
{
}
