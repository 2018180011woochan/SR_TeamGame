#pragma once
#ifndef __STAR_POOL_H__
#include "GameObject.h"
USING(Engine)
class CStarParticle;
class CStarPool final : public CGameObject
{
private:
	list<CStarParticle*> m_StarParticles;
	//list<CStarParticle*> m_StarContainer;
	float				m_fTime;
	float				m_fScatterInterval;
private:
	explicit CStarPool();
	explicit CStarPool(const CStarPool& _rOther);
	virtual ~CStarPool() = default;
	// CBase��(��) ���� ��ӵ�
	virtual void Free() override;
public:
	static CStarPool* Create();
	// CGameObject��(��) ���� ��ӵ�
	virtual CGameObject * Clone() override;
public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;
private:
	void Scatter(const float _fDeltaTime);
public:
	void PushToPool(CStarParticle* _pStarParticle);
};
#define __STAR_POOL_H__
#endif // !__STAR_POOL_H__
