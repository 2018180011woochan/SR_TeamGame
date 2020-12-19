#pragma once
#ifndef __COLLIDER_H__
#include "Component.h"
BEGIN(Engine)
typedef struct tagBoundBox
{
	D3DXVECTOR3 vMin;
	D3DXVECTOR3 vMax;
}BOUNDINGBOX, *LPBOUNDINGBOX;
class CMesh;
class CMeshManager;
class ENGINE_DLL CCollider final : public CComponent
{
private:
	CMeshManager*	m_pMeshManager;
	CMesh*			m_pCollisionMesh;
	BOUNDINGBOX		m_tBoundingBox;
private:
	explicit CCollider(CGameObject* const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice);
	virtual ~CCollider() = default;
	// CBase��(��) ���� ��ӵ�
	virtual void Free() override;
public:
	static CCollider* Create(CGameObject * const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice);
public:
	// CComponent��(��) ���� ��ӵ�
	virtual HRESULT Initialize() override;

public:
	HRESULT SetMesh(const TSTRING& _sMesh);
	HRESULT Draw();
private:
	void SetBound();
public:
	BOUNDINGBOX GetBound();
};
END
#define __COLLIDER_H__
#endif // !__COLLIDER_H__
