
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
	: CDialogEx(IDD_HEADERSTONE_DECK_MAKER_DIALOG, pParent), m_nCurCard(0), m_bInit(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHeaderStoneDeckMakerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHeaderStoneDeckMakerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CHeaderStoneDeckMakerDlg 메시지 처리기

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

	CCardListMgr *pCardListMgr = CCardListMgr::GetInstance();
	std::vector<CCard*> vecCardList = pCardListMgr->GetCardList();

	CRect temp;
	GetClientRect(temp);
	m_ctrlCard1.Create(CRect(20, 20, temp.right - 20 - DECK_LIST_OFFSET, temp.bottom - 20), this);

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
	Document d;
	d.Parse((CStringA)strResult);

	if (d.HasParseError())
		ASSERT(0);

	CCardListMgr *pCardListMgr = CCardListMgr::GetInstance();
	for (Value::ConstMemberIterator itrSet = d.MemberBegin(); itrSet != d.MemberEnd(); ++itrSet)
	{
		if (!itrSet->value.IsArray()) { ASSERT(0); continue;  }

		for ( auto& Value : itrSet->value.GetArray())
		{
			if (!Value.IsObject()) { ASSERT(0); continue; }

			// Delete는 ListMgr내부에서 소멸자로 호출
			CCard* pCard = new CCard();
			for (Value::ConstMemberIterator itrValue = Value.MemberBegin(); itrValue != Value.MemberEnd(); ++itrValue)
			{
				ASSERT(itrValue->name.IsString());

				std::string strCompName = itrValue->name.GetString();
				std::string strCompValue;

				if (itrValue->value.IsInt())
					strCompValue = std::to_string(itrValue->value.GetInt());
				else if (itrValue->value.IsInt64())
					strCompValue = std::to_string(itrValue->value.GetInt64());
				else if (itrValue->value.IsString())
					strCompValue = itrValue->value.GetString();

				pCard->ImportCardData(strCompName, strCompValue);
			}
			pCardListMgr->AddCard(pCard);
		}
	}
	pCardListMgr->TraceAll();
	pCardListMgr->DownloadAllImg();
}

void CHeaderStoneDeckMakerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_bInit == TRUE)
	{
		m_ctrlCard1.SetWindowPos(&wndTop, 0, 0, cx - 40 - DECK_LIST_OFFSET, cy - 40, SWP_NOMOVE | SWP_NOZORDER);
		m_ctrlCard1.CalcColRowMaxPage();
	}
}
