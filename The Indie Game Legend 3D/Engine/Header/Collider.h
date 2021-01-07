#pragma once
#ifndef __COLLIDER_H__
#include "Component.h"
BEGIN(Engine)
typedef struct tagBound
{
	enum BOUNDTYPE { BOX, SPHERE };
	BOUNDTYPE	eType;
	D3DXVECTOR3 vMin;
	D3DXVECTOR3 vMax;
	D3DXVECTOR3 vCenter;
	float		fRadius;
	float		fLength;
	float		fHeight;
	float		fDepth;
}BOUND, *LPBOUND;
class CMesh;
class CMeshManager;
class ENGINE_DLL CCollider final : public CComponent
{
private:
	CMeshManager*	m_pMeshManager;
	CMesh*			m_pCollisionMesh;
	CMesh*			m_pDrawMesh;
	BOUND			m_tBound;
public:
	bool			m_bIsRigid;
	bool			m_bExcept;
	bool			m_bIsStand;
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
	HRESULT SetMesh(const TSTRING& _sMesh, const BOUND::BOUNDTYPE _eBoundType);
	HRESULT Draw();
private:
	void SetBound();
public:
	//BOUNDINGBOX GetBound();
	BOUND				GetBound();
	const D3DXVECTOR3*	GetVertices();
	const UINT			GetVertexCount();

	void				SetRadius(const float _fRadius);

	bool IsRayPicking(OUT D3DXVECTOR3& _pOut,OUT float& _Dis, const D3DXVECTOR3 _vRayPv, const D3DXVECTOR3 _vRayDir);
};
END
#define __COLLIDER_H__
#endif // !__COLLIDER_H__
