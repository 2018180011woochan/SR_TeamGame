#pragma once
#ifndef __PLAYERCAMERA_H__
#include "Camera.h"
USING(Engine)

#define PlayerCameraEye 10.f
#define CameraYMax 89.f
#define CameraYMin -89.f

class CPlayerCamera final : public CCamera
{

	enum class eCameraWork
	{
		ZoomIn,
		ZoomOut,
		End
	};
private:
	float	m_fDistanceToAt = 10.f;
	float	m_fCameraAngleX = 0.f;
	float	m_fCameraAngleSpeed = 1.f;
	float	m_fCameraHeight = 0.f;
	float	m_fcameraVertical = 0.f;
	class CPlayer* m_pPlayer;
	class CMouse*  m_pMouse;

	//  [1/6/2021 wades]
	CAMERA_DESC	m_tOriCamera_Desc; //������ �����
	eCameraWork m_eCurCameraWork, m_ePreCameraWork; // pre���� �ӽ÷� 

	bool	m_bAnimate;
	float	m_fFovAnimateMax;
	float	m_fFovAnimateMin;
	float	m_fFovAmount;
	
	float	m_fFov;
private:
	HRESULT UpdateCamera(float _fDeletaTime);

	//  [1/6/2021 wades]
	void CameraWorking(const float& _fTimeDelta);


public:
	//Camera Work
	void SetHeghitPersent(const float& _fHeight);
	void SetShakeVertical(const float& _fVertical);
	// �ð�ó���� ���������� ������� ����ó�� �ϴ� ������ ���߿�...
	// ������ �ְ� �ּ��ִ밪 ���� ó�� [1/6/2021 wades]
	void SetCameraZoomIn(const float& _fMinFov, const float& _fAmont);
	void SetCameraZoomOut(const float& _fMaxFov, const float& _fAmont);

	void	ResetCamera();
	void	StopCameraWorking() { m_eCurCameraWork = eCameraWork::End; };
private:
	explicit CPlayerCamera();
	explicit CPlayerCamera(const CPlayerCamera& _rOther);
public:
	virtual ~CPlayerCamera() = default;

	// CCamera��(��) ���� ��ӵ�
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;
	virtual CGameObject * Clone() override;

	static CPlayerCamera* Create();
private:
	virtual void Free();

};
#define __PLAYERCAMERA_H__
#endif
