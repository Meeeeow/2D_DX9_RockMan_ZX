#pragma once
#ifndef __AREA_D5_H__
#define __AREA_D5_H__

#include "Scene.h"
class CAreaD5 :
	public CScene
{
public:
	static CScene* Create_AreaD5();
private:
	CAreaD5();
public:
	virtual ~CAreaD5();

	// CScene을(를) 통해 상속됨
	virtual HRESULT Init_Scene() override;
	virtual void Update_Scene() override;
	virtual void Late_Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;
};


#endif // !__AREA_D5_H__
