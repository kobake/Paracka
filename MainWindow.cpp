#include "main.h"
#include "src/CDropFiles.h"
#include <StringLib.h>

Font font(L"ＭＳ ゴシック",9);


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// コンストラクタ・デストラクタ
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

MainWindow::MainWindow(const wstring& caption,int x,int y,int w,int h,Window *_parent)
: CustomWindow(caption,x,y,w,h,_parent)
, qes()
{
	initAnsEdit();
	qnum=0;
	qindex=0;
	qstep=0;
	Window::setFontOnCreate(font);
	//ﾒﾆｭｰ
	MENUDEF menudef[]={
		L"ファイル(&F)",			-1,					KMENU_POPUP,
			L"開く(&O)\tCtrl+O",	IDM_FILE_OPEN,		0,
			L"再読み込み(&R)",		IDM_FILE_RELOAD,	0,
			L"-",					-1,					0,
			L"終了(&X)\tAlt+F4",	IDM_FILE_QUIT,		KMENU_POPEND,
		L"機能(&F)",				-1,					KMENU_POPUP,
			L"初めから(&S)",		IDM_FUNC_START,		0,
			L"-",					-1,					0,
			L"添削(&R)",			IDM_FUNC_ENTER,		0,
			L"ﾊﾟｽ(&P)",				IDM_FUNC_PASS,		0,
			L"解答を見る(&L)",		IDM_FUNC_VIEW,		0,
			L"-",					-1,					0,
			L"問題と解答の反転(&T)",IDM_FUNC_TURN,		KMENU_POPEND,
		L"ﾍﾙﾌﾟ(&H)",				-1,					KMENU_POPUP,
			L"ﾄﾋﾟｯｸの検索(&H)\tF1",	IDM_HELP_TOPIC,		0,
			L"-",					-1,					0,
			L"ﾊﾞｰｼﾞｮﾝ情報(&A)",		IDM_HELP_VER,		KMENU_POPEND,
		//
		NULL,						-1,					KMENU_END
	};
	mnuMain=CreateDefMenu(menudef,false);
	mnuMain->adjustTo(this);
	//ｺﾝﾄﾛｰﾙ
	edtQes=new EditBox(0,0,0,0,this,EDITF_FLAG_READONLY | WINDOWF_NOTABSTOP);
	createAnsEdit(1);//edtAns=new EditField(0,0,0,0,this);
	btnView=new Button(L"解答を見る(&L)",0,0,0,0,this,WINDOWF_NOTABSTOP,IDM_FUNC_VIEW);
	btnPass=new Button(L"パス(&P)",0,0,0,0,this,WINDOWF_NOTABSTOP,IDM_FUNC_PASS);
//	btnEnter=new Button(L"添削(Enter)",0,0,0,0,this,BUTTONF_KIND_DEFAULTBUTTON,IDM_FUNC_ENTER);
	//ｻｳﾝﾄﾞ
	sndOk=new WaveSound();
	sndOk->loadResource(app,WAVE_OK);
	sndNg=new WaveSound();
	sndNg->loadResource(app,WAVE_NG);
	sndClear=new WaveSound();
	sndClear->loadResource(app,WAVE_CLEAR);
	//
	messageNotify(true);
	//
	showNone();
	//
	if(wcscmp(app->fs->GetPath().GetTitle(),L"...")!=0){
		fileLoad(app->fs->GetPath().GetFullPath());
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
	app->postQuit();
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
				if(rndtable.getSize()==1){
					int n;
					n=0;
				}
				rndtable.clear(qindex);
				updateCaption();
				if(rndtable.getSize()>0){
					sendMenuClick(IDM_FUNC_PASS);
				}else{
					sndClear->play();
					showCongratulation();
				}
			}else if(rndtable.getSize()>0){
				//解答チェック
				wchar_t *p=edtAns[i]->getTextTemp();
				if(wcscmp(p, qes.getAns(qindex,i).c_str())==0){
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

void MainWindow::fileReload(const wstring& fpath)
{
	bool ret=qes.loadFile(fpath.c_str());
	if(ret){
		//乱数ﾘｽﾄの更新
		if(rndtable.getSize()>qes.getAllNum()){
			int from=qes.getAllNum();
			int to=rndtable.getSize()-1;
			for(int i=from;i<=to;i++)
				rndtable.clear(i);
		}
		//
		if(rndtable.getSize()>0){
			if(!rndtable.exists(qindex)){
				qindex=rndtable.getNext();
			}
			showQes();
		}else{
			showNone();
		}
	}else{
		rndtable.clearAll();
		messageBox(L"読み込みに失敗しました",L"エラー");
		qes.dispose();
		qindex=0;
		showNone();
		setText(L"Paracka");
	}
	updateCaption();
}

void MainWindow::fileLoad(const wstring& fpath)
{
	bool ret=qes.loadFile(fpath.c_str());
	if(ret){
		//乱数ﾘｽﾄの作成
		rndtable.create(qes.getAllNum());
		//
		if(rndtable.getSize()>0){
			qindex=rndtable.getNext();
			showQes();
		}else{
			showNone();
		}
	}else{
		rndtable.clearAll();
		messageBox(L"読み込みに失敗しました",L"エラー");
		qes.dispose();
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
		app->fs->GetPath().SetFullPath(drop.GetPath(0));
		fileLoad(app->fs->GetPath().GetFullPath());
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
		if(app->fs->showOpen(L"開く",this->getHWND())){
			fileLoad(app->fs->GetPath().GetFullPath());
		}
		break;
	case IDM_FILE_RELOAD:
		fileReload(app->fs->GetPath().GetFullPath());
		break;
	case IDM_FILE_QUIT:
		close();
		break;
	case IDM_FUNC_START:
		//乱数ﾘｽﾄの作成
		rndtable.create(qes.getAllNum());
		//
		if(rndtable.getSize()>0){
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
			if(rndtable.getSize()==1){
				int n;
				n=0;
			}
			rndtable.clear(qindex);
			updateCaption();
			if(rndtable.getSize()>0){
				sendMenuClick(IDM_FUNC_PASS);
			}else{
				sndClear->play();
				showCongratulation();
			}
		}else{
			for(int i=0;i<n_edtAns;i++){
				if(GetFocus()==edtAns[i]->getHWND()){
					//sendMenuClick(IDM_FUNC_ENTER);
					if(rndtable.getSize()>0){
						//解答チェック
						wchar_t *p=edtAns[i]->getTextTemp();
						if(wcscmp(p, qes.getAns(qindex,i).c_str())==0){
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
		if(rndtable.getSize()>0){
			qindex=rndtable.getNext();
			showQes();
		}
		break;
	case IDM_FUNC_VIEW:
		if(rndtable.getSize()>0){
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
		if(rndtable.getSize()>0){
			turnQesAns();
		}
		break;
	case IDM_HELP_TOPIC:
		app->shellOpen(L"Paracka.txt",this);
		break;
	case IDM_HELP_VER:
		app->showVersion(this);
		break;
	}
	return 0L;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// Qes処理 
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

int *mark=NULL; int nmark=0;
bool mark_is_all_good()
{
	for(int i=0;i<nmark;i++)if(mark[i]!=1)return false;
	return true;
}

void MainWindow::showGood(int a_index)
{
	if(qes.getQesKind(qindex)==0){
		qstep=2;
		edtQes->setTextF(L"%ls\r\n\r\n ++ ++ Good !! ++ ++", qes.getQes(qindex).c_str());
	}else{
		mark[a_index]=1;
		if(mark_is_all_good()){
			qstep=2;
		}
		edtQes->setTextF(L"%ls\r\n\r\n ++ ++ Good !! ++ ++",getMarkString());
	}
	setButtonEnabled(true);
}

void MainWindow::showBad()
{
	qstep=3;
	if(qes.getQesKind(qindex)==0){
		edtQes->setTextF(L"%ls\r\n\r\n ++ ++ Bad ... ++ ++",qes.getQes(qindex).c_str());
	}else{
		edtQes->setTextF(L"%ls\r\n\r\n ++ ++ Bad ... ++ ++",getMarkString());
	}
	setButtonEnabled(true);
}

const wchar_t *MainWindow::getMarkString()
{
	static wchar_t tmp[1024];
	wcscpy(tmp,qes.getQes(qindex).c_str());
	for(int i=0;i<qes.getAnsNum(qindex);i++){
		if(mark[i]==-1){
			wcsreplace_once(tmp,L"(    )",tmp_swprintf(L"( %s )",qes.getAns(qindex,i)));
		}else if(mark[i]==1){
			wcsreplace_once(tmp,L"(    )",tmp_swprintf(L"( %s！ )",qes.getAns(qindex,i)));
		}else{
			wcsreplace_once(tmp,L"(    )",L"()");
		}
	}
	wcsreplace(tmp,L"()",L"(    )");
	return tmp;
}

void MainWindow::showQes(bool reset_ans)
{
	qstep=0;
	edtQes->setText(qes.getQes(qindex));
	//
	createAnsEdit(qes.getAnsNum(qindex));
	if(reset_ans){ for(int i=0;i<n_edtAns;i++)edtAns[i]->setText(L""); }
//	btnEnter->setText(L"添削(Enter)");	mnuMain->setItemTextByID(IDM_FUNC_ENTER,L"添削(&R)");
//	btnView->setText(L"答えを見る(&L)");	mnuMain->setItemTextByID(IDM_FUNC_VIEW,L"答えを見る(&L)");
	setButtonEnabled(true);
	edtAns[0]->setFocus();
	nmark=qes.getAnsNum(qindex);
	mark=(int*)realloc(mark,sizeof(int)*nmark);
	memset(mark,0,sizeof(int)*nmark);
}


void MainWindow::showAns(int a_index)
{
	qstep=1;
	if(qes.getQesKind(qindex)==0){
		edtQes->setTextF(L"%ls\r\n\r\n%ls", qes.getQes(qindex).c_str(),qes.getAns(qindex,0).c_str());
	}else{
		mark[a_index]=-1;
		//
		edtQes->setText(getMarkString());
	}
//	btnEnter->setText(L"添削(Enter)");	mnuMain->setItemTextByID(IDM_FUNC_ENTER,L"添削(&R)");
//	btnView->setText(L"答えを隠す(&L)");	mnuMain->setItemTextByID(IDM_FUNC_VIEW,L"答えを隠す(&L)");
//	setButtonEnabled(true);
}

void MainWindow::showCongratulation()
{
	qstep=4;
	edtQes->setText(L"\r\n ++ ++ 全問クリア！！ ++ ++");
	for(int i=0;i<n_edtAns;i++)edtAns[i]->setText(L"");
//	btnEnter->setText(L"添削(Enter)");	mnuMain->setItemTextByID(IDM_FUNC_ENTER,L"添削(&R)");
//	btnView->setText(L"答えを見る(&L)");	mnuMain->setItemTextByID(IDM_FUNC_VIEW,L"答えを見る(&L)");
//	setButtonEnabled(false);
}

void MainWindow::hideAns(int a_index)
{
	if(qes.getQesKind(qindex)==0){
		showQes(false);
	}else{
		mark[a_index]=0;
		edtQes->setText(getMarkString());
	}
}


void MainWindow::turnQesAns()
{
	//#####
	qes.turn();
	if(qstep==0 || qstep==3){
		showQes();
	}else if(qstep==1){
		showAns(0); //####引数0は仮
	}else if(qstep==2){
		showGood(0); //####引数0は仮
		edtAns[0]->setText(qes.getAns(qindex,0)); //####引数0は仮
	}else if(qstep==4){
		showCongratulation();
	}else if(qstep==-1){
		;
	}
}

bool MainWindow::isGood()
{
	return qstep==2;
}

bool MainWindow::visibleAns(int a_index)
{
	if(qes.getQesKind(qindex)==0){
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
//	btnEnter->setText(L"添削(Enter)");	mnuMain->setItemTextByID(IDM_FUNC_ENTER,L"添削(&R)");
//	btnView->setText(L"答えを見る(&L)");	mnuMain->setItemTextByID(IDM_FUNC_VIEW,L"答えを見る(&L)");
	setButtonEnabled(false);
}

void MainWindow::updateCaption()
{
	if(qes.getAllNum()){
		int n;
		n=0;
	}
	setTextF(L"%ls (残り %02d/%02d) - Paracka",
		app->fs->GetPath().GetTitle(),
		rndtable.getSize(),
		qes.getAllNum()
	);
}
