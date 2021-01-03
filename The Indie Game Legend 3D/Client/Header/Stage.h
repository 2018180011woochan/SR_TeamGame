#pragma once
#ifndef __STAGE_H__
#include "Scene.h"
USING(Engine)
class CStage : public CScene
{
	enum class RoomID : _int
	{
		MazeRoom = 29,
		SandRoom = 6,
		SwampRoom = 10,
		ForestRoom= 26,
		MetalRoom = 30,
		End= -1
	};


private:
	CGameObject* m_pPlayer;
	_int		 m_nPreRoomID;
	_int		 m_nCurRoomID;

private:
	explicit CStage();
	virtual ~CStage() = default;

public:
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;

private:
	HRESULT AddLight();
	HRESULT AddUIObject();
	HRESULT AttachObj();

	void	CheckRoomEvent();
public:
	static CStage* Create();
	virtual void Free() override;
};

#define __STAGE_H__
#endif // !__STAGE_H__

