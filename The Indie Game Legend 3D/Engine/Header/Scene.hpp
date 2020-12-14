#pragma once
#ifndef __SCENE_HPP__
#include "Management.h"
BEGIN(Engine)
template<typename T>
inline CGameObject * CScene::AddGameObject()
{
	if (nullptr == m_pManagement)
		return nullptr;
	return m_pManagement->AddGameObject<T>(m_nSceneID);
}
END
#define __SCENE_HPP__
#endif // !__SCENE_HPP__

