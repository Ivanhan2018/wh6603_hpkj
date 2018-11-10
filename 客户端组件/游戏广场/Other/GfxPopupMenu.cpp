// GfxPopupMenu.cpp: implementation of the CGfxPopupMenu class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GfxPopupMenu.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGfxPopupMenu::CGfxPopupMenu()
{
	crMenuText = GetSysColor(COLOR_MENUTEXT);
	crMenuTextSel = GetSysColor(COLOR_HIGHLIGHTTEXT);

	cr3dFace = GetSysColor(COLOR_3DFACE);
	crMenu = GetSysColor(COLOR_MENU);
	crHighlight = GetSysColor(COLOR_HIGHLIGHT);
	cr3dHilight = GetSysColor(COLOR_3DHILIGHT);
	cr3dShadow = GetSysColor(COLOR_3DSHADOW);
	crGrayText = GetSysColor(COLOR_GRAYTEXT);

	m_clrBtnFace = GetSysColor(COLOR_BTNFACE);
	m_clrBtnHilight = GetSysColor(COLOR_BTNHILIGHT);
	m_clrBtnShadow = GetSysColor(COLOR_BTNSHADOW);

	iSpawnItem = 0;
	pSpawnItem = NULL;

	iImageItem = 0;
	pImageItem = NULL;

	szImage = CSize(20,20);

	hMenuFont = NULL;
/*	COLORMAP cMap[3] = { 
		{ RGB(128,128,128), cr3dShadow }, 
		{ RGB(192,192,192), cr3dFace }, 
		{ RGB(255,255,255), cr3dHilight }
	};
	CBitmap bmp;
	bmp.LoadMappedBitmap(IDB_MENUCHK, 0, cMap, 3);
	ilOther.Create(19, 19, ILC_COLOR4|ILC_MASK, 1, 0);
	ilOther.Add(&bmp, cr3dFace);
	bmp.DeleteObject();
*/
	NONCLIENTMETRICS ncm;
	memset(&ncm, 0, sizeof(ncm));
	ncm.cbSize = sizeof(ncm);

	::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, (PVOID) &ncm, 0);

	hGuiFont = ::CreateFontIndirect(&ncm.lfMenuFont);

	// David 08/04/98 - start - bold font handling
	hMenuBoldFont = NULL;
	CreateBoldFont();
	// David 08/04/98 - end - bold font handling
}

CGfxPopupMenu::~CGfxPopupMenu()
{
	if (iSpawnItem > 0)
	{
		for (int t = 0; t < iSpawnItem; t++)
			if (pSpawnItem[t]) delete pSpawnItem[t];

		GlobalFree((HGLOBAL) pSpawnItem);
	}
	if (iImageItem > 0)
	{
		GlobalFree((HGLOBAL) pImageItem);
	}
	if (hMenuFont) 
		::DeleteObject((HGDIOBJ)hMenuFont);
	if (hMenuBoldFont) 
		::DeleteObject((HGDIOBJ)hMenuBoldFont);
}

