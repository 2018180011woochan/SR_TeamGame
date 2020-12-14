#pragma once
#include "EngineInclude.h"
class ENGINE_DLL CFileInfo final
{
public:
	static TSTRING FilePath(const TSTRING& _sFileName);
private:
	static TSTRING FindPath(const TSTRING& _sFileName, const TSTRING& _sPath = TEXT("../Resources"));
};

