// ImportDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "HeaderStone_Deck_Maker.h"
#include "ExportDlg.h"

#include "CardListMgr.h"

#include "afxdialogex.h"


// CExportDlg 대화 상자

IMPLEMENT_DYNAMIC(CExportDlg, CDialogEx)

CExportDlg::CExportDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DECKCODE_DLG, pParent)
{
}

CExportDlg::~CExportDlg()
{
}

void CExportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DECKCODE_EDT, m_ctrlDeckCode);
}


BEGIN_MESSAGE_MAP(CExportDlg, CDialogEx)
END_MESSAGE_MAP()


// CExportDlg 메시지 처리기


void CExportDlg::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CDialogEx::PostNcDestroy();
}


void CExportDlg::SetDeckCode(CString strDeckCode)
{
	m_ctrlDeckCode.SetWindowText(strDeckCode);
}

void CExportDlg::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	DestroyWindow();
}


void CExportDlg::OnCancel()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	DestroyWindow();
	//CDialogEx::OnCancel();
}


BOOL CExportDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
	m_ctrlDeckCode.SetReadOnly();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
