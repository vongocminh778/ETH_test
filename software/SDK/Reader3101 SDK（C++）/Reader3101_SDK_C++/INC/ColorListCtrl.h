//Download by http://www.NewXing.com
// Author:      SFr frydaysoft@gmx.de

#ifndef AFX_COLORLISTCTRL_H__9FC7D36B_FC88_42FD_8DAA_2F70016038BA__INCLUDED_
#define AFX_COLORLISTCTRL_H__9FC7D36B_FC88_42FD_8DAA_2F70016038BA__INCLUDED_


#include "color.h"


/////////////////////////////////////////////////////////////////////////////
// class CMyLParam  

class CMyLParam  
{
public:
    CMyLParam(int column_count)
	{
		m_column_count=column_count;
        m_pBackColor=new ITEM_COLOR[column_count];
        m_pTextColor=new ITEM_COLOR[column_count];
		m_MyData= NULL;
        for (int x=0; x<m_column_count; x++)
        {
            m_pBackColor[x]=DEF_BACKGROUND;
            m_pTextColor[x]=DEF_TEXT;
        }
	}

	virtual   ~CMyLParam()                      
    {
        if (m_pBackColor) delete[] m_pBackColor;
        if (m_pTextColor) delete[] m_pTextColor;
    }

    BOOL  SetBackColor(ITEM_COLOR color, int iSub)	{   if (iSub<m_column_count) { m_pBackColor[iSub] = color; return TRUE;} else return FALSE;   }

    BOOL  SetTextColor(ITEM_COLOR color, int iSub)	{   if (iSub<m_column_count) { m_pTextColor[iSub] = color; return TRUE;} else return FALSE;   }
    
    COLORREF  GetBackColor(int iSub)  { if (iSub<m_column_count) return GetColorRef(m_pBackColor[iSub]); else return GetColorRef(DEF_BACKGROUND);}
	
    COLORREF  GetTextColor(int iSub)  { if (iSub<m_column_count) return GetColorRef(m_pTextColor[iSub]); else return GetColorRef(DEF_TEXT);}

	void	  SetData(DWORD Mydata)	            {m_MyData	= Mydata;}
	DWORD	  GetData()			                {return DWORD(m_MyData);}

protected:
	DWORD		 m_MyData;
	ITEM_COLOR	*m_pBackColor;
	ITEM_COLOR	*m_pTextColor;
    int          m_column_count;
};

//////////////////////////////////////////////////////////////////////////

class CHeaderCtrlCx : public CHeaderCtrl
{
	//DECLARE_DYNAMIC(CHeaderCtrlCx)
		
public:
	CHeaderCtrlCx();
	virtual ~CHeaderCtrlCx();
	
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	CStringArray m_HChar;
	CString m_Format; //表示对齐类型的整型数组,0表示左对齐，1表示中间对齐，2表示右对齐
public:
	int m_R;
	int m_G;
	int m_B;
	int m_Gradient;	// 画立体背景，渐变系数
	float m_Height;  //表头高度，这是倍数,
	int m_fontHeight; //字体高度
	int m_fontWith;   //字体宽度
	COLORREF m_color;
	LRESULT OnLayout( WPARAM wParam, LPARAM lParam );
};
//////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// class CColorListCtrl 

#define LISTCTRL_ERROR 0xFFFFFFFF

class CColorListCtrl : public CListCtrl
{
    int  m_columnSize;
    BOOL m_fullColumnLines;
    BOOL m_withSingleRect;
    BOOL m_BigFocusRect;
    BOOL IsCreated;

    // get Rect of single (sub)item
    CRect GetCellRect(int nItem, int nColumn, BOOL outside);
    // get Rect of Item-line
    CRect GetFullCellRect(int nItem, BOOL outside);
    // return internal data-member (Colors/ItemUserPointer)
    CMyLParam*	GetMyLParam(int Index);

    void InvalidateFullItem(int nItem, BOOL bErase=TRUE);

// Konstruktion
public:
    // colSize = number of columns
	CColorListCtrl(int colSize);

    // init the head of the CListCtrl
    int InitCtrl(CStringArray *phead, CWordArray *pProcent);
    //returns:    0: successfully
    //            1: phead or pProcent==NULL
    //            2: phead.GetSize()!=number of columns
    //            3: phead and pProcent have not same GetSize()
    //            4: sum of all elements from pProcent not 100%

    // add an item (+subitems) to the Ctrl
    int AddItem(CStringArray *line, int pos=LISTCTRL_ERROR);
    // return:   LISTCTRL_ERROR for an error else index of added item

    // delete all items from Ctrl
	BOOL	DeleteAllItems();

    // delete a single items from Ctrl
	BOOL	DeleteItem(int nItem );

    // set properties 
    void SetColumnType(BOOL bNew);
    void SetRectType(BOOL bNew);
    void SetFocusType(BOOL bNew);

    BOOL GetColumnType() {return m_fullColumnLines;}
    BOOL GetRectType()   {return m_withSingleRect;}
    BOOL GetFocusType()  {return m_BigFocusRect;}


// Attribute
public:

    // return the 32-bit value associated with the item
    DWORD GetItemData( int nItem ) const;
    // set the 32-bit value associated with the item
    BOOL  SetItemData( int nItem, DWORD dwData );


    BOOL SetItemTextColor(ITEM_COLOR color, int Index, int iSub);
    BOOL SetItemBackgndColor(ITEM_COLOR color, int Index, int iSub);
    COLORREF GetItemTextColor(int Index, int iSub);
    COLORREF GetItemBackgndColor(int Index, int iSub);
    void RepaintSelectedItems();
    BOOL SetSel(int pos, BOOL bSelect);

// Operationen
public:

