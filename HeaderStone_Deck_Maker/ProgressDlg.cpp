#include "stdafx.h"
#include "HeaderStone_Deck_Maker.h"
#include "ProgressDlg.h"
#include "afxdialogex.h"

CProgressDlg::CProgressDlg() : m_nCurIdx(0)
{
}


CProgressDlg::~CProgressDlg()
{
}

void CProgressDlg::SetLoadingString(CString strLoading)
{
	m_ctrlLoading.SetWindowText(strLoading);
	UpdateWindow();
}

void CProgressDlg::IndexPlusOne()
{
	m_nCurIdx++;
	if (m_nCurIdx > m_nMaxIdx)
		m_nMaxIdx = m_nCurIdx;

	CString strAmount;
	strAmount.Format(_T("%d / %d"), m_nCurIdx, m_nMaxIdx);
	m_ctrlAmount.SetWindowText(strAmount);

	m_ctrlProgress.SetPos(m_nCurIdx);
	UpdateWindow();
}

void CProgressDlg::SetProgressMax(int nMax)
{
	m_nCurIdx = 0;
	m_nMaxIdx = nMax;

	CString strAmount;
	strAmount.Format(_T("%d / %d"), m_nCurIdx, m_nMaxIdx);
	m_ctrlAmount.SetWindowText(strAmount);

	m_ctrlProgress.SetRange(0, nMax);
	m_ctrlProgress.SetPos(0);
	UpdateWindow();
}


void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_LOADING_STC, m_ctrlLoading);
	DDX_Control(pDX, IDC_AMOUNT_STC, m_ctrlAmount);
	DDX_Control(pDX, IDC_PROGRESS_BAR, m_ctrlProgress);
}

void CProgressDlg::PostNcDestroy()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	delete this;
	//CDialogEx::PostNcDestroy();
}


void CProgressDlg::OnOK()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	//CDialogEx::OnOK();
}


void CProgressDlg::OnCancel()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	//CDialogEx::OnCancel();
}
