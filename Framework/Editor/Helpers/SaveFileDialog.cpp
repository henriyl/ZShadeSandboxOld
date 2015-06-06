#include "SaveFileDialog.h"
//--------------------------------------------------------------------------------------------------------------------
SaveFileDialog::SaveFileDialog()
{
}
//--------------------------------------------------------------------------------------------------------------------
SaveFileDialog::SaveFileDialog(const SaveFileDialog& other)
{
}
//--------------------------------------------------------------------------------------------------------------------
SaveFileDialog::~SaveFileDialog()
{
}
//--------------------------------------------------------------------------------------------------------------------
bool SaveFileDialog::OpenDialog(HWND hwnd)
{
	OPENFILENAME saveFileDialog;
	char szSaveFileName[MAX_PATH];
	
	ZeroMemory(&saveFileDialog, sizeof(saveFileDialog));

	saveFileDialog.lStructSize= sizeof(saveFileDialog);
	saveFileDialog.lpstrFile = szSaveFileName;
	saveFileDialog.lpstrFile[0] = '\0';
	saveFileDialog.hwndOwner = hwnd;
	saveFileDialog.nMaxFile = sizeof(szSaveFileName);
	saveFileDialog.lpstrFilter = "XML (*.xml)\0*.xml\0";
	saveFileDialog.nFilterIndex = 1;
	saveFileDialog.lpstrInitialDir = NULL;
	saveFileDialog.lpstrFileTitle = NULL;
	saveFileDialog.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;
	saveFileDialog.lpstrDefExt = "xml";
	
	if(GetSaveFileName(&saveFileDialog))
	{
		m_filename = saveFileDialog.lpstrFile;
		return true;
	}

	return false;
}
//--------------------------------------------------------------------------------------------------------------------
void SaveFileDialog::SaveFile(LPSTR filename)
{
	DWORD nBytesWritten;
	
	HANDLE hFile;
	DWORD dwSize;
	DWORD dw;

	LPBYTE lpBuffer = NULL;

	hFile = CreateFile(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	dwSize = GetFileSize(hFile, NULL);
	lpBuffer = (LPBYTE)HeapAlloc(GetProcessHeap(), HEAP_GENERATE_EXCEPTIONS, dwSize + 1);
	
	WriteFile(hFile, (LPWSTR)lpBuffer, dwSize, &dw, NULL);

	CloseHandle(hFile);

	lpBuffer[dwSize] = 0;

	m_filename = filename;

	//SetWindowText(m_hwnd, (LPSTR)lpBuffer);
	HeapFree(GetProcessHeap(), 0, lpBuffer);
}
//--------------------------------------------------------------------------------------------------------------------