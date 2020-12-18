#pragma once
#ifndef __MESH_H__
#include "Base.h"
#include "SubSet.h"
BEGIN(Engine)

typedef struct tagVertex
{
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Normal;
	D3DXVECTOR2 UV;
	static const DWORD FVF;
}VERTEX, *LPVERTEX;

typedef struct tagVertices : public CBase
{
	VERTEX* pVertices;
private:
	explicit tagVertices();
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static tagVertices* Create();

}VERTICES, *LPVERTICES;
class CGraphicDevice;
class CMesh final : public CBase
{
private:
	typedef vector<CSubSet*> SUBSET;
	SUBSET					m_Subset;
	LPDIRECT3DDEVICE9		m_pDevice;
	LPDIRECT3DVERTEXBUFFER9	m_pVertexBuffer;
	UINT					m_nVertexCount;
	UINT					m_nFaceCount;
	LPVERTICES				m_pVertices;
private:
	explicit CMesh();
	explicit CMesh(const CMesh& _rOther);
	virtual ~CMesh() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CMesh* Create(const TSTRING& _sFileName);
	CMesh* Clone();

private:
	HRESULT		LoadMesh(const TSTRING& _sFileName);
	HRESULT		LoadMaterial(const TSTRING& _sFullPath, unordered_map<TSTRING, MATERIAL>& _materials);

public:
	HRESULT Draw();
	HRESULT DrawSubSet(const UINT _nSubSet);

public:
	const LPVERTEX	GetVertices();
	const UINT		GetVertexCount();
	HRESULT SetTexture(const UINT _nSubSet, CTexture* const _pTexture);
};
END
#define __MESH_H__
#endif // !__MESH_H__
