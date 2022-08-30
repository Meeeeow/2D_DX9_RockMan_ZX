#include "stdafx.h"
#include "SerpantWave.h"


CEffect * CSerpantWave::Create_Wave(float fX, float fY, int iDirection)
{
	CEffect* pEffect = new CSerpantWave;
	pEffect->Init_Effect();
	static_cast<CSerpantWave*>(pEffect)->Setting(fX, fY, iDirection);
	return pEffect;
}

void CSerpantWave::Setting(float fX, float fY, int iDirection)
{
	m_tInfo.vecPos = { fX,fY };
	m_tInfo.iDirection = iDirection;
}

CSerpantWave::CSerpantWave()
{
}


CSerpantWave::~CSerpantWave()
{
}

HRESULT CSerpantWave::Init_Effect()
{
	return E_NOTIMPL;
}

int CSerpantWave::Update_Effect()
{
	return 0;
}

void CSerpantWave::Late_Update_Effect()
{
}

void CSerpantWave::Render_Effect()
{
}

void CSerpantWave::HitBox_Update()
{
}
