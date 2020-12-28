#pragma once
#ifndef __BOSS_TEXT_H__
#include "GameObject.h"
USING(Engine)
class CBossText final : public CGameObject
{
private:
	CText* m_pText;
private:
	explicit CBossText();
	explicit CBossText(const CBossText& _rOther);
	virtual ~CBossText() = default;
	// CBase��(��) ���� ��ӵ�
	virtual void Free() override;
public:
	static CBossText* Create();
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
};
#define __BOSS_TEXT_H__
#endif // !__BOSS_TEXT_H__
