/* 
util.h
*/	
/**
@file util.h
@brief simple utilities that don't belong anywhere else
*/
	
/**
@defgroup utility_functions Utility Functions
@brief a grab bag of utilities designed to keep the code clean

These consist of functions to load files, convert partial paths
to full paths, and various unicode and ansi conversion routines
@{
*/


/** @brief converts a TCHAR (unicode or ansi) into a wide character string

	This method uses the bstr_t object to provide character conversion 
	@param str - [in] input string, either unicode or MBCS
	@return unicode string 
*/
std::wstring TCHAR_2_wstring(const TCHAR *str);

/**@brief converts a unicode string into an MBCS string

	This is used to help print unicode strings to ansi streams
	@param str - [in] unicode string
	@return MBCS string
**/
std::string  wstring_2_string(const std::wstring& str);

/// retrieve the unicode bytes of the specified text file
HRESULT GetTextFile(const TCHAR* pszFileName,WCHAR ** ppwszCoMem,
    ULONG * pcch);
