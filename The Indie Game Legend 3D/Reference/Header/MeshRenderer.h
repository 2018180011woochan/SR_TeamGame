#pragma once
#ifndef __MESHRENDERER_H__
#include "Component.h"
BEGIN(Engine)
class CMesh;
class CMeshManager;
class ENGINE_DLL CMeshRenderer final : public CComponent
{
private:
	CMeshManager*	m_pMeshManager;
	CMesh*			m_pMesh;
private:
	explicit CMeshRenderer(CGameObject* const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice);
	virtual ~CMeshRenderer() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	// CComponent을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	HRESULT Render();
	static CMeshRenderer* Create(CGameObject* const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice);

	HRESULT SetMesh(const TSTRING& _sMeshID);
	HRESULT SetTexture(const UINT _nGroupIndex, class CTexture* const _pTexture);

	HRESULT SetMaterial(const UINT _nSubSet, const D3DMATERIAL9 _tMaterial);
	D3DMATERIAL9 GetMaterial(const UINT _nSubSet);
};
END
#define __MESHRENDERER_H__
#endif // !__MESHRENDERER_H__
