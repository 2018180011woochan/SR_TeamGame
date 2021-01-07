#pragma once
#ifndef __CAMERAMANAGER_H__
#include "Base.h"
#include "Camera.h"

USING(Engine)

//ī�޶� ������ �����ѵ� 
//FSM������ Ȱ��ȭ ������ �����ϴ� �� 

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
