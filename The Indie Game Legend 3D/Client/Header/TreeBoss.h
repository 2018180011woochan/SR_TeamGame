#pragma once
#ifndef __TREEBOSS_H__
#include "Monster.h"
#include "TexturePoolManager.h"
USING(Engine)
class CTreeBoss final : public CMonster
{
private:
	explicit CTreeBoss();
	explicit CTreeBoss(const CTreeBoss& other);
	virtual ~CTreeBoss() = default;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;

public:
	void YukPoongHyul(float _fDeltaTime);
	void RootAttack(float _fDeltaTime);

public:
	virtual CGameObject * Clone() override;
	static CTreeBoss* Create();
private:
	virtual void Free() override;

private:
	CMeshRenderer* m_pMeshRenderer = nullptr;
	CTexturePool*  m_pTexturePool;
	DWORD		   m_dwFireTime;
	float		   m_fFireSpeed;
	float		   m_fFireDeltaTime;

	float			m_fCreateDeltaTime;
	float			m_fCreateSpeed;

	float			m_fAttackDeltaTime;
	float			m_fAttackSpeed;

	float			m_fWinckDeltaTime;
	float			m_fWinckSpeed;

	bool			m_bisCreate;

	
	UINT nCreateIndex;
	UINT nIndex;
};

#define __TREEBOSS_H__
#endif // !__TREEBOSS_H__

