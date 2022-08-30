#pragma once
#ifndef __KUNAI_H__
#define __KUNAI_H__

#include "Effect.h"
class CKunai :
	public CEffect
{
public:
	static CEffect* Create_Kunai(_vec2 vPos, int _iDirection);
	void Setting_Kunai(_vec2 vPos, int _iDirection);
public:
	CKunai();
	virtual ~CKunai();

	// CEffect을(를) 통해 상속됨
	virtual HRESULT Init_Effect() override;
	virtual int Update_Effect() override;
	virtual void Late_Update_Effect() override;
	virtual void Render_Effect() override;
	virtual void HitBox_Update() override;

	void Hit_Check();
};


#endif // !__KUNAI_H__
