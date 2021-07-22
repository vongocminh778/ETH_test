// Author:      SFr frydaysoft@gmx.de
// Download by http://www.NewXing.com
#include "stdafx.h"
#include "ColorListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
CHeaderCtrlCx::CHeaderCtrlCx()
: m_R(171)
, m_G(199)
, m_B(235)
, m_Gradient(8)
{
	m_Format = "";
	m_Height = 1;
	m_fontHeight = 15;
	m_fontWith = 0;
	m_color = RGB(0,0,0);
}

CHeaderCtrlCx::~CHeaderCtrlCx()
{
}


BEGIN_MESSAGE_MAP(CHeaderCtrlCx, CHeaderCtrl)
	ON_WM_PAINT()
	ON_MESSAGE(HDM_LAYOUT, OnLayout)
END_MESSAGE_MAP()



// CHeaderCtrlCl 消息处理程序



void CHeaderCtrlCx::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CHeaderCtrl::OnPaint()
	int nItem; 
	nItem = GetItemCount();//得到有几个单元 
	for(int i = 0; i<nItem;i ++) 
	{ 
		CRect tRect;
		GetItemRect(i,&tRect);//得到Item的尺寸
		int R = m_R,G = m_G,B = m_B;
		CRect nRect(tRect);//拷贝尺寸到新的容器中 
		nRect.left++;//留出分割线的地方 
		//绘制立体背景 
		for(int j = tRect.top;j<=tRect.bottom;j++) 
		{ 
			nRect.bottom = nRect.top+1; 
			CBrush _brush; 
			_brush.CreateSolidBrush(RGB(R,G,B));//创建画刷 
			dc.FillRect(&nRect,&_brush); //填充背景 
			_brush.DeleteObject(); //释放画刷 
			R-=m_Gradient;G-=m_Gradient;B-=m_Gradient;
			if (R<0)R = 0;
			if (G<0)G = 0;
			if (B<0)B= 0;
			nRect.top = nRect.bottom; 
		} 
		dc.SetBkMode(TRANSPARENT);
		CFont nFont ,* nOldFont; 
		//dc.SetTextColor(RGB(250,50,50)); 
		dc.SetTextColor(m_color);
		nFont.CreateFont(m_fontHeight,m_fontWith,0,0,0,FALSE,FALSE,0,0,0,0,0,0,_TEXT("宋体"));//创建字体 
		nOldFont = dc.SelectObject(&nFont);

		UINT nFormat = 1;
		if (m_Format[i]=='0')
		{
			nFormat = DT_LEFT;
			tRect.left+=3;
		}
		else if (m_Format[i]=='1')
		{
			nFormat = DT_CENTER;
		}
		else if (m_Format[i]=='2')
		{
			nFormat = DT_RIGHT;
			tRect.right-=3;
		}
		TEXTMETRIC metric;
		dc.GetTextMetrics(&metric);
		int ofst = 0;
		ofst = tRect.Height() - metric.tmHeight;
		tRect.OffsetRect(2,ofst/2);//(0,ofst/2);
		
		dc.DrawText(m_HChar[i],&tRect,nFormat);
		dc.SelectObject(nOldFont); 
		nFont.DeleteObject(); //释放字体 
	} 
	//画头部剩余部分
	CRect rtRect;
	CRect clientRect;
	GetItemRect(nItem - 1,rtRect);
	GetClientRect(clientRect);
	rtRect.left = rtRect.right+1;
	rtRect.right = clientRect.right;
	int R = m_R,G = m_G,B = m_B;
	CRect nRect(rtRect);
	//绘制立体背景 
	for(int j = rtRect.top;j<=rtRect.bottom;j++) 
	{ 
		nRect.bottom = nRect.top+1; 
		CBrush _brush; 
		_brush.CreateSolidBrush(RGB(R,G,B));//创建画刷 
		dc.FillRect(&nRect,&_brush); //填充背景 
		_brush.DeleteObject(); //释放画刷 
		R-=m_Gradient;G-=m_Gradient;B-=m_Gradient;
		if (R<0)R = 0;
		if (G<0)G = 0;
		if (B<0)B= 0;
		nRect.top = nRect.bottom; 
	} 
}
LRESULT CHeaderCtrlCx::OnLayout( WPARAM wParam, LPARAM lParam )
{
	LRESULT lResult = CHeaderCtrl::DefWindowProc(HDM_LAYOUT, 0, lParam); 
	HD_LAYOUT &hdl = *( HD_LAYOUT * ) lParam; 
	RECT *prc = hdl.prc; 
	WINDOWPOS *pwpos = hdl.pwpos; 

	//表头高度为原来1.5倍，如果要动态修改表头高度的话，将1.5设成一个全局变量 
	int nHeight = (int)(pwpos->cy * m_Height);
	pwpos->cy = nHeight; 
	prc->top = nHeight; 
	return lResult; 
}
//////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CColorListCtrl
CColorListCtrl::CColorListCtrl()
{
    m_fullColumnLines=TRUE;     // paint vert. lines | | |
    m_withSingleRect=FALSE;     // paint single rect for all sub-items
#ifdef _DEBUG
    m_BigFocusRect=TRUE;        // focus-rect with small/big dots
#endif
#ifndef _DEBUG
    m_BigFocusRect=FALSE;       // focus-rect with small/big dots
#endif
    IsCreated=FALSE;
	
	m_nRowHeight = 0;
}

