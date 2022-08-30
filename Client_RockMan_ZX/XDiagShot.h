#pragma once
#ifndef __X_DIAG_SHOT_H__
#define __X_DIAG_SHOT_H__

#include "Effect.h"
class CXDiagShot :
	public CEffect
{
public:
	static CEffect* Create_DiagShot(float fX, float fY, int iDirection);
	void Setting(float fX, float fY, int iDirection);
private:	
	CXDiagShot();
public:
	virtual ~CXDiagShot();

	// CEffect을(를) 통해 상속됨
	virtual HRESULT Init_Effect() override;
	virtual int Update_Effect() override;
	virtual void Late_Update_Effect() override;
	virtual void Render_Effect() override;
	virtual void HitBox_Update() override;
private:
};


#endif // !__X_DIAG_SHOT_H__
