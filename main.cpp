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
	IDM_FILE_OPEN,		'O',	ACCELF_CONTROL,
	IDM_FILE_RELOAD,	'R',	ACCELF_CONTROL,
	-1,					-1,		0,
};

int AppRoutine()
{
#if 0
	return app->windowsLoop();
#else
	Accelerator *acc=new Accelerator();
	acc->setEntries(accdef);
	int ret=app->dialogLoop(winMain,acc);
	delete acc;
	return ret;
#endif
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
