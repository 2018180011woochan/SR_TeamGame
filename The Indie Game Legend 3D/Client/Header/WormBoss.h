#pragma once
#ifndef __WORMBOSS_H__
#include "Monster.h"
#include "TexturePoolManager.h"
USING(Engine)
class CWormBoss final : public CMonster
{
private:
	explicit CWormBoss();
	explicit CWormBoss(const CWormBoss& other);
	virtual ~CWormBoss() = default;

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
	static CWormBoss* Create();
private:
	virtual void Free() override;

private:
	CMeshRenderer* m_pMeshRenderer = nullptr;
	CTexturePool*  m_pTexturePool;

	UINT nIndex;
};

#define __WORMBOSS_H__
#endif // !__WORMBOSS_H__

