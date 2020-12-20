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
public:	
	_vector Get_ClientCenterVector() { return m_vClinetCenterPos; };
	RECT GetClientRc() { return m_tClinetRect; }
	POINT	Get_ClientCenterPoint();
	void ReSizeClient();
private:
	explicit CMsgManager();
	virtual ~CMsgManager();
public:
	virtual void Free() override;
};

#define __MSGMANAGER_H__
#endif