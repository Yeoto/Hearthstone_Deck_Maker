#if !defined(__CARDNOTIFIER_H__)
#define __CARDNOTIFIER_H__

#pragma once

#include "NotifyMsg.h"
#include <list>

class CCardNotifier
{
public:
	CCardNotifier(NOTIFYMSG Sender);
	~CCardNotifier();

	BOOL SendNotify(WPARAM wParam, LPARAM lParam);
	virtual BOOL ExecuteNotify(NOTIFYMSG eSender, WPARAM wParam, LPARAM lParam) = 0;

private:
	static std::list<CCardNotifier*> m_lInstance;
	NOTIFYMSG eMySender;
};

#endif