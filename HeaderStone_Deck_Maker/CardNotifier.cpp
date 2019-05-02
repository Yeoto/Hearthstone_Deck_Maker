#include "stdafx.h"
#include "CardNotifier.h"

std::list<CCardNotifier*> CCardNotifier::m_lInstance;


CCardNotifier::CCardNotifier(NOTIFYMSG Sender) : eMySender(Sender)
{
	m_lInstance.push_back(this);
}


CCardNotifier::~CCardNotifier()
{
	m_lInstance.remove(this);
}

BOOL CCardNotifier::SendNotify(WPARAM wParam, LPARAM lParam)
{
	//�ϳ��� �����ϸ� TRUE ����
	BOOL bReturn = FALSE;
	for (CCardNotifier* pNotifier : m_lInstance)
		if (pNotifier != this)
			bReturn |= pNotifier->ExecuteNotify(eMySender, wParam, lParam);

	return bReturn;
}
