#pragma once
#ifndef __FINAL_CAMERA_H__
#include "Camera.h"
USING(Engine)
class CFinalCamera final : public CCamera
{
private:
	explicit CFinalCamera();
	explicit CFinalCamera(const CFinalCamera& _rOther);
	virtual ~CFinalCamera() = default;
	// CComponent��(��) ���� ��ӵ�
	virtual void Free() override;
public:
	static CFinalCamera* Create();
	// CCamera��(��) ���� ��ӵ�
	virtual CGameObject * Clone() override;
public:
	// CCamera��(��) ���� ��ӵ�
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;
};
#define __FINAL_CAMERA_H__
#endif // !__FINAL_CAMERA_H__
