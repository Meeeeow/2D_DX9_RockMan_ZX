#pragma once
#ifndef __SCROLL_MGR_H__
#define __SCROLL_MGR_H__

#include "Singleton.h"
class CScrollMgr :
	public CSingleton<CScrollMgr>
{
public:
	enum ID { S_HORI, S_VERT};

	_vec2 Get_Scroll() const { return m_vScroll; }
	float Get_ScrollX() const { return m_vScroll.x; }
	void Set_Scroll(const _vec2& vScroll) { m_vScroll += vScroll; }
	void Set_ScrollX(const float _ScrollX) { m_vScroll.x += _ScrollX; }
	void Set_CenterPos(const _vec2& vCenter) { m_vCenterPos = vCenter; }

	_vec2 Get_ShowScroll() { return m_vShowScroll; }
	void Set_ShowScroll(const _vec2& vScroll) { m_vShowScroll += vScroll; }
	void Set_ShowScrollX(const float _vScroll) { m_vShowScroll.x += _vScroll; }

	_vec2 Get_QuakeScroll() { return m_vQuakeScroll; }

	void Set_Quake() { m_vQuakeScroll = m_vScroll; }
	void Set_StartQuake();
	void Set_QuakeScrollX(const int _ScrollX) { m_vQuakeScroll.x += _ScrollX; }

	void Set_ScrollLock(int _maxxSize, int _maxysize);
	_vec2 Get_CenterPos() { return m_vCenterPos; }
private:
	_vec2 m_vScroll;
	_vec2 m_vShowScroll;
	_vec2 m_vQuakeScroll;
	_vec2 m_vCenterPos;
public:
	CScrollMgr();
	virtual ~CScrollMgr();

};


#endif // !__SCROLL_MGR_H__
