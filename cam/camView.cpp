
// camView.cpp : CcamView ���ʵ��
//

#include "stdafx.h"

// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#include <opencv2/opencv.hpp>
using namespace cv;
#ifndef SHARED_HANDLERS
#include "cam.h"
#endif

#include "camDoc.h"
#include "camView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CcamView

IMPLEMENT_DYNCREATE(CcamView, CView)

BEGIN_MESSAGE_MAP(CcamView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_START_CAM, &CcamView::OnStartCam)
	ON_COMMAND(ID_STOP_CAM, &CcamView::OnStopCam)
	ON_COMMAND(ID_HANDLE_BINARY, &CcamView::OnHandleBinary)
	ON_COMMAND(ID_HANDLE_FRAME, &CcamView::OnHandleFrame)
	ON_COMMAND(ID_HANDLE_ORIGINAL, &CcamView::OnHandleOriginal)
	ON_COMMAND(ID_FILE_OPEN, &CcamView::OnFileOpen)
END_MESSAGE_MAP()

// CcamView ����/����

CcamView::CcamView()
{
	// TODO: �ڴ˴���ӹ������

}

CcamView::~CcamView()
{
}

BOOL CcamView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CcamView ����

void CcamView::OnDraw(CDC* pDC)
{
	CcamDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	if (m_dib.m_bLoaded == true)  //�ж��Ƿ����ͼ��
	{
		//��ȡͼ���͸�
		int nw = m_dib.GetDIBWidth();
		int nh = m_dib.GetDIBHeight();
		// ��ʾͼ�񣨾���Ĳ�����CDIB��ĸú���˵����
		m_dib.ShowDIB(pDC, 10, 10, nw, nh, m_dib.m_pDIBData, m_dib.m_pBMI);
		m_dib.ShowDIB(pDC, 400, 10, nw, nh, m_dib.m_pDumpDIBData, m_dib.m_pBMI);
	}
	if (m_isCapturing)
	{
		Mat cap;					//����һ��Mat���������ڴ洢ÿһ֡��ͼ��  
		capture >> cap;				//��ȡ��ǰ֡  
		if (!cap.empty()) {			//�жϵ�ǰ֡�Ƿ�׽�ɹ� **�ⲽ����Ҫ  
			switch (m_iHandleId)	//��ȡ��ǰ�����״̬����
			{
			case 1:cap = Handle_Binary(cap); break;//���ж�ֵ������
			case 0:
			default:
				break;
			}
			imshow("����ͷ����", cap);		//����ǰ֡��׽�ɹ�����ʾ  
		}

		else {
			printf("error!");
		}

		waitKey(30); //��ʱ30����  
	}
}


// CcamView ��ӡ

BOOL CcamView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CcamView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CcamView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CcamView ���

#ifdef _DEBUG
void CcamView::AssertValid() const
{
	CView::AssertValid();
}

void CcamView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CcamDoc* CcamView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CcamDoc)));
	return (CcamDoc*)m_pDocument;
}
#endif //_DEBUG


// CcamView ��Ϣ�������


void CcamView::OnStartCam()
{
	// TODO: �ڴ���������������
	
	capture.open(0);			//������ͷ������Ƶ 0��ʾ������ͷ����  
	m_dImageHeight = capture.get(CAP_PROP_FRAME_HEIGHT);
	m_dImageWidth  = capture.get(CAP_PROP_FRAME_WIDTH);
	if (!capture.isOpened())	//���ж��Ƿ������ͷ  
	{
		printf("can not open camera!");
	}
	namedWindow("����ͷ����");
	m_isCapturing = true;

}


void CcamView::OnStopCam()
{
	// TODO: �ڴ���������������
	m_isCapturing = false;
}
Mat CcamView::Handle_Binary(Mat cap)	//��ֵ��������,�Լ�����д
{
	uchar *p;
	for (int i = 0; i < m_dImageHeight; i++) {
		p = cap.ptr<uchar> (i);
		for (int j = 0; j < m_dImageWidth*3 ; j+=3) {
			if (p[j] + p[j + 1] + p[j + 2] > 256 / 2) {
				p[j] = 255;
				p[j + 1] = 255;
				p[j + 2] = 255;
			}
				
			else {
				p[j] = 0;
				p[j + 1] = 0;
				p[j + 2] = 0;
			}
				
		}
	}
	return cap;
}
Mat CcamView::Handle_Frame(Mat cap) {
	return cap;
}


void CcamView::OnHandleBinary()
{
	// TODO: �ڴ���������������
	m_iHandleId = 1;
}

void CcamView::OnHandleFrame()
{
	// TODO: �ڴ���������������
	m_iHandleId = 2;
}


void CcamView::OnHandleOriginal()
{
	// TODO: �ڴ���������������
	m_iHandleId = 0;
}

void CcamView::OnFileOpen()
{
	// TODO: �ڴ���������������
	static char szFilter[] = "BMP�ļ�(*.bmp)|*.bmp||";  //��������ļ�������
	CFileDialog dlg(TRUE, "bmp", NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);//�����ļ��Ի������
	CString filename;
	int ret = dlg.DoModal();  //���д��ļ��Է���
	if (ret == IDOK)
	{
		filename = dlg.GetPathName();     //��ȡ��ѡ��ͼ���·��
		m_dib.LoadFromFile(filename);   //����ͼ��
		if (!m_dib.m_bLoaded)            //�ж��Ƿ����ͼ��ɹ�
		{
			AfxMessageBox("ͼ��򲻿�");
			return;
		}
	}
	Invalidate(1);	//ˢ����Ļ
					// TODO: �ڴ���������������
}
