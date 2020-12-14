#pragma once
#ifndef __MESH_H__
#include "Base.h"
#include "Group.h"
BEGIN(Engine)

class CGraphicDevice;

class CMesh final : public CBase
{
private:
	typedef vector<CGroup*> GROUPS;
	GROUPS	m_Groups;
	LPDIRECT3DDEVICE9 m_pDevice;
private:
	explicit CMesh();
	explicit CMesh(const CMesh& _rOther);
	virtual ~CMesh() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;

private:
	HRESULT		LoadMesh(const TSTRING& _sFileName);
	HRESULT		LoadMaterial(const TSTRING& _sFullPath, unordered_map<TSTRING, MATERIAL>& _materials);

public:
	HRESULT Render();
	HRESULT SetTexture(const UINT _nGroupIndex, CTexture* const _pTexture);

	static CMesh* Create(const TSTRING& _sFileName);
	CMesh* Clone();
};
END
#define __MESH_H__
#endif // !__MESH_H__
