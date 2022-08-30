#pragma once
#ifndef __LEO_WALL_H__
#define __LEO_WALL_H__

#include "Effect.h"
class CLeoWall :
	public CEffect
{
public:
	static CEffect* Create_Wall(float fX, int iDirection);
	void Setting(float fX, int iDirection);
private:
	CLeoWall();
public:
	virtual ~CLeoWall();

	// CEffect을(를) 통해 상속됨
	virtual HRESULT Init_Effect() override;
	virtual int Update_Effect() override;
	virtual void Late_Update_Effect() override;
	virtual void Render_Effect() override;
	virtual void HitBox_Update() override;
	
	void Move_Start() { m_bMove = true; }
private:
	bool m_bMove;

};


#endif // !__LEO_WALL_H__
