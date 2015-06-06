//=========================================================================
// ZShadeINIWriter.h class

#ifndef __ZSHADEINIWRITER_H_
#define __ZSHADEINIWRITER_H_

#pragma once

#include <windows.h>
#include <string>
using namespace std;

class ZShadeINIWriter
{
public:
	bool SetFile(std::string p_FilePath, bool editor);
    void WriteInt(std::string p_SectionName, int ivalue);
	void WriteFloat(std::string p_SectionName, float fvalue);
	void WriteBoolean(std::string p_SectionName, bool bvalue);
    void WriteString(std::string p_SectionName, std::string svalue);
	void GetFilePath() { return m_FilePath; }
	
    ZShadeINIWriter(std::string p_FilePath, bool editor);
    virtual ~ZShadeINIWriter();

private:
    std::string m_FilePath;
};

#endif /* __ZSHADEINIWRITER_H_ */
