// ImportDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "HeaderStone_Deck_Maker.h"
#include "ImportDlg.h"

#include "DeckListCtrl.h"
#include "afxdialogex.h"

#include "CardListMgr.h"
// CImportDlg 대화 상자

IMPLEMENT_DYNAMIC(CImportDlg, CDialogEx)

CImportDlg::CImportDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DECKCODE_DLG, pParent), CCardNotifier(NTM_IMPORTDLG)
{
}

CImportDlg::~CImportDlg()
{
}

BOOL CImportDlg::ExecuteNotify(NOTIFYMSG eSender, WPARAM wParam, LPARAM lParam)
{
	// 필요 없음.
	return FALSE;
}

void CImportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DECKCODE_EDT, m_ctrlDeckCode);
}


BEGIN_MESSAGE_MAP(CImportDlg, CDialogEx)
END_MESSAGE_MAP()


// CImportDlg 메시지 처리기


void CImportDlg::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CDialogEx::PostNcDestroy();
}


void CImportDlg::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CString strDeckCodeTemp;
	m_ctrlDeckCode.GetWindowText(strDeckCodeTemp);
	if (strDeckCodeTemp.GetLength() >= 500)
		VERIFY(0);

	if (strDeckCodeTemp == _T(""))
	{
		ASSERT(0);
		return;
	}

	CCardListMgr* pListMgr = CCardListMgr::GetInstance();
	E_CARDCLASS eDeckClass = E_CARDCLASS_NONE;
	std::map<CCard*, int> mapDeckList;
	std::wstring tcharTemp(strDeckCodeTemp);
	std::string strDeckCode;
	strDeckCode.assign(tcharTemp.begin(), tcharTemp.end());

	pListMgr->DeckCode2CardList(strDeckCode, eDeckClass, mapDeckList);
	SendNotify((WPARAM)eDeckClass, (LPARAM)&mapDeckList);
	DestroyWindow();
}


void CImportDlg::OnCancel()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	DestroyWindow();
	//CDialogEx::OnCancel();
}
