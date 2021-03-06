
// HeaderStone_Deck_MakerDlg.h: 헤더 파일
//

#pragma once

#include <string>
#include "CardCtrl.h"
#include "CardListCtrl.h"
#include "DeckListCtrl.h"

// CHeaderStoneDeckMakerDlg 대화 상자
class CCardCtrl;
class CFilterDlg;
class CImportDlg;
class CExportDlg;
class CImportByMetaDeck;
class CImportByLocalDlg;

class CHeaderStoneDeckMakerDlg : public CDialogEx, CCardNotifier
{
// 생성입니다.
public:
	CHeaderStoneDeckMakerDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HEADERSTONE_DECK_MAKER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

// 구현입니다.
protected:
	HICON m_hIcon;

	virtual BOOL ExecuteNotify(NOTIFYMSG eSender, WPARAM wParam, LPARAM lParam);
	BOOL ExecuteNotifyFromDlg(WPARAM wParam, LPARAM lParam);

	void InitializeDeckType(BOOL bAdd, CCard* pCard);
	void SetRdoEnable(E_CARDCLASS eDeckClass, BOOL bPageReset = TRUE);
	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClickRdoBtn(UINT nID);
	afx_msg void OnClickFilterBtn();
	afx_msg void OnClickResetBtn();
	afx_msg void OnMenuImportByDeckCode();
	afx_msg void OnMenuImportByMetaDeck();
	afx_msg void OnMenuImportByLocal();
	afx_msg void OnMenuExportByDeckCode();
	afx_msg void OnMenuExportByLocal();
	DECLARE_MESSAGE_MAP()

protected:
	void LoadCardData();
	void LoadMetaDeckData();
	void LoadArcheType(std::map<int, std::pair<E_CARDCLASS, CString>>& mapArcheType);
	void SetFilterByRdo(UINT nID, BOOL bPageReset = TRUE);
	void Copy2Clipboard(const CString strText);

protected:
	CWinThread* m_pThread;
	static BOOL m_bThreadContinue;
	static UINT ThreadFunc(LPVOID pParam);

protected:
	BOOL m_bModify;
	int m_nCurCard;
	//CCardListCtrl m_ctrlCard1;
	//CDeckListCtrl m_ctrlDeck;
	BOOL m_bInit;
	CWnd m_ctrlFrameCard;
	int m_nFrmCardX, m_nFrmCardY;	//대화상자 X/Y 왼쪽 위와 컨트롤의 X/Y 왼쪽 위의 넓이
	int m_nFrmCardCX, m_nFrmCardCY;	//대화상자의 X/Y 오른쪽 밑과 컨트롤의 X/Y 오른쪽 밑의 넓이
	CWnd m_ctrlFrameDeck;
	int m_nFrmDeckX, m_nFrmDeckY;	//대화상자 X/Y 왼쪽 위와 컨트롤의 X/Y 왼쪽 위의 넓이
	int m_nFrmDeckCX, m_nFrmDeckCY;	//대화상자 X/Y 오른쪽 밑와 컨트롤의 X/Y 오른쪽 밑의 넓이

	int m_nOffsetY_Btns;
	int m_nOffsetX_reset;
	int m_nOffsetX_import;
	int m_nOffsetX_export;

	CCardListCtrl m_ctrlCardList;
	CDeckListCtrl m_ctrlDeckList;

	E_CARDCLASS m_eDeckClass;
	int m_nClassCardCnt;

	CFilterDlg* m_pFilterDlg;
	CImportDlg* m_pImportDlg;
	CExportDlg* m_pExportDlg;
	CImportByMetaDeck* m_pMetaDeckImportDlg;
	CImportByLocalDlg* m_pImportByLocal;
	CSplitButton m_ctrlImportSpl;
	CSplitButton m_ctrlExportSpl;

	BOOL m_bInitMetaDeck;
public:
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void PostNcDestroy();
};
