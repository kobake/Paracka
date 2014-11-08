class MainWindow;

#ifndef _MainWindow_
#define _MainWindow_

#include "c_CustomWindow.h"

#include "QesData.h"
#include "RndTable.h"
class WaveSound;

class MainWindow : public CustomWindow{
protected:
	//�ƭ�
	Menu *mnuMain;
	//���۰�
	EditBox *edtQes;
	EditField **edtAns; int n_edtAns;
//	Button *btnEnter;
	Button *btnPass;
	Button *btnView;
	//�����
	WaveSound *sndOk;
	WaveSound *sndNg;
	WaveSound *sndClear;
	//
	QesData qes;
	RndTable rndtable;
	int qnum;
	int qindex;
	int qstep;
public:
	void initAnsEdit()
	{
		edtAns=NULL;
		n_edtAns=0;
	}
	void destroyAnsEdit()
	{
		for(int i=0;i<n_edtAns;i++){
			delete edtAns[i];
		}
		m_free(edtAns);
		n_edtAns=0;
	}
	void createAnsEdit(int n)
	{
		//�]�v�ȕ�������
		int n_old=n_edtAns;
		int n_new=n;
		int i;
		for(i=n_new;i<n_old;i++){
			delete edtAns[i];
			edtAns[i]=NULL;
		}
		//ؽĂ��m�ۂ�����
		edtAns=(EditField**)realloc(edtAns,sizeof(EditField*)*n_new);
		//�V�����������
		for(i=n_old;i<n_new;i++){
			edtAns[i]=new EditField(0,0,0,0,this);
		}
		n_edtAns=n_new;
		//�X�V
		postSize();
/*

		//

		destroyAnsEdit();
		//
		n_edtAns=n;
		//
		if(n_edtAns>0){
			edtAns=(EditField**)malloc(sizeof(EditField*)*n_edtAns);
			for(int i=0;i<n_edtAns;i++){
				edtAns[i]=new EditField(0,0,0,0,this);
			}
		}
		//
		postSize();
		*/
	}
public:
	MainWindow(const wstring& caption,int x,int y,int w,int h,Window *_parent);
	virtual ~MainWindow();
	LRESULT onDestroy(UINT msg,WPARAM wParam,LPARAM lParam);
	LRESULT onSize(UINT msg,WPARAM wParam,LPARAM lParam);
	LRESULT onCommand(UINT msg,WPARAM wParam,LPARAM lParam);
	LRESULT onNotify(UINT msg,WPARAM wParam,LPARAM lParam);
	LRESULT onCtlColor(UINT msg,WPARAM wParam,LPARAM lParam);
	LRESULT onMenu(UINT msg,WPARAM wParam,LPARAM lParam);
	LRESULT onDropFiles(UINT msg,WPARAM wParam,LPARAM lParam);
	LRESULT onActivate(UINT msg,WPARAM wParam,LPARAM lParam);
	//
	void showGood(int a_index);
	void showBad();
	void showQes(bool reset_ans=true);
	void showAns(int a_index);
	void hideAns(int a_index);
	void showCongratulation();
	void turnQesAns();
	bool isGood();
	bool visibleAns(int a_index);
	void setButtonEnabled(bool b);
	void showNone();
	//
	void updateCaption();
	void fileLoad(const wstring& fpath);
	void fileReload(const wstring& fpath);
	//
	const wchar_t *MainWindow::getMarkString();
};

#include "c_WaveSound.h"

#endif
