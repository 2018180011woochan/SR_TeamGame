#pragma once
#ifndef __DIALOG_HUD_H__
#include "GameObject.h"
USING(Engine)
class CDialogFrame;
class CDialogPortrait;
class CDialogText;
class CDialogTextArrow;
class CDialogHUD final : public CGameObject
{
public:
	enum SPEAKER { PLAYER = 0, NPC, SPEAKER_END };
private:
	CDialogFrame*		m_pDialogFrame;
	CDialogPortrait*	m_pDialogPortrait;
	CDialogText*		m_pDialogText;
	CDialogTextArrow*	m_pDialogTextArrow;

	const D3DXVECTOR3	m_vPortraitPos[SPEAKER_END];
	const D3DXVECTOR3	m_vTextPos[SPEAKER_END];
	const D3DXVECTOR3	m_vArrowPos[SPEAKER_END];
private:
	explicit CDialogHUD();
	explicit CDialogHUD(const CDialogHUD& _rOther);
	virtual ~CDialogHUD() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CDialogHUD* Create();
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
	void SetDialog(const SPEAKER _eSpeaker, const TSTRING _sText, const TSTRING _sPortrait = TEXT("Bean"));
};
#define __DIALOG_HUD_H__
#endif // !__DIALOG_HUD_H__
