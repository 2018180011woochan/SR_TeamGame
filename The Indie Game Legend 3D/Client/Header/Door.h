#pragma once
#ifndef __DOOR_H__
#include "InteractionObj.h"
class CDoor final : public CInteractionObj
{
private:
	CTexturePool*   m_pTexturePool;
	float			m_fTextureIndex = 0.f;
	float			m_fAnimateSpeed = 0.f;
	float			m_fAnimateOneCycleTime = 0.f; // 한번 순회하는데 몇초로 설정할거냐	
	_uint			m_nMaxFrame = 0;
	bool			m_bOpen;
public:
	void Open();
	void Close();
	void SetOpen(const bool _bOpen) { m_bOpen; }
	bool IsOpen() { return m_bOpen; }
private:
	void Animate(const float _fDeltaTime);
private:
	explicit CDoor();
	explicit CDoor(const CDoor& _rOther);
	virtual ~CDoor() = default;

public:
	// CInteractionObj을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;
	virtual void OnCollision(CGameObject * _pGameObject) override;
	virtual void OnNotify() override;
public:
	virtual CGameObject * Clone() override;
	static CDoor* Create();
private:
	virtual void Free() override;


};
#define __DOOR_H__
#endif
