#include "MainWindow.h"
#include "main.h"
#include "src/CDropFiles.h"
#include <StringLib.h>
#include "RecordVisitor.h"
#include "ClearStates.h"
#include <GraphicsLib.h>
#include <KppSound.h>

Font font(L"ＭＳ ゴシック",9);


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// コンストラクタ・デストラクタ
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

MainWindow::MainWindow(const wstring& caption,int x,int y,int w,int h,Window *_parent)
: CustomWindow(caption,x,y,w,h,_parent)
{
	initAnsEdit();
	qnum=0;
	qindex=0;
	qstep=0;
	m_turned = false;
	Window::setFontOnCreate(font);
	//ﾒﾆｭｰ
	MENUDEF menudef[]={
		L"ファイル(&F)",								-1,							KMENU_POPUP,
			L"開く(&O)\tCtrl+O",						IDM_FILE_OPEN,				0,
			L"再読み込み(&R)\tCtrl+R",					IDM_FILE_RELOAD,			0,
			L"再読み込み＆最初から(&R)\tCtrl+T",		IDM_FILE_RELOAD_RESTART,	0,
			L"-",										-1,							0,
			L"終了(&X)\tAlt+F4",						IDM_FILE_QUIT,				KMENU_POPEND,
		L"機能(&F)",									-1,							KMENU_POPUP,
			L"マーキング(&M)\tCtrl+M",					IDM_FUNC_MARKING,			0,
			L"マーキングのみに絞り込む(&F)\tCtrl+F",	IDM_FUNC_FILTER_MARKING,	0,
			L"-",						-1,					0,
			L"初めから(&S)",			IDM_FUNC_START,		0,
			L"-",						-1,					0,
			L"添削(&R)",				IDM_FUNC_ENTER,		0,
			L"ﾊﾟｽ(&P)",					IDM_FUNC_PASS,		0,
			L"解答を見る(&L)",			IDM_FUNC_VIEW,		0,
			L"-",						-1,					0,
			L"問題と解答の反転(&T)",	IDM_FUNC_TURN,		KMENU_POPEND,
		L"音量(&V)",					-1,					KMENU_POPUP,
			L"最小(&L)",				IDM_VOLUME_LOW,		0,
			L"中間(&M)",				IDM_VOLUME_MIDDLE,	0,
			L"最大(&H)",				IDM_VOLUME_HIGH,	KMENU_POPEND,
		L"ﾍﾙﾌﾟ(&H)",					-1,					KMENU_POPUP,
			L"ﾄﾋﾟｯｸの検索(&H)\tF1",		IDM_HELP_TOPIC,		0,
			L"-",						-1,					0,
			L"ﾊﾞｰｼﾞｮﾝ情報(&A)",			IDM_HELP_VER,		KMENU_POPEND,
		//
		NULL,							-1,					KMENU_END
	};
	mnuMain=CreateDefMenu(menudef,false);
	mnuMain->adjustTo(this);
	//ｺﾝﾄﾛｰﾙ
	edtQes=new EditBox(0,0,0,0,this,EDITF_FLAG_READONLY | WINDOWF_NOTABSTOP);
	createAnsEdit(1);
	btnView=new Button(L"解答を見る(&L)",0,0,0,0,this,WINDOWF_NOTABSTOP,IDM_FUNC_VIEW);
	btnPass=new Button(L"パス(&P)",0,0,0,0,this,WINDOWF_NOTABSTOP,IDM_FUNC_PASS);
//	btnEnter=new Button(L"添削(Enter)",0,0,0,0,this,BUTTONF_KIND_DEFAULTBUTTON,IDM_FUNC_ENTER);
	//ｻｳﾝﾄﾞ
	sndOk=new WaveSound();
	sndOk->loadResource(g_app,WAVE_OK);
	sndNg=new WaveSound();
	sndNg->loadResource(g_app,WAVE_NG);
	sndClear=new WaveSound();
	sndClear->loadResource(g_app,WAVE_CLEAR);
	//
	messageNotify(true);
	//
	showNone();
	//
	if(wcscmp(static_cast<Paracka*>(g_app)->fs->GetPath().GetTitle(),L"...")!=0){
		std::vector<std::wstring> paths;
		paths.push_back(static_cast<Paracka*>(g_app)->fs->GetPath().GetFullPath());
		fileLoad(paths);
	}
	dragAcceptFiles(true);
}
MainWindow::~MainWindow()
{
	delete mnuMain;
	delete edtQes;
	delete edtAns;
//	delete btnEnter;
	delete btnView;
	delete sndOk;
	delete sndNg;
	delete sndClear;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// ﾒｯｾｰｼﾞ処理 
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

LRESULT MainWindow::onActivate(UINT msg,WPARAM wParam,LPARAM lParam)
{
	if(wParam!=WA_INACTIVE){
		if(n_edtAns>0){
			edtAns[0]->setFocus();
		}else{
			edtQes->setFocus();
		}
	}
	return 0L;
}


LRESULT MainWindow::onDestroy(UINT msg,WPARAM wParam,LPARAM lParam)
{
	g_app->postQuit();
	return 0L;
}

LRESULT MainWindow::onSize(UINT msg,WPARAM wParam,LPARAM lParam)
{
	CustomWindow::onSize(msg,wParam,lParam);
	//
	int w=getClientWidth();
	int h=getClientHeight();
	int y=getClientBottom();
	y-=2+24;
//	btnEnter->move( w-100- 2,y,100,24);
	btnPass->move(  w-200- 6,y,100,24);
	btnView->move(  w-300-10,y,100,24);
	for(int i=0;i<n_edtAns;i++){
		y-=2+24;
		edtAns[n_edtAns-1-i]->move(         2,y,w-4,24);
	}
	edtQes->move(         2,2,w-4,y-4);
	return 0L;
}

LRESULT MainWindow::onCtlColor(UINT msg,WPARAM wParam,LPARAM lParam)
{
	if((HWND)lParam==edtQes->getHWND()){
		return (LRESULT)GetSysColorBrush(COLOR_WINDOW);
	}else{
		return CustomWindow::onCtlColor(msg,wParam,lParam);
	}
}

LRESULT MainWindow::onMenu(UINT msg,WPARAM wParam,LPARAM lParam)
{
	if(msg==WM_INITMENU){
//		bool b=btnEnter->getEnabled();
//		mnuMain->enableItemByID(IDM_FUNC_ENTER,b);
//		mnuMain->enableItemByID(IDM_FUNC_PASS,b);
//		mnuMain->enableItemByID(IDM_FUNC_VIEW,b);
//		mnuMain->enableItemByID(IDM_FUNC_TURN,b);
		return 0L;
	}else{
		return CustomWindow::onMenu(msg,wParam,lParam);
	}
}


LRESULT MainWindow::onNotify(UINT msg,WPARAM wParam,LPARAM lParam)
{
	int id=wParam;
	NMHDR *nm=(NMHDR*)lParam;
	//
	int i;
	MessageBeep(MB_OK);
	for(i=0;i<n_edtAns;i++){
		if(nm->hwndFrom==edtAns[i]->getHWND() && nm->code==EN_ENTER){
			//sendMenuClick(IDM_FUNC_ENTER);
			if(isGood()){
				//次の問題へ
				rndtable.clear(qindex);
				updateCaption();
				if(rndtable.getCurrentSize()>0){
					sendMenuClick(IDM_FUNC_PASS);
				}else{
					sndClear->play();
					showCongratulation();
				}
			}else if(rndtable.getCurrentSize()>0){
				//解答チェック
				wchar_t *p=edtAns[i]->getTextTemp();
				if(wcscmp(p, m_allList.getTotalAt(qindex).m_a.c_str())==0){
					sndOk->play();
					showGood(i);
				}else{
					sndNg->play();
					showBad();
				}
			}
			return 0L;
		}
	}
	return CustomWindow::onNotify(msg,wParam,lParam);
}

void MainWindow::fileReload(bool restart)
{
	ClearStates states;
	if(!restart){
		states = this->rndtable.getClearStates(m_allList);
	}
	fileLoad(m_paths);

	if(!restart){
		this->rndtable.applyClearStates(m_allList, states);

		if(rndtable.getCurrentSize()>0){
			qindex=rndtable.getNext();
			showQes();
		}else{
			showNone();
		}
	}
}

void MainWindow::fileLoad(const std::vector<std::wstring>& paths)
{
	bool ret=m_allList.loadFile(paths);
	if(ret){
		m_paths = paths;
		
		//乱数ﾘｽﾄの作成
		rndtable.generateTable(m_allList);
		//
		if(rndtable.getCurrentSize()>0){
			qindex=rndtable.getNext();
			showQes();
		}else{
			showNone();
		}
	}else{
		rndtable.clearAll();
		messageBox(L"読み込みに失敗しました",L"エラー");
		m_allList.dispose();
		qindex=0;
		showNone();
		setText(L"Paracka");
	}
	updateCaption();
}


LRESULT MainWindow::onDropFiles(UINT msg,WPARAM wParam,LPARAM lParam)
{
	CDropFiles drop((HDROP)wParam);
	if(drop.size()>0){
		// パス設定 //
		static_cast<Paracka*>(g_app)->fs->GetPath().SetFullPath(drop.GetPath(0));
		// パスリスト構築 //
		std::vector<std::wstring> paths;
		for(int i = 0; i < (int)drop.size(); i++){
			paths.push_back(drop.GetPath(i));
		}
		// ロード //
		//fileLoad(static_cast<Paracka*>(g_app)->fs->GetPath().GetFullPath());
		fileLoad(paths);
	}
	drop.finish();
	return 0L;
}
bool mark_is_all_good();

LRESULT MainWindow::onCommand(UINT msg,WPARAM wParam,LPARAM lParam)
{
	int cmd_id=LOWORD(wParam);
	int cmd_notify=HIWORD(wParam);
	if(cmd_notify==0 || cmd_notify==1)switch(cmd_id){
	case IDM_FILE_OPEN:
		if(static_cast<Paracka*>(g_app)->fs->showOpen(L"開く",this->getHWND())){
			std::vector<std::wstring> paths;
			paths.push_back(static_cast<Paracka*>(g_app)->fs->GetPath().GetFullPath());
			fileLoad(paths);
		}
		break;
	case IDM_FILE_RELOAD:
		fileReload(false);
		break;
	case IDM_FILE_RELOAD_RESTART:
		fileReload(true);
		break;
	case IDM_FILE_QUIT:
		close();
		break;
	case IDM_FUNC_MARKING:
		const_cast<Record&>(m_allList.getTotalAt(qindex)).toggleMarking();
		m_allList.saveFile();
		refreshText();
		break;
	case IDM_FUNC_FILTER_MARKING:
		// フィルタリング切り替え
		m_allList.toggleFilter();

		// 乱数テーブル構築しなおし
		rndtable.generateTable(m_allList);

		// 問題の表示
		if(rndtable.getCurrentSize()>0){
			qindex=rndtable.getNext();
			showQes();
		}else{
			showNone();
		}
		updateCaption();
		break;
	case IDM_FUNC_START:
		//乱数ﾘｽﾄの作成
		rndtable.generateTable(m_allList);
		//
		if(rndtable.getCurrentSize()>0){
			qindex=rndtable.getNext();
			showQes();
		}else{
			showNone();
		}
		updateCaption();
		break;
	case IDM_FUNC_ENTER:
		if(isGood()){
			//次の問題へ
			rndtable.clear(qindex);
			updateCaption();
			if(rndtable.getCurrentSize()>0){
				sendMenuClick(IDM_FUNC_PASS);
			}else{
				sndClear->play();
				showCongratulation();
			}
		}else{
			for(int i=0;i<n_edtAns;i++){
				if(GetFocus()==edtAns[i]->getHWND()){
					//sendMenuClick(IDM_FUNC_ENTER);
					if(rndtable.getCurrentSize()>0){
						//解答チェック
						wchar_t *p=edtAns[i]->getTextTemp();
						bool result = RecordVisitor(m_allList.getTotalAt(qindex), m_turned).checkAnswer(p);
						if(result){
							sndOk->play();
							showGood(i);
							if(!mark_is_all_good()){
								edtAns[(i+1+n_edtAns)%n_edtAns]->setFocus();
							}
							break;
						}else{
							sndNg->play();
							showBad();
						}
					}
					return 0L;
				}
			}
		}
		break;
	case IDM_FUNC_PASS:
		if(rndtable.getCurrentSize()>0){
			qindex=rndtable.getNext();
			showQes();
		}
		break;
	case IDM_FUNC_VIEW:
		if(rndtable.getCurrentSize()>0){
			int f=0;
			//ﾌｫｰｶｽのあるｴﾃﾞｨｯﾄﾎﾞｯｸｽについて適用
			for(int i=0;i<n_edtAns;i++){
				if(GetFocus()==edtAns[i]->getHWND()){
					if(!visibleAns(i)){
						showAns(i);
					}else{
						hideAns(i);
					}
					f=1;
					break;
				}
			}
			//ﾌｫｰｶｽがどれでもない場合は、すべてに適用
			if(f==0){
				for(int i=0;i<n_edtAns;i++){
					if(!visibleAns(i)){
						showAns(i);
					}else{
						hideAns(i);
					}
				}
			}
		}
		break;
	case IDM_FUNC_TURN:
		if(rndtable.getCurrentSize()>0){
			turnQesAns();
		}
		break;
	case IDM_VOLUME_LOW: WaveSound::globalVolume(0.2f); break;
	case IDM_VOLUME_MIDDLE: WaveSound::globalVolume(0.6f); break;
	case IDM_VOLUME_HIGH: WaveSound::globalVolume(1.0f); break;
	case IDM_HELP_TOPIC:
		g_app->shellOpen(L"Paracka.txt",this);
		break;
	case IDM_HELP_VER:
		g_app->showVersion(this);
		break;
	}
	return 0L;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// Qes処理 
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

int mark[1] = {0};
bool mark_is_all_good()
{
	for(int i=0;i<1;i++)if(mark[i]!=1)return false;
	return true;
}

void MainWindow::showGood(int a_index)
{
	qstep=2;
	edtQes->setTextF(RecordVisitor(m_allList.getTotalAt(qindex), m_turned).getGoodText().c_str());
	setButtonEnabled(true);
}

void MainWindow::showBad()
{
	qstep=3;
	edtQes->setTextF(RecordVisitor(m_allList.getTotalAt(qindex), m_turned).getBadText().c_str());
	setButtonEnabled(true);
}

void MainWindow::showQes(bool reset_ans)
{
	qstep=0;
	edtQes->setText(RecordVisitor(m_allList.getTotalAt(qindex), m_turned).getQuestionText());
	//
	createAnsEdit(m_allList.getTotalAt(qindex).m_a.size());
	if(reset_ans){ for(int i=0;i<n_edtAns;i++)edtAns[i]->setText(L""); }
//	btnEnter->setText(L"添削(Enter)");	mnuMain->setItemTextByID(IDM_FUNC_ENTER,L"添削(&R)");
//	btnView->setText(L"答えを見る(&L)");	mnuMain->setItemTextByID(IDM_FUNC_VIEW,L"答えを見る(&L)");
	setButtonEnabled(true);
	edtAns[0]->setFocus();
	mark[0] = 0;

	updateCaption();
}


void MainWindow::showAns(int a_index)
{
	qstep=1;
	edtQes->setTextF(RecordVisitor(m_allList.getTotalAt(qindex), m_turned).getQuestionWithAnswerText().c_str());
}

void MainWindow::showCongratulation()
{
	qstep=4;
	edtQes->setText(L"\r\n ++ ++ 全問クリア！！ ++ ++");
	for(int i=0;i<n_edtAns;i++)edtAns[i]->setText(L"");
}

void MainWindow::hideAns(int a_index)
{
	showQes(false);
}

void MainWindow::refreshText()
{
	if(qstep==0 || qstep==3){
		showQes();
	}else if(qstep==1){
		showAns(0); //####引数0は仮
	}else if(qstep==2){
		showGood(0); //####引数0は仮
		edtAns[0]->setText(RecordVisitor(m_allList.getTotalAt(qindex), m_turned).getA());
	}else if(qstep==4){
		showCongratulation();
	}else if(qstep==-1){
		;
	}
}

void MainWindow::turnQesAns()
{
	m_turned = !m_turned;
	refreshText();
}

bool MainWindow::isGood()
{
	return qstep==2;
}

bool MainWindow::visibleAns(int a_index)
{
	if(m_allList.getTotalAt(qindex).isNormal()){
		return qstep==1;
	}else{
		return mark[a_index]!=0;
	}
}

void MainWindow::setButtonEnabled(bool b)
{
//	btnEnter->setEnable(b);
	btnPass->setEnable(b);
	btnView->setEnable(b);
}

void MainWindow::showNone()
{
	qstep=-1;
	edtQes->setText(L"\r\n ++ 問題がありません ++");
	for(int i=0;i<n_edtAns;i++)edtAns[i]->setText(L"");
	setButtonEnabled(false);
}

void MainWindow::updateCaption()
{
	// 名前の決定
	std::wstring name = L"";
	if(m_paths.size() == 0){
		name = L"無題";
	}
	else if(m_paths.size() == 1){
		CFilePath pathinfo = m_paths[0];
		name = pathinfo.GetTitle();
	}
	else{
		name = L"複数";
		if(qindex >= 0 && qindex < m_allList.getTotalCount()){
			name += L" (" + m_allList.getTotalAt(qindex).getFileName() + L")";
		}
	}

	// フィルタリングモード
	mystring mode = m_allList.getFilterLevelString();

	// キャプション設定
	setTextF(L"%ls (残り %02d/%02d) (%ls) - Paracka",
		name.c_str(),
		rndtable.getCurrentSize(),
		rndtable.getFirstSize(),
		mode.c_str()
	);
}
