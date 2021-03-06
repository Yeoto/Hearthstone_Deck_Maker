// HeaderStone_Deck_MakerDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "HeaderStone_Deck_Maker.h"
#include "HeaderStone_Deck_MakerDlg.h"
#include "afxdialogex.h"

#include "Base64.h"
#include "MCurl.h"
#include "CardListMgr.h"
#include "Card.h"
#include "CardEnum.h"
#include "CardFilter.h"
#include "MetaDeckData.h"

#include "FilterDlg.h"
#include "ProgressDlg.h"
#include "ImportDlg.h"
#include "ExportDlg.h"
#include "ImportByMetaDeck.h"
#include "ImportByLocalDlg.h"
#include "ManaCurveDlg.h"
#include "DeckListMgr.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CHeaderStoneDeckMakerDlg 대화 상자

#define DECK_LIST_OFFSET 300

BOOL CHeaderStoneDeckMakerDlg::m_bThreadContinue = TRUE;

CHeaderStoneDeckMakerDlg::CHeaderStoneDeckMakerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HEADERSTONE_DECK_MAKER_DIALOG, pParent), CCardNotifier(NTM_MAINDLG), m_nCurCard(0), m_bInit(FALSE), m_nClassCardCnt(0), m_eDeckClass(E_CARDCLASS_NONE),
	m_pImportDlg(nullptr), m_pFilterDlg(nullptr), m_pMetaDeckImportDlg(nullptr), m_pImportByLocal(nullptr),
	m_bModify(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitMetaDeck = FALSE;
}

void CHeaderStoneDeckMakerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CARD_PAGE, m_ctrlFrameCard);
	DDX_Control(pDX, IDC_DECK_LIST, m_ctrlFrameDeck);
	DDX_Control(pDX, IDC_IMPORT_SPL, m_ctrlImportSpl);
	DDX_Control(pDX, IDC_EXPORT_SPL, m_ctrlExportSpl);
}

BEGIN_MESSAGE_MAP(CHeaderStoneDeckMakerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CLASS_DRUID_RDO, IDC_CLASS_FREE_RDO, OnClickRdoBtn)
	ON_BN_CLICKED(IDC_RESET_BTN, OnClickResetBtn)
	ON_BN_CLICKED(IDC_FILTER_BTN, OnClickFilterBtn)
	ON_BN_CLICKED(IDC_IMPORT_SPL, OnMenuImportByDeckCode)
	ON_BN_CLICKED(IDC_EXPORT_SPL, OnMenuExportByDeckCode)
	ON_COMMAND(ID_IMPORT_IMPORTBYDECKCODE, OnMenuImportByDeckCode)
	ON_COMMAND(ID_IMPORT_IMPORTMETADECK, OnMenuImportByMetaDeck)
	ON_COMMAND(ID_IMPORT_IMPORTBYLOCAL, OnMenuImportByLocal)
	ON_COMMAND(ID_EXPORT_EXPORTBYDECKCODE, OnMenuExportByDeckCode)
	ON_COMMAND(ID_EXPORT_EXPORTBYLOCAL, OnMenuExportByLocal)
END_MESSAGE_MAP()


// CHeaderStoneDeckMakerDlg 메시지 처리기

