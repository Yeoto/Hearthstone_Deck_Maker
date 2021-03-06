#pragma once

#include <map>
#include "ChartViewer.h"

// CImportByLocalDlg 대화 상자
class CCard;

class CManaCurveDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CManaCurveDlg)

public:
	CManaCurveDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CManaCurveDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMPORT_BY_LOCAL_DLG };
#endif

public:
	void SetDeckData(std::map<CCard*, int> mapDeckList);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	CChartViewer m_ctrlChart;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
};
