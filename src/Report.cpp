#include <string>
#include "Report.h"
#include <Windows.h>

void ReportError(std::string txt, std::string title)
{
	MessageBox(NULL,txt.c_str(),title.c_str(),MB_OK|MB_ICONSTOP);
}
