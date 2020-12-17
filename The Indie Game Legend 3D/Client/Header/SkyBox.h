#pragma once
#ifndef __SKYBOX_H__
#include "Camera.h"
USING(Engine)
class CSkyBox final : public CGameObject
{
private:
	CMeshRenderer*	m_pMeshRenderer;
	CCamera*		m_pCamera;
private:
	explicit CSkyBox();
	explicit CSkyBox(const CSkyBox& _rOther);
	virtual ~CSkyBox() = default;
	// CBase��(��) ���� ��ӵ�
	virtual void Free() override;
public:
	static CSkyBox* Create();
	// CGameObject��(��) ���� ��ӵ�
	virtual CGameObject * Clone() override;
public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;
};

#define __SKYBOX_H__
#endif // !__SKYBOX_H__