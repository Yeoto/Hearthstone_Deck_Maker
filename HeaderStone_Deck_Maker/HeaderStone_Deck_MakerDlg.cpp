
// HeaderStone_Deck_MakerDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "HeaderStone_Deck_Maker.h"
#include "HeaderStone_Deck_MakerDlg.h"
#include "afxdialogex.h"

#include "MCurl.h"
#include "CardData.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CHeaderStoneDeckMakerDlg 대화 상자

#define DECK_LIST_OFFSET 300

CHeaderStoneDeckMakerDlg::CHeaderStoneDeckMakerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HEADERSTONE_DECK_MAKER_DIALOG, pParent), CCardNotifier(NTM_MAINDLG), m_nCurCard(0), m_bInit(FALSE), m_nClassCardCnt(0), m_eDeckClass(E_CARDCLASS_NONE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHeaderStoneDeckMakerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CARD_PAGE, m_ctrlFrameCard);
	DDX_Control(pDX, IDC_DECK_LIST, m_ctrlFrameDeck);
}

BEGIN_MESSAGE_MAP(CHeaderStoneDeckMakerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CLASS_DRUID_RDO, IDC_CLASS_FREE_RDO, OnClickRdoBtn)
END_MESSAGE_MAP()


// CHeaderStoneDeckMakerDlg 메시지 처리기

BOOL CHeaderStoneDeckMakerDlg::ExecuteNotify(NOTIFYMSG eSender, WPARAM wParam, LPARAM lParam)
{
	CCard* pCard = (CCard*)lParam;
	switch (eSender)
	{
	case NTM_CARDCTRL:
	{
		if (pCard->eClass != E_CARDCLASS_NONE)
			m_nClassCardCnt++;
		break;
	}
	case NTM_DECKLISTCTRL:
	{
		if (pCard->eClass != E_CARDCLASS_NONE)
			m_nClassCardCnt--;
		break;
	}
	}

	if (m_nClassCardCnt == 0)
	{
		m_eDeckClass = E_CARDCLASS_NONE;
		for ( UINT uID = IDC_CLASS_DRUID_RDO ; uID < IDC_CLASS_FREE_RDO; uID++ )
			GetDlgItem(uID)->EnableWindow(TRUE);
	}
	else if ( pCard->eClass != E_CARDCLASS_NONE)
	{
		UINT uiID = NULL;
		switch (pCard->eClass)
		{
		case E_CARDCLASS_DRUID:
			uiID = IDC_CLASS_DRUID_RDO;
			break;
		case E_CARDCLASS_HUNTER:
			uiID = IDC_CLASS_HUNTER_RDO;
			break;
		case E_CARDCLASS_WARLOCK:
			uiID = IDC_CLASS_WARLOCK_RDO;
			break;
		case E_CARDCLASS_WARRIOR:
			uiID = IDC_CLASS_WARRIOR_RDO;
			break;
		case E_CARDCLASS_MAGE:
			uiID = IDC_CLASS_MAGE_RDO;
			break;
		case E_CARDCLASS_SHAMAN:
			uiID = IDC_CLASS_SHAMAN_RDO;
			break;
		case E_CARDCLASS_PALADIN:
			uiID = IDC_CLASS_PALADIN_RDO;
			break;
		case E_CARDCLASS_PREIST:
			uiID = IDC_CLASS_PRIEST_RDO;
			break;
		case E_CARDCLASS_ROGUE:
			uiID = IDC_CLASS_ROGUE_RDO;
			break;
		default:
			break;
		}

		m_eDeckClass = pCard->eClass;
		GetDlgItem(uiID)->EnableWindow(TRUE);
		GetDlgItem(IDC_CLASS_FREE_RDO)->EnableWindow(TRUE);
		for (UINT uID = IDC_CLASS_DRUID_RDO; uID < IDC_CLASS_FREE_RDO; uID++)
			if ( uID != uiID)
				GetDlgItem(uID)->EnableWindow(FALSE);
	}
	m_ctrlDeckList.SetDeckClass(m_eDeckClass);
	return TRUE;
}

BOOL CHeaderStoneDeckMakerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	LoadCardData();

	CRect rtDlg;
	GetClientRect(rtDlg);
	
	CRect rtCardList;
	m_ctrlFrameCard.GetWindowRect(rtCardList);
	m_ctrlFrameCard.ShowWindow(SW_HIDE);
	ScreenToClient(rtCardList);
	m_ctrlCardList.Create(rtCardList, this);
	m_nFrmCardX = rtCardList.left - rtDlg.left; m_nFrmCardY = rtCardList.top - rtDlg.top;
	m_nFrmCardCX = rtDlg.right - rtCardList.right; m_nFrmCardCY = rtDlg.bottom - rtCardList.bottom;

	CRect rtDeckList;
	m_ctrlFrameDeck.GetWindowRect(rtDeckList);
	m_ctrlFrameDeck.ShowWindow(SW_HIDE);
	ScreenToClient(rtDeckList);
	m_ctrlDeckList.Create(rtDeckList, this);
	m_nFrmDeckX = rtDlg.right - rtDeckList.left; m_nFrmDeckY = rtDeckList.top - rtDlg.top;
	m_nFrmDeckCX = rtDlg.right - rtDeckList.right; m_nFrmDeckCY = rtDlg.bottom - rtDeckList.bottom;

	m_bInit = TRUE;
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CHeaderStoneDeckMakerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CHeaderStoneDeckMakerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CHeaderStoneDeckMakerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CHeaderStoneDeckMakerDlg::OnClickRatioButton()
{
}

