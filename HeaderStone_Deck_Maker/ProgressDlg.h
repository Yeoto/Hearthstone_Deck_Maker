#pragma once

class CProgressDlg :
	public CDialogEx
{
public:
	CProgressDlg();
	virtual ~CProgressDlg();

	void SetLoadingString(CString strLoading);
	//void SetCurrentIndex(int nCurIdx);
	void IndexPlusOne();
	void SetProgressMax(int nMax);

private:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

private:
	int m_nCurIdx;
	int m_nMaxIdx;
	CStatic m_ctrlLoading;
	CStatic m_ctrlAmount;
	CProgressCtrl m_ctrlProgress;
	virtual void PostNcDestroy();
	virtual void OnOK();
	virtual void OnCancel();
};

