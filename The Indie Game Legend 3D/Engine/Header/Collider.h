#pragma once
#ifndef __COLLIDER_H__
#include "Component.h"
BEGIN(Engine)

typedef struct tagBound
{
	D3DXVECTOR3 vCenter;
	float		fRadius;
}BOUND, *LPBOUND;

class CMesh;
class CMeshManager;
class ENGINE_DLL CCollider final : public CComponent
{
private:
	CMeshManager*	m_pMeshManager;
	CMesh*			m_pCollisionMesh;
	BOUND			m_tBound;
private:
	explicit CCollider(CGameObject* const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice);
	virtual ~CCollider() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CCollider* Create(CGameObject * const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice);
public:
	// CComponent을(를) 통해 상속됨
	virtual HRESULT Initialize() override;

public:
	HRESULT SetMesh(const TSTRING& _sMesh);
	HRESULT Draw();
private:
	void SetBound();

};
END
#define __COLLIDER_H__
#endif // !__COLLIDER_H__
