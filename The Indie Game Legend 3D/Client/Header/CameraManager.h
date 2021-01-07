#pragma once
#ifndef __CAMERAMANAGER_H__
#include "Base.h"
#include "Camera.h"

USING(Engine)

//카메라 여러개 세팅한뒤 
//FSM구조로 활성화 설정만 관리하는 넘 

class CCameraManager : public CBase
{
public:
	enum  ECameraID
	{
		Player,UFO,PuzzleRoom,End
	};
	DECLARE_SINGLETON(CCameraManager)
private:
	vector<CCamera*>	m_vecCameras;
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
