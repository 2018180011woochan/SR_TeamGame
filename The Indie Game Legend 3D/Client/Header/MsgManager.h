#pragma once
#ifndef __MSGMANAGER_H__
#include "Base.h"

#define AirStrikeBulletCount 15

USING(Engine)
class CMsgManager : public CBase
{
	DECLARE_SINGLETON(CMsgManager)
private:
	RECT		m_tClinetRect;
	_vector		m_vClinetCenterPos;
	bool		m_bFreezeTrigger;
	bool		m_bAirStrikeTrigger;
	bool		m_bAirStrikeReady;
	bool		m_bSkillCooldown;

	float		m_fSkillDuration;
	float		m_fSkillDurationTime;

	//  [1/5/2021 wades]
	bool		m_bHighNoonTrigger;
	bool		m_bHighNoonTriggerReady;
	_int		m_nHighNoonBulletCount;
	_int		m_nHighNoonDmg;
	//공습공격 간격
	float		m_fFireDelay;
	float		m_fFireDelayTime;
	//공습 컷씬 시간
	float		m_fAirStrikeCutSceneDuration;
	bool		m_bAirStrikeSceneTrigger;
	float		m_fSirenLightColor; // 경고등컬리
	float		m_fSirenLightChangValue;

	vector<CGameObject*>		m_vecHighNoon;
	_vector						m_vAirStrikePos;
	_uint						m_nAirStrikeCount;
	_uint						m_nSceneID;
	bool						m_bMouseMode;
public:	
	_vector GetClientCenterVector() { return m_vClinetCenterPos; };
	RECT	GetClientRc() { return m_tClinetRect; }
	POINT	GetClientCenterPoint();
	bool	GetHighNoon(){return m_bHighNoonTrigger;}
	bool	GetHighNoonReady() { return m_bHighNoonTriggerReady; }
	bool	GetAirStrikeReady() { return m_bAirStrikeReady; }
	bool	GetMouseMode() { return m_bMouseMode; }
	void	Freeze(const float* _fTimeDeleta);
	void	ReSizeClient();
	void    AirStrikeStart(const _uint& _nSceneID,const _vector& _vPosition);
	void	HighNoonStart(const _uint& _nSceneID, list<CGameObject*>& _listGameObj, const _int& _nDmg);
	void	UpdateSkillTime(const float& _fTimeDelta);
public:
	void	AirStrikeReady();
	void	AirStrikeFire();
	void    WargingLight(const float& _fTimeDelta);
	void	FreezingStart(const float& _fTime);
	void    HighNoonReady(const float& _fTime);

	void	MouseModeOnOff();
private:
	void	HighNoon(const float& _fTime);
private:
	explicit CMsgManager();
	virtual ~CMsgManager();
	virtual void Free() override;
};

#define __MSGMANAGER_H__
#endif
