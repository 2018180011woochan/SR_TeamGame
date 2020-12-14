#include "..\Header\FileInfo.h"

TSTRING CFileInfo::FilePath(const TSTRING & _sFileName)
{
	return FindPath(_sFileName);
}

TSTRING CFileInfo::FindPath(const TSTRING & _sFileName, const TSTRING & _sPath)
{
	TSTRING sPath = _sPath;

	TCHAR szBuf[MAX_PATH] = TEXT("");
	const TCHAR* ppOtherDir[] = { sPath.c_str(),NULL };
	_tcscpy_s(szBuf, MAX_PATH, _sFileName.c_str());

	if (TRUE == PathFindOnPath(szBuf, ppOtherDir))
		return TSTRING(szBuf);

	WIN32_FIND_DATA tFindData;
	TSTRING sFindPath = sPath + TEXT("/*.*");
	HANDLE hFind = NULL;
	hFind = FindFirstFile(sFindPath.c_str(), &tFindData);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		FindClose(hFind);
		return TSTRING(TEXT(""));
	}

	do
	{
		if (TEXT('.') == tFindData.cFileName[0])
			continue;
		else if (FILE_ATTRIBUTE_DIRECTORY == tFindData.dwFileAttributes)
		{
			TSTRING sResult = FindPath(_sFileName, sPath + TEXT("/") + tFindData.cFileName);

			if (TEXT("") == sResult)
				continue;
			else
				return sResult;
		}
	} while (FindNextFile(hFind, &tFindData));

	return TSTRING(TEXT(""));
}
