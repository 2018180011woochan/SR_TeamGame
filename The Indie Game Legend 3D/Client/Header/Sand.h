#pragma once
#ifndef __SAND_H__
#include "GameObject.h"
USING(Engine)
class CTexturePool;
class CWormPart;
class CSand final : public CGameObject
{
public:
	enum Direction { UP, DOWN};
private:
	CMeshRenderer*	m_pMeshRenderer;
	
	CTexturePool*	m_pTexturePool;
	TSTRING			m_sTextureKey;

	UINT			m_nIndex;
	UINT			m_nMaxFrame;

	float			m_fTime;
	float			m_fAnimationSpeed;

	CWormPart*		m_pWormTail;
	CWormPart*		m_pWorm;
	
	Direction			m_eDirection;

	D3DXVECTOR3			m_vWormPrev;

	bool				m_bRender;

	bool				m_bRemove;
private:
	explicit CSand();
	explicit CSand(const CSand& _rOther);
	virtual ~CSand() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CSand* Create();
	// CGameObject을(를) 통해 상속됨
	virtual CGameObject * Clone() override;
public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;
public:
	void SetSand(Direction _eDir, CWormPart* _pWorm, CWormPart* _pTail);
	void Remove();
private:
	void OnRender();
	void OffRender();
	void UpdatePrevPos();

	void Billboard();

	void Animate(const float _fDeltaTime);
};
#define __SAND_H__
#endif // !__SAND_H__