BOOL CHeaderStoneDeckMakerDlg::ExecuteNotify(NOTIFYMSG eSender, WPARAM wParam, LPARAM lParam)
{
	switch (eSender)
	{
	case NTM_IMPORTDLG:
	{
		OnClickResetBtn();
		std::map<CCard*, int> mapDeckList = *(std::map<CCard*, int>*)lParam;
		for (auto Cards : mapDeckList)
			for (int i = 0; i < Cards.second; i++)
				InitializeDeckType(1, Cards.first);
		return TRUE;
	}
		
	case NTM_DECKLISTCTRL:
		if (wParam != 0 && wParam != 1)
			return FALSE;
		InitializeDeckType((BOOL)wParam, (CCard*)lParam);
		return TRUE;
	case NTM_FILTERDLG:
		{
		CCardFilter* pFilter = CCardListMgr::GetInstance()->GetCardFilter();
		if(pFilter->bUseClass == FALSE)
			for (UINT uID = IDC_CLASS_DRUID_RDO; uID < IDC_CLASS_FREE_RDO; uID++)
				((CButton*)GetDlgItem(uID))->SetCheck(BST_UNCHECKED);
		int temp = 0;
		temp += ((pFilter->nClass & E_CARDCLASS_WARRIOR) > 0) ? 1 : 0;
		temp += ((pFilter->nClass & E_CARDCLASS_SHAMAN ) > 0) ? 1 : 0;
		temp += ((pFilter->nClass & E_CARDCLASS_ROGUE  ) > 0) ? 1 : 0;
		temp += ((pFilter->nClass & E_CARDCLASS_PALADIN) > 0) ? 1 : 0;
		temp += ((pFilter->nClass & E_CARDCLASS_HUNTER ) > 0) ? 1 : 0;
		temp += ((pFilter->nClass & E_CARDCLASS_DRUID  ) > 0) ? 1 : 0;
		temp += ((pFilter->nClass & E_CARDCLASS_WARLOCK) > 0) ? 1 : 0;
		temp += ((pFilter->nClass & E_CARDCLASS_MAGE   ) > 0) ? 1 : 0;
		temp += ((pFilter->nClass & E_CARDCLASS_PREIST ) > 0) ? 1 : 0;
		if (temp > 1)
			for (UINT uID = IDC_CLASS_DRUID_RDO; uID < IDC_CLASS_FREE_RDO; uID++)
				((CButton*)GetDlgItem(uID))->SetCheck(BST_UNCHECKED);
		}
		return TRUE;
	case NTM_IMPORTLOCALDLG:
	case NTM_IMPORTMETADECKDLG:
		return ExecuteNotifyFromDlg(wParam, lParam);
	}
	
	return TRUE;
}

BOOL CHeaderStoneDeckMakerDlg::ExecuteNotifyFromDlg(WPARAM wParam, LPARAM lParam)
{
	if (lParam == NULL)
		return FALSE;

	switch (wParam)
	{
	case LocalDeckMsg::MSG_SETLOCALDECKMODIFY:
	{
		OnClickResetBtn();
		int nKey = *(int*)lParam;
		m_bModify = nKey > 0;
		return TRUE;
	}
	case MetaDeckMsg::MSG_SETMETADECKLIST:
	case LocalDeckMsg::MSG_SETLOCALDECKLIST:
	{
		std::map<CCard*, int> mapDeckList = *(std::map<CCard*, int>*)lParam;
		for (auto Cards : mapDeckList)
			for (int i = 0; i < Cards.second; i++)
				InitializeDeckType(1, Cards.first);
		return TRUE;
	}
	default:
		return FALSE;
	}
}

void CHeaderStoneDeckMakerDlg::InitializeDeckType(BOOL bAdd, CCard* pCard)
{
	if (pCard != NULL)
	{
		switch (bAdd)
		{
		case TRUE:
			if (pCard->eClass != E_CARDCLASS_NONE)
				m_nClassCardCnt++;
			break;
		case FALSE:
			if (pCard->eClass != E_CARDCLASS_NONE)
				m_nClassCardCnt--;
			break;
		}
	}

	if (m_nClassCardCnt == 0)
	{
		m_eDeckClass = E_CARDCLASS_NONE;
		for (UINT uID = IDC_CLASS_DRUID_RDO; uID < IDC_CLASS_FREE_RDO; uID++)
			GetDlgItem(uID)->EnableWindow(TRUE);
	}
	else if (pCard->eClass != E_CARDCLASS_NONE && (pCard->eClass == m_eDeckClass || m_eDeckClass == E_CARDCLASS_NONE))
	{
		CCardFilter* pFilter = CCardListMgr::GetInstance()->GetCardFilter();
		if (pCard->eClass != m_eDeckClass)
		{
			if (pFilter->bUseClass == TRUE)
			{
				SetRdoEnable(pCard->eClass, pFilter->nClass != pCard->eClass);
			}
			else
			{
				SetRdoEnable(pCard->eClass);
			}
		}
	}
	m_ctrlDeckList.SetDeckClass(m_eDeckClass);
}

