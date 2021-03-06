#pragma once

#include "CardNotifier.h"
#include "NotifyMsg.h"
#include <string>
// CImportDlg 대화 상자

class CDeckListCtrl;
class CCardListMgr;

class CImportDlg : public CDialogEx, public CCardNotifier
{
	DECLARE_DYNAMIC(CImportDlg)

public:
	CImportDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CImportDlg();

	virtual BOOL ExecuteNotify(NOTIFYMSG eSender, WPARAM wParam, LPARAM lParam);
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum {IDD = IDD_DECKCODE_DLG};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();

private:
	CEdit m_ctrlDeckCode;
	virtual void OnOK();
	virtual void OnCancel();
};
