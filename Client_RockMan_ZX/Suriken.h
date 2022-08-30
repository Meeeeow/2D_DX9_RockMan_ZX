#pragma once
#ifndef __SURIKEN_H__
#define __SURIKEN_H__

#include "Effect.h"
class CSuriken :
	public CEffect
{
public:
	static CEffect* Create_Suriken(_vec2 vPos, int iDirection);
	void Setting_Suriken(_vec2 vPos, int iDirection);
public:
	CSuriken();
	virtual ~CSuriken();

	// CEffect을(를) 통해 상속됨
	virtual HRESULT Init_Effect() override;
	virtual int Update_Effect() override;
	virtual void Late_Update_Effect() override;
	virtual void Render_Effect() override;
	virtual void HitBox_Update() override;

	void Hit_Check();
};


#endif // !__SURIKEN_H__