CColorListCtrl::CColorListCtrl(int colSize)
{
    m_columnSize=colSize;       // number of columns
    m_fullColumnLines=TRUE;     // paint vert. lines | | |
    m_withSingleRect=FALSE;     // paint single rect for all sub-items
#ifdef _DEBUG
    m_BigFocusRect=TRUE;        // focus-rect with small/big dots
#endif
#ifndef _DEBUG
    m_BigFocusRect=FALSE;       // focus-rect with small/big dots
#endif
    IsCreated=FALSE;

	m_nRowHeight = 0;
}

//***************************************************************
CColorListCtrl::~CColorListCtrl()
{
}

//***************************************************************
BEGIN_MESSAGE_MAP(CColorListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CColorListCtrl)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_WM_MEASUREITEM()
	ON_WM_MEASUREITEM_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen f黵 Nachrichten CColorListCtrl 
void CColorListCtrl::OnDestroy() 
{
	DeleteAllItems();	
	CListCtrl::OnDestroy();
}

//***************************************************************
void CColorListCtrl::SetColumnType(BOOL bNew)
{
    m_fullColumnLines=bNew;     // paint no/vert. lines | | |
    if (IsCreated) 
    {
        RedrawWindow();
    }
}
void CColorListCtrl::SetRectType(BOOL bNew)
{
    m_withSingleRect=bNew;     // paint no/single rect for all sub-items
    if (IsCreated)
    {
        RedrawWindow();
    }
}
void CColorListCtrl::SetFocusType(BOOL bNew)
{
    m_BigFocusRect=bNew;       // focus-rect with small/big dots
    if (IsCreated)
    {
        RedrawWindow();
    }
}


void CColorListCtrl::SetRowHeigt(int nHeight) //高置行高
{
	m_nRowHeight = nHeight;
	
	CRect rcWin;
	GetWindowRect(&rcWin);
	WINDOWPOS wp;
	wp.hwnd = m_hWnd;
	wp.cx = rcWin.Width();
	wp.cy = rcWin.Height();
	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	SendMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp);
}

void CColorListCtrl::SetColSize(int nSize)
{
	m_columnSize=nSize;       // number of columns
}

void CColorListCtrl::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	CListCtrl::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}
void CColorListCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if (m_nRowHeight>0)
	{
		lpMeasureItemStruct->itemHeight = m_nRowHeight;
	}
}

//***************************************************************
CMyLParam*	CColorListCtrl::GetMyLParam(int Index)
{
    LVITEM	Item;
	Item.mask		= LVIF_PARAM;
	Item.iItem		= Index;
	Item.iSubItem	= 0;
	CListCtrl::GetItem(&Item);
	return (CMyLParam*)Item.lParam;
}

