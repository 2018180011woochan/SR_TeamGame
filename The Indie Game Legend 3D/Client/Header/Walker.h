#pragma once
#ifndef __WALKER_H__
#include "Monster.h"
#include "TexturePoolManager.h"
USING(Engine)
class CWalker final : public CMonster
{
private:
	explicit CWalker();
	explicit CWalker(const CWalker& other);
	virtual ~CWalker() = default;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;

public:
	void BulletFire();

private:
	HRESULT Movement(float fDeltaTime);

public:
	virtual CGameObject * Clone() override;
	static CWalker* Create();
private:
	virtual void Free() override;

private:
	CMeshRenderer*	m_pMeshRenderer = nullptr;
	CTexturePool*	m_pTexturePool;
	float			m_fFireSpeed;
	float			m_fFireDeltaTime;
	float			m_fWalkSpeed;
	float			m_fWalkDeltaTime;
	float			m_fMoveSpeed;
	UINT			nIndex = 0;
};

#define __WALKER_H__
#endif // !__WALKER_H__

