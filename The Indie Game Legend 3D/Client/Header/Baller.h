#pragma once
#ifndef __BALLER_H__

#include "Monster.h"
#include "TexturePoolManager.h"
USING(Engine)
class CBaller final : public CMonster
{
private:
	explicit CBaller();
	explicit CBaller(const CBaller& other);
	virtual ~CBaller() = default;

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
	void Jumping(float fDeltaTime);

public:

public:
	virtual CGameObject * Clone() override;
	static CBaller* Create();
private:
	virtual void Free() override;

private:
	CMeshRenderer* m_pMeshRenderer = nullptr;
	CTexturePool*  m_pTexturePool;

	float				m_fJumpingCnt;
	float				m_fJumpSpeed;
	float				m_fMaxJump;
	float				m_fMoveSpeed;
	bool				m_isMaxJump;
	bool				m_isJumping;
	DWORD				m_dwSwitch;

	UINT nIndex = 0;

	bool m_bIsTPS;
};

#define __BALLER_H__
#endif // !__BALLER_H__
