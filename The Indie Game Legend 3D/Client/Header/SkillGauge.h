#pragma once
#ifndef __SKILLGAUGE_H__
#include "GameObject.h"
USING(Engine)
class CTexturePool;
class CSkillGauge final : public CGameObject
{
private:
	CTexturePool*	m_pTexturePool;
	Image*			m_pImage;
	TSTRING			m_sTextureKey;
private:
	explicit CSkillGauge();
	explicit CSkillGauge(const CSkillGauge& _rOther);
	virtual ~CSkillGauge() = default;
	// CBase��(��) ���� ��ӵ�
	virtual void Free() override;
public:
	static CSkillGauge* Create();
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
	void SetSkillGauge(const float _fValue);
};
#define __SKILLGAUGE_H__
#endif // !__SKILLGAUGE_H__
