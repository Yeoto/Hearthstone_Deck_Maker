#pragma once

#include <string>
// CExportDlg 대화 상자

class CDeckListCtrl;
class CExportDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CExportDlg)

public:
	CExportDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CExportDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum {IDD = IDD_DECKCODE_DLG};
#endif

public:
	void SetDeckCode(CString strDeckCode);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();

private:
	CEdit m_ctrlDeckCode;
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
};
