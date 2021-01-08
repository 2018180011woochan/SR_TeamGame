#pragma once
#ifndef __SELECTOR_H__
#include "GameObject.h"
USING(Engine)
class CTexturePool;
class CKeyManager;
class CSelector final: public CGameObject
{
private:
	CTexturePool* m_pTexturePool;
	Image*		  m_pImage = nullptr;
	TSTRING		  m_sTextureKey;
	CKeyManager*		m_pKeyManager;

private:
	explicit CSelector();
	explicit CSelector(const CSelector& _rOther);
	virtual ~CSelector() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CSelector* Create();
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
};
#define __SELECTOR_H__
#endif // !__SELECTOR_H__
