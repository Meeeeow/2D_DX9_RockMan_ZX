#pragma once
#include "Effect.h"
class COXSlash :
	public CEffect
{
public:
	static CEffect* Create_Slash(_vec2 vPos);
	void Setting_Slash(_vec2 vPos);
private:
	COXSlash();
public:
	virtual ~COXSlash();

	// CEffect��(��) ���� ��ӵ�
	virtual HRESULT Init_Effect() override;
	virtual int Update_Effect() override;
	virtual void Late_Update_Effect() override;
	virtual void Render_Effect() override;
	virtual void HitBox_Update() override;

	void Attack();
};

