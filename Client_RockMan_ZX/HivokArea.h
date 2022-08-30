#pragma once
#ifndef __HIVOK_AREA_H__
#define __HIVOK_AREA_H__

#include "Scene.h"
class CHivokArea :
	public CScene
{
public:
	static CScene* Create_HivokStage();
private:
	CHivokArea();
public:
	virtual ~CHivokArea();

	// CScene을(를) 통해 상속됨
	virtual HRESULT Init_Scene() override;
	virtual void Update_Scene() override;
	virtual void Late_Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;
};


#endif // !__HIVOK_AREA_H__
