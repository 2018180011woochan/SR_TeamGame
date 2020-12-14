#pragma once
#ifndef __LAYER_H__
#include "Base.h"
BEGIN(Engine)
class CLayer final : public CBase
{
private:
	explicit CLayer();
	virtual ~CLayer() = default;
public:
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
};
END
#define __LAYER_H__
#endif // !__LAYER_H__