//***************************************************************
CRect CColorListCtrl::GetFullCellRect(int nItem, BOOL outside)
{
	static CRect rectCell;
    CListCtrl::GetItemRect(nItem, rectCell, LVIR_BOUNDS);
    if (!outside)
    {   //decrease 1 pixel
        rectCell.left++;    rectCell.right--;
        rectCell.top++;     rectCell.bottom--;
    }
	return rectCell;
}

//***************************************************************
CRect CColorListCtrl::GetCellRect(int nItem, int nColumn, BOOL outside)
{
	static CRect rectCell;

	// Get the height
    CListCtrl::GetItemRect(nItem, rectCell, LVIR_BOUNDS);
    LV_COLUMN column;
	column.mask = LVCF_WIDTH;        // Set the mask to only return the width

    for (int x=0; x<nColumn; x++)
    {
        column.iSubItem=1;
        CListCtrl::GetColumn(x, &column); // Get rect-edge (left/top)
        rectCell.left+=column.cx;
    }
    column.iSubItem=1;
    CListCtrl::GetColumn(nColumn, &column); // Get rect-edge (right/bottom)
    rectCell.right = rectCell.left + column.cx;

    if (!outside)
    {   //decrease 1 pixel
        rectCell.left++;    rectCell.right--;
        rectCell.top++;     rectCell.bottom--;
    }
	return rectCell;
}

//***************************************************************
void CColorListCtrl::InvalidateFullItem(int nItem, BOOL bErase)
{
    CRect rcBounds=GetFullCellRect(nItem, TRUE);
	InvalidateRect(rcBounds, bErase);

}

//***************************************************************
// create ListCtrl-head
int CColorListCtrl::InitCtrl(CStringArray *phead, CWordArray *pProcent)
{
	int x = 0;
    if ( (!phead)||(!pProcent)) return 1;
	LV_COLUMN lvc;
	int cx=0;
    int count = phead->GetSize(); 
    if (count!=m_columnSize) return 2;
    if (pProcent->GetSize() != count) return 3;
    //for (int x=0; x<count; x++) cx+=pProcent->GetAt(x);
    //if (cx!=100) return 4;

	CRect rect_listbox;

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	GetWindowRect( rect_listbox );
	cx=int((rect_listbox.right-rect_listbox.left)-20); //some Pixel for Scrollbar

    //set head-columns in ListBox
	lvc.iSubItem = 0;
	lvc.fmt = LVCFMT_LEFT ;	
	
	
    for (x=0; x<count; x++)
    {
        //lvc.cx = int(cx*pProcent->GetAt(x)/10);//100);
		lvc.cx = pProcent->GetAt(x);
	    lvc.pszText = (char*)LPCSTR(phead->GetAt(x));
		m_Header.m_HChar.Add(lvc.pszText);
		if (lvc.fmt==LVCFMT_LEFT)
		{
			m_Header.m_Format = m_Header.m_Format + "0";
		}
		else if (lvc.fmt==LVCFMT_CENTER)
		{
			m_Header.m_Format = m_Header.m_Format + "1";
		}
		else if (lvc.fmt==LVCFMT_RIGHT)
		{
			m_Header.m_Format = m_Header.m_Format + "2";
		}
		else
		{
			m_Header.m_Format = m_Header.m_Format + "1";
		}
	    CListCtrl::InsertColumn(x,&lvc);
    }
    return 0;
}

// Gradient - 渐变系数，立体背景用,不用渐变设为0
void CColorListCtrl::SetHeaderBKColor(int R, int G, int B, int Gradient) //设置表头背景色
{
	m_Header.m_R = R;
	m_Header.m_G = G;
	m_Header.m_B = B;
	m_Header.m_Gradient = Gradient;
}

// 设置表头高度
void CColorListCtrl::SetHeaderHeight(float Height) //设置表头高度
{
	m_Header.m_Height = Height;
}

void CColorListCtrl::SetHeaderFontHW(int nHeight,int nWith) //设置头部字体宽和高
{
	m_Header.m_fontHeight = nHeight;
	m_Header.m_fontWith = nWith;
}
void CColorListCtrl::SetHeaderTextColor(COLORREF color) //设置头部字体颜色
{
	m_Header.m_color = color;
}

