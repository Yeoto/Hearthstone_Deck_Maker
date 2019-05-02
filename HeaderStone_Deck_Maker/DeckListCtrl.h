#pragma once
#include <afxwin.h>

#include <vector>
#include <map>
#include "CardData.h"
#include "CardNotifier.h"
#include "NotifyMsg.h"

class CDeckListCtrl :
	public CWnd, CCardNotifier
{
public:
	CDeckListCtrl();
	~CDeckListCtrl();

	DECLARE_MESSAGE_MAP()

public:
	BOOL Create(CRect rect, CWnd* pParent);
	virtual BOOL ExecuteNotify(NOTIFYMSG eSender, WPARAM wParam, LPARAM lParam);

private:
	std::map<CCard*, int> m_mapCards;
	int m_nStartIdx;

public:
	afx_msg void OnPaint();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};

