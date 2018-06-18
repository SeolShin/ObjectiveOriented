#include "CommonStuff.h"
#include <strsafe.h>

void AREMsg(TCHAR *szFormat, ...)
{
	TCHAR szBuffer[1024];  // Large buffer for long filenames or URLs
	const size_t NUMCHARS = sizeof(szBuffer) / sizeof(szBuffer[0]);
	const int LASTCHAR = NUMCHARS - 1;

	// Format the input string
	va_list pArgs;
	va_start(pArgs, szFormat);

	StringCchVPrintfA(szBuffer, NUMCHARS - 1, szFormat, pArgs);
	va_end(pArgs);

	::MessageBox(NULL, szBuffer, TEXT("Message\0"), MB_OK);
}
