#pragma once
class CSoundMgr
{
public:
	static CSoundMgr* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CSoundMgr; 

		return m_pInstance; 
	}
	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance; 
			m_pInstance = nullptr; 
		}
	}
public:
	enum CHANNELID {BGM,
		/* Player  */ BEFORM, BEFORMEFFECT, FORMSELECT, FORMMOVE, HIT, 
		SABER_FIRST, SABER_SECOND, SABER_THIRD, DASH_ATTACK, JUMP_ATTACK, CHARGE, CHARGEMAX,
		OX_SPECIAL, HX_SPECIAL, FX_SPECIAL, LX_SPECIAL, PX_SPECIAL , LX_DASH, OX_BEFORM,
		WALK, DASH , JUMP, AIRDASH,VERTDASH, HOVER, AIRDASHATTACK,
		/* Monster */HELMET_HIT, ROID_HIT, ACROBAT_HIT, DESTROY, WARNING,
		/* Hivolt  */HI_HIT,HI_INTRO,HI_DASH,HI_SUMMON,HI_F,HI_S,HI_T,
		/* FistLeo */FL_HIT, FL_INTRO,FL_DASH,FL_JUMP,FL_ATT,FL_FORGE,FL_FINAL, FL_WALL,
		/* Serpant */SERPANT_HIT, SERPANT_INTRO, SERPANT_JUMP, SERPANT_FIRE, SERPANT_UPPER, SERPANT_BLINK, SERPANT_SLIDE,
		/* Copy  X */COPYX_INTRO,COPYX_SHOT,COPYX_TACKLE,COPYX_JD,COPYX_DOOM,COPYX_OVER,COPYX_HIT,
		COPYX_ELECT, COPYX_FLAME, MAXCHANNEL};
private:
	CSoundMgr();
	~CSoundMgr();

public:
	void Initialize(); 

	void Release(); 
public:
	void PlaySound(TCHAR* pSoundKey, CHANNELID eID);
	void PlayBGM(TCHAR* pSoundKey);
	void StopSound(CHANNELID eID);
	void StopAll();

private:
	void LoadSoundFile(); 

private:
	static CSoundMgr* m_pInstance; 
	// 사운드 리소스 정보를 갖는 객체 
	map<TCHAR*, FMOD_SOUND*> m_mapSound; 
	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL]; 
	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pSystem; 

};

