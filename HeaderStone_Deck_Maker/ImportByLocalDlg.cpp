// ImportByLocalDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "HeaderStone_Deck_Maker.h"
#include "ImportByLocalDlg.h"
#include "afxdialogex.h"

#include "CardListMgr.h"
#include "ManaCurveDlg.h"

// CImportByLocalDlg 대화 상자

IMPLEMENT_DYNAMIC(CImportByLocalDlg, CDialogEx)

CImportByLocalDlg::CImportByLocalDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IMPORT_BY_LOCAL_DLG, pParent), CCardNotifier(NTM_IMPORTLOCALDLG), m_pManaCurveDlg(nullptr)
{

}

CImportByLocalDlg::~CImportByLocalDlg()
{
}

BOOL CImportByLocalDlg::LoadLocalDeck()
{
	if (this->GetSafeHwnd() == NULL)
		return FALSE;

	for (auto& itr : m_mapLocalDeck)
	{
		if (itr.second != NULL)
			delete itr.second;
	}
	m_mapLocalDeck.clear();
	CDeckListMgr::Import2Json(m_mapLocalDeck);
	InitClassCmb();
	return TRUE;
}

void CImportByLocalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DECKCLASS_CMB, m_ctrlClassCmb);
	DDX_Control(pDX, IDC_DECKTYPE_CMB, m_ctrlDeckCmb);
}


BEGIN_MESSAGE_MAP(CImportByLocalDlg, CDialogEx)
	ON_BN_CLICKED(IDC_MANACURVE_BTN, OnClickManaCurve)
	ON_CBN_SELCHANGE(IDC_DECKCLASS_CMB, OnSelChangeDeckClass)
	ON_CBN_SELCHANGE(IDC_DECKTYPE_CMB, OnSelChangeDeckType)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CImportByLocalDlg 메시지 처리기


void CImportByLocalDlg::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	int nIdx = m_ctrlDeckCmb.GetCurSel();
	if ( nIdx != -1 )
	{
		using namespace LocalDeckMsg;
		LocalDeckData* pLocalDeck = (LocalDeckData*)m_ctrlDeckCmb.GetItemDataPtr(nIdx);
		//SendNotify((WPARAM)pLocalDeck->eClass, (LPARAM)&pLocalDeck->mapCardList);
		SendNotify(MSG_SETLOCALDECKMODIFY, (LPARAM)&pLocalDeck->nKey);
		SendNotify(MSG_SETLOCALDECKSTRING, (LPARAM)&pLocalDeck->strName);
		SendNotify(MSG_SETLOCALDECKCLASS, (LPARAM)&pLocalDeck->eClass);
		SendNotify(MSG_SETLOCALDECKLIST, (LPARAM)&pLocalDeck->mapCardList);
	}

	for (auto& itr : m_mapLocalDeck)
	{
		if (itr.second != NULL)
			delete itr.second;
	}
	m_mapLocalDeck.clear();
	DestroyWindow();
	//CDialogEx::OnOK();
}


void CImportByLocalDlg::OnCancel()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	for (auto& itr : m_mapLocalDeck)
	{
		if (itr.second != NULL)
			delete itr.second;
	}
	m_mapLocalDeck.clear();
	DestroyWindow();
	//CDialogEx::OnCancel();
}

BOOL CImportByLocalDlg::ExecuteNotify(NOTIFYMSG eSender, WPARAM wParam, LPARAM lParam)
{
	//들어올꺼 없음
	return TRUE;
}

void CImportByLocalDlg::InitClassCmb()
{
	CCardListMgr* pCardListMgr = CCardListMgr::GetInstance();
	m_ctrlClassCmb.ResetContent();
	for (int i = 2; i <= E_CARDCLASS_PREIST; i <<= 1)
	{
		CString strClass = pCardListMgr->GetClassStringByEnum((E_CARDCLASS)i);

		auto itrMap = std::find_if(m_mapLocalDeck.begin(), m_mapLocalDeck.end(), 
		[&i](std::pair<int, LocalDeckData*> pLocalDeck)
		{
			return pLocalDeck.second->eClass == i;
		});

		if ( itrMap == m_mapLocalDeck.end() )
			continue;

		int nIdx = m_ctrlClassCmb.AddString(strClass);
		m_ctrlClassCmb.SetItemData(nIdx, i);
	}

	m_ctrlClassCmb.SetCurSel(0);
	OnSelChangeDeckClass();
}

void CImportByLocalDlg::OnClickManaCurve()
{
	CPoint pt;
	GetCursorPos(&pt);

	if (m_pManaCurveDlg == NULL)
		m_pManaCurveDlg = new CManaCurveDlg(this);

	if (m_pManaCurveDlg->GetSafeHwnd() == NULL)
	{
		CString strDeckName;
		std::map<CCard*, int> mapDeckList;
		int nIdx = m_ctrlDeckCmb.GetCurSel();
		if (nIdx != -1)
			mapDeckList = ((LocalDeckData*)m_ctrlDeckCmb.GetItemDataPtr(nIdx))->mapCardList;
		else
			return;

		m_pManaCurveDlg->Create(IDD_MANACURVE_DLG);
		CRect rtDlg;
		m_pManaCurveDlg->GetClientRect(rtDlg);
		m_pManaCurveDlg->SetWindowPos(&wndTop, pt.x - rtDlg.Width(), pt.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

		m_pManaCurveDlg->SetDeckData(mapDeckList);
		m_pManaCurveDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pManaCurveDlg->SetActiveWindow();
	}
}

void CImportByLocalDlg::OnSelChangeDeckClass()
{
	UpdateData();
	E_CARDCLASS eCardClass = (E_CARDCLASS)m_ctrlClassCmb.GetItemData(m_ctrlClassCmb.GetCurSel());

	m_ctrlDeckCmb.ResetContent();
	for (auto& itr : m_mapLocalDeck)
	{
		LocalDeckData* pDeckData = itr.second;

		if (pDeckData->eClass == eCardClass)
		{
			int nIdx = m_ctrlDeckCmb.AddString(pDeckData->strName);
			m_ctrlDeckCmb.SetItemDataPtr(nIdx, pDeckData);
		}
	}
	m_ctrlDeckCmb.SetCurSel(0);
	OnSelChangeDeckType();
}

void CImportByLocalDlg::OnSelChangeDeckType()
{
	//SendNotify((WPARAM))
}


int CImportByLocalDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	return 0;
}


BOOL CImportByLocalDlg::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	InitClassCmb();
	return TRUE;  // return TRUE unless you set the focus to a control
}
