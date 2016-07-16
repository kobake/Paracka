#pragma once

#include <WinSystemLib.h>

//class FileSelectDialog;
#include "WinSystemLib.h"
#include "src/dlg/FileDialog.h"

class Paracka : public App{
public:
	Paracka(HINSTANCE _hInst,HINSTANCE _hPrevInst,const wchar_t *_softname,const wchar_t *_mutex=NULL);
	~Paracka();
	int init();
public:
	FileDialog *fs;
};