void CGfxPopupMenu::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
//	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
//	CRect rcItem(lpDrawItemStruct->rcItem);
//	pDC->FillSolidRect(rcItem, RGB(255,0,0));
	if (lpDrawItemStruct->CtlType == ODT_MENU)
	{
		UINT id = lpDrawItemStruct->itemID;
		UINT state = lpDrawItemStruct->itemState;
		bool bEnab = !(state & ODS_DISABLED);
		bool bSelect = (state & ODS_SELECTED) ? true : false;
		bool bChecked = (state & ODS_CHECKED) ? true : false;
		// David 08/04/98 - start - bold font handling
		bool bBold = (state & ODS_DEFAULT) ? true : false;
		// David 08/04/98 - end - bold font handling

		SpawnItem * pItem = (SpawnItem *) lpDrawItemStruct->itemData;
		if (pItem)
		{
			CDC * pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
			CFont * pft;
			// David 08/04/98 - start - bold font handling
			if (!bBold) pft = CFont::FromHandle((HFONT) hMenuFont ? hMenuFont : hGuiFont);
			else pft = CFont::FromHandle((HFONT) hMenuBoldFont ? hMenuBoldFont : hGuiFont);
			// David 08/04/98 - end - bold font handling
			CFont * of = pDC->SelectObject(pft);

			CRect rc(lpDrawItemStruct->rcItem);
			CRect rcImage(rc), rcText(rc);
			rcImage.right = rcImage.left + rc.Height();
			rcImage.bottom = rc.bottom;

			if (pItem->iCmd == -3) // is a separator
			{
				CPen pnDk(PS_SOLID,1,cr3dShadow);
				CPen pnLt(PS_SOLID,1,cr3dHilight);
				CPen * opn = pDC->SelectObject(&pnDk);
				pDC->MoveTo(rc.left + 2, rc.top + 2);
				pDC->LineTo(rc.right - 2, rc.top + 2);
				pDC->SelectObject(&pnLt);
				pDC->MoveTo(rc.left + 2, rc.top + 3);
				pDC->LineTo(rc.right - 2, rc.top + 3);
				pDC->SelectObject(opn);
			}
			else if (pItem->iCmd == -4) // is a title item
			{
				CString cs(pItem->cText), cs1;
				CRect rcBdr(rcText);

				if (bSelect && bEnab)
				{
					rcText.top ++;
					rcText.left += 2;
				}
				pDC->FillSolidRect(rcText, crMenu);
				pDC->DrawText(cs, rcText, DT_VCENTER|DT_CENTER|DT_SINGLELINE);
				if (bSelect && bEnab) pDC->Draw3dRect(rcBdr,cr3dShadow,cr3dHilight);
			}
			else
			{
				rcText.left += rcImage.right + 1;

				int obk = pDC->SetBkMode(TRANSPARENT);
				
				COLORREF ocr;
				if (bSelect)
				{
					if (pItem->iImageIdx >= 0 || (state & ODS_CHECKED))
						pDC->FillSolidRect(rcText, crHighlight);
					else
						pDC->FillSolidRect(rc, crHighlight);

					ocr = pDC->SetTextColor(crMenuTextSel);
				}
				else
				{
					if (pItem->iImageIdx >= 0 || (state & ODS_CHECKED))
						pDC->FillSolidRect(rcText, crMenu);
					else
						pDC->FillSolidRect(rc/*rcText*/, crMenu);
					ocr = pDC->SetTextColor(crMenuText);
				}

				if (pItem->iImageIdx >= 0)
				{
					int ay = (rcImage.Height() - szImage.cy) / 2;
					int ax = (rcImage.Width()  - szImage.cx) / 2;

					if (bSelect && bEnab)
						pDC->Draw3dRect(rcImage,cr3dHilight,cr3dShadow);
					else
					{
						pDC->Draw3dRect(rcImage,crMenu,crMenu);
					}


					if (bEnab)
					{
						ilList.Draw(pDC, pItem->iImageIdx, CPoint(rcImage.left + ax, rcImage.top +ay), ILD_NORMAL);
					}
					else
					{
						HICON hIcon = ilList.ExtractIcon( pItem->iImageIdx );
						pDC->DrawState( CPoint(rcImage.left + ax, rcImage.top + ay ), szImage, (HICON)hIcon, DST_ICON | DSS_DISABLED, (CBrush *)NULL );
					}
				}
				else
				{
					if (bChecked)
					{
						int ay = (rcImage.Height() - szImage.cy) / 2;
						int ax = (rcImage.Width()  - szImage.cx) / 2;

						ilOther.Draw(pDC, 0, CPoint(rcImage.left + ax, rcImage.top + ay - 2), ILD_NORMAL);
					}
				}

				CString cs(pItem->cText), cs1;
				CSize sz;
				sz = pDC->GetTextExtent(cs);
				int ay1 = (rcText.Height() - sz.cy) / 2;
				rcText.top += ay1;
				rcText.left += 2;
				rcText.right -= 15;

				int tf = cs.Find('\t');
				if (tf >= 0)
				{
					cs1 = cs.Right(cs.GetLength() - tf - 1);
					cs = cs.Left(tf);
					if (!bEnab)
					{
						if (!bSelect)
						{
							CRect rcText1(rcText);
							rcText1.InflateRect(-1,-1);
							pDC->SetTextColor(cr3dHilight);
							pDC->DrawText(cs, rcText1, DT_VCENTER|DT_LEFT);
							pDC->DrawText(cs1, rcText1, DT_VCENTER|DT_RIGHT);
							pDC->SetTextColor(crGrayText);
							pDC->DrawText(cs, rcText, DT_VCENTER|DT_LEFT);
							pDC->DrawText(cs1, rcText, DT_VCENTER|DT_RIGHT);
						}
						else
						{
							pDC->SetTextColor(crMenu);
							pDC->DrawText(cs, rcText, DT_VCENTER|DT_LEFT);
							pDC->DrawText(cs1, rcText, DT_VCENTER|DT_RIGHT);
						}
					}
					else
					{
						pDC->DrawText(cs, rcText, DT_VCENTER|DT_LEFT);
						pDC->DrawText(cs1, rcText, DT_VCENTER|DT_RIGHT);
					}
				}
				else 
				{
					if (!bEnab)
					{
						if (!bSelect)
						{
							CRect rcText1(rcText);
							rcText1.InflateRect(-1,-1);
							pDC->SetTextColor(cr3dHilight);
							pDC->DrawText(cs, rcText1, DT_VCENTER|DT_LEFT|DT_EXPANDTABS);
							pDC->SetTextColor(crGrayText);
							pDC->DrawText(cs, rcText, DT_VCENTER|DT_LEFT|DT_EXPANDTABS);
						}
						else
						{
							pDC->SetTextColor(crMenu);
							pDC->DrawText(cs, rcText, DT_VCENTER|DT_LEFT|DT_EXPANDTABS);
						}
					}
					else
						pDC->DrawText(cs, rcText, DT_VCENTER|DT_LEFT|DT_EXPANDTABS);
				}
				pDC->SetTextColor(ocr);
				pDC->SetBkMode(obk);
			}

			pDC->SelectObject(of);
		}
	}
}

