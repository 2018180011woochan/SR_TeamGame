#pragma once
#ifndef __CAMERAMANAGER_H__
#include "Base.h"
#include "Camera.h"

USING(Engine)
class CCameraManager : public CBase
{
	enum  ECameraID : _uint
	{
		Player,UFO,End
	};
	DECLARE_SINGLETON(CCameraManager)
private:
	CCamera*	m_pCameraList[ECameraID::End];
	ECameraID	m_eCurMainCameraID;
	ECameraID	m_ePreMainCameraID;

public:
	HRESULT SetCurrentMainCamera(const ECameraID& _eCameraID);
	HRESULT RegisteCamera(const ECameraID& _eCameraID, const CCamera* pCamera);
private:
	explicit CCameraManager();
	virtual ~CCameraManager() = default;
	virtual void Free() override;
};

#define __CAMERAMANAGER_H__
#endif