//***************************************************************
// add an item
int CColorListCtrl::AddItem(CStringArray *line, int pos)
{
    int ret_index=LISTCTRL_ERROR;
    if (!line) return ret_index;

	LV_ITEM lvi;
    
	//set List-properties
	lvi.mask = LVIF_TEXT ; 
	lvi.iSubItem = 0;
    lvi.pszText = "";

    int count=GetItemCount();
    int columns=line->GetSize();

    if (m_columnSize!=columns) return ret_index;

    if (DWORD(pos)<=DWORD(count)) count=pos;
	
    lvi.iItem = count;
    //lvi.lParam= count;
	lvi.mask = LVIF_TEXT ; 
	lvi.iSubItem = 0;   //refer to no Item
    // insert an new line
    ret_index=InsertItem(&lvi);
	//SetItemState(ret_index,LVNI_FOCUSED | LVIS_SELECTED, LVNI_FOCUSED | LVIS_SELECTED); //设置行index为选中并高亮（如果未设置Focuse，被选中行会变为灰色）
	//SetItemState(ret_index,LVNI_FOCUSED, LVNI_FOCUSED); 
	EnsureVisible(ret_index, FALSE); 
    // set item text for additional columns		
    for (int x=0; x<columns; x++)
    {
        CListCtrl::SetItemText(count,x,(char*)LPCSTR(line->GetAt(x)) );
    }
	lvi.mask = LVIF_STATE;
	lvi.iSubItem = 1;
	lvi.stateMask = 0xFFFFFFFF ; 
	lvi.state = 0;

	if (CListCtrl::SetItem(&lvi)) 
    {
        CMyLParam *p=new CMyLParam(m_columnSize); //init with: DEF_BACKGROUND, def_text

        #ifdef TESTCOLOR
            // only for TEST
            if (ret_index<3)
            {
                p->SetBackColor(ITEM_COLOR(ret_index+BLUE),0);
                p->SetTextColor(ITEM_COLOR(ret_index+RED ),0);
            }
            else
            if (ret_index<=6)
            {
                p->SetBackColor(ITEM_COLOR(ret_index+BLUE),1);
                p->SetTextColor(ITEM_COLOR(ret_index+RED ),1);
            }
            else
            {
                p->SetBackColor(ITEM_COLOR((ret_index+BLUE)&0xF),2);
                p->SetTextColor(ITEM_COLOR((ret_index+RED )&0xF),2);
            }
            // end: only for TEST
        #endif

        CListCtrl::SetItemData( ret_index, DWORD(p) );
        return ret_index;
    }
    return LISTCTRL_ERROR;
}

//***************************************************************
BOOL CColorListCtrl::DeleteAllItems()
{
	while (CListCtrl::GetItemCount())
        CColorListCtrl::DeleteItem(0);

    return TRUE;
}

//***************************************************************
BOOL CColorListCtrl::DeleteItem(int nItem )
{
    if (CListCtrl::GetItemCount()<=nItem) return FALSE;
    CMyLParam *p=(CMyLParam*)CListCtrl::GetItemData(nItem);
    if (p)
    {
        CListCtrl::SetItemData( nItem, p->GetData() );
        delete p;
        return CListCtrl::DeleteItem( nItem );
    }
    return TRUE;
}


//***************************************************************
DWORD CColorListCtrl::GetItemData( int nItem ) const
{
    CMyLParam *p=(CMyLParam*)CListCtrl::GetItemData(nItem);
    if (p)
    {
        return p->GetData();
        return TRUE;
    }
    return 0;
}

//***************************************************************
BOOL  CColorListCtrl::SetItemData( int nItem, DWORD dwData )
{
    CMyLParam *p=(CMyLParam*)CListCtrl::GetItemData(nItem);
    if (p)
    {
        p->SetData(dwData);
        return TRUE;
    }
    return FALSE;
}

