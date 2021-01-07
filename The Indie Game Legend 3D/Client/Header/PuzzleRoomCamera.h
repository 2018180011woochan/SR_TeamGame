#pragma once
#ifndef __PUZZLEROOMCAMERA_H__	
#include "Camera.h"
USING(Engine)
class CPuzzleRoomCamera final : public CCamera
{
private:
	explicit CPuzzleRoomCamera();
	explicit CPuzzleRoomCamera(const CPuzzleRoomCamera& _rOther);
public:
	virtual ~CPuzzleRoomCamera() = default;
public:

	// CCamera을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;
	virtual CGameObject * Clone() override;


	static CPuzzleRoomCamera* Create();
private:
	HRESULT UpdateCamera(float _fDeletaTime);
private:
	virtual void Free();
};
#define __PUZZLEROOMCAMERA_H__
#endif
