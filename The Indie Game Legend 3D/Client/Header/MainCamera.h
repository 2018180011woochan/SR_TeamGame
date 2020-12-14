#pragma once
#ifndef __MAINCAMERA_H__
#include "Camera.h"
USING(Engine)
class CMainCamera final : public CCamera
{
private:
	explicit CMainCamera();
	explicit CMainCamera(const CMainCamera& other);
	virtual ~CMainCamera() = default;

public:
	// CCamera을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;

public:
	HRESULT Movement(float fDeltaTime);

public:
	static CMainCamera* Create();
	virtual CGameObject * Clone() override;
	virtual void Free() override;

private:
	float	m_fDistanceToTarget = 5.f;
	float	m_fCameraAngle = 30.f;

private:
	class CTransform* m_pTransformCom = nullptr;
};

#define __MAINCAMERA_H__
#endif // !__MAINCAMERA_H__