//***************************************************************
BOOL CColorListCtrl::SetItemBackgndColor(ITEM_COLOR color, int Index, int iSub)	
{
    CMyLParam *p=GetMyLParam(Index);
    if (!p) return FALSE;
    if ( p->SetBackColor(color, iSub) )
    {
        RedrawItems( Index, Index );
        return TRUE;
    }
    return FALSE;
}

//***************************************************************
BOOL CColorListCtrl::SetItemTextColor(ITEM_COLOR color, int Index, int iSub)	
{
    CMyLParam *p=GetMyLParam(Index);
    if (!p) return FALSE;
    if ( p->SetTextColor(color, iSub) )
    {
        RedrawItems( Index, Index );
        return TRUE;
    }
    return FALSE;
}

//***************************************************************
COLORREF CColorListCtrl::GetItemBackgndColor(int Index, int iSub)
{
    CMyLParam *p=GetMyLParam(Index);
    if (!p) return 0;
    return p->GetBackColor(iSub);
}

//***************************************************************
COLORREF CColorListCtrl::GetItemTextColor(int Index, int iSub)
{
    CMyLParam *p=GetMyLParam(Index);
    if (!p) return 0;
    return p->GetTextColor(iSub);
}

//***************************************************************
void CColorListCtrl::OnPaint() 
{
	// First let the control do its default drawing.
	const MSG *msg = GetCurrentMessage();
	DefWindowProc( msg->message, msg->wParam, msg->lParam );

    if (!m_fullColumnLines) return;

	// Draw the lines only for LVS_REPORT mode
	if( (GetStyle() & LVS_TYPEMASK) == LVS_REPORT )
	{
		// Get the number of columns
		CClientDC dc(this );
		CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
		int nColumnCount = pHeader->GetItemCount();

		// The bottom of the header corresponds to the top of the line 
		RECT rect;
		pHeader->GetClientRect( &rect );
		int top = rect.bottom;

		// Now get the client rect so we know the line length and
		// when to stop
		GetClientRect( &rect );

		// The border of the column is offset by the horz scroll
		int borderx = 0 - GetScrollPos( SB_HORZ );

        CPen       *pOldPen;
        CPen        pen;
        CGdiObject *pOldBrush;
        pen.CreatePen( PS_DOT, 0, GetColorRef(DEF_DESELTEXT) );
        pOldPen  =dc.SelectObject(&pen);
        pOldBrush=dc.SelectStockObject(NULL_BRUSH);

		for( int i = 0; i < nColumnCount; i++ )
		{
			// Get the next border
			borderx += GetColumnWidth( i );

			// if next border is outside client area, break out
			if( borderx >= rect.right ) break;

			// Draw the line.
			dc.MoveTo( borderx-1, top);
			dc.LineTo( borderx-1, rect.bottom );
		}
        dc.SelectObject(pOldPen);
        dc.SelectObject(pOldBrush);

	}
	// Do not call CListCtrl::OnPaint() for painting messages
}

//***************************************************************
void CColorListCtrl::RepaintSelectedItems()
{
	// Invalidate focused item so it can repaint 
	int nItem = GetNextItem(-1, LVNI_FOCUSED);

	if(nItem != -1)
	{
        InvalidateFullItem(nItem, FALSE);
	}

	// Invalidate selected items depending on LVS_SHOWSELALWAYS
	if((GetStyle() & LVS_SHOWSELALWAYS))
	{
		for(nItem = GetNextItem(-1, LVNI_SELECTED);
			nItem != -1; nItem = GetNextItem(nItem, LVNI_SELECTED))
		{
            InvalidateFullItem(nItem, FALSE);
		}
	}
	UpdateWindow();
}

