#pragma once

#include "NotifyMsg.h"
#include "CardEnum.h"
#include "CardNotifier.h"
#include "DeckListMgr.h"

#include <vector>
#include <map>

// CImportByLocalDlg 대화 상자
namespace LocalDeckMsg
{
	enum LocalDeckMessage
	{
		MSG_SETLOCALDECKMODIFY = 1001,
		MSG_SETLOCALDECKSTRING = 1002,
		MSG_SETLOCALDECKCLASS  = 1003,
		MSG_SETLOCALDECKLIST   = 1004,
		MSG_LOCALDECKMSGMAX
	};
};

class CManaCurveDlg;
class CCardListMgr;

class CImportByLocalDlg : public CDialogEx, CCardNotifier
{
	DECLARE_DYNAMIC(CImportByLocalDlg)

public:
	CImportByLocalDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CImportByLocalDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMPORT_BY_LOCAL_DLG };
#endif

public:
	BOOL LoadLocalDeck();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
	virtual void OnCancel();

protected:
	virtual BOOL ExecuteNotify(NOTIFYMSG eSender, WPARAM wParam, LPARAM lParam);

	void InitClassCmb();
	afx_msg void OnClickManaCurve();
	afx_msg void OnSelChangeDeckClass();
	afx_msg void OnSelChangeDeckType();

private:
	CManaCurveDlg* m_pManaCurveDlg;
	std::map<int, LocalDeckData*> m_mapLocalDeck;

	CComboBox m_ctrlClassCmb;
	CComboBox m_ctrlDeckCmb;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
};