void CGfxPopupMenu::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
//	lpMeasureItemStruct->itemWidth = 200;
//	lpMeasureItemStruct->itemHeight = 25;
	bool res = false;
	if (lpMeasureItemStruct->CtlType == ODT_MENU)
	{
		UINT id = lpMeasureItemStruct->itemID;

		SpawnItem * pItem = (SpawnItem *) lpMeasureItemStruct->itemData;
		if (pItem)
		{
			if (pItem->iCmd == -3) // is a separator
			{
				lpMeasureItemStruct->itemWidth  = 10;
				lpMeasureItemStruct->itemHeight = 6;
			}
			else
			{
				CString cs(pItem->cText);
				if (!cs.IsEmpty())
				{
					CClientDC dc(AfxGetMainWnd());
					CFont * pft = CFont::FromHandle(hMenuFont ? hMenuFont : hGuiFont);
					CFont * of = dc.SelectObject(pft);
					CSize osz = dc.GetOutputTabbedTextExtent(cs,0,NULL);
					if (pItem->iCmd == -4)
					{
						CRect rci(0,0,0,0);
						dc.DrawText(cs, rci, DT_CALCRECT|DT_TOP|DT_VCENTER|DT_SINGLELINE);
						lpMeasureItemStruct->itemHeight = rci.Height();
						lpMeasureItemStruct->itemWidth = rci.Width();
					}
					else
					{
						lpMeasureItemStruct->itemHeight = szImage.cy + 5;
						if (osz.cy > (int) lpMeasureItemStruct->itemHeight) lpMeasureItemStruct->itemHeight = (int) osz.cy;
						lpMeasureItemStruct->itemWidth  = osz.cx + 2 + 15;
						lpMeasureItemStruct->itemWidth += lpMeasureItemStruct->itemHeight > (UINT) szImage.cx ? (UINT) lpMeasureItemStruct->itemHeight : (UINT) szImage.cx;
					}
					dc.SelectObject(of);
				}
				else
				{
					lpMeasureItemStruct->itemHeight = szImage.cy + 5;
					lpMeasureItemStruct->itemWidth  = 100;
				}
			}
		}
	}
}

bool CGfxPopupMenu::CreateBoldFont()
{
	if (hMenuBoldFont) 
		::DeleteObject((HGDIOBJ)hMenuBoldFont);

	LOGFONT lgFont;
	::GetObject (hMenuFont ? hMenuFont : hGuiFont, sizeof (lgFont), &lgFont);
	lgFont.lfWeight = FW_BOLD;

	hMenuBoldFont = CreateFontIndirect (&lgFont);
	return !!hMenuBoldFont;
}

