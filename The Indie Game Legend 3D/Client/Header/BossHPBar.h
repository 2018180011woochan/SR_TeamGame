#pragma once
#ifndef __BOSS_HP_BAR_H__
#include "GameObject.h"
USING(Engine)
class CTexturePool;
class CBossHPBar final : public CGameObject
{
private:
	Image*			m_pImage;
	CTexturePool*	m_pTexturePool;
	TSTRING			m_sTextureKey;
private:
	explicit CBossHPBar();
	explicit CBossHPBar(const CBossHPBar& _rOther);
	virtual ~CBossHPBar() = default;
	// CBase��(��) ���� ��ӵ�
	virtual void Free() override;
public:
	static CBossHPBar* Create();
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

public:
	//�Ű����� ������ ���� ü�� / �ִ� ü�� �� �Ѱ��ָ� ��.
	void SetHPBar(float _fRatio);
};
#define __BOSS_HP_BAR_H__
#endif // !__BOSS_HP_BAR_H__