#pragma once
#ifndef __BLOOD_H__

#include "GameObject.h"
#include "TexturePoolManager.h"
USING(Engine)
class CBlood final : public CGameObject
{
private:
	explicit CBlood();
	explicit CBlood(const CBlood& other);
	virtual ~CBlood() = default;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;

private:
	void Spreading(float _fDeltaTime);
	void IsBillBording();
	
public:
	void SetPos(const _vector _EggPos);

public:
	virtual CGameObject * Clone() override;
	static CBlood* Create();
private:
	virtual void Free() override;

private:
	CMeshRenderer* m_pMeshRenderer = nullptr;
	CTexturePool*  m_pTexturePool;

	float				m_fMoveSpeed;

	float				m_fWalkSpeed;
	float				m_fWalkDeltaTime;

	float				m_fJumpPower;
	float				m_fJumpTime;
	float				m_fYTest;
	bool				m_bJump;
	bool				m_bDead;

	UINT nIndex;

	bool m_bIsTPS;
};

#define __BLOOD_H__
#endif // !__BLOOD_H__
