#include "main.h"

Paracka::Paracka(HINSTANCE _hInst,HINSTANCE _hPrevInst,const wchar_t *_softname,const wchar_t *_mutex)
: App(_hInst,_hPrevInst,_softname,_mutex)
{
	fs=new FileDialog(L"Paracka Text File (*.txt)\0*.txt\0Any File (*.*)\0*.*\0\0",L"txt");
	fs->GetPath().SetTitle(L"...");
	fs->GetPath().SetDirPath(inifile->getString(L"path",L"lastdir",getExeDir().c_str()).c_str());
}

Paracka::~Paracka()
{
	inifile->WriteString(L"path",L"lastdir",fs->GetPath().GetDirPath());
	delete fs;
}

int Paracka::init()
{
	return 0;
}

