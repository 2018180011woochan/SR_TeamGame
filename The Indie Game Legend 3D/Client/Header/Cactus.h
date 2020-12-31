#pragma once
#ifndef __CACTUS_H__

#include "GameObject.h"
#include "TexturePoolManager.h"
USING(Engine)
class CCactus final : public CGameObject
{
private:
	explicit CCactus();
	explicit CCactus(const CCactus& other);
	virtual ~CCactus() = default;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;

	virtual void OnCollision(CGameObject* _pGameObject) override;
private:
	HRESULT Movement(float fDeltaTime);
	void Jumping(float fDeltaTime);
	
public:
	void SetNPCPos(const _vector _EggPos);
	HRESULT IsBillboarding();

public:
	virtual CGameObject * Clone() override;
	static CCactus* Create();
private:
	virtual void Free() override;

private:
	CMeshRenderer* m_pMeshRenderer = nullptr;
	CTexturePool*  m_pTexturePool;
	const CTransform*	m_pPlayerTransform;

	float				m_fMoveSpeed;

	float				m_fWalkSpeed;
	float				m_fWalkDeltaTime;

	float				m_fJumpPower;
	float				m_fJumpTime;
	float				m_fYTest;
	bool				m_bJump;

	float				m_fJumpSpeed;
	float				m_fJumpDeltaTime;

	UINT nIndex;

	bool m_bIsTPS;
};

#define __CACTUS_H__
#endif // !__CACTUS_H__
