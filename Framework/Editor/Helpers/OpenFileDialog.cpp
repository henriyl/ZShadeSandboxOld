#include "OpenFileDialog.h"
//#include "CGlobal.h"
#include <string>
using std::string;
//--------------------------------------------------------------------------------------------------------------------
OpenFileDialog::OpenFileDialog()
{
}
//--------------------------------------------------------------------------------------------------------------------
OpenFileDialog::OpenFileDialog(const OpenFileDialog& other)
{
}
//--------------------------------------------------------------------------------------------------------------------
OpenFileDialog::~OpenFileDialog()
{
}
//--------------------------------------------------------------------------------------------------------------------
bool OpenFileDialog::OpenDialog(HWND hwnd)
{
	OPENFILENAME ofn;
	TCHAR szFile[MAX_PATH];

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.hwndOwner = hwnd;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "XML (*.xml)\0*.xml\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrFileTitle = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
	ofn.lpstrDefExt = "xml";
	
	if (GetOpenFileName(&ofn))
	{
		m_filename = ofn.lpstrFile;
		return true;
	}
	
	return false;
}
//--------------------------------------------------------------------------------------------------------------------
void OpenFileDialog::LoadFile(LPSTR filename)
{
	HANDLE hFile;
	DWORD dwSize;
	DWORD dw;

	LPBYTE lpBuffer = NULL;

	hFile = CreateFile(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	dwSize = GetFileSize(hFile, NULL);
	lpBuffer = (LPBYTE)HeapAlloc(GetProcessHeap(), HEAP_GENERATE_EXCEPTIONS, dwSize + 1);
	
	ReadFile(hFile, (LPWSTR)lpBuffer, dwSize, &dw, NULL);

	CloseHandle(hFile);

	lpBuffer[dwSize] = 0;

	m_filename = filename;

	//SetWindowText(m_hwnd, (LPSTR)lpBuffer);
	HeapFree(GetProcessHeap(), 0, lpBuffer);
}
//--------------------------------------------------------------------------------------------------------------------