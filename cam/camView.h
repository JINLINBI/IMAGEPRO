
// camView.h : CcamView ��Ľӿ�
//
#include "dib.h"
#pragma once
#include <opencv2/opencv.hpp>

using namespace cv;

class CcamView : public CView
{
protected: // �������л�����
	CcamView();
	DECLARE_DYNCREATE(CcamView)

// ����
public:
	CDIB m_dib;
	double m_dImageHeight;
	double m_dImageWidth;
	bool m_isCapturing = false;
	int m_iHandleId = 0;
	VideoCapture capture;		//������Ƶ������  

	CcamDoc* GetDocument() const;

// ����
public:
	Mat CcamView::Handle_Binary(Mat);
	Mat CcamView::Handle_Frame(Mat);

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CcamView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStartCam();
	afx_msg void OnStopCam();
	afx_msg void OnHandleBinary();
	afx_msg void OnHandleFrame();
	afx_msg void OnHandleOriginal();
	afx_msg void OnFileOpen();
};

#ifndef _DEBUG  // camView.cpp �еĵ��԰汾
inline CcamDoc* CcamView::GetDocument() const
   { return reinterpret_cast<CcamDoc*>(m_pDocument); }
#endif

