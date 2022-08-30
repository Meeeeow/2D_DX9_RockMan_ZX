#pragma once
#ifndef __DESTROY_H__
#define __DESTROY_H__

#include "Effect.h"
class CDestroy :
	public CEffect
{
public:
	static CEffect* Create_Destroy(_vec2 vPos);
	void Setting_Pos(_vec2 vPos);
private:
	CDestroy();
public:
	virtual ~CDestroy();

	// CEffect을(를) 통해 상속됨
	virtual HRESULT Init_Effect() override;
	virtual int Update_Effect() override;
	virtual void Late_Update_Effect() override;
	virtual void Render_Effect() override;
	virtual void HitBox_Update() override;
};


#endif // !__DESTROY_H__
