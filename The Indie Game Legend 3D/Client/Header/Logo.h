#pragma once
#ifndef __LOGO_H__
#include "GameObject.h"
USING(Engine)
class CLogo : public CGameObject
{
private:
	Image*	m_pImage;
private:
	explicit CLogo();
	explicit CLogo(const CLogo& _rOther);
	virtual ~CLogo() = default;
	// CBase��(��) ���� ��ӵ�
	virtual void Free() override;
public:
	static CLogo* Create();
	virtual CGameObject * Clone() override;

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;
};
#define __LOGO_H__
#endif // !__LOGO_H__
