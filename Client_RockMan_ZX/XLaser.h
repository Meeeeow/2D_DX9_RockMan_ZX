#pragma once
#ifndef __X_LASER_H__
#define __X_LASER_H__

#include "Effect.h"
class CXLaser :
	public CEffect
{
public:
	static CEffect* Create_Laser(float fX);
	void Setting(float fX);
public:
	CXLaser();
	virtual ~CXLaser();

	// CEffect��(��) ���� ��ӵ�
	virtual HRESULT Init_Effect() override;
	virtual int Update_Effect() override;
	virtual void Late_Update_Effect() override;
	virtual void Render_Effect() override;
	virtual void HitBox_Update() override;
};


#endif // !__X_LASER_H__
 