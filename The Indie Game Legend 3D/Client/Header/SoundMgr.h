#pragma once
#ifndef __SOUNDMANAGER_H__
#include "Base.h"
USING(Engine)
class CSoundMgr final : public CBase
{
DECLARE_SINGLETON(CSoundMgr)
public:
	enum CHANNELID {BGM
		,Player_Bullet
		,Player_Effect
		,Item_Ammo
		,Item_Heart
		,Item_Coin
		,Player_Hit
		,Player_Action
		,Object_SFX
		,BOSS
		,HURT
		,BULLET
		,EFFECT
		,PlayerSkill
		,MonsterKill
		,MONSTER_BULLET, BNT1,BNT2,UI,MOUSE, WALL, Floor, MONSTER, MAXCHANNEL};
private:
	explicit CSoundMgr();
	virtual ~CSoundMgr() = default;

public:
	void Initialize(); 
public:
	void Play(const wstring& pSoundKey, CHANNELID eID);
	void PlayContinue(const wstring& pSoundKey, CHANNELID eID);

	void PlayBGM(const wstring& _strSoundKey);
	void StopSound(CHANNELID eID);
	void StopAll();
	void SetVolume(CHANNELID eID, float _Volume);
	void SetPitch(CHANNELID _eID, float _fPitch);//ÀÛ¾÷Áß
	void Pause(CHANNELID _eID, const FMOD_BOOL& _Bool);

private:
	void LoadSoundFile(char FilePath[]); 
	HRESULT LoadSound(const TSTRING& wstrPath);
	virtual void Free() override;
private:
	map<wstring, FMOD_SOUND*> m_mapSound; 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL]; 
	FMOD_SYSTEM* m_pSystem; 
	wstring		m_wstrNowBGM;

};
#define __SOUNDMANAGER_H__
#endif