	BOOL SetItemState(int nItem, LV_ITEM* pItem)
    {
	    if (LB_ERR	== nItem) return FALSE;
        InvalidateFullItem(nItem, TRUE);
        return CListCtrl::SetItemState(nItem, pItem);
    }
	BOOL SetItemState(int nItem, UINT nState, UINT nMask)
    {
	    if (LB_ERR	== nItem) return FALSE;
        InvalidateFullItem(nItem, TRUE);
        return CListCtrl::SetItemState(nItem, nState, nMask);
    }

    // these functions are disabled --> constant number of columns
	int InsertColumn(int /*nCol*/, const LV_COLUMN* /*pColumn*/){return -1;}
	int InsertColumn(int /*nCol*/, LPCTSTR /*lpszColumnHeading*/,	int /*nFormat = LVCFMT_LEFT*/, int /*nWidth = -1*/, int /*nSubItem = -1*/){return -1;}
	BOOL DeleteColumn(int /*nCol*/){return FALSE;}

	// 行高
	int m_nRowHeight;
	void SetRowHeigt(int nHeight); //设置行高
	void SetColSize(int nSize);
	// Gradient - 渐变系数，立体背景用,不用渐变设为0
	void SetHeaderBKColor(int R, int G, int B, int Gradient);
public:
	// 设置表头高度
	void SetHeaderHeight(float Height);
	void SetHeaderFontHW(int nHeight,int nWith); //设置表头字体大小
	void SetHeaderTextColor(COLORREF color);
// 躡erschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktions黚erschreibungen
	//{{AFX_VIRTUAL(CColorListCtrl)
	//}}AFX_VIRTUAL

// Implementierung
public:
	CHeaderCtrlCx m_Header;
	CColorListCtrl();
	virtual ~CColorListCtrl();

	// Generierte Nachrichtenzuordnungsfunktionen
protected:
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//{{AFX_MSG(CColorListCtrl)
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


/*
all default ClistCtrl Function:
	CImageList* GetImageList(int nImageList) const{return NULL;}
	CImageList* SetImageList(CImageList* pImageList, int nImageListType){return NULL;}
	BOOL GetItem(LV_ITEM* pItem) const{return TRUE;}
	BOOL SetItem(const LV_ITEM* pItem){return TRUE;}
	BOOL SetItem(int nItem, int nSubItem, UINT nMask, LPCTSTR lpszItem,
		int nImage, UINT nState, UINT nStateMask, LPARAM lParam){return TRUE;}
	UINT GetCallbackMask() const{return 1;}
	BOOL SetCallbackMask(UINT nMask){return TRUE;}
	int GetNextItem(int nItem, int nFlags) const{return TRUE;}
	BOOL SetItemPosition(int nItem, POINT pt){return TRUE;}
	BOOL GetItemPosition(int nItem, LPPOINT lpPoint) const{return TRUE;}
	int GetStringWidth(LPCTSTR lpsz) const{return 1;}
	CEdit* GetEditControl() const{return NULL;}
	BOOL GetColumn(int nCol, LV_COLUMN* pColumn) const{return TRUE;}
	BOOL SetColumn(int nCol, const LV_COLUMN* pColumn){return TRUE;}
	int GetColumnWidth(int nCol) const{return 1;}
	BOOL RedrawItems(int nFirst, int nLast);
	BOOL GetViewRect(LPRECT lpRect) const{return TRUE;}
	int GetTopIndex() const{return 1;}
	int GetCountPerPage() const{return 1;}
	BOOL GetOrigin(LPPOINT lpPoint) const{return TRUE;}
	UINT GetItemState(int nItem, UINT nMask) const{return 1;}
	CString GetItemText(int nItem, int nSubItem) const{return "";}
	int GetItemText(int nItem, int nSubItem, LPTSTR lpszText, int nLen) const{return 1;}
	BOOL SetItemText(int nItem, int nSubItem, LPCTSTR lpszText){return TRUE;}
	void SetItemCount(int nItems);
    UINT GetSelectedCount() const{return 1;}

Operations:
	int InsertItem(const LV_ITEM* pItem){return 1;}
	int InsertItem(int nItem, LPCTSTR lpszItem){return 1;}
	int InsertItem(int nItem, LPCTSTR lpszItem, int nImage){return 1;}
	int FindItem(LV_FINDINFO* pFindInfo, int nStart = -1) const{return 1;}
	int HitTest(LV_HITTESTINFO* pHitTestInfo) const{return 1;}
	int HitTest(CPoint pt, UINT* pFlags = NULL) const{return 1;}
	BOOL EnsureVisible(int nItem, BOOL bPartialOK){return TRUE;}
	BOOL Scroll(CSize size){return TRUE;}
	BOOL Arrange(UINT nCode){return TRUE;}
	CEdit* EditLabel(int nItem){return NULL;}
	int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1){return 1;}
	BOOL DeleteColumn(int nCol){return TRUE;}
	CImageList* CreateDragImage(int nItem, LPPOINT lpPoint){return NULL;}
	BOOL Update(int nItem){return TRUE;}
	BOOL SortItems(PFNLVCOMPARE pfnCompare, DWORD dwData){return TRUE;}
	BOOL GetItemRect(int nItem, LPRECT lpRect, UINT nCode) const{return TRUE;}

	BOOL SetColumnWidth(int nCol, int cx);
	COLORREF GetTextColor() const;
	BOOL SetTextColor(COLORREF cr);
	COLORREF GetTextBkColor() const;
	BOOL SetTextBkColor(COLORREF cr);
*/

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio f黦t zus鋞zliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // AFX_COLORLISTCTRL_H__9FC7D36B_FC88_42FD_8DAA_2F70016038BA__INCLUDED_