bool CGfxPopupMenu::AddToolBarResource(unsigned int resId)
{
	// David 08/04/98 - start - put CMenuSpawn in DLL
	HINSTANCE hInst = AfxFindResourceHandle (MAKEINTRESOURCE(resId), RT_TOOLBAR);
	if (!hInst)
		return false;
	// David 08/04/98 - end - put CMenuSpawn in DLL

	HRSRC hRsrc = ::FindResource(/*AfxGetResourceHandle()*/hInst, MAKEINTRESOURCE(resId), RT_TOOLBAR);
	if (hRsrc == NULL) return false;

	HGLOBAL hGlb = ::LoadResource(/*AfxGetResourceHandle()*/hInst, hRsrc);
	if (hGlb == NULL) return false;


	ToolBarData* pTBData = (ToolBarData*) ::LockResource(hGlb);
	if (pTBData == NULL) return false;
	
	ASSERT(pTBData->wVersion == 1);

	CBitmap bmp;
	bmp.LoadBitmap(resId);
	int nBmpItems = ilList.Add(&bmp, RGB(192,192,192));
	bmp.DeleteObject();

	WORD* pItem = (WORD*)(pTBData+1);
	
	for(int i=0; i<pTBData->wItemCount; i++, pItem++)
	{
		if(*pItem != ID_SEPARATOR)
			AddImageItem(nBmpItems++, (WORD) *pItem);
	}
// ** it seem that Windows doesn't free these resource (from Heitor Tome)
    ::UnlockResource(hGlb);
    ::FreeResource(hGlb);
// **
	return true;
}

bool CGfxPopupMenu::LoadToolBarResource(unsigned int resId)
{
	//David 08/04/98 - start - put CMenuSpawn in DLL
	HINSTANCE hInst = AfxFindResourceHandle (MAKEINTRESOURCE(resId), RT_TOOLBAR);
	if (!hInst)
		return false;
	//David 08/04/98 - end - put CMenuSpawn in DLL

	HRSRC hRsrc = ::FindResource(/*AfxGetResourceHandle()*/hInst, MAKEINTRESOURCE(resId), RT_TOOLBAR);
	if (hRsrc == NULL) return false;

	HGLOBAL hGlb = ::LoadResource(/*AfxGetResourceHandle()*/hInst, hRsrc);
	if (hGlb == NULL) return false;

	ToolBarData* pTBData = (ToolBarData*) ::LockResource(hGlb);
	if (pTBData == NULL) return false;
	
	ASSERT(pTBData->wVersion == 1);

	szImage.cx = (int) pTBData->wWidth;
	szImage.cy = (int) pTBData->wHeight;

	if (ilList.Create(szImage.cx, szImage.cy, ILC_COLOR4|ILC_MASK, pTBData->wItemCount, 0) == false)
		return false;

	ilList.SetBkColor(cr3dFace);

	CBitmap bmp;
	bmp.LoadBitmap(resId);
	ilList.Add(&bmp, RGB(192,192,192));
	bmp.DeleteObject();

	WORD* pItem = (WORD*)(pTBData+1);
	int nBmpItems = 0;
	for(int i=0; i<pTBData->wItemCount; i++, pItem++)
	{
		if(*pItem != ID_SEPARATOR)
			AddImageItem(nBmpItems++, (WORD) *pItem);
	}
// ** it seem that Windows doesn't free these resource (from Heitor Tome)
    ::UnlockResource(hGlb);
    ::FreeResource(hGlb);
// **
	return true;
}

void CGfxPopupMenu::AddImageItem(const int idx, WORD cmd)
{
	if (iImageItem == 0)
		pImageItem = (ImageItem *) GlobalAlloc(GPTR, sizeof(ImageItem));
	else
		pImageItem = (ImageItem *) GlobalReAlloc((HGLOBAL) pImageItem, sizeof(ImageItem) * (iImageItem + 1), GMEM_MOVEABLE|GMEM_ZEROINIT);
	
	ASSERT(pImageItem);
	pImageItem[iImageItem].iCmd = (int) cmd;
	pImageItem[iImageItem].iImageIdx = idx;
	iImageItem ++;
}

