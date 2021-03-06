// ImportByLocalDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "HeaderStone_Deck_Maker.h"
#include "ManaCurveDlg.h"
#include "afxdialogex.h"

#include "Card.h"

// CImportByLocalDlg 대화 상자

IMPLEMENT_DYNAMIC(CManaCurveDlg, CDialogEx)

CManaCurveDlg::CManaCurveDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MANACURVE_DLG, pParent)
{

}

CManaCurveDlg::~CManaCurveDlg()
{
}

void CManaCurveDlg::SetDeckData(std::map<CCard*, int> mapDeckList)
{
	double data[8] = { };
	const char *labels[] = { "0", "1", "2", "3", "4", "5", "6", "7+" };
	for (auto& pairDeck : mapDeckList)
	{
		if (pairDeck.first->nCost >= 7)
			data[7]++;
		else
			data[pairDeck.first->nCost]++;
	}

	CRect rt;
	GetDlgItem(IDC_FRM_MANA_CURVE)->GetClientRect(rt);
	ScreenToClient(rt);

	XYChart *c = new XYChart(rt.Width(), rt.Height());

	c->setPlotArea(50, 10, rt.Width() - 60, rt.Height() - 40, Chart::Transparent, -1, Chart::Transparent, 0xcccccc);
	c->xAxis()->setColors(Chart::Transparent);
	c->yAxis()->setColors(Chart::Transparent);
	c->xAxis()->setTitle("Mana Cost");
	c->yAxis()->setTitle("Card Count");
	c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));
	c->xAxis()->setLabelStep(1, 1);
	c->yAxis()->setLabelStep(1, 1);

	// Add a bar chart layer using the given data
	c->addBarLayer(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), RGB(211, 155, 80));
	// Output the chart
	c->makeChart();
	//include tool tip for the chart
	m_ctrlChart.setChart(c);
}

void CManaCurveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MANACURVE, m_ctrlChart);
}


BEGIN_MESSAGE_MAP(CManaCurveDlg, CDialogEx)
END_MESSAGE_MAP()


// CImportByLocalDlg 메시지 처리기


BOOL CManaCurveDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	return TRUE;  
}


void CManaCurveDlg::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	DestroyWindow();
}


void CManaCurveDlg::OnCancel()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	DestroyWindow();
}