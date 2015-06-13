//*************************************************************************************************************************
// SaveFileDialog.h
//*************************************************************************************************************************
#ifndef __SAVEFILEDIALOG_H
#define __SAVEFILEDIALOG_H
//--------------------------------------------------------------------------------------------------------------------
#include <Windows.h>
//--------------------------------------------------------------------------------------------------------------------
class SaveFileDialog
{
	LPSTR m_filename;
	void SaveFile(LPSTR filename);
public:
	SaveFileDialog();
	SaveFileDialog(const SaveFileDialog& other);
	~SaveFileDialog();

	bool OpenDialog(HWND hwnd);
	LPSTR GetFilename() { return m_filename; }
};
//--------------------------------------------------------------------------------------------------------------------
#endif//__SAVEFILEDIALOG_H