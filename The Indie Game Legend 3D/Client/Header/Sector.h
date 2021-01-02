#pragma once
#ifndef __SECTOR1_H__
#include "GameObject.h"
USING(Engine)
class CTexturePool;
class CSector final : public CGameObject
{
private:
	explicit CSector();
	explicit CSector(const CSector& _rOther);
	virtual ~CSector() = default;
public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;
	virtual CGameObject * Clone() override;
public:
	static CSector* Create();
private:
	virtual void Free();
public:
	void SetSectorName(const TSTRING& _sSectorName);
private:
	void Animate(const float _fDeltaTime);

private:
	TSTRING		m_sSectorName;
	CMeshRenderer* m_pMeshRenderer = nullptr;
	CTexturePool*  m_pTexturePool;
	TSTRING		   m_sTextureKey;

	UINT		   m_nIndex;
	UINT		   m_nMaxFrame;

	float		   m_fTime;
	float		   m_fAnimationSpeed;
};

#define __SECTOR1_H__
#endif
