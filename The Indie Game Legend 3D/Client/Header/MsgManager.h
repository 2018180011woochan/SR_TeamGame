#pragma once
#ifndef __MSGMANAGER_H__
#include "Base.h"

#define AirStrikeFireDelay 0.1f
#define AirStrikeBulletCount 15
#define AirstrikeSceneDuration 10.f
USING(Engine)
class CMsgManager : public CBase
{
	DECLARE_SINGLETON(CMsgManager)
private:
	RECT		m_tClinetRect;
	_vector		m_vClinetCenterPos;
	bool		m_bFreezeTrigger;
	bool		m_bAutoAimTrigger;
	bool		m_bAirStrikeTrigger;
	bool		m_bAirStrikeReady;

	float		m_fSkillDuration;
	float		m_fSkillDurationTime;
	//공습공격 간격
	float		m_fAirStrikeDelay;
	float		m_fAirStrikeTime;
	//공습 컷씬 시간
	float		m_fAirStrikeCutSceneDuration;
	//float		m_fAirStrikeCutSceneTime;
	bool		m_bAirStrikeSceneTrigger;

	_vector		m_vAirStrikePos;
	_uint		m_nAirStrikeCount;
	_uint		m_nSceneID;
public:	
	_vector GetClientCenterVector() { return m_vClinetCenterPos; };
	RECT	GetClientRc() { return m_tClinetRect; }
	POINT	GetClientCenterPoint();
	bool	GetAutoAimEnable(){return m_bAutoAimTrigger;}
	bool	GetAirStrikeReady() { return m_bAirStrikeReady; }

public:
	void	ReSizeClient();
	void	Freeze(const float* _fTimeDeleta);
	void    AirStrikeSetting(const _uint& _nSceneID,const _vector& _vPosition);
	void	AirStrikeReady();
	void	AirStrikeFire();
	void	UpdateSkillTime(const float& _fTimeDelta);

	void	FreezingStart(const float& _fTime);
	void    AutoAimStart(const float& _fTime);
private:
	explicit CMsgManager();
	virtual ~CMsgManager();
	virtual void Free() override;
};

#define __MSGMANAGER_H__
#endif
