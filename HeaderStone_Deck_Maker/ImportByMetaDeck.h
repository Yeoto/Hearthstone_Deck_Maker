#pragma once

#include <vector>
#include <map>
#include "CardEnum.h"
#include "CardNotifier.h"
#include "NotifyMsg.h"

// CImportByMetaDeck 대화 상자
namespace MetaDeckMsg
{
	enum LocalDeckMessage
	{
		MSG_SETMETADECKSTRING = 2001,
		MSG_SETMETADECKLIST = 2002,
		MSG_SETMETADECKCLASS = 2003,
		MSG_LOCALDECKMSGMAX
	};
};

class CMetaDeckData;
class CImportByMetaDeck : public CDialogEx, public CCardNotifier
{
	DECLARE_DYNAMIC(CImportByMetaDeck)

public:
	CImportByMetaDeck(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CImportByMetaDeck();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMPORT_BY_METADECK_DLG };
#endif

protected:
	virtual BOOL ExecuteNotify(NOTIFYMSG eSender, WPARAM wParam, LPARAM lParam);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnCbnSelchangeDeckclassCmb();
	afx_msg void OnCbnSelchangeDecktypeCmb();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

protected:
	CEdit m_ctrlAvgTime;
	CEdit m_ctrlAvgTurn;
	CEdit m_ctrlTotalGame;
	CEdit m_ctrlWinRate;
	CComboBox m_ctrlDeckClass;
	CComboBox m_ctrlDeckType;

	std::map<E_CARDCLASS, std::vector<CMetaDeckData*>> m_mapMetaData;
};
