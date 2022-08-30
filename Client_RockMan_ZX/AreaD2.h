#pragma once
#include "Scene.h"
class CAreaD2 :
	public CScene
{
public:
	static CScene* Create_AreaD2();
private:
	CAreaD2();
public:
	virtual ~CAreaD2();

	// CScene��(��) ���� ��ӵ�
	virtual HRESULT Init_Scene() override;
	virtual void Update_Scene() override;
	virtual void Late_Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;
private:
	RECT rc;
};

