#pragma once
#ifndef __EGG_H__
#include "Monster.h"
#include "TexturePoolManager.h"
USING(Engine)
class CEgg final : public CMonster
{
private:
	explicit CEgg();
	explicit CEgg(const CEgg& other);
	virtual ~CEgg() = default;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;

public:
	void EggDrop();

public:
	virtual CGameObject * Clone() override;
	static CEgg* Create();
private:
	virtual void Free() override;

private:
	CMeshRenderer* m_pMeshRenderer = nullptr;
	CTexturePool*  m_pTexturePool;
	DWORD		   m_dwFireTime;
	float		   m_fBreathSpeed;
	float		   m_fBreathDeltaTime;

	UINT nIndex;
};

#define __EGG_H__
#endif // !__EGG_H__

