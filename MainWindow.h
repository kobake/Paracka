class MainWindow;

#ifndef _MainWindow_
#define _MainWindow_

#include "c_CustomWindow.h"

#include "RecordList.h"
#include "RndTable.h"
class WaveSound;

class MainWindow : public CustomWindow{
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
		n = 1;
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
	void fileLoad(const std::vector<std::wstring>& paths);
	void fileReload();
	//
	const wchar_t *MainWindow::getMarkString();
private:
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

	//�f�[�^
	std::vector<std::wstring>	m_paths;

	RecordList					m_allList; // �R�����g���܂ޑS���R�[�h

	RndTable rndtable;
	int qnum;
	int qindex;
	int qstep;
};

#include "c_WaveSound.h"

#endif
