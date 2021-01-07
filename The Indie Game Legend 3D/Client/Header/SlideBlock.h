#pragma once
#ifndef __SLIDEBLOCK_H__

#include "Obstacle.h"
class CSlideBlock final : public CObstacle
{
public:
	enum class eMoveDir
	{
		UD,LR,End
	};
private:
	eMoveDir	m_eMoveDir;
	bool		m_bPick;
	float		m_fSpeed;
public:
public:
	void SetBlockMoveDir(const eMoveDir& _eID);
	void SetPicking(const bool& _bPick);
	void SetSize(float _x , float _y);
public:
	explicit CSlideBlock();
	explicit CSlideBlock(const CSlideBlock& _rOther);
	virtual ~CSlideBlock() =default;
	
	// CObstacle을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;
	virtual CGameObject * Clone() override;

	virtual void OnCollision(CGameObject* _pGameObject) override;
	static CSlideBlock* Create();
private:
	virtual void Free() override;
};

#define __SLIDEBLOCK_H__
#endif // !__SLIDEBLOCK_H__