void CHeaderStoneDeckMakerDlg::SetRdoEnable(E_CARDCLASS eDeckClass, BOOL bPageReset /*= TRUE*/)
{
	UINT uiID = NULL;
	switch (eDeckClass)
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

	m_eDeckClass = eDeckClass;
	GetDlgItem(IDC_CLASS_FREE_RDO)->EnableWindow(TRUE);
	((CButton*)GetDlgItem(uiID))->SetCheck(BST_CHECKED);

	((CButton*)GetDlgItem(IDC_CLASS_FREE_RDO))->SetCheck(BST_UNCHECKED);
	for (UINT uID = IDC_CLASS_DRUID_RDO; uID < IDC_CLASS_FREE_RDO; uID++)
	{
		if (uID != uiID)
		{
			((CButton*)GetDlgItem(uID))->SetCheck(BST_UNCHECKED);
			GetDlgItem(uID)->EnableWindow(uID == uiID);
		}
	}
	((CButton*)GetDlgItem(uiID))->SetCheck(BST_CHECKED);
	UpdateData(FALSE);
	SetFilterByRdo(uiID, bPageReset);
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

	CRect rtReset;
	GetDlgItem(IDC_RESET_BTN)->GetWindowRect(rtReset);
	ScreenToClient(rtReset);
	m_nOffsetX_reset = rtDlg.right - rtReset.left;
	m_nOffsetY_Btns = rtReset.top;

	CRect rtImport;
	GetDlgItem(IDC_IMPORT_SPL)->GetWindowRect(rtImport);
	ScreenToClient(rtImport);
	m_nOffsetX_import = rtDlg.right - rtImport.left;

	CRect rtExport;
	GetDlgItem(IDC_EXPORT_SPL)->GetWindowRect(rtExport);
	ScreenToClient(rtExport);
	m_nOffsetX_export = rtDlg.right - rtExport.left;

	CCardFilter* pFilter = CCardListMgr::GetInstance()->GetCardFilter();
	m_pFilterDlg = new CFilterDlg(pFilter);

	((CButton*)GetDlgItem(IDC_CLASS_DRUID_RDO))->SetCheck(BST_CHECKED);

	m_ctrlImportSpl.SetDropDownMenu(IDR_MENU, 0);
	m_ctrlExportSpl.SetDropDownMenu(IDR_MENU, 1);

	m_bInit = TRUE;
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CHeaderStoneDeckMakerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID == SC_CLOSE)
	{
		return OnCancel();
	}
	else if ((nID & 0xFFF0) == IDM_ABOUTBOX)
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

void CHeaderStoneDeckMakerDlg::LoadCardData()
{
	CProgressDlg dlg;
	dlg.Create(IDD_LOADING_DATA_PROGRESS_DLG);
	dlg.CenterWindow(CWnd::GetDesktopWindow());
	dlg.SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	dlg.ShowWindow(SW_SHOW);

	MCurl* pCurl = MCurl::GetInstance();

	MURL UrlData;
	UrlData.strCmd = ("https://omgvamp-hearthstone-v1.p.rapidapi.com/cards?locale=koKR&collectible=1");
	UrlData.Header.push_back(MURLHeader(("X-RapidAPI-Host"), pCurl->GetAPIHost()));
	UrlData.Header.push_back(MURLHeader(("X-RapidAPI-Key"),  pCurl->GetAPIKey()));

	dlg.SetLoadingString(_T("여관 주인에게 카드 물어보는 중..."));
	dlg.SetProgressMax(1);
	CString strResult = pCurl->get(UrlData);
	
	using namespace rapidjson;
	GenericDocument<UTF16<>> d;
	d.Parse((CString)strResult);
	dlg.IndexPlusOne();

	if (d.HasParseError())
		ASSERT(0);

	int nCardCnt = 0;
	for (GenericValue<UTF16<>>::ConstMemberIterator itrSet = d.MemberBegin(); itrSet != d.MemberEnd(); ++itrSet)
	{
		if (!itrSet->value.IsArray()) { ASSERT(0); continue; }
		nCardCnt += itrSet->value.GetArray().Size();
	}

	dlg.SetLoadingString(_T("가져온 카드 정리하는 중..."));
	dlg.SetProgressMax(nCardCnt);
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
			dlg.IndexPlusOne();
		}
	}

	pCardListMgr->TraceAll();
	m_bThreadContinue = TRUE;
	m_pThread = AfxBeginThread(ThreadFunc, (LPVOID)m_bThreadContinue);
}

