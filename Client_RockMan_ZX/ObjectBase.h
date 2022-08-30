#pragma once
#ifndef __OBJECT_BASE_H__
#define __OBJECT_BASE_H__

class CObjectBase
{
public:
	virtual HRESULT Init_Object() PURE;
	virtual int Update_Object() PURE;
	virtual void Late_Update_Object() PURE;
	virtual void Render_Object() PURE;
	virtual void Release_Object() PURE;
	virtual void Default_Hit(int iDmg, RECT HitBox) PURE;
	void	 LineCheck(CObjectBase* _pObejct);
	void	 CheckLineActive() { m_bCheckLine = true; }
	void	 CheckLineInActive() { m_bCheckLine = false; }
	UNITINFO Get_Info() { return m_tInfo; }
	RECT	 Get_Rect() { return m_tHitBox; }
	void	 Set_PosX(float fX) { m_tInfo.vPos.x += fX; }
	void	 Setting_PosX(float fX) { m_tInfo.vPos.x = fX; }
	void	 Pos_Setting(float fX, float fY) { m_tInfo.vPos.x = fX; m_tInfo.vPos.y = fY; }
	void	 Pos_SettingYPlus(float fX, float fY) { m_tInfo.vPos.x = fX; m_tInfo.vPos.y -= fY; }
	void	 Hurricane_Hit(float fY,int iDmg, RECT HitBox);
	void	 FX_Hit(float fX,int iDmg ,RECT HitBox);
	int		 Get_Direction() { return m_iDirection; }
public:
	explicit CObjectBase();
	virtual ~CObjectBase();

public:
	UNITINFO	m_tInfo;
	UNITFRAME	m_tFrame;
	RECT		m_tHitBox;
	bool		m_bDead;
	bool		m_bCheckLine;
	int			m_iDirection;
	_vec2		m_vecRect[5];
};


#endif // !__OBJECT_BASE_H__
