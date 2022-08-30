#pragma once
#ifndef __GONG_H__
#define __GONG_H__

#include "Effect.h"
class CGong :
	public CEffect
{
public:
	static CEffect* Create_Gong(float fX, float fY, int iDirection);
	void Setting(float fX, float fY, int iDirection);
private:
	CGong();
public:
	virtual ~CGong();

	// CEffect을(를) 통해 상속됨
	virtual HRESULT Init_Effect() override;
	virtual int Update_Effect() override;
	virtual void Late_Update_Effect() override;
	virtual void Render_Effect() override;
	virtual void HitBox_Update() override;
};


#endif // !__GONG_H__