void CGfxPopupMenu::RemapMenu(CMenu * pMenu)
{
	static int iRecurse = 0;
	iRecurse ++;

	ASSERT(pMenu);
	int nItem = pMenu->GetMenuItemCount();
	while ((--nItem)>=0)
	{
		UINT itemId = pMenu->GetMenuItemID(nItem);
		if (itemId == (UINT) -1)
		{
			CMenu *pops = pMenu->GetSubMenu(nItem);
			if (pops) RemapMenu(pops);
			if (iRecurse > 0)
			{
				CString cs;
				pMenu->GetMenuString(nItem, cs, MF_BYPOSITION);
				if (!cs.IsEmpty())
				{
					SpawnItem * sp = AddSpawnItem(cs, (iRecurse == 1) ? -4 : -2);
					pMenu->ModifyMenu(nItem,MF_BYPOSITION|MF_OWNERDRAW, (UINT) -1, (LPCTSTR)sp);
				}
			}
		}
		else
		{
			if (itemId != 0)
			{
				UINT oldState = pMenu->GetMenuState(nItem,MF_BYPOSITION);
				if (!(oldState&MF_OWNERDRAW) && !(oldState&MF_BITMAP))
				{
					ASSERT(oldState != (UINT)-1);
					CString cs;
					pMenu->GetMenuString(nItem, cs, MF_BYPOSITION);
					SpawnItem * sp = AddSpawnItem(cs, itemId);
					pMenu->ModifyMenu(nItem,MF_BYPOSITION|MF_OWNERDRAW|oldState, (LPARAM)itemId, (LPCTSTR)sp);
				}
			}
			else
			{
				UINT oldState = pMenu->GetMenuState(nItem,MF_BYPOSITION);
				if (!(oldState&MF_OWNERDRAW) && !(oldState&MF_BITMAP))
				{
					ASSERT(oldState != (UINT)-1);
					SpawnItem * sp = AddSpawnItem(TEXT("--"), -3);
					pMenu->ModifyMenu(nItem,MF_BYPOSITION|MF_OWNERDRAW|oldState, (LPARAM)itemId, (LPCTSTR)sp);
				}
			}
		}
	}
	iRecurse --;
}

CGfxPopupMenu::SpawnItem * CGfxPopupMenu::AddSpawnItem(const TCHAR * txt, const int cmd)
{
	if (iSpawnItem == 0)
		pSpawnItem = (SpawnItem **) GlobalAlloc(GPTR, sizeof(SpawnItem));
	else
		pSpawnItem = (SpawnItem **) GlobalReAlloc((HGLOBAL) pSpawnItem, sizeof(SpawnItem) * (iSpawnItem + 1), GMEM_MOVEABLE|GMEM_ZEROINIT);

	ASSERT(pSpawnItem);

	SpawnItem * p = new SpawnItem;
	ASSERT(p);
	pSpawnItem[iSpawnItem] = p;
	wcscpy(p->cText, txt);
	p->iCmd = cmd;

	if (cmd >= 0) 
		p->iImageIdx = FindImageItem(cmd);
	else 
		p->iImageIdx = cmd;

	iSpawnItem ++;
	return p;
}

int CGfxPopupMenu::FindImageItem(const int cmd)
{
	for (int t = 0; t < iImageItem; t++)
		if (pImageItem[t].iCmd == cmd) return pImageItem[t].iImageIdx;

	return -1;
}

void CGfxPopupMenu::EnableMenuItems(CMenu * pMenu, CWnd * pParent)
{
	ASSERT(pMenu);
	ASSERT(pParent);

	int nItem = pMenu->GetMenuItemCount();
	CCmdUI state;
	state.m_pMenu = pMenu;
	state.m_nIndex = nItem-1;
	state.m_nIndexMax = nItem;

	while ((--nItem)>=0)
	{
		UINT itemId = pMenu->GetMenuItemID(nItem);
		if (itemId == (UINT) -1)
		{
			CMenu *pops = pMenu->GetSubMenu(nItem);
			if (pops) EnableMenuItems(pops, pParent);
		}
		else
		{
			if (itemId != 0)
			{
				state.m_nID = itemId;
				pParent->OnCmdMsg(itemId, CN_UPDATE_COMMAND_UI, &state, NULL);
				state.DoUpdate(pParent, true);
			}
		}
		state.m_nIndex = nItem-1;
	}
}
