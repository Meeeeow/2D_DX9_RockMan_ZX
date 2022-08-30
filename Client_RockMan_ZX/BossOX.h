#pragma once
#ifndef __BOSS_OX_H__
#define __BOSS_OX_H__

#include "Scene.h"
class CBossOX :
	public CScene
{
public:
	static CScene* Create_Boss_OX();
private:
	CBossOX();
public:
	virtual ~CBossOX();

	// CScene을(를) 통해 상속됨
	virtual HRESULT Init_Scene() override;
	virtual void Update_Scene() override;
	virtual void Late_Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;
};


#endif // !__BOSS_OX_H__
