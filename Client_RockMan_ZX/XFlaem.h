#pragma once
#ifndef __X_FLAME_H__
#define __X_FLAME_H__

#include "Effect.h"
class CXFlaem :
	public CEffect
{
public:
	static CEffect* Create_Flame(float fX, float fY, int iDirection);
	void Setting(float fX, float fY, int iDirection);
private:
	CXFlaem();
public:
	virtual ~CXFlaem();

	// CEffect을(를) 통해 상속됨
	virtual HRESULT Init_Effect() override;
	virtual int Update_Effect() override;
	virtual void Late_Update_Effect() override;
	virtual void Render_Effect() override;
	virtual void HitBox_Update() override;
};


#endif // !__X_FLAME_H__
