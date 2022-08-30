#pragma once
#ifndef __AREA_E1_H__
#define __AREA_E1_H__


#include "Scene.h"
class CAreaE1 :
	public CScene
{
public:
	static CScene* Create_AreaE1();
private:
	CAreaE1();
public:
	virtual ~CAreaE1();

	// CScene을(를) 통해 상속됨
	virtual HRESULT Init_Scene() override;
	virtual void Update_Scene() override;
	virtual void Late_Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;
};


#endif // !__AREA_E1_H__