//***************************************************************
BOOL CColorListCtrl::SetSel(int pos, BOOL bSelect)
{
    // LVS_ES_FULLROWSELECT only works with newer version of Comctl32
    // This style and some of the other new extended styles 
    // only work with version 4.72 (or higer) of COMCTL32.DLL 

    UINT nState=0;
    if (bSelect) nState=LVIS_SELECTED|LVS_EX_FULLROWSELECT; else nState=0;
    if (SetItemState(pos, nState, LVIS_SELECTED|LVS_EX_FULLROWSELECT))
    {
        if((GetStyle() & LVS_SHOWSELALWAYS))
        {
            InvalidateFullItem(pos, TRUE);
        }
        return TRUE;
    }
    return FALSE;
}
//***************************************************************
void CColorListCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	SendMessage(WM_PAINT);
    CListCtrl::OnKillFocus(pNewWnd);
	// check if we are losing focus
	if(pNewWnd != NULL && pNewWnd->GetParent() == this)
		return;

	// repaint items that should change appearance
	if((GetStyle() & LVS_TYPEMASK) == LVS_REPORT)
		RepaintSelectedItems();
}

//***************************************************************
void CColorListCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	CListCtrl::OnSetFocus(pOldWnd);
	
	// check if we are getting focus
	if(pOldWnd!=NULL && pOldWnd->GetParent()==this)
		return;

	// repaint items that should change appearance
	if((GetStyle() & LVS_TYPEMASK)==LVS_REPORT)
		RepaintSelectedItems();
}

void CColorListCtrl::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	//BOOL bFlag = ModifyStyle(0,LVS_OWNERDRAWFIXED);
	CListCtrl::PreSubclassWindow();
	CHeaderCtrl *pHeader = GetHeaderCtrl();
	m_Header.SubclassWindow(pHeader->GetSafeHwnd());
}

