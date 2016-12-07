
// camView.h : CcamView 类的接口
//
#include "dib.h"
#pragma once
#include <opencv2/opencv.hpp>

using namespace cv;

class CcamView : public CView
{
protected: // 仅从序列化创建
	CcamView();
	DECLARE_DYNCREATE(CcamView)

// 特性
public:
	CDIB m_dib;
	double m_dImageHeight;
	double m_dImageWidth;
	bool m_isCapturing = false;
	int m_iHandleId = 0;
	VideoCapture capture;		//声明视频读入类  

	CcamDoc* GetDocument() const;

// 操作
public:
	Mat CcamView::Handle_Binary(Mat);
	Mat CcamView::Handle_Frame(Mat);

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CcamView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
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

#ifndef _DEBUG  // camView.cpp 中的调试版本
inline CcamDoc* CcamView::GetDocument() const
   { return reinterpret_cast<CcamDoc*>(m_pDocument); }
#endif

