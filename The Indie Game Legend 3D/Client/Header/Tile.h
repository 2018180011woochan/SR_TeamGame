#pragma once
#ifndef __TILE_H__
#include "GameObject.h"
#include "TexturePoolManager.h"

USING(Engine)
class CTile abstract: public CGameObject
{
protected:
	CMeshRenderer* m_pMeshRenderer = nullptr;
	CTexturePool*  m_pTexturePool = nullptr;

protected:
	explicit CTile();
	explicit CTile(const CTile& _rOther);
	virtual ~CTile() = default;

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() PURE;
	virtual HRESULT Awake() PURE;
	virtual HRESULT Start() PURE;
	virtual UINT Update(const float _fDeltaTime) PURE;
	virtual UINT LateUpdate(const float _fDeltaTime) PURE;
	virtual HRESULT Render() PURE;
	virtual CGameObject * Clone() PURE;

protected:
	virtual void Free();
};
#define __TILE_H__
#endif // !__TILE_H__
