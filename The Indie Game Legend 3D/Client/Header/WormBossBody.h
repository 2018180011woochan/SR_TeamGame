#pragma once
#ifndef __WORMBOSSBODY_H__
#include "Monster.h"
#include "TexturePoolManager.h"
USING(Engine)
class CWormBossBody final : public CMonster
{
private:
	explicit CWormBossBody();
	explicit CWormBossBody(const CWormBossBody& other);
	virtual ~CWormBossBody() = default;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;

private:
	HRESULT Movement(float fDeltaTime);

public:
	virtual CGameObject * Clone() override;
	static CWormBossBody* Create();
private:
	virtual void Free() override;

private:
	CMeshRenderer* m_pMeshRenderer = nullptr;
	CTexturePool*  m_pTexturePool;

	float		   m_fFrameSpeed;
	float		   m_fFrameDeltaTime;
	
	float			m_fMoveSpeed;

	_vector vRightDir;
	_vector vLeftDir;

	UINT nIndex;
};

#define __WORMBOSSBODY_H__
#endif // !__WORMBOSSBODY_H__

