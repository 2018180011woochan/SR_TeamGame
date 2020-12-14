#pragma once
#ifndef __CAMERA_H__
#include "GameObject.h"
BEGIN(Engine)
class ENGINE_DLL CCamera abstract : public CGameObject
{
protected:
	explicit CCamera();
	explicit CCamera(const CCamera& _rOther);
protected:
	virtual ~CCamera() = default;
protected:
	CAMERA_DESC m_tCameraDesc;

public:
	/*Getter*/
	const CAMERA_DESC& Get_Camera() { return m_tCameraDesc; }
	void Set_Camera(CAMERA_DESC _CameraDesc);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() PURE;
	virtual HRESULT Awake() PURE;
	virtual HRESULT Start() PURE;
	virtual UINT Update(const float _fDeltaTime) PURE;
	virtual UINT LateUpdate(const float _fDeltaTime) PURE;
	virtual HRESULT Render() PURE;
	virtual CGameObject * Clone() PURE;
public:
	// CComponent을(를) 통해 상속됨
	virtual void Free() override;
};
END
#define __CAMERA_H__
#endif