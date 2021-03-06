// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface
// (the "Fluent UI") and is provided only as referential material to supplement the
// Microsoft Foundation Classes Reference and related electronic documentation
// included with the MFC C++ library software.
// License terms to copy, use or distribute the Fluent UI are available separately.
// To learn more about our Fluent UI licensing program, please visit
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// IfcQueryMFCView.cpp : implementation of the CIfcQueryMFCView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "IfcQueryMFC.h"
#endif

#include "IfcQueryDoc.h"
#include "IfcQueryMFCView.h"
#include "SceneGraph/SceneGraphUtils.h"
#include "SceneGraph/SoWinCADViewer.h"
#include "window/MainFrame.h"

#include <Inventor/SoPickedPoint.h>
#include <Inventor/actions/SoBoxHighlightRenderAction.h>
#include <Inventor/actions/SoLineHighlightRenderAction.h>
#include <Inventor/draggers/SoRotateCylindricalDragger.h>
#include <Inventor/draggers/SoRotateDiscDragger.h>
#include <Inventor/manips/SoCenterballManip.h>
#include <Inventor/manips/SoTrackballManip.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(IfcQueryMFCView, CView)

BEGIN_MESSAGE_MAP(IfcQueryMFCView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &IfcQueryMFCView::OnFilePrintPreview)
	//{{AFC_MSG_MAP(CIfcQueryMFCView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

IfcQueryMFCView::IfcQueryMFCView()
{
}

IfcQueryMFCView::~IfcQueryMFCView()
{
	if( m_viewer != nullptr )
	{
		delete m_viewer;
	}
}

BOOL IfcQueryMFCView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	return CView::PreCreateWindow(cs);
}

void IfcQueryMFCView::OnUpdate( CView* pSender, LPARAM lHint, CObject* pHint )
{
	PostMessage( WM_ViewToFrame );
}

SoPath* pickFilterCallback( void *, const SoPickedPoint *pick )
{
	// See which child of selection got picked
	SoPath *p = pick->getPath();
	int i;
	for( i = 0; i < p->getLength() - 1; i++ )
	{
		SoNode *n = p->getNode( i );
		if( n->isOfType( SoSelection::getClassTypeId() ) )
		{
			break;
		}
	}
	// Copy 2 nodes from the path: selection and the picked child
	return p->copy( i, 2 );
}

void IfcQueryMFCView::OnDraw(CDC* /*pDC*/)
{
	IfcQueryDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if( !pDoc )
	{
		return;
	}

	if( !m_viewer )
	{
		SoSeparator* root = pDoc->m_root_node.get();
		SoSelection* model_node = pDoc->m_model_node.get();

		m_viewer = new SoWinCADViewer( m_hWnd );
		m_viewer->setDecoration( FALSE );
		m_viewer->setBackgroundColor( SbColor( 0.9f, 0.91f, 0.91f ) );
		m_viewer->m_rootnode = root;
		m_viewer->m_model_node = model_node;
		
		// Need to set up our own camera in the "user scenegraph", or else the raypick action will fail because the camera is hidden in the viewer-specific root of the scenegraph.
		m_viewer->setCamera( new SoPerspectiveCamera() );
		m_viewer->getCamera()->nearDistance = 0.1f;
		m_viewer->getCamera()->farDistance = 1000;
		m_viewer->setSceneGraph( root );
		m_viewer->getCamera()->position = SbVec3f( 75, 75, 20 );
		m_viewer->getCamera()->pointAt( SbVec3f( 15, 15, 0 ), SbVec3f( 0, 0, 1 ) );
		m_viewer->getCamera()->viewAll( root, m_viewer->getViewportRegion() );

		// something is wrong with picking and selection here...
		SoBoxHighlightRenderAction* highlight_action = new SoBoxHighlightRenderAction();
		highlight_action->setColor( SbColor( 0.5f, 0.95f, 0.5f ) );
		highlight_action->setLineWidth( 7.f );
		m_viewer->setGLRenderAction( highlight_action );
		m_viewer->redrawOnSelectionChange( model_node );
		model_node->setPickFilterCallback( pickFilterCallback );

		// setup message redirection
		CMainFrame* pmain = (CMainFrame*)AfxGetMainWnd();
		if( pmain )
		{
			m_viewer->m_viewer_message_callback = pmain->m_message_target;
		}
		m_viewer->getCamera()->setName( "PerspectiveCamera" );
		if( m_viewer->getSceneGraph()->getName().getLength() == 0 )
		{
			m_viewer->getSceneGraph()->setName( "root" );
		}
	}
}

void IfcQueryMFCView::zoomToModelNode()
{
	if( m_viewer )
	{
		if( m_viewer->m_model_node )
		{
			SoNode* node = m_viewer->m_model_node;
			m_viewer->getCamera()->viewAll( node, m_viewer->getViewportRegion() );
		}
	}
}


// CIfcQueryMFCView printing

void IfcQueryMFCView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL IfcQueryMFCView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void IfcQueryMFCView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void IfcQueryMFCView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void IfcQueryMFCView::OnLButtonUp( UINT /* nFlags */, CPoint point )
{
	ClientToScreen( &point );

	// attempting raypick in the event_cb() callback method
	SoRayPickAction rp( m_viewer->getViewportRegion() );
	rp.setPoint( SbVec2s( short(point.x), short(point.y) ) );
	rp.apply( m_viewer->getSceneGraph() );
}

void IfcQueryMFCView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void IfcQueryMFCView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

// CIfcQueryMFCView diagnostics

#ifdef _DEBUG
void IfcQueryMFCView::AssertValid() const
{
	CView::AssertValid();
}

void IfcQueryMFCView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

IfcQueryDoc* IfcQueryMFCView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS( IfcQueryDoc )));
	return (IfcQueryDoc*)m_pDocument;
}
#endif //_DEBUG
