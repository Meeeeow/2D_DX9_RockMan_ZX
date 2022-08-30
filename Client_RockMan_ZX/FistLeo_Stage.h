#pragma once
#include "Scene.h"
class CFistLeo_Stage :
	public CScene
{
public:
	static CScene* Create_Magma();
private:
	CFistLeo_Stage();
public:
	virtual ~CFistLeo_Stage();

	// CScene을(를) 통해 상속됨
	virtual HRESULT Init_Scene() override;
	virtual void Update_Scene() override;
	virtual void Late_Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;
};

