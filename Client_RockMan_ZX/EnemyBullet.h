#pragma once
#ifndef __ENEMY_BULLET_H__
#define __ENEMY_BULLET_H__

#include "Effect.h"
class CEnemyBullet :
	public CEffect
{
public:
	static CEffect* Create_EnemyBullet(float fX, float fY, int iDirection);
	void Set_Pos(float fX, float fY, int iDirection);
public:
	CEnemyBullet();
	virtual ~CEnemyBullet();

	// CEffect을(를) 통해 상속됨
	virtual HRESULT Init_Effect() override;
	virtual int Update_Effect() override;
	virtual void Late_Update_Effect() override;
	virtual void Render_Effect() override;
	virtual void HitBox_Update() override;
};


#endif // !__ENEMY_BULLET_H__
