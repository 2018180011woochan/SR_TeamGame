#pragma once
#ifndef __DIALOG_PORTRAIT_H__
#include "GameObject.h"
USING(Engine)
class CTexturePool;
class CDialogPortrait final : public CGameObject
{
private:
	Image*			m_pImage;
	CTexturePool*	m_pTexturePool;
	TSTRING			m_sTextureKey;

	UINT			m_nIndex;
	UINT			m_nMaxFrame;

	float			m_fTime;
	float			m_fAnimationSpeed;

	bool			m_bPlaying;
private:
	explicit CDialogPortrait();
	explicit CDialogPortrait(const CDialogPortrait& _rOther);
	virtual ~CDialogPortrait() = default;
	// CBase��(��) ���� ��ӵ�
	virtual void Free() override;
public:
	static CDialogPortrait* Create();
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
	void OnAnimation(bool _bPlaying);
public:
	void SetPortrait(const TSTRING& _sPortrait, const D3DXVECTOR3 _vPosition);
private:
	void AnimatePortrait(const float _fDeltaTime);
};
#define __DIALOG_PORTRAIT_H__
#endif // !__DIALOG_PORTRAIT_H__
