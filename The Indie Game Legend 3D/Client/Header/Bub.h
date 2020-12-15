#pragma once
#ifndef __BUB_H__

#include "GameObject.h"
#include "TexturePoolManager.h"
USING(Engine)
class CBub final : public CGameObject
{
private:
	explicit CBub();
	explicit CBub(const CBub& other);
	virtual ~CBub() = default;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;

public:
	virtual CGameObject * Clone() override;
	static CBub* Create();
private:
	virtual void Free() override;

private:
	CMeshRenderer* m_pMeshRenderer = nullptr;
	CTexturePool*  m_pTexturePool;

	bool m_bIsTPS;
};

#define __BUB_H__
#endif // !__BUB_H__
