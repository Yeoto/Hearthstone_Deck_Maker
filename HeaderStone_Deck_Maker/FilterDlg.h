#pragma once

#include "CardFilter.h"
#include "CardNotifier.h"
// CFilterDlg 대화 상자

class CFilterDlg : public CDialogEx, CCardNotifier
{
	DECLARE_DYNAMIC(CFilterDlg)

public:
	CFilterDlg(CCardFilter* pFilter, CWnd* pParent = nullptr);   // 표준 생성자입니다.
	CFilterDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CFilterDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_FILTER_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL ExecuteNotify(NOTIFYMSG eSender, WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();

	afx_msg void OnClickReset();

	virtual void OnOK();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	virtual void OnCancel();

	afx_msg void OnClickCardTypeChk();
	afx_msg void OnClickCardSetChk();
	afx_msg void OnClickCardRarityChk();
	afx_msg void OnClickCardClassChk();
	afx_msg void OnClickCardRaceChk();
	afx_msg void OnClickCardCostChk();
	afx_msg void OnClickCardAttackChk();
	afx_msg void OnClickCardHealthChk();
	afx_msg void OnClickCardTextChk();

public:
	void Dlg2Data();
	void Data2Dlg();

private:
	CCardFilter m_ModFilter;
	CCardFilter* m_pFilter;

	CButton m_ctrlBookmarkChk;
	CButton m_ctrlCardTypeChk;
	CButton m_ctrlCardSetChk;
	CButton m_ctrlCardRarityChk;
	CButton m_ctrlCardClassChk;
	CButton m_ctrlCardRaceChk;
	CButton m_ctrlCardCostChk;
	CButton m_ctrlCardAttackChk;
	CButton m_ctrlCardHealthChk;
	CButton m_ctrlCardTextChk;

	CButton m_ctrlCardType[4];		//Card Type : minions, spell, weapon, hero
	CButton m_ctrlCardSet_Regular[5];	//Card Set : 오리, 마녀숲 , 폭심, 라스타칸, 어둠의반격
	CButton m_ctrlCardSet_Wild[12];	//Card Set :	명전, 낙스, 고노 검바
									//				마상, 탐험가, 고대신, 카라잔
									//				가젯잔, 운고로, 얼왕기, 코볼트
	CButton m_ctrlCardRarity[5];		//Card Rarity : 무료, 일반, 희귀, 특급, 전설
	CButton m_ctrlCardClass[10];		//Card Class :	노루, 앰오, 법사, 좆사, 사제
									//				돚거, 씹랄, 흑마, 치킨, 중립
	CButton m_ctrlCardRace[8];		//Card Race :	멀록, 악마, 야수, 용족
									//				토템, 해적, 기계, 정령
	CEdit m_ctrlCardCost[2];		// From, To
	CEdit m_ctrlCardAttack[2];		// From, To
	CEdit m_ctrlCardHealth[2];		// From, To

	CEdit m_ctrlText;
	
public:
	virtual BOOL OnInitDialog();
};
