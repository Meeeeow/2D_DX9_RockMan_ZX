#pragma once
#include "Effect.h"
class CAquaField :
	public CEffect
{
public:
	static CEffect* Create_AquaField();
public:
	CAquaField();
	virtual ~CAquaField();

	// CEffect��(��) ���� ��ӵ�
	virtual HRESULT Init_Effect() override;
	virtual int Update_Effect() override;
	virtual void Late_Update_Effect() override;
	virtual void Render_Effect() override;
	virtual void HitBox_Update() override;

private:
	bool	AquaTime;
};

