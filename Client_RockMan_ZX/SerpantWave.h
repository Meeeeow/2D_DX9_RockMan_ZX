#pragma once
#ifndef __SERPANT_WAVE_H__
#define __SERPANT_WAVE_H__

#include "Effect.h"
class CSerpantWave :
	public CEffect
{
public:
	static CEffect* Create_Wave(float fX, float fY, int iDirection);
	void Setting(float fX, float fY, int iDirection);
private:
	CSerpantWave();
public:
	virtual ~CSerpantWave();

	// CEffect을(를) 통해 상속됨
	virtual HRESULT Init_Effect() override;
	virtual int Update_Effect() override;
	virtual void Late_Update_Effect() override;
	virtual void Render_Effect() override;
	virtual void HitBox_Update() override;
};


#endif // !__SERPANT_WAVE_H__
