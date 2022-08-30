#pragma once
#ifndef __CLOAK_H__
#define __CLOAK_H__

#include "Effect.h"
#include "ObjectBase.h"

class CObjectBase;
class CPXCloak :
	public CEffect
{
public:
	static CEffect* Create_Cloak(float fX, float fY, int iDirection,CObjectBase* pObj);
	void Setting(float fX, float fY, int iDirection, CObjectBase* pObj);
private:
	CPXCloak();
public:
	virtual ~CPXCloak();

	// CEffect을(를) 통해 상속됨
	virtual HRESULT Init_Effect() override;
	virtual int Update_Effect() override;
	virtual void Late_Update_Effect() override;
	virtual void Render_Effect() override;
	virtual void HitBox_Update() override;

private:
	float m_fAngle;
	int   m_iCount;
	CObjectBase* m_pParent;
};


#endif // !__CLOAK_H__