//***************************************************************
void CColorListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
    if (!lpDrawItemStruct) return;
    IsCreated=TRUE;
	CDC*		pDC			= CDC::FromHandle(lpDrawItemStruct->hDC);
	CMyLParam*	pMyLParam	= GetMyLParam(lpDrawItemStruct->itemID);
	CRect		rcItem;

    if (!pMyLParam) return;
    int x=0;
    typedef enum {R_NOTHINGS=0, R_FOCUS, R_SELECT, R_NORMAL, R_SEL_NOFOCUS} redrawmode;
    redrawmode redraw=R_NORMAL;

	// Get item image and state info
	LV_ITEM lvi;
	lvi.mask = LVIF_IMAGE | LVIF_STATE;
	lvi.iItem = lpDrawItemStruct->itemID;
	lvi.iSubItem = 0;
	lvi.stateMask = 0xFFFF;		// get all state flags
	GetItem(&lvi);

	BOOL bHighlight =   (   (lvi.state & LVIS_DROPHILITED)
                            ||
				            (
                                (lvi.state & LVIS_SELECTED) 
                                && 
                                (  ( GetFocus() == this )  || ( GetStyle() & LVS_SHOWSELALWAYS ) )
					        )
				        );

    BOOL bLostFocus=(GetFocus()!=this);


    if ( (lpDrawItemStruct->itemAction&ODA_FOCUS)==ODA_FOCUS)
    {   //the control gains or loses input focus
        if (bLostFocus) redraw=R_SEL_NOFOCUS;
        else
        if ((lvi.state&LVIS_FOCUSED)==LVIS_FOCUSED) redraw=R_FOCUS;
        else 
        {
            if (bHighlight) redraw=R_SELECT;
            else redraw=R_NORMAL;
        }
    }
    else
    if ((lpDrawItemStruct->itemAction&ODA_SELECT)==ODA_SELECT)
    {   //the selection status has changed
            if (bHighlight)
            {
                if (bLostFocus) redraw=R_SEL_NOFOCUS;
                 else redraw=R_SELECT;
            }
            else redraw=R_NORMAL;
    }
    else
    {   //redraw the item
        if (bLostFocus)
        {
            if (bHighlight) redraw=R_SEL_NOFOCUS;
             else
              redraw=R_NORMAL;
        }
        else 
        {
            if ((lvi.state&LVIS_FOCUSED)==LVIS_FOCUSED) redraw=R_FOCUS;
            else 
            {
                if (bHighlight) redraw=R_SELECT;
                 else redraw=R_NORMAL;
            }
        }
    }

    CPen       *pOldPen;
    CPen        pen;
    CGdiObject *pOldBrush;
	CRect      RectTmp;

    switch(redraw)
    {

        case R_FOCUS:
            rcItem=GetFullCellRect(lpDrawItemStruct->itemID, TRUE);
            pDC->FillSolidRect(rcItem, GetColorRef(DEF_DESELBACKGROUND));//RED));//DEF_SELBACKGROUND));
            pDC->SetTextColor(GetColorRef(DEF_SELTEXT));
            if (m_BigFocusRect)
            {
                pen.CreatePen( PS_DOT, 0, GetColorRef(DEF_SELTEXT) );
                pOldPen  =pDC->SelectObject(&pen);
                pOldBrush=pDC->SelectStockObject(NULL_BRUSH);
                pDC->Rectangle(rcItem);
                pDC->SelectObject(pOldPen);
                pDC->SelectObject(pOldBrush);
            }
            else pDC->DrawFocusRect(rcItem);
            for (x=0; x<m_columnSize; x++)
            {
                rcItem	= GetCellRect(lpDrawItemStruct->itemID, x, FALSE);
				RectTmp = rcItem;
				RectTmp.left += 5;
                pDC->DrawText(CListCtrl::GetItemText(lpDrawItemStruct->itemID,x), RectTmp, DT_SINGLELINE | DT_LEFT | DT_VCENTER );
            }
            break;
        case R_SELECT:
            rcItem=GetFullCellRect(lpDrawItemStruct->itemID, TRUE);
            pDC->FillSolidRect(rcItem, GetColorRef(DEF_SELBACKGROUND));
            pDC->SetTextColor(GetColorRef(DEF_SELTEXT));
            for (x=0; x<m_columnSize; x++)
            {
                rcItem	= GetCellRect(lpDrawItemStruct->itemID, x, FALSE);
				RectTmp = rcItem;
				RectTmp.left += 5;
                pDC->DrawText(CListCtrl::GetItemText(lpDrawItemStruct->itemID,x), RectTmp, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
            }
            break;
        case R_SEL_NOFOCUS:
            rcItem=GetFullCellRect(lpDrawItemStruct->itemID, TRUE);
            pDC->FillSolidRect(rcItem, GetColorRef(DEF_DESELBACKGROUND));//RED));//DEF_DESELBACKGROUND));
            pDC->SetTextColor(pMyLParam->GetTextColor(DEF_DESELTEXT));
            pen.CreatePen( PS_DOT, 0, GetColorRef(DEF_DESELBACKGROUND));//RED));//DEF_DESELBACKGROUND) );
            pOldPen  =pDC->SelectObject(&pen);
            pOldBrush=pDC->SelectStockObject(NULL_BRUSH);
            if (!m_withSingleRect)
                pDC->Rectangle(rcItem);
            for (x=0; x<m_columnSize; x++)
            {
                rcItem	= GetCellRect(lpDrawItemStruct->itemID, x, FALSE);
				RectTmp = rcItem;
				RectTmp.left += 5;
                pDC->DrawText(CListCtrl::GetItemText(lpDrawItemStruct->itemID,x), RectTmp, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
            }
            pDC->SelectObject(pOldPen);
            pDC->SelectObject(pOldBrush);
            break;
        case R_NORMAL:
            for (x=0; x<m_columnSize; x++)
            {
	            rcItem	= GetCellRect(lpDrawItemStruct->itemID, x, TRUE);
                pDC->FillSolidRect(rcItem, pMyLParam->GetBackColor(x));
                pDC->SetTextColor(pMyLParam->GetTextColor(x));
                if (m_withSingleRect)
                {
                    pOldBrush=pDC->SelectStockObject(NULL_BRUSH);
                    pDC->Rectangle(rcItem);
                    pDC->SelectObject(pOldBrush);
                    
                }
                rcItem	= GetCellRect(lpDrawItemStruct->itemID, x, FALSE);
				RectTmp = rcItem;
				RectTmp.left += 5;
                pDC->DrawText(CListCtrl::GetItemText(lpDrawItemStruct->itemID,x), RectTmp, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
            }
            break;
    }

    return;
}

void CColorListCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CListCtrl::OnSize(nType, cx, cy);
    if (IsCreated) 
    {
        RedrawWindow();
    }
}
