#pragma once
#ifndef __MSGMANAGER_H__
#include "Base.h"
USING(Engine)
class CMsgManager : public CBase
{
	DECLARE_SINGLETON(CMsgManager)
private:
	RECT		m_tClinetRect;
	_vector		m_vClinetCenterPos;
	bool		m_bTrigger;
	float		m_fFreezing;
	float		m_fFreezingTime;

public:	
	_vector Get_ClientCenterVector() { return m_vClinetCenterPos; };
	RECT	GetClientRc() { return m_tClinetRect; }
	POINT	Get_ClientCenterPoint();
public:
	void	ReSizeClient();
	void	Freeze(const float* _fTimeDeleta);
	void	UpdateFreezing(const float& _fTimeDelta);

	void	Freezingstart(const float& _fFreeze);
private:
	explicit CMsgManager();
	virtual ~CMsgManager();
public:
	virtual void Free() override;
};

#define __MSGMANAGER_H__
#endif
