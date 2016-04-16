#include "main.h"

App *AppCreate(HINSTANCE hInstance,HINSTANCE hPrevInstance)
{
	return new Paracka(hInstance,hPrevInstance,L"Paracka");
}


MainWindow *winMain;


int AppInit(int argc,const wchar_t* argv[],int nCmdShow)
{
	if(argc>=2){
		app->fs->GetPath().SetFullPath(argv[1]);
	}
	winMain=new MainWindow(app->getSoftName(),-1,-1,500,200,NULL);
	winMain->show(nCmdShow);
	return 0;
}


ACCELDEF accdef[]={
	IDM_FILE_OPEN,				'O',	ACCELF_CONTROL,
	IDM_FILE_RELOAD,			'R',	ACCELF_CONTROL,
	IDM_FILE_RELOAD_RESTART,	'T',	ACCELF_CONTROL,
	IDM_FUNC_MARKING,			'M',	ACCELF_CONTROL,
	IDM_FUNC_FILTER_MARKING,	'F',	ACCELF_CONTROL,
	-1,					-1,		0,
};

int AppRoutine()
{
	CoInitialize(NULL);

	Accelerator *acc=new Accelerator();
	acc->setEntries(accdef);
	int ret=app->dialogLoop(winMain,acc);
	delete acc;

	CoUninitialize();
	return ret;
}

int AppError(int code)
{
	if(code==KERR_ANY){
		screen->messageBox(app->getErrorMessage());
	}
	return 0;
}

int AppEnd()
{
	delete winMain;
	return 0;
}
