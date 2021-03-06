// ImportByMetaDeck.cpp: 구현 파일
//

#include "stdafx.h"
#include "HeaderStone_Deck_Maker.h"
#include "ImportByMetaDeck.h"
#include "afxdialogex.h"

#include "CardListMgr.h"
#include "MetaDeckData.h"

// CImportByMetaDeck 대화 상자

IMPLEMENT_DYNAMIC(CImportByMetaDeck, CDialogEx)

CImportByMetaDeck::CImportByMetaDeck(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IMPORT_BY_METADECK_DLG, pParent), CCardNotifier(NTM_IMPORTMETADECKDLG)
{
	CCardListMgr* pListMgr = CCardListMgr::GetInstance();
	m_mapMetaData = pListMgr->GetMetaDeckList();
}

CImportByMetaDeck::~CImportByMetaDeck()
{
}

BOOL CImportByMetaDeck::ExecuteNotify(NOTIFYMSG eSender, WPARAM wParam, LPARAM lParam)
{
	//안씀
	return FALSE;
}

void CImportByMetaDeck::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DECK_AVG_LENGTH_EDT, m_ctrlAvgTime);
	DDX_Control(pDX, IDC_DECK_AVG_TURN_EDT, m_ctrlAvgTurn);
	DDX_Control(pDX, IDC_DECK_TOTAL_GAMES_EDT, m_ctrlTotalGame);
	DDX_Control(pDX, IDC_DECK_WIN_RATE_EDT, m_ctrlWinRate);
	DDX_Control(pDX, IDC_DECKCLASS_CMB, m_ctrlDeckClass);
	DDX_Control(pDX, IDC_DECKTYPE_CMB, m_ctrlDeckType);
}


BEGIN_MESSAGE_MAP(CImportByMetaDeck, CDialogEx)
	ON_CBN_SELCHANGE(IDC_DECKCLASS_CMB, &CImportByMetaDeck::OnCbnSelchangeDeckclassCmb)
	ON_CBN_SELCHANGE(IDC_DECKTYPE_CMB, &CImportByMetaDeck::OnCbnSelchangeDecktypeCmb)
END_MESSAGE_MAP()


// CImportByMetaDeck 메시지 처리기


void CImportByMetaDeck::OnCbnSelchangeDeckclassCmb()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();
	E_CARDCLASS eCardClass = (E_CARDCLASS)m_ctrlDeckClass.GetItemData(m_ctrlDeckClass.GetCurSel());
	
	if (m_mapMetaData.find(eCardClass) == m_mapMetaData.end())
	{
		ASSERT(0);
		return;
	}

	m_ctrlDeckType.ResetContent();
	std::vector<CMetaDeckData*> vecMetaDeck = m_mapMetaData[eCardClass];
	std::sort( vecMetaDeck.begin(), vecMetaDeck.end(), 
		[](CMetaDeckData* pData1, CMetaDeckData* pData2)
	{
		return pData1->m_dWinRate > pData2->m_dWinRate;
	});

	for (CMetaDeckData* pMetaDeck : vecMetaDeck)
	{
		int nIdx = m_ctrlDeckType.AddString(pMetaDeck->m_strArcheType.c_str());
		m_ctrlDeckType.SetItemDataPtr(nIdx, pMetaDeck);
	}
	m_ctrlDeckType.SetCurSel(0);
	OnCbnSelchangeDecktypeCmb();
}


void CImportByMetaDeck::OnCbnSelchangeDecktypeCmb()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();
	CMetaDeckData* pMetaDeck = (CMetaDeckData*)m_ctrlDeckType.GetItemData(m_ctrlDeckType.GetCurSel());

	if (pMetaDeck == NULL)
		return;

	CString strTemp;

	strTemp.Format(_T("%.2lf"), pMetaDeck->m_dAvgGameLength / 60.0);
	m_ctrlAvgTime.SetWindowText(strTemp);

	strTemp.Format(_T("%.2lf"), pMetaDeck->m_dAvgPlayerTurn);
	m_ctrlAvgTurn.SetWindowText(strTemp);

	strTemp.Format(_T("%d"), pMetaDeck->m_nTotalGames);
	m_ctrlTotalGame.SetWindowText(strTemp);

	strTemp.Format(_T("%.2lf"), pMetaDeck->m_dWinRate);
	m_ctrlWinRate.SetWindowText(strTemp);
}


BOOL CImportByMetaDeck::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CCardListMgr* pCardListMgr = CCardListMgr::GetInstance();
	for (int i = 2; i <= E_CARDCLASS_PREIST; i <<= 1)
	{
		CString strClass = pCardListMgr->GetClassStringByEnum((E_CARDCLASS)i);
		int nIdx = m_ctrlDeckClass.AddString(strClass);
		m_ctrlDeckClass.SetItemData(nIdx, i);
	}

	m_ctrlDeckClass.SetCurSel(0);
	OnCbnSelchangeDeckclassCmb();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CImportByMetaDeck::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	UpdateData();
	CMetaDeckData* pMetaDeck = (CMetaDeckData*)m_ctrlDeckType.GetItemData(m_ctrlDeckType.GetCurSel());
	CString strDeckName(pMetaDeck->m_strArcheType.c_str());
	SendNotify((WPARAM)MetaDeckMsg::MSG_SETMETADECKSTRING, (LPARAM)&strDeckName);
	SendNotify((WPARAM)MetaDeckMsg::MSG_SETMETADECKLIST, (LPARAM)&pMetaDeck->m_mapDeckList);
	SendNotify((WPARAM)MetaDeckMsg::MSG_SETMETADECKCLASS, (LPARAM)&pMetaDeck->m_eDeckClass);
	DestroyWindow();
}


void CImportByMetaDeck::OnCancel()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	DestroyWindow();
}
