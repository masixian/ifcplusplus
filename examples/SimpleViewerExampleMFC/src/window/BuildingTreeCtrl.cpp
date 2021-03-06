
#include "stdafx.h"
#include "BuildingTreeCtrl.h"
#include "MainFrame.h"
#include <string>

BEGIN_MESSAGE_MAP( BuildingTreeCtrl, CTreeCtrl )
	ON_NOTIFY_REFLECT( TVN_ITEMEXPANDING, OnItemexpanding )
	ON_NOTIFY_REFLECT( NM_CLICK, OnClick )
	ON_NOTIFY_REFLECT( NM_DBLCLK, OnDblclk )
	//ON_NOTIFY_REFLECT( NM_SETFOCUS, OnFocus )
	ON_WM_LBUTTONUP()
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT( TVN_SELCHANGED, &BuildingTreeCtrl::OnTvnSelchanged )
END_MESSAGE_MAP()

BuildingTreeCtrl::BuildingTreeCtrl()
{
}

BuildingTreeCtrl::~BuildingTreeCtrl(){}

BOOL BuildingTreeCtrl::PreCreateWindow( CREATESTRUCT& cs )
{
	cs.style |= TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_SHOWSELALWAYS;
	if( !CTreeCtrl::PreCreateWindow( cs ) )
	{
		return FALSE;
	}
	return TRUE;
}

int BuildingTreeCtrl::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if( CTreeCtrl::OnCreate( lpCreateStruct ) == -1 )
	{
		return -1;
	}


	return 0;
}

void BuildingTreeCtrl::OnItemexpanding( NMHDR* pNMHDR, LRESULT* pResult )
{

}

void BuildingTreeCtrl::OnClick( NMHDR* pNMHDR, LRESULT* pResult )
{
	if( m_internal_call_to_select )
	{
		return;
	}

	HTREEITEM sel = this->GetSelectedItem();
	if( sel )
	{
		::ReleaseCapture();
		CString item_text_cstr = this->GetItemText( sel );
		std::wstring item_text = item_text_cstr;

		size_t begin_id_index = item_text.find_first_of( L"#" );
		if( item_text.length() > begin_id_index + 1 )
		{
			std::wstring node_name_id = item_text.substr( begin_id_index + 1 );
			size_t last_index = node_name_id.find_first_not_of( L"0123456789" );
			std::wstring id_str = node_name_id.substr( 0, last_index );
			if( id_str.size() > 0 )
			{
				const int id = std::stoi( id_str.c_str() );

				CWnd* parent = GetParent();
				if( parent )
				{
					parent = parent->GetParent();
					CMainFrame* parent_as_mainframe = (CMainFrame*)parent;
					if( parent_as_mainframe )
					{
						std::wstringstream wstr;
						wstr << L"select:#" << id;
						parent_as_mainframe->m_message_target->messageCallback( wstr.str().c_str(), StatusCallback::MESSAGE_TYPE_GENERAL_MESSAGE, __FUNCTION__ );
					}
				}
			}
		}

		// this always gives the previously selected item, not the current selected item

		//GetParent()->SendMessage( WM_FLOAT_CTRL, (WPARAM)str.AllocSysString(), 0 );
	}
}

void BuildingTreeCtrl::OnSelchanging( NMHDR *pNMHDR, LRESULT *pResult )
{
	HTREEITEM treeitem = GetSelectedItem();
	CString treeitemtext = GetItemText( treeitem );

	MessageBox( treeitemtext );
}

void BuildingTreeCtrl::OnSelChangedTreeCtrl( NMHDR* pNMHDR, LRESULT* pResult )
{
	//if( m_wndBuildingElmentsTree != 0 )
	{
		HTREEITEM treeitem = GetSelectedItem();
		CString treeitemtext = GetItemText( treeitem );

		MessageBox( treeitemtext );
	}

	*pResult = 0;
}

void BuildingTreeCtrl::OnDblclk( NMHDR* pNMHDR, LRESULT* pResult )
{


}

void BuildingTreeCtrl::OnLButtonUp( UINT nFlags, CPoint point )
{
	HTREEITEM sel = this->GetSelectedItem();
	if( sel )
	{
		::ReleaseCapture();
		CString str = this->GetItemText( sel );

		// this always gives the previously selected item, not the current selected item

		GetParent()->SendMessage( WM_FLOAT_CTRL, (WPARAM)str.AllocSysString(), 0 );
	}

	CRect rect;
	GetWindowRect( &rect );
	CPoint pt1 = point;
	ClientToScreen( &pt1 );

	if( !rect.PtInRect( pt1 ) )
	{
		// TODO: select part in 3d viewer
	}
	CTreeCtrl::OnLButtonUp( nFlags, point );
}


void BuildingTreeCtrl::OnTvnSelchanged( NMHDR *pNMHDR, LRESULT *pResult )
{
	if( m_internal_call_to_select )
	{
		return;
	}
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	HTREEITEM sel = this->GetSelectedItem();
	if( sel )
	{
		::ReleaseCapture();
		CString str = this->GetItemText( sel );
		std::wstring item_text = str;

		size_t begin_id_index = item_text.find_first_of( L"#" );
		if( begin_id_index != std::string::npos && item_text.length() > begin_id_index + 1 )
		{
			std::wstring node_name_id = item_text.substr( begin_id_index + 1 );
			size_t last_index = node_name_id.find_first_not_of( L"0123456789" );
			std::wstring id_str = node_name_id.substr( 0, last_index );

			if( id_str.length() > 0 )
			{
				const int entity_id = std::stoi( id_str.c_str() );

				CWnd* parent = GetParent();
				if( parent )
				{
					parent = parent->GetParent();
					CMainFrame* parent_as_mainframe = (CMainFrame*)parent;
					if( parent_as_mainframe )
					{
						std::wstringstream wstr;
						wstr << L"select:#" << entity_id;
						parent_as_mainframe->m_message_target->messageCallback( wstr.str().c_str(), StatusCallback::MESSAGE_TYPE_GENERAL_MESSAGE, __FUNCTION__ );
					}
				}
			}
		}
		

		//GetParent()->SendMessage( WM_FLOAT_CTRL, (WPARAM)str.AllocSysString(), 0 );
	}

	// TODO: Fügen Sie hier Ihren Kontrollbehandlungscode für die Benachrichtigung ein.
	*pResult = 0;
}
