
// camView.cpp : CcamView 类的实现
//

#include "stdafx.h"

// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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
	// 标准打印命令
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

// CcamView 构造/析构

CcamView::CcamView()
{
	// TODO: 在此处添加构造代码

}

CcamView::~CcamView()
{
}

BOOL CcamView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CcamView 绘制

void CcamView::OnDraw(CDC* pDC)
{
	CcamDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	if (m_dib.m_bLoaded == true)  //判断是否加载图像
	{
		//获取图像宽和高
		int nw = m_dib.GetDIBWidth();
		int nh = m_dib.GetDIBHeight();
		// 显示图像（具体的参数见CDIB类的该函数说明）
		m_dib.ShowDIB(pDC, 10, 10, nw, nh, m_dib.m_pDIBData, m_dib.m_pBMI);
		m_dib.ShowDIB(pDC, 400, 10, nw, nh, m_dib.m_pDumpDIBData, m_dib.m_pBMI);
	}
	if (m_isCapturing)
	{
		Mat cap;					//定义一个Mat变量，用于存储每一帧的图像  
		capture >> cap;				//读取当前帧  
		if (!cap.empty()) {			//判断当前帧是否捕捉成功 **这步很重要  
			switch (m_iHandleId)	//读取当前处理的状态号码
			{
			case 1:cap = Handle_Binary(cap); break;//进行二值化处理
			case 0:
			default:
				break;
			}
			imshow("摄像头内容", cap);		//若当前帧捕捉成功，显示  
		}

		else {
			printf("error!");
		}

		waitKey(30); //延时30毫秒  
	}
}


// CcamView 打印

BOOL CcamView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CcamView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CcamView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CcamView 诊断

#ifdef _DEBUG
void CcamView::AssertValid() const
{
	CView::AssertValid();
}

void CcamView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CcamDoc* CcamView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CcamDoc)));
	return (CcamDoc*)m_pDocument;
}
#endif //_DEBUG


// CcamView 消息处理程序


void CcamView::OnStartCam()
{
	// TODO: 在此添加命令处理程序代码
	
	capture.open(0);			//从摄像头读入视频 0表示从摄像头读入  
	m_dImageHeight = capture.get(CAP_PROP_FRAME_HEIGHT);
	m_dImageWidth  = capture.get(CAP_PROP_FRAME_WIDTH);
	if (!capture.isOpened())	//先判断是否打开摄像头  
	{
		printf("can not open camera!");
	}
	namedWindow("摄像头内容");
	m_isCapturing = true;

}


void CcamView::OnStopCam()
{
	// TODO: 在此添加命令处理程序代码
	m_isCapturing = false;
}
Mat CcamView::Handle_Binary(Mat cap)	//二值化处理函数,自己动手写
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
	// TODO: 在此添加命令处理程序代码
	m_iHandleId = 1;
}

void CcamView::OnHandleFrame()
{
	// TODO: 在此添加命令处理程序代码
	m_iHandleId = 2;
}


void CcamView::OnHandleOriginal()
{
	// TODO: 在此添加命令处理程序代码
	m_iHandleId = 0;
}

void CcamView::OnFileOpen()
{
	// TODO: 在此添加命令处理程序代码
	static char szFilter[] = "BMP文件(*.bmp)|*.bmp||";  //定义过滤文件的类型
	CFileDialog dlg(TRUE, "bmp", NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);//定义文件对话框对象
	CString filename;
	int ret = dlg.DoModal();  //运行打开文件对方框
	if (ret == IDOK)
	{
		filename = dlg.GetPathName();     //获取所选择图像的路径
		m_dib.LoadFromFile(filename);   //加载图像
		if (!m_dib.m_bLoaded)            //判断是否加载图像成功
		{
			AfxMessageBox("图像打不开");
			return;
		}
	}
	Invalidate(1);	//刷新屏幕
					// TODO: 在此添加命令处理程序代码
}