void CHeaderStoneDeckMakerDlg::LoadMetaDeckData()
{
	CProgressDlg dlg;
	dlg.Create(IDD_LOADING_DATA_PROGRESS_DLG);
	dlg.CenterWindow(CWnd::GetDesktopWindow());
	dlg.SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	dlg.ShowWindow(SW_SHOW);

	dlg.SetLoadingString(_T("HSReplay.net에서 덱 타입 불러오는 중..."));
	dlg.SetProgressMax(1);

	std::map<int, std::pair<E_CARDCLASS, CString>> mapArcheType;
	LoadArcheType(mapArcheType);

	dlg.IndexPlusOne();

	MURL UrlData;
	MCurl* pCurl = MCurl::GetInstance();
	UrlData.strCmd = ("https://hsreplay.net/analytics/query/list_decks_by_win_rate/?GameType=RANKED_STANDARD&RankRange=LEGEND_THROUGH_TWENTY&TimeRange=LAST_30_DAYS");

	dlg.SetLoadingString(_T("HSReplay.net에서 덱 리스트 불러오는 중..."));
	dlg.SetProgressMax(1);

	CString strMeta = pCurl->get(UrlData);

	dlg.IndexPlusOne();

	using namespace rapidjson;
	GenericDocument<UTF16<>> docMetaDeck;
	docMetaDeck.Parse((CString)strMeta);

	if (docMetaDeck.HasParseError())
		ASSERT(0);

	CCardListMgr *pCardListMgr = CCardListMgr::GetInstance();
	for (GenericValue<UTF16<>>::ConstMemberIterator itrJSON = docMetaDeck.MemberBegin(); itrJSON != docMetaDeck.MemberEnd(); ++itrJSON)
	{
		rapidjson::Type type = itrJSON->value.GetType();

		if (!itrJSON->value.IsObject()) { continue; }
		for (GenericValue<UTF16<>>::ConstMemberIterator itrValue = itrJSON->value.MemberBegin(); itrValue != itrJSON->value.MemberEnd(); ++itrValue)
		{
			if (!itrValue->value.IsObject()) { continue; }
			if (itrValue->name != _T("data")) { continue; }

			for (GenericValue<UTF16<>>::ConstMemberIterator itrClass = itrValue->value.MemberBegin(); itrClass != itrValue->value.MemberEnd(); ++itrClass)
			{
				ASSERT(itrClass->value.IsArray());

				std::map<int, CMetaDeckData*[3]> mapMetaDeck;	//key : deck type, value : meta deck data;
				for (auto& Deck : itrClass->value.GetArray())
				{
					if (!Deck.IsObject()) { ASSERT(0); continue; }

					CMetaDeckData* pMetaDeckTemp = new CMetaDeckData();
					for (GenericValue<UTF16<>>::ConstMemberIterator itrDeckData = Deck.MemberBegin(); itrDeckData != Deck.MemberEnd(); ++itrDeckData)
					{
						ASSERT(itrDeckData->name.IsString());

						if (itrDeckData->value.IsInt())
						{
							int nData = (itrDeckData->value.GetInt());
							pMetaDeckTemp->ImportMetaDeckData(itrDeckData->name.GetString(), &nData);
						}
						else if (itrDeckData->value.IsString())
						{
							std::wstring strData = (itrDeckData->value.GetString());
							pMetaDeckTemp->ImportMetaDeckData(itrDeckData->name.GetString(), &strData);
						}
						else if (itrDeckData->value.GetDouble())
						{
							double dData = (itrDeckData->value.GetDouble());
							pMetaDeckTemp->ImportMetaDeckData(itrDeckData->name.GetString(), &dData);
						}
					}

					if (pMetaDeckTemp->m_nArcheType < 0)
					{
						//실레합니다만, 아키타입도 없는 구데기 덱한테는 관심 없어요
						delete pMetaDeckTemp;
						continue;
					}

					if ( mapMetaDeck.find(pMetaDeckTemp->m_nArcheType) == mapMetaDeck.end() )
					{
						mapMetaDeck[pMetaDeckTemp->m_nArcheType][0] = pMetaDeckTemp;
					}
					else
					{
						CMetaDeckData** temp = mapMetaDeck[pMetaDeckTemp->m_nArcheType];

						BOOL bDelete = TRUE;
						for (int nIdx = 0; nIdx < 3; nIdx++)
						{
							if (temp[nIdx] == NULL)
							{
								temp[nIdx] = pMetaDeckTemp;
								bDelete = FALSE;
								break;
							}

							if (temp[nIdx]->m_nTotalGames < pMetaDeckTemp->m_nTotalGames)
							{
								delete temp[2];
								temp[2] = NULL;

								memcpy_s(temp + nIdx + 1, sizeof(CMetaDeckData*) * (2-nIdx), temp + nIdx, sizeof(CMetaDeckData*) * (2 - nIdx));
								temp[nIdx] = pMetaDeckTemp;
								bDelete = FALSE;
								break;
							}
						}
						if (bDelete == TRUE)
							delete pMetaDeckTemp;
					}
				}

				for (auto itrPair : mapMetaDeck)
				{
					std::pair<E_CARDCLASS, CString> pairArcheType = mapArcheType[itrPair.first];

					for (int nIdx = 0; nIdx < 3; nIdx++)
					{
						if (itrPair.second[nIdx] != NULL)
						{
							CMetaDeckData* pMetaDeck = itrPair.second[nIdx];

							pMetaDeck->m_eDeckClass = pairArcheType.first;
							pMetaDeck->m_strArcheType = pairArcheType.second;
							pMetaDeck->DeckListStr2DeckListMap();
							pCardListMgr->AddMetaDeck(pMetaDeck->m_eDeckClass, pMetaDeck);
						}
					}
				}
			}
		}
	}
}

