#pragma once
#ifndef __NPC_DIALOG_H__
#include "GameObject.h"
USING(Engine)
class CDialogFrame;
class CNPCPortrait;
class CNPCText;
class CNPCDialog final : public CGameObject
{
private:
	CDialogFrame*	m_pDialogFrame;
	CNPCPortrait*	m_pNPCPortrait;
	CNPCText*		m_pNPCText;
private:
	explicit CNPCDialog();
	explicit CNPCDialog(const CNPCDialog& _rOther);
	virtual ~CNPCDialog() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CNPCDialog* Create();
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

	virtual void OnEnable() override;
	virtual void OnDisable() override;

public:
	void SetPortrait(const TSTRING _sPortrait);
	void SetText(const TSTRING _sText);

};
#define __NPC_DIALOG_H__
#endif // !__NPC_DIALOG_H__