void CHeaderStoneDeckMakerDlg::LoadCardData()
{
	MCurl* pCurl = MCurl::GetInstance();

	MURL UrlData;
	UrlData.strCmd = ("https://omgvamp-hearthstone-v1.p.rapidapi.com/cards?locale=koKR&collectible=1");
	UrlData.Header.push_back(MURLHeader(("X-RapidAPI-Host"), pCurl->GetAPIHost()));
	UrlData.Header.push_back(MURLHeader(("X-RapidAPI-Key"),  pCurl->GetAPIKey()));
	CString strResult = pCurl->get(UrlData);
	
	using namespace rapidjson;
	GenericDocument<UTF16<>> d;
	d.Parse((CString)strResult);

	if (d.HasParseError())
		ASSERT(0);

	CCardListMgr *pCardListMgr = CCardListMgr::GetInstance();
	for (GenericValue<UTF16<>>::ConstMemberIterator itrSet = d.MemberBegin(); itrSet != d.MemberEnd(); ++itrSet)
	{
		if (!itrSet->value.IsArray()) { ASSERT(0); continue;  }

		for ( auto& Value : itrSet->value.GetArray())
		{
			if (!Value.IsObject()) { ASSERT(0); continue; }

			// Delete는 ListMgr내부에서 소멸자로 호출
			CCard* pCard = new CCard();
			for (GenericValue<UTF16<>>::ConstMemberIterator itrValue = Value.MemberBegin(); itrValue != Value.MemberEnd(); ++itrValue)
			{
				ASSERT(itrValue->name.IsString());

				std::wstring strCompName = itrValue->name.GetString();
				std::wstring strCompValue;

				if (itrValue->value.IsInt())
					strCompValue = std::to_wstring(itrValue->value.GetInt());
				else if (itrValue->value.IsInt64())
					strCompValue = std::to_wstring(itrValue->value.GetInt64());
				else if (itrValue->value.IsString())
					strCompValue = itrValue->value.GetString();

				pCard->ImportCardData(strCompName, strCompValue);
			}
			if (pCardListMgr->CheckCardData(pCard))
			{
				pCardListMgr->AddCard(pCard);
			}
			else
			{
				delete pCard;
			}
		}
	}
	pCardListMgr->TraceAll();
	pCardListMgr->DownloadAllImg();
}

void CHeaderStoneDeckMakerDlg::OnSize(UINT nType, int cx, int cy)
{
	//1060, 727
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_bInit == TRUE)
	{
		m_ctrlCardList.SetWindowPos(&wndTop, 0, 0, cx - m_nFrmCardX - m_nFrmCardCX, cy - m_nFrmCardY - m_nFrmCardCY, SWP_NOMOVE | SWP_NOZORDER);
		m_ctrlCardList.CalcColRowMaxPage();

		m_ctrlDeckList.MoveWindow(cx - m_nFrmDeckX, m_nFrmDeckY, m_nFrmDeckX - m_nFrmDeckCX, cy - m_nFrmDeckY - m_nFrmDeckCY);
		m_ctrlDeckList.Invalidate();
	}
}


void CHeaderStoneDeckMakerDlg::OnClickRdoBtn(UINT nID)
{
	CCardListMgr* pListMgr = CCardListMgr::GetInstance();
	CCardFilter* pFilter = pListMgr->GetCardFilter();
	switch (nID)
	{
	case IDC_CLASS_DRUID_RDO:
		pFilter->nClass = E_CARDCLASS::E_CARDCLASS_DRUID;
		break;
	case IDC_CLASS_MAGE_RDO:
		pFilter->nClass = E_CARDCLASS::E_CARDCLASS_MAGE;
		break;
	case IDC_CLASS_SHAMAN_RDO:
		pFilter->nClass = E_CARDCLASS::E_CARDCLASS_SHAMAN;
		break;
	case IDC_CLASS_WARRIOR_RDO:
		pFilter->nClass = E_CARDCLASS::E_CARDCLASS_WARRIOR;
		break;
	case IDC_CLASS_PALADIN_RDO:
		pFilter->nClass = E_CARDCLASS::E_CARDCLASS_PALADIN;
		break;
	case IDC_CLASS_PRIEST_RDO:
		pFilter->nClass = E_CARDCLASS::E_CARDCLASS_PREIST;
		break;
	case IDC_CLASS_ROGUE_RDO:
		pFilter->nClass = E_CARDCLASS::E_CARDCLASS_ROGUE;
		break;
	case IDC_CLASS_FREE_RDO:
		pFilter->nClass = E_CARDCLASS::E_CARDCLASS_NONE;
		break;
	case IDC_CLASS_WARLOCK_RDO:
		pFilter->nClass = E_CARDCLASS::E_CARDCLASS_WARLOCK;
		break;
	case IDC_CLASS_HUNTER_RDO:
		pFilter->nClass = E_CARDCLASS::E_CARDCLASS_HUNTER;
		break;
	default:
		break;
	}
	m_ctrlCardList.SetStartIndex(0);
	m_ctrlCardList.CalcColRowMaxPage(TRUE);
}

void CHeaderStoneDeckMakerDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	lpMMI->ptMinTrackSize.x = 1200;
	lpMMI->ptMinTrackSize.y = 800;

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}


void CHeaderStoneDeckMakerDlg::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	//CDialogEx::OnOK();
}


void CHeaderStoneDeckMakerDlg::OnCancel()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CDialogEx::OnCancel();
}


BOOL CHeaderStoneDeckMakerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}
