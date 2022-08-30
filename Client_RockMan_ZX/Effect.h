#pragma once
#ifndef __EFFECT_H__
#define __EFFECT_H__

class CEffect abstract
{
public:
	CEffect();
	virtual ~CEffect();

public:
	virtual HRESULT		Init_Effect() PURE;
	virtual int			Update_Effect() PURE;
	virtual void		Late_Update_Effect() PURE;
	virtual void		Render_Effect() PURE;
	virtual void		HitBox_Update() PURE;

protected:
	EFFECTINFO m_tInfo;
};


#endif // !__EFFECT_H__