void CHeaderStoneDeckMakerDlg::LoadArcheType(std::map<int, std::pair<E_CARDCLASS, CString>>& mapArcheType)
{
	mapArcheType.clear();

	MURL UrlData;
	MCurl* pCurl = MCurl::GetInstance();

	UrlData.strCmd = "https://hsreplay.net/api/v1/archetypes/?format=json&GameType=RANKED_STANDARD&RankRange=LEGEND_THROUGH_TWENTY&TimeRange=LAST_30_DAYS";
	CString strArcheType = pCurl->get(UrlData);

	using namespace rapidjson;
	GenericDocument<UTF16<>> docArcheType;
	docArcheType.Parse((CString)strArcheType);

	if (docArcheType.HasParseError())
		ASSERT(0);

	ASSERT(docArcheType.IsArray());

	for (auto& DeckType : docArcheType.GetArray())
	{
		if (!DeckType.IsObject()) { continue; }

		int nArcheType = -1;
		E_CARDCLASS eCardClass = E_CARDCLASS_NONE;
		CString strArcheType = _T("");
		for (GenericValue<UTF16<>>::ConstMemberIterator itrComp = DeckType.MemberBegin(); itrComp != DeckType.MemberEnd(); ++itrComp)
		{
			if (itrComp->name == _T("id"))
				nArcheType = itrComp->value.GetInt();
			else if (itrComp->name == _T("name"))
				strArcheType = itrComp->value.GetString();
			else if (itrComp->name == _T("player_class_name"))
				eCardClass = CCard::GetCardClassByStr(itrComp->value.GetString());
		}

		if (mapArcheType.find(nArcheType) != mapArcheType.end())
		{
			ASSERT(0);
			continue;
		}

		mapArcheType[nArcheType] = std::pair<E_CARDCLASS, CString>(eCardClass, strArcheType);
	}
}

void CHeaderStoneDeckMakerDlg::SetFilterByRdo(UINT nID, BOOL bPageReset /*= TRUE*/)
{
	CCardListMgr* pListMgr = CCardListMgr::GetInstance();
	CCardFilter* pFilter = pListMgr->GetCardFilter();
	pFilter->bUseClass = TRUE;
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
	if (bPageReset)
	{
		m_ctrlCardList.SetStartIndex(0);
		m_ctrlCardList.CalcColRowMaxPage(TRUE);
	}
	SendNotify(NULL, NULL);
}

