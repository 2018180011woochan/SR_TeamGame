#pragma once
#ifndef __PLAYERCAMERA_H__
#include "Camera.h"
USING(Engine)

class CPlayerCamera final : public CCamera
{
private:
	float	m_fDistanceToAt = 10.f;
	float	m_fCameraAngleX = 0.f;
	float	m_fCameraAngleSpeed = 1.f;
	class CPlayer* m_pPlayer;
	class CMouse*  m_pMouse;
private:
	HRESULT UpdateMove(float _fDeletaTime);
private:
	explicit CPlayerCamera();
	explicit CPlayerCamera(const CPlayerCamera& _rOther);
public:
	virtual ~CPlayerCamera() = default;

	// CCamera을(를) 통해 상속됨
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
