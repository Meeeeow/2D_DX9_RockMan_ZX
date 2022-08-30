#pragma once
#ifndef __SERPANT_LASER_H__
#define __SERPANT_LASER_H__

#include "Effect.h"
class CSerpantLaser :
	public CEffect
{
public:
	static CEffect* Create_Laser(float fX, float fY, int iDirection);
	void Setting(float fX, float fY, int iDirection);
private:
	CSerpantLaser();
public:
	virtual ~CSerpantLaser();

	// CEffect��(��) ���� ��ӵ�
	virtual HRESULT Init_Effect() override;
	virtual int Update_Effect() override;
	virtual void Late_Update_Effect() override;
	virtual void Render_Effect() override;
	virtual void HitBox_Update() override;
};


#endif // !__SERPANT_LASER_H__