void CHeaderStoneDeckMakerDlg::Copy2Clipboard(const CString strText)
{
	HGLOBAL h;
	LPTSTR arr;

	size_t bytes = (strText.GetLength() + 1) * sizeof(TCHAR);
	h = GlobalAlloc(GMEM_MOVEABLE, bytes);
	arr = (LPTSTR)GlobalLock(h);
	ZeroMemory(arr, bytes);
	_tcscpy_s(arr, strText.GetLength() + 1, strText);
	//strText.ReleaseBuffer();
	GlobalUnlock(h);

	::OpenClipboard(NULL);
	EmptyClipboard();
	SetClipboardData(CF_UNICODETEXT, h);
	CloseClipboard();
}

UINT CHeaderStoneDeckMakerDlg::ThreadFunc(LPVOID pParam)
{
	CCardListMgr* pCardListMgr = CCardListMgr::GetInstance();
	pCardListMgr->DownloadAllImg((BOOL*)pParam);
	return 1;
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

		GetDlgItem(IDC_RESET_BTN)->SetWindowPos(&wndTop, cx - m_nOffsetX_reset, m_nOffsetY_Btns, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		GetDlgItem(IDC_RESET_BTN)->Invalidate();

		GetDlgItem(IDC_IMPORT_SPL)->SetWindowPos(&wndTop, cx - m_nOffsetX_import, m_nOffsetY_Btns, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		GetDlgItem(IDC_IMPORT_SPL)->Invalidate();

		GetDlgItem(IDC_EXPORT_SPL)->SetWindowPos(&wndTop, cx - m_nOffsetX_export, m_nOffsetY_Btns, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		GetDlgItem(IDC_EXPORT_SPL)->Invalidate();
	}
}


void CHeaderStoneDeckMakerDlg::OnClickRdoBtn(UINT nID)
{
	SetFilterByRdo(nID, TRUE);
}

void CHeaderStoneDeckMakerDlg::OnClickFilterBtn()
{
	CCardFilter* pFilter = CCardListMgr::GetInstance()->GetCardFilter();
	if (m_pFilterDlg->GetSafeHwnd() == NULL)
	{
		m_pFilterDlg->Create(IDD_FILTER_DLG);
		m_pFilterDlg->Data2Dlg();
		m_pFilterDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pFilterDlg->SetActiveWindow();
	}
}

void CHeaderStoneDeckMakerDlg::OnClickResetBtn()
{
	m_bModify = FALSE;
	m_nClassCardCnt = 0;
	m_eDeckClass = E_CARDCLASS_NONE;

	for (UINT uID = IDC_CLASS_DRUID_RDO; uID < IDC_CLASS_FREE_RDO; uID++)
		GetDlgItem(uID)->EnableWindow(TRUE);

	m_ctrlDeckList.ResetDeck();
}

void CHeaderStoneDeckMakerDlg::OnMenuImportByDeckCode()
{
	if (m_pImportDlg == NULL)
		m_pImportDlg = new CImportDlg(this);

	if (m_pImportDlg->GetSafeHwnd() == NULL)
	{
		CPoint pt;
		GetCursorPos(&pt);

		m_pImportDlg->Create(IDD_DECKCODE_DLG);
		CRect rtDlg;
		m_pImportDlg->GetClientRect(rtDlg);
		m_pImportDlg->SetWindowPos(&wndTop, pt.x - rtDlg.Width(), pt.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_pImportDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pImportDlg->SetActiveWindow();
	}
}

void CHeaderStoneDeckMakerDlg::OnMenuImportByMetaDeck()
{
	CPoint pt;
	GetCursorPos(&pt);

	if (m_bInitMetaDeck == FALSE)
	{
		LoadMetaDeckData();
		m_bInitMetaDeck = TRUE;
	}

	if (m_pMetaDeckImportDlg == NULL)
		m_pMetaDeckImportDlg = new CImportByMetaDeck(this);

	if (m_pMetaDeckImportDlg->GetSafeHwnd() == NULL)
	{
		m_pMetaDeckImportDlg->Create(IDD_IMPORT_BY_METADECK_DLG);
		CRect rtDlg;
		m_pMetaDeckImportDlg->GetClientRect(rtDlg);
		m_pMetaDeckImportDlg->SetWindowPos(&wndTop, pt.x - rtDlg.Width(), pt.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_pMetaDeckImportDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pMetaDeckImportDlg->SetActiveWindow();
	}
}

void CHeaderStoneDeckMakerDlg::OnMenuImportByLocal()
{
	CPoint pt;
	GetCursorPos(&pt);

	if (m_pImportByLocal == NULL)
		m_pImportByLocal = new CImportByLocalDlg(this);

	if (m_pImportByLocal->GetSafeHwnd() == NULL)
	{
		m_pImportByLocal->Create(IDD_IMPORT_BY_LOCAL_DLG);

		if (!m_pImportByLocal->LoadLocalDeck())
			return;

		CRect rtDlg;
		m_pImportByLocal->GetClientRect(rtDlg);
		m_pImportByLocal->SetWindowPos(&wndTop, pt.x - rtDlg.Width(), pt.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_pImportByLocal->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pImportByLocal->SetActiveWindow();
	}
}

void CHeaderStoneDeckMakerDlg::OnMenuExportByDeckCode()
{
	if (m_pExportDlg == NULL)
		m_pExportDlg = new CExportDlg(this);

	CString strDeckCode = _T("");
	CCardListMgr *pListMgr = CCardListMgr::GetInstance();
	CString strDeckName;
	std::map<CCard*, int> mapDeckList;
	m_ctrlDeckList.GetDeck(strDeckName, mapDeckList);
	pListMgr->CardList2DeckCode(m_eDeckClass, mapDeckList, strDeckCode);
	Copy2Clipboard(strDeckCode);

	if (m_pExportDlg->GetSafeHwnd() == NULL)
	{
		CPoint pt;
		GetCursorPos(&pt);

		m_pExportDlg->Create(IDD_DECKCODE_DLG);
		CRect rtDlg;
		m_pExportDlg->GetClientRect(rtDlg);
		m_pExportDlg->SetWindowPos(&wndTop, pt.x - rtDlg.Width(), pt.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_pExportDlg->SetDeckCode(strDeckCode);
		m_pExportDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pExportDlg->SetActiveWindow();
	}
}

void CHeaderStoneDeckMakerDlg::OnMenuExportByLocal()
{
	int nKey = -1;
	CString strName;
	std::map<CCard*, int> mDeck;
	m_ctrlDeckList.GetDeck(strName, mDeck);
	m_ctrlDeckList.GetLocalDeckKey(nKey);
	E_CARDCLASS eClass = m_ctrlDeckList.GetDeckClass();
	CDeckListMgr::Export2Json(nKey, strName, eClass, mDeck);
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
	//if (pMsg->wParam == VK_ESCAPE) return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CHeaderStoneDeckMakerDlg::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_bThreadContinue = FALSE;
	if (::WaitForSingleObject(m_pThread->m_hThread, 0) == WAIT_TIMEOUT)
	{
		if (::WaitForSingleObject(m_pThread->m_hThread, 10000) == WAIT_TIMEOUT)
			TerminateThread(m_pThread->m_hThread, 0);

		m_pThread = NULL;
	}

	if (m_pFilterDlg != NULL)
	{
		if (m_pFilterDlg->GetSafeHwnd() != NULL)
			m_pFilterDlg->DestroyWindow();
		delete m_pFilterDlg;
	}
	if (m_pImportDlg != NULL)
	{
		if (m_pImportDlg->GetSafeHwnd() != NULL)
			m_pImportDlg->DestroyWindow();
		delete m_pImportDlg;
	}
	if (m_pExportDlg != NULL)
	{
		if (m_pExportDlg->GetSafeHwnd() != NULL)
			m_pExportDlg->DestroyWindow();
		delete m_pExportDlg;
	}
	if (m_pImportByLocal != NULL)
	{
		if (m_pImportByLocal->GetSafeHwnd() != NULL)
			m_pImportByLocal->DestroyWindow();
		delete m_pImportByLocal;
	}
	__super::PostNcDestroy();
}
