/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

class CWnd;
class CDataExchange;
class CString;

namespace Yelo
{
	namespace Guerilla
	{
//see: #include <afxmsg_.h>
#define AFXAPI __stdcall
		struct AFX_MSGMAP_ENTRY
		{
			UINT nMessage;   // windows message
			UINT nCode;      // control code or WM_NOTIFY code
			UINT nID;        // control ID (or 0 for windows messages)
			UINT nLastID;    // used for entries specifying a range of control id's
			UINT_PTR nSig;   // signature type (action) or pointer to message #
			void* pfn;       // routine to call (or special value)
		};
		struct AFX_MSGMAP
		{
			const AFX_MSGMAP* pBaseMap;
			const AFX_MSGMAP_ENTRY* lpEntries;
		};


		//////////////////////////////////////////////////////////////////////////
		// CBrush
		class MFC_CBrush
		{
			void** vftable;
			PAD32; // not actually padding

		public:
			MFC_CBrush(COLORREF color);
			~MFC_CBrush();
		};

		//////////////////////////////////////////////////////////////////////////
		// CDC
		COLORREF /*__thiscall*/ CDC__SetBkMode(void* CDC_, int mode);
		COLORREF /*__thiscall*/ CDC__SetBkColor(void* CDC_, COLORREF color);
		COLORREF /*__thiscall*/ CDC__SetTextColor(void* CDC_, COLORREF color);

		//////////////////////////////////////////////////////////////////////////
		// CCmdUI
		class MFC_CCmdUI
		{
		public:
			virtual void Enable(BOOL bOn);
			virtual void SetCheck(int nCheck = 1);   // 0, 1 or 2 (indeterminate)
			virtual void SetRadio(BOOL bOn);
			virtual void SetText(LPCTSTR lpszText);

			UINT m_nID;
			UINT m_nIndex;			// menu item or other index

			// if a menu item
			class CMenu* m_pMenu;   // NULL if not a menu
			class CMenu* m_pSubMenu;// sub containing menu item
									// if a popup sub menu - ID is for first in popup

			// if from some other window
			class CWnd* m_pOther;	// NULL if a menu or not a CWnd

			BOOL m_bEnableChanged;
			BOOL m_bContinueRouting;
			UINT m_nIndexMax;		// last + 1 for iterating m_nIndex

			class CMenu* m_pParentMenu;	// NULL if parent menu not easily determined
										//  (probably a secondary popup menu)
		};
	};
};