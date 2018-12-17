#include "StdAfx.h"
#include "FormatPaintTool.h"

extern Log logObj;

CFormatPaintTool::CFormatPaintTool(void)
{
	m_pCanvas      = NULL;
	hasTransInfo   = false;
	m_pPlaceHolder = NULL;
	dEnlargeRatio  = 1;
	hasStreets     = false;
	hasSechwys     = false;
	hasMajHwys     = false;
	hasDetailroads = false;
	hasMainroads   = false;
	isXMLOutput    = false;
	isXMLUTM       = false;
	isGIFOutput    = false;
	isPNGOutput    = false;
	m_pTransformer = NULL;

	strHLArea      = "";
	isHLArea       = false;
	isLatLong      = false;

	nldensity      = 0;
	iSpecifiedLableColumn = -1;
}

CFormatPaintTool::~CFormatPaintTool(void)
{
}
bool CFormatPaintTool::BeginDrawShape(void)
{
	//initialization
	int	rColor =  (rand()%240);
	int	gColor =  (rand()%240);
	int	bColor =  (rand()%240);
	// Create a solid red pen of width 2.
	VERIFY(defaultPen.CreatePen(PS_SOLID, 5, RGB( rColor, gColor, bColor)));
	oldPen = (m_pCanvas->getCanvasDC())->SelectObject(&defaultPen);
	return true;
}
// end Draw Shape 
bool CFormatPaintTool::EndDrawShape(void)
{
	//initialization
	m_pCanvas->getCanvasDC()->SelectObject(&oldPen);
	defaultPen.DeleteObject();
	return true;
}

bool CFormatPaintTool::BeginDrawLabel(void)
{
	VERIFY(labelFont.CreateFont(
		12,                        // nHeight
		15,                         // nWidth
		(int)0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		false,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		CString("Arial")));         // lpszFacename

	//GetTextAlign ????
	(m_pCanvas->getCanvasDC())->SetTextColor(RGB(155, 0, 0));
	old_font = (m_pCanvas->getCanvasDC())->SelectObject( &labelFont );
	(m_pCanvas->getCanvasDC())->SetBkMode(TRANSPARENT);
	return true;
}

bool CFormatPaintTool::EndDrawLabel(void)
{
	//should recover the GDI and other things.
	(m_pCanvas->getCanvasDC())->SelectObject(old_font);
	labelFont.DeleteObject();
	return false;
}

bool CFormatPaintTool::DrawShape(CShape* pShape, ShapeUtility::ShapeType shapeType, CString strColor, int nPenWidth)
{
	CPen shapePen;
	//initialization for shape color
	int	rColor =  255, gColor =  255, bColor =  0;
	// Create a solid red pen of width 2.
	// VERIFY(myPen1.CreatePen(PS_SOLID, 5, RGB(0,0,255)));
	VERIFY(shapePen.CreatePen(PS_SOLID, 1, RGB( rColor, gColor, bColor)));
	oldPen = (m_pCanvas->getCanvasDC())->SelectObject(&shapePen);

	CDC* pDC = m_pCanvas->getCanvasDC();
	ASSERT( pDC != NULL );	

	switch ( shapeType )
	{
	case ShapeUtility::unknown:
		VERIFY(false);
		//LOG ..... impossible!!!
		break ;

	case ShapeUtility::point:
		{


			//if( FA_OWNER == iSpecifiedLableColumn)
			//{
			//	//olny show owner, not show addr shape
			//	CLabelManager lm;
			//	if("" == lm.GetSpecifiedLable((CPointShape*)pShape, FA_OWNER))//if this is addr, the owner column should be blank
			//	{
			//		break;
			//	}
			//}
			//if( FA_ADDR == iSpecifiedLableColumn)
			//{
			//	//only show addr, not show owner shape
			//	CLabelManager lm;
			//	if("" != lm.GetSpecifiedLable((CPointShape*)pShape, FA_OWNER))//if this is owner, the owner column should not be blank
			//	{
			//		break;
			//	}
			//}

			if( -1 != iSpecifiedLableColumn )
			{
				CLabelManager lm;
				if( lm.HasIgnoredLableColumn((CPointShape*)pShape, iSpecifiedLableColumn) )
				{
					break;
				}
			}



			int nRadius = 4;
			/*int nCenterPointX = (int) pShape->m_aPointArray[0].dX; 
			int nCenterPointY = (int) pShape->m_aPointArray[0].dY;*/

			int nCenterPointX = round( pShape->m_aPointArray[0].dX ); 
			int nCenterPointY = round( pShape->m_aPointArray[0].dY );


			int nImageHeight = 0, nImageWidth = 0;
#ifdef	TERRA_NODE
			CString dir;

			if( isGIFOutput )
			{
				//dir = _T("c:\\degree\\bitmap-opc\\gifIcon\\");
				//dir = _T("e:\\degree\\bitmap-opc\\gifIcon\\");
				dir = _T(DISK_LOCATION_ICON);
			}
			else
			{
				//dir = _T("c:\\degree\\bitmap-opc\\");
				dir = _T(DISK_LOCATION_ICON_JPEG);
			}
#else
			/*CString dir;

			if( isGIFOutput )
			{
				dir = _T("D:\\FMap_Data\\degree\\bitmap-opc\\gifIcon\\");
			}
			else
			{
				dir = _T("D:\\FMap_Data\\degree\\bitmap-opc\\");
			}*/
			CString dir;

			if( isGIFOutput )
			{
				//dir = _T("c:\\degree\\bitmap-opc\\gifIcon\\");
				dir = _T(DISK_LOCATION_ICON_LOCAL);
			}
			else
			{
				//dir = _T("c:\\degree\\bitmap-opc\\");
				dir = _T(DISK_LOCATION_ICON_LOCAL_JPEG);
			}

#endif
			//if( (!pShape->m_aProperties[2].IsEmpty()) || (!pShape->m_aProperties[8].IsEmpty()/*for tfgauges*/) )
			if( (!pShape->m_aProperties[2].IsEmpty()) || (!pShape->m_aProperties[8].IsEmpty()/*for tfgauges*/) || (pShape->m_aProperties[2].IsEmpty()))
			{
				CString file = pShape->m_aProperties[2];
				//hotel point stars picto
				if( !pShape->m_aProperties[5].IsEmpty() )
				{
					CString star = pShape->m_aProperties[5];//the hotel star number
					file.Insert(5, star);//insert the star number to the "hotel.raw"
				}

				if( !pShape->m_aProperties[8].IsEmpty() && file.IsEmpty() ) /*for tfgauges*/
				{
					file = "rtstream.raw";
				}

				logObj.varWriteToLog(_INFO, "ss", "bitmap file name: ", file);

				if(!pShape->m_aProperties[2].IsEmpty())
				{
					file = ExtractPictoFile( file, dir );
				}
				else
				{
					/*file = "default.raw";
					file = ExtractPictoFile( file, dir );*/
					int nRadius1 = 1;
					CPen iconPen;
					iconPen.CreatePen( PS_SOLID, 3, RGB(0, 0, 255) );
					pDC->SelectObject(&iconPen);
					pDC->Ellipse( nCenterPointX-nRadius1,
						nCenterPointY-nRadius1,
						nCenterPointX+nRadius1,
						nCenterPointY+nRadius1 );
					
				}
				
				//////////////////////////////////////////////////////////////////
				//for XML
				if( (m_pPlaceHolder->IsCircleInBoundary( 
					CCircle(nCenterPointX, nCenterPointY, 6)) ) &&
					isXMLOutput )//6 means the picto height by default
				{
					double dTempX1 = m_pTransformer->m_pLinearTransformer->MapToDate_X( 
						pShape->m_aPointArray[0].dX );
					double dTempY1 = m_pTransformer->m_pLinearTransformer->MapToDate_Y( 
						pShape->m_aPointArray[0].dY );

					if( isXMLUTM )
					{
						m_pTransformer->m_pLinearTransformer->GDCToUTM(dTempX1, dTempY1);
					}

					( m_pCanvas->nTotalIcon ) ++;   
					m_pCanvas->AddXMLData(CXMLData(ICON_TYPE, file.GetString(), 
						dTempX1, dTempY1));

					break;
				}
				//////////////////////////////////////////////////////////////////
				logObj.varWriteToLog(_INFO, "ss", "bitmap file name after processed: ", file);

				if(_access((const char*)LPCTSTR(file), 4) == 0)
				{
					CImage image;
					image.Load(file);
					//log it is point holder
					m_pPlaceHolder->isPointPlaceholder = true;
					m_pPlaceHolder->hasBigSpaceForLabelConflict = false;
					/////////////////////////////////////////////////////////////////
					//SM
					if( 1 == (int)dEnlargeRatio )
					{
						nImageHeight = image.GetHeight();
						nImageWidth  = image.GetWidth();
						////////////////////////////////////////////////////////////////////////////
						////picto collision detection
						CCircleList pictoPlaceholder;
						/*
						int nXPicto = nCenterPointX - ( nImageWidth  / 2 );  
						int nYPicto = nCenterPointY - ( nImageHeight / 2 );
						*/
						int nXPicto = nCenterPointX;  
						int nYPicto = nCenterPointY;

						pictoPlaceholder.AddHead( CCircle(nXPicto, nYPicto, 7) );
						/*pDC->Ellipse( nXPicto-5,
							nYPicto-5,
							nXPicto+5,
							nYPicto+5 );*/

						if ( m_pPlaceHolder->IsPlaceAvailable(pictoPlaceholder) )
						{
							m_pPlaceHolder->AddHolder(pictoPlaceholder);
							m_pPlaceHolder->isPointPlaceholder = false;
						}
						else
						{
							m_pPlaceHolder->isPointPlaceholder = false;
							break;
						}
						////////////////////////////////////////////////////////////////////////////

						//Picto size is 16*16
						//image.BitBlt(pDC->m_hDC, nCenterPointX - ( nImageWidth / 2 ), nCenterPointY - nImageHeight);
						image.BitBlt( pDC->m_hDC, nCenterPointX - ( nImageWidth / 2 ), nCenterPointY- (nImageHeight/2) );
					}
					else
					{
						//enlargeratio > 1, the isPointPlaceholder = false
						m_pPlaceHolder->isPointPlaceholder = false;

						if( (int)dEnlargeRatio > 2)
						{
							m_pPlaceHolder->hasBigSpaceForLabelConflict = true;
						}
						nImageHeight = (int)(image.GetHeight() * dEnlargeRatio);
						nImageWidth  = (int)(image.GetWidth() * dEnlargeRatio);
						////////////////////////////////////////////////////////////////////////////
						////picto collision detection
						CCircleList pictoPlaceholder;
						int nXPicto = nCenterPointX - ( nImageWidth  / 2 );  
						int nYPicto = nCenterPointY - ( nImageHeight / 2 );
						pictoPlaceholder.AddHead( CCircle(nXPicto, nYPicto, (nImageWidth / 4)) );

						if ( m_pPlaceHolder->IsPlaceAvailable(pictoPlaceholder) )
						{
							m_pPlaceHolder->AddHolder(pictoPlaceholder);
						}
						else
						{
							break;
						}
						////////////////////////////////////////////////////////////////////////////
						//enlarge the Picto
						image.StretchBlt( pDC->m_hDC, nCenterPointX - ( nImageWidth / 2 ), nCenterPointY - nImageHeight + 16, nImageWidth, nImageHeight );
					}
					//////////////////////////////////////////////////////////////////

				}
				else
				{
					logObj.writeToLog(_ERROR, "Can't access picto file!");
				}
			}
			else
			{
				CPen iconPen;
				iconPen.CreatePen( PS_SOLID, 3, RGB(0, 255, 0) );
				pDC->SelectObject(&iconPen);
				pDC->Ellipse( nCenterPointX-nRadius,
					nCenterPointY-nRadius,
					nCenterPointX+nRadius,
					nCenterPointY+nRadius );
				//pDC->SetPixel
				//COLORREF SetPixel(
				//	int x,
				//	int y,
				//	COLORREF crColor 
				//	);
			}
		}
		break;

	case ShapeUtility::arc:
		{ 
			/*Draw the line of this record */
			CColorManager arcColorManager;
			COLORREF arcColor;
			int	iVertex = 0;
			int nCount = (int) pShape->m_aPointArray.GetCount();

			//int nPenWidth = 2;
			int nPenWidth = this->nlinewidth;


			ASSERT( pShape->GetType() == ShapeUtility::arc );
			CLineShape* pLineShape = (CLineShape*) pShape;

			//CString sStreetLable = pLineShape->m_aProperties[0];
			CLabelManager arcLabelXML;
			CString sStreetLable = arcLabelXML.ExtractArcLabel( pLineShape );

			CPen arcPen;

			if(colorInfo.line != "")
			{
				arcColorManager.SetArcColor( colorInfo );
				arcColor = arcColorManager.GetArcColor();
				/////////////////////////////////////////////////////////////////
				//SM
				if( 1 == (int)dEnlargeRatio )
				{
					arcPen.CreatePen( PS_SOLID, nPenWidth, arcColor );
				}
				else
				{
					arcPen.CreatePen( PS_SOLID, (int)(nPenWidth*dEnlargeRatio), arcColor );
				}
				//////////////////////////////////////////////////////////////////
				pDC->SelectObject(&arcPen);
			}
			else
			{
				arcColor = arcColorManager.GetDefaultArcColor();
				/////////////////////////////////////////////////////////////////
				//SM
				if( 1 == dEnlargeRatio )
				{
					arcPen.CreatePen(PS_SOLID, nPenWidth, arcColor );
				}
				else
				{
					arcPen.CreatePen(PS_SOLID, (int)(nPenWidth*dEnlargeRatio), arcColor );
				}
				//////////////////////////////////////////////////////////////////
				pDC->SelectObject(&arcPen);
			}
			///////////////////////////////////////////////////////////////////
			//XML output
			double dArcX1 = 0.0, dArcY1 = 0.0;
			int nStartArcX = 0, nStartArcY = 0;
			nStartArcX = round(pShape->m_aPointArray[0].dX);
			nStartArcY = round(pShape->m_aPointArray[0].dY);
			CArcPointList arcPointList;



			if( ( m_pPlaceHolder->IsCircleInBoundary(
				CCircle(nStartArcX, nStartArcY, 0))) &&
				(isXMLOutput) )
			{
				( m_pCanvas->nTotalArc ) ++;   

				for( iVertex = 0; iVertex < nCount; iVertex++ )
				{
					dArcX1 = m_pTransformer->m_pLinearTransformer->MapToDate_X(
						pShape->m_aPointArray[iVertex].dX);
					dArcY1 = m_pTransformer->m_pLinearTransformer->MapToDate_Y(
						pShape->m_aPointArray[iVertex].dY);

					if( isXMLUTM )
					{
						m_pTransformer->m_pLinearTransformer->GDCToUTM(dArcX1, dArcY1);
					}

					arcPointList.AddTail( CXMLCoordinates(dArcX1, dArcY1) );
				}
				m_pCanvas->AddXMLData(CXMLData(ARC_TYPE, sStreetLable.GetString(), 
					arcPointList));
				break;
			}
			///////////////////////////////////////////////////////////////////
			/*pDC->MoveTo( (int) pShape->m_aPointArray[0].dX,
				(int) pShape->m_aPointArray[0].dY );*/

			pDC->MoveTo( round(pShape->m_aPointArray[0].dX),
				round(pShape->m_aPointArray[0].dY) );

			double dtempX = round(pShape->m_aPointArray[0].dX);
			double dtempY = round(pShape->m_aPointArray[0].dY);
			double dtempEX = round(pShape->m_aPointArray[nCount - 1].dX);
			double dtempEY = round(pShape->m_aPointArray[nCount - 1].dY);



			/*if(( 0 == (int) pShape->m_aPointArray[0].dX ) && (0 == (int) pShape->m_aPointArray[0].dY))
			{
				printf("this is grid line");
			}*/

			for( iVertex = 1; iVertex < nCount; iVertex++ )
			{
				/*if(( 0 == (int) pShape->m_aPointArray[iVertex].dX ) && (0 == (int) pShape->m_aPointArray[0].dY))
				{
					printf("this is grid line");
				}*/

				/*pDC->LineTo( (int) pShape->m_aPointArray[iVertex].dX,
					(int) pShape->m_aPointArray[iVertex].dY );*/
				pDC->LineTo( round(pShape->m_aPointArray[iVertex].dX),
					round(pShape->m_aPointArray[iVertex].dY) );

			}
		}
		break;

	case ShapeUtility::polygon:
		{
			

			CColorManager AreaColorManager;
			//CLabelManager AreaLabel;

			CString colorStr;
			
			if( "" != colorInfo.area)
			{
				colorStr = colorInfo.area.c_str();
			}
			else 
			{
				colorStr = pShape->m_aProperties[3];// area color from dbf
			}	
			


			// draw area
			/*if( (hasTransInfo || !colorStr.CompareNoCase("HHHHHH")) &&
				(!isGIFOutput) )*/


			//if(hasTransInfo)	
			//{
			//	int trans = transInfo.values[1];
			//	if( 0 == trans )// .xx0 means label invisible in gif version
			//	{
			//		break;
			//	}
			//}

			if( ((hasTransInfo || !colorStr.CompareNoCase("HHHHHH"))) && ("" != colorInfo.area) &&
				(0 != transInfo.values[1]))
			{
				// this is areacq
				//COLORREF color = bgr(str2colorref(colorStr, 0));	
				AreaColorManager.SetAreaColor( colorStr );
				COLORREF color = AreaColorManager.GetAreaColor();
				/*if( RGB(0,0,0) == color)
				{
					color = RGB(200, 250, 250);
				}*/



				CTransInfoManager tranManager;
				if(hasTransInfo && !isGIFOutput)	// encode transparency info from request
				{
					int trans = transInfo.values[1];
					tranManager.SetTransInfo( trans, color );
					color = tranManager.GetTransInfo();
				}
				// else transparency info has already encoded in dbf file
				
				int	iVertex = 0;


				int nCount = (int) pShape->m_aPointArray.GetCount();
				////////////////this is for point reduce
				//int nCount = (int) pShape->m_aRectPointArray.GetCount();


				//int nNewCount = 0;
				//CPoint* paIntPoint = new CPoint[nCount];

				//for( iVertex = 0; iVertex < nCount ; iVertex++ )
				//{

				//	/////////////////////////////////////////this is for point reduce
				//	//merge the points which have the same coordinates
    // //               if( 0 == iVertex)
				//	//{
				//	//	/*(paIntPoint + nNewCount)->x = (int) pShape->m_aRectPointArray[iVertex].dX;
				//	//	(paIntPoint + nNewCount)->y = (int) pShape->m_aRectPointArray[iVertex].dY;*/

				//	//	(paIntPoint + nNewCount)->x = round( pShape->m_aRectPointArray[iVertex].dX );
				//	//	(paIntPoint + nNewCount)->y = round( pShape->m_aRectPointArray[iVertex].dY );
				//	//	nNewCount++;
				//	//}
				//	//else
				//	//{
				//	//	/*int nTempPredx = (int) pShape->m_aRectPointArray[iVertex - 1].dX;
				//	//	int nTempPredy = (int) pShape->m_aRectPointArray[iVertex - 1].dY;
				//	//	int nTempCurdx = (int) pShape->m_aRectPointArray[iVertex].dX;
				//	//	int nTempCurdy = (int) pShape->m_aRectPointArray[iVertex].dY;*/

				//	//	int nTempPredx = round( pShape->m_aRectPointArray[iVertex - 1].dX );
				//	//	int nTempPredy = round( pShape->m_aRectPointArray[iVertex - 1].dY );
				//	//	int nTempCurdx = round( pShape->m_aRectPointArray[iVertex].dX );
				//	//	int nTempCurdy = round( pShape->m_aRectPointArray[iVertex].dY );

				//	//	if( (nTempPredx == nTempCurdx) && (nTempPredy == nTempCurdy) )
				//	//	{
				//	//		//continue
				//	//	}
				//	//	else
				//	//	{
				//	//		/*(paIntPoint + nNewCount)->x = (int) pShape->m_aRectPointArray[iVertex].dX;
				//	//		(paIntPoint + nNewCount)->y = (int) pShape->m_aRectPointArray[iVertex].dY;*/

				//	//		(paIntPoint + nNewCount)->x = round( pShape->m_aRectPointArray[iVertex].dX );
				//	//		(paIntPoint + nNewCount)->y = round( pShape->m_aRectPointArray[iVertex].dY );

				//	//		nNewCount++;
				//	//	}
				//	//}
				//	/////////////////////////////////////////
				//	(paIntPoint + iVertex)->x = round( pShape->m_aPointArray[iVertex].dX );
				//	(paIntPoint + iVertex)->y = round( pShape->m_aPointArray[iVertex].dY );

				//	///////////////this is for point reduce
				//	/*(paIntPoint + iVertex)->x = (int) pShape->m_aRectPointArray[iVertex].dX;
				//	(paIntPoint + iVertex)->y = (int) pShape->m_aRectPointArray[iVertex].dY;*/
				//} 

				//nCount = nNewCount;



				/////////////////////////////////////////////////////////////////
				//part paint

				CLabelManager areaManagertest;
				CString testLable = areaManagertest.ExtractAreaLabel( (CPolygonShape*)pShape );
				

				LOGBRUSH logBrush;
				logBrush.lbStyle = BS_SOLID;
				if(isGIFOutput)
				{
					logBrush.lbColor = AreaColorManager.GetGifPaletteColor(bgr(color));//color;//RGB(255,255,0);//
				}
				else
				{
					logBrush.lbColor = color;//RGB(255,255,0);//
				}
				logBrush.lbHatch = HS_CROSS;
				// Declare an uninitialized CBrush ...
				CBrush brush;
				// ... and initialize it with the LOGBRUSH.
				VERIFY(brush.CreateBrushIndirect(&logBrush));

				



				int nPartStartPaint = 0, nPartEndPaint = 0, nPointsPaint = 0;
				int nPartsPaint = pShape->m_aPartsStartingIndex.GetCount();
				int nStartPointXPaint = round( pShape->m_aPointArray[0].dX );
				int nStartPointYPaint = round( pShape->m_aPointArray[0].dY );
				if (nPartsPaint < 1)
				{
					VERIFY(false);
				}
				for(int i = 0; i < nPartsPaint; i++ )
				{
					int nPartStartIndexPaint = pShape->m_aPartsStartingIndex[i];

					int nPartEndIndexPaint = 0;

					if ( i != (nPartsPaint - 1) )
					{
						nPartEndIndexPaint = pShape->m_aPartsStartingIndex[i+1];														
					}
					else
					{	
						nPartEndIndexPaint = pShape->m_aPointArray.GetCount();
					}	 
					//if(nParts > 1)
					{
						/*pDC->MoveTo( round( pShape->m_aPointArray[nPartStartIndex].dX),
							round( pShape->m_aPointArray[nPartStartIndex].dY ));

						for( iVertex = nPartStartIndex; iVertex < nPartEndIndex; iVertex++ )
						{

							pDC->LineTo( round( pShape->m_aPointArray[iVertex].dX ),
								round( pShape->m_aPointArray[iVertex].dY ) );
						}*/

						int npaIntPoint = 0;
						int nPartSize = nPartEndIndexPaint-nPartStartIndexPaint;
						CPoint* paIntPoint = new CPoint[nPartSize];
						for(int j=nPartStartIndexPaint;j<nPartEndIndexPaint;j++)
						{	
							(paIntPoint + npaIntPoint)->x = round( pShape->m_aPointArray[j].dX );
							(paIntPoint + npaIntPoint)->y = round( pShape->m_aPointArray[j].dY );
							npaIntPoint++;
						}
						CRgn rgnShape;
						rgnShape.CreatePolygonRgn( paIntPoint, nPartSize, WINDING);
						if( !(pDC->FillRgn( &rgnShape, &brush)) )
						{
							return false;
						}

						delete[] paIntPoint;

					}

				}//end for

				/////////////////////////////////////////////////////////////////




				logObj.writeToLog(_INFO, "bEGINGing draw seasareaSSSSSSSSSSSSSSSSSSSSSSS.");
				//logObj.varWriteToLog(_INFO, "ss", "bitmap file name: ", file);
				//int n = rgnShape.CreatePolygonRgn( paIntPoint, nCount, ALTERNATE);
				//VERIFY( rgnShape.CreatePolygonRgn( paIntPoint, nCount, ALTERNATE) );//WINDING ALTERNATE));
				//if( ( nCount > 8000 ) || (!(rgnShape.CreatePolygonRgn( paIntPoint, nCount, ALTERNATE))) )

				/*if( (!(rgnShape.CreatePolygonRgn( paIntPoint, nCount, WINDING))) )
				{
					logObj.writeToLog(_INFO, "fail to draw seasareaXXXXXXXXXXXXXXXXXXXXXXXXXXX.");
					return false;
				}*/
				//logObj.writeToLog(_INFO, "ending draw seasareaOKOKOKOKOKOKOKOKOKOKOKOKOKOK.");

				//LOGBRUSH logBrush;
				//
				//logBrush.lbStyle = BS_SOLID;
				//if(isGIFOutput)
				//{
				//	logBrush.lbColor = AreaColorManager.GetGifPaletteColor(color);//color;//RGB(255,255,0);//
				//}
				//else
				//{
				//	logBrush.lbColor = color;//RGB(255,255,0);//
				//}
				//
				//logBrush.lbHatch = HS_CROSS;
				//// Declare an uninitialized CBrush ...
				//CBrush brush;
				//// ... and initialize it with the LOGBRUSH.
				//VERIFY(brush.CreateBrushIndirect(&logBrush));
				//delete[] paIntPoint;
				//VERIFY(pDC->FillRgn( &rgnShape, &brush) );      //  Filled 
				/*if( !(pDC->FillRgn( &rgnShape, &brush)) )
				{
					return false;
				}*/


				//delete[] paIntPoint;
				//CPen AreaPenTest;
				//AreaPenTest.CreatePen(PS_SOLID, 6, RGB(255,255,0));
				//pDC->SelectObject(&AreaPenTest);
				//pDC->MoveTo( (int) pShape->m_aRectPointArray[0].dX,
				//	(int) pShape->m_aRectPointArray[0].dY );

				//for( int i = 1; i < (int) pShape->m_aRectPointArray.GetCount();i++ ) 
				//{
				//	//AreaPenTest.CreatePen(PS_SOLID, 6, RGB(255,255,1*i));
				//	//pDC->SelectObject(&AreaPenTest);

				//	pDC->LineTo( (int) pShape->m_aRectPointArray[i].dX,
				//		(int) pShape->m_aRectPointArray[i].dY );
				//}
				
			}
			CPen AreaPen;
			//colorStr = pShape->m_aProperties[2];	// border color from dbf

			if( "" != colorInfo.line)
			{
				colorStr = colorInfo.line.c_str(); //border color from URL
			}
			else 
			{
				colorStr = pShape->m_aProperties[2];// border color from dbf
			}	

			//draw board
			if(hasTransInfo || !colorStr.CompareNoCase("HHHHHH"))
			{	
				AreaColorManager.SetAreaBorderColor(colorStr);

				//highlight Area
				///////////////////////////////////////////////////////////////////////
				//CPolygonShape* pPolygonShape = (CPolygonShape*) pShape;
				//CString strALabel = AreaLabel.ExtractAreaLabel( pPolygonShape );
				//bool isHLLabel    = AreaLabel.HasHighlightLabel(strALabel, strHLArea);
				//if( isHLArea && isHLLabel )


				//this is for highlight border drawing
				if( "" != strColor )
				{
					AreaColorManager.SetAreaBorderColor( strColor );
				}
                ///////////////////////////////////////////////////////////////////////


				COLORREF color = AreaColorManager.GetAreBorderColor();
				//COLORREF color = bgr(str2colorref(colorStr, 3));// border color
				CTransInfoManager tranManager;

				if( AreaColorManager.M_BLACK == color )
				{
					//color = RGB(255,255,255);
					color = AreaColorManager.M_WHITE;
				}
				if(hasTransInfo)	// encode transparency info from request
				{
					int trans = transInfo.values[0];
					if( 0 == trans )// .0xx means boundary invisible in gif version
					{
						break;
					}
					tranManager.SetTransInfo( trans, color );
					color = tranManager.GetTransInfo();
				}
				if( isGIFOutput )
				{
					color = AreaColorManager.GetGifPaletteColor(color);
				}

				//if(isGIFOutput)
				//{
				//	logBrush.lbColor = AreaColorManager.GetGifPaletteColor(color);//color;//RGB(255,255,0);//
				//}
				//else
				//{
				//	logBrush.lbColor = color;//RGB(255,255,0);//
				//}


				// else transparency info has already encoded in dbf file
				if( (0 != nPenWidth) && (!isLatLong) )
				{			
					if(!isGIFOutput)
					{
						AreaPen.CreatePen(PS_SOLID, 8, color);
					}
					else
					{
						AreaPen.CreatePen(PS_SOLID, 8, bgr(color));
					}
				}
 				else
				{
					AreaPen.CreatePen(PS_SOLID, 1, color);
				}
				pDC->SelectObject(&AreaPen);
				//draw polygon border
				int	iVertex = 0;
				int nCount = (int) pShape->m_aPointArray.GetCount();
				/*
				int nStartPointX = (int)pShape->m_aPointArray[0].dX;
				int nStartPointY = (int)pShape->m_aPointArray[0].dY;*/
				int nPartStart = 0, nPartEnd = 0, nPoints = 0;
				int nParts = pShape->m_aPartsStartingIndex.GetCount();
				int nStartPointX = round( pShape->m_aPointArray[0].dX );
				int nStartPointY = round( pShape->m_aPointArray[0].dY );
				if (nParts < 1)
				{
					VERIFY(false);
				}
				for(int i = 0; i < nParts; i++ )
				{
					int nPartStartIndex = pShape->m_aPartsStartingIndex[i];

					int nPartEndIndex = 0;

					if ( i != (nParts - 1) )
					{
						nPartEndIndex = pShape->m_aPartsStartingIndex[i+1];														
					}
					else
					{	
						nPartEndIndex = pShape->m_aPointArray.GetCount();
					}	 
					//if(nParts > 1)
					{
						pDC->MoveTo( round( pShape->m_aPointArray[nPartStartIndex].dX),
							round( pShape->m_aPointArray[nPartStartIndex].dY ));

						for( iVertex = nPartStartIndex; iVertex < nPartEndIndex; iVertex++ )
						{
							/*pDC->LineTo( (int) pShape->m_aPointArray[iVertex].dX,
							(int) pShape->m_aPointArray[iVertex].dY );*/

							pDC->LineTo( round( pShape->m_aPointArray[iVertex].dX ),
								round( pShape->m_aPointArray[iVertex].dY ) );
						}
					}

				}//end for
			}
		}
		break;

	default:
		VERIFY(false);
		//LOG ..... impossible!!!
		break;
	}
	return true;
}

bool CFormatPaintTool::DrawLabel(CShape* pShape, ShapeUtility::ShapeType shapeType)
{
	CDC* pDC = m_pCanvas->getCanvasDC();
	ASSERT( pDC != NULL );	
	double nFontBufferWidthRatio = 1.24999;

	switch ( shapeType )
	{
	case ShapeUtility::unknown:
		VERIFY(false);
		break ;
	case ShapeUtility::point:
		{ 
			ASSERT( pShape->GetType() == ShapeUtility::point );




			//if( FA_OWNER == iSpecifiedLableColumn)
			//{
			//	//olny show owner, not show addr shape
			//	CLabelManager lm;
			//	if("" == lm.GetSpecifiedLable((CPointShape*)pShape, FA_OWNER))//if this is addr, the owner column should be blank
			//	{
			//		break;
			//	}
			//}
			//if( FA_ADDR == iSpecifiedLableColumn)
			//{
			//	//only show addr, not show owner shape
			//	CLabelManager lm;
			//	if("" != lm.GetSpecifiedLable((CPointShape*)pShape, FA_OWNER))//if this is owner, the owner column should not be blank
			//	{
			//		break;
			//	}
			//}



			CPointShape* pPointShape = (CPointShape*) pShape;
			CLabelManager pointLabel;

			/*if( -1 != iSpecifiedLableColumn )
			{
				if( pointLabel.HasIgnoredLableColumn(pPointShape, iSpecifiedLableColumn) )
				{
					break;
				}
			}*/

			//here adjust the font size and the SM size
			int nFontHeight = (int)( 12 * dEnlargeRatio );//12
			int nFontWidth = 8;//8
			int nIndexLabel = 0;//get name of point index from properties
			double nLetterSpaceWidth = 1;
			//nFontBufferWidthRatio = 0.54999;
			////////////////////////////////////////////////////////

			
			////////////////////////////////////////////////////////
			if( !(m_pPlaceHolder->IsCircleInBoundary( 
				CCircle(pShape->m_aPointArray[0].dX, 
				pShape->m_aPointArray[0].dY, 
				(nFontWidth/2))) ) )
			{
				break;
			}
			//////////////////////////////////////////////////////////////////


			//SM
			m_pPlaceHolder->isPointPlaceholder = false;//initial it
			m_pPlaceHolder->hasBigSpaceForLabelConflict = false;//just for lines, coz lines ask bigger space for looking nice

			if( (1 == (int)dEnlargeRatio ) || (0 == (int)dEnlargeRatio) )
			{
				nLetterSpaceWidth = 1;
				////log it is point holder
				m_pPlaceHolder->isPointPlaceholder = true;
			}
			else
			{
				//nLetterSpaceWidth = (int)( 8 * dEnlargeRatio );
				nLetterSpaceWidth = ( 5 * dEnlargeRatio );
				//enlargeratio>1, the isPointPlaceholder = false
				m_pPlaceHolder->isPointPlaceholder = false;
				if( dEnlargeRatio > 2 )
				{
					m_pPlaceHolder->hasBigSpaceForLabelConflict = true;
				}
			}
			////////////////////////////////////////////////////////
			//CLabelManager pointLabel;
			CString sLable;
			if( -1 == iSpecifiedLableColumn )
			{
				sLable = pointLabel.ExtractPointLabel( pPointShape );
			}
			else
			{
				sLable = pointLabel.GetSpecifiedLable( pPointShape, this->iSpecifiedLableColumn );
			}
			if( "" == sLable )
			{
				break;
			}


			//////////////////////////////////////////////////////////////////
			//for XML
			if( isXMLOutput )
			{
				double dTempX1 = m_pTransformer->m_pLinearTransformer->MapToDate_X( 
					pShape->m_aPointArray[0].dX );
				double dTempY1 = m_pTransformer->m_pLinearTransformer->MapToDate_Y( 
					pShape->m_aPointArray[0].dY );

				if( isXMLUTM )
				{
					m_pTransformer->m_pLinearTransformer->GDCToUTM(dTempX1, dTempY1);
				}			

				if( m_pPlaceHolder->IsCircleInBoundary( 
					CCircle((int)(pShape->m_aPointArray[0].dX), 
					(int)(pShape->m_aPointArray[0].dY), (nFontWidth/2))) )
				{
					( m_pCanvas->nTotalPoint ) ++;   
					m_pCanvas->AddXMLData(CXMLData(POINT_TYPE, sLable.GetString(), 
						dTempX1, dTempY1));

					break;
				}
			}
			//////////////////////////////////////////////////////////////////
			////get the first line letters length
			////just for the realtor point, this point has one line "xx/yy" label
			////we need to split it into two lines
			int nFirstLineNumLetter = 0;
			nFirstLineNumLetter = sLable.Find("/") - 1;
			if(0 > nFirstLineNumLetter)
			{
				nFirstLineNumLetter = 0;
			}
			//get the total letters length
			int nNumLetter = (int) sLable.GetLength();
			//label length = width + space
			int nLengthLabel = 0;
			if( ("" != pPointShape->m_aProperties[6]) | (0 != nFirstLineNumLetter) )
			{
				nLengthLabel = nFontWidth * nFirstLineNumLetter + nLetterSpaceWidth * (nFirstLineNumLetter -1);
			}
			else
			{
				nLengthLabel = nFontWidth * nNumLetter + nLetterSpaceWidth * (nNumLetter -1);
			}
			//everyletter's room
			int nLetterRoom = nFontWidth + nLetterSpaceWidth;
			int nStartDistance = - ( nLengthLabel / 2 );
			double dRadianLetter = 0; 
			double degree = 0;
			//double nFontBufferWidthRatio = 1.24999;//1.245;//width ratio, 1.5 is pretty good
			int nFontBufferWidth = (int)(nFontBufferWidthRatio * nFontWidth)/2;
			//int nFontBufferWidth = (int)(1 * nFontWidth)/3;
			int nPictoWidth  = 16, nPictoHeight = 16;			
			CCircleList letterPlaceListBottom, letterPlaceListRight, letterPlaceListUp, letterPlaceListLeft;
			CDoublePoint posLetterBottom, posLetterRight, posLetterUp, posLetterLeft;

			/////////////////////////////////////////////////////////////////////
			////for two lines point
			CDoublePoint firstPosLetterBottom, firstLetterBottomForPlaceholder;
			bool isTwoLines = false, hasTwoLines = false;
			int nextLineLetter = 0, nextLineLetter1 = 0;
			int nTotalSlash = 0;
			//////////////////////////////////////////////////////////////////////

			for(int i = 0; i < nNumLetter; i++ )
			{
				if( '/' == sLable.GetAt(i) )
				{
					isTwoLines     = true;
					nextLineLetter = 0;
					nTotalSlash++;
					continue;
				}
				if( !isTwoLines )////only has one line point
				{
					pPointShape->getPosTangentRadianByDistance(
						nStartDistance + i*nLetterRoom, &posLetterBottom, &dRadianLetter );
				}
				else////go to the next line
				{
					pPointShape->getPosTangentRadianByDistance(
						nStartDistance + nextLineLetter*nLetterRoom, &posLetterBottom, &dRadianLetter );
					posLetterBottom.dY = posLetterBottom.dY + nFontHeight*nTotalSlash;
					nextLineLetter++;
				}
				//letter position
				//move the location of label down one nPictoHeight
				posLetterBottom.dY = posLetterBottom.dY + ( nPictoHeight / 2 );

				posLetterRight.dX = posLetterBottom.dX + ( nLengthLabel / 2 ) + ( nPictoWidth / 2 ); 
				//posLetterRight.dY = posLetterBottom.dY - ( nPictoHeight / 2 );
				posLetterRight.dY = posLetterBottom.dY - nPictoHeight;
				posLetterUp.dX    = posLetterBottom.dX;
				posLetterUp.dY    = posLetterBottom.dY - nPictoHeight - nFontHeight;
				posLetterLeft.dX  = posLetterRight.dX - nPictoWidth - nLengthLabel - (nPictoWidth/2) ;
				posLetterLeft.dY  = posLetterRight.dY;

				double dAngleCos = 1;
				double dAngleSin = 0;

				/*int nXCenterBottom = (int)( (posLetterBottom.dX )+ ((nFontWidth * (dAngleCos) + (nFontHeight) * (-dAngleSin)) / 2) );
				int nYCenterBottom = (int)( (posLetterBottom.dY )+ ((nFontWidth * (dAngleSin) + (nFontHeight) * ( dAngleCos)) / 2) );

				int nXCenterRight  = (int)( (posLetterRight.dX )+ ((nFontWidth * (dAngleCos) + (nFontHeight) * (-dAngleSin)) / 2) );
				int nYCenterRight  = (int)( (posLetterRight.dY )+ ((nFontWidth * (dAngleSin) + (nFontHeight) * ( dAngleCos)) / 2) );

				int nXCenterUp     = (int)( (posLetterUp.dX )+ ((nFontWidth * (dAngleCos) + (nFontHeight) * (-dAngleSin)) / 2) );
				int nYCenterUp     = (int)( (posLetterUp.dY )+ ((nFontWidth * (dAngleSin) + (nFontHeight) * ( dAngleCos)) / 2) );

				int nXCenterLeft   = (int)( (posLetterLeft.dX )+ ((nFontWidth * (dAngleCos) + (nFontHeight) * (-dAngleSin)) / 2) );
				int nYCenterLeft   = (int)( (posLetterLeft.dY )+ ((nFontWidth * (dAngleSin) + (nFontHeight) * ( dAngleCos)) / 2) );*/



				int nXCenterBottom = round( (posLetterBottom.dX )+ ((nFontWidth * (dAngleCos) + (nFontHeight) * (-dAngleSin)) / 2) );
				int nYCenterBottom = round( (posLetterBottom.dY )+ ((nFontWidth * (dAngleSin) + (nFontHeight) * ( dAngleCos)) / 2) );

				int nXCenterRight  = round( (posLetterRight.dX )+ ((nFontWidth * (dAngleCos) + (nFontHeight) * (-dAngleSin)) / 2) );
				int nYCenterRight  = round( (posLetterRight.dY )+ ((nFontWidth * (dAngleSin) + (nFontHeight) * ( dAngleCos)) / 2) );

				int nXCenterUp     = round( (posLetterUp.dX )+ ((nFontWidth * (dAngleCos) + (nFontHeight) * (-dAngleSin)) / 2) );
				int nYCenterUp     = round( (posLetterUp.dY )+ ((nFontWidth * (dAngleSin) + (nFontHeight) * ( dAngleCos)) / 2) );

				int nXCenterLeft   = round( (posLetterLeft.dX )+ ((nFontWidth * (dAngleCos) + (nFontHeight) * (-dAngleSin)) / 2) );
				int nYCenterLeft   = round( (posLetterLeft.dY )+ ((nFontWidth * (dAngleSin) + (nFontHeight) * ( dAngleCos)) / 2) );



				letterPlaceListBottom.AddHead(CCircle(nXCenterBottom, nYCenterBottom, nFontBufferWidth ));
				letterPlaceListRight.AddHead(CCircle(nXCenterRight, nYCenterRight, nFontBufferWidth ));
				letterPlaceListUp.AddHead(CCircle(nXCenterUp, nYCenterUp, nFontBufferWidth ));
				letterPlaceListLeft.AddHead(CCircle(nXCenterLeft, nYCenterLeft, nFontBufferWidth ));
			}

			bool isPutBottom = false;
			bool isPutRight  = false;
			bool isPutUp     = false;
			bool isPutLeft   = false;

			if ( m_pPlaceHolder->IsPlaceAvailable(letterPlaceListBottom) )
			{
				isPutBottom = true;
				m_pPlaceHolder->AddHolder(letterPlaceListBottom);
				m_pPlaceHolder->isPointPlaceholder = false;
			}
			else if( m_pPlaceHolder->IsPlaceAvailable(letterPlaceListRight) )
			{
				isPutRight = true;
				m_pPlaceHolder->AddHolder(letterPlaceListRight);
				m_pPlaceHolder->isPointPlaceholder = false;
			}
			else if( m_pPlaceHolder->IsPlaceAvailable(letterPlaceListUp) )
			{
				isPutUp = true;
				m_pPlaceHolder->AddHolder(letterPlaceListUp);
				m_pPlaceHolder->isPointPlaceholder = false;
			}
			else if( m_pPlaceHolder->IsPlaceAvailable(letterPlaceListLeft) )
			{
				isPutLeft = true;
				m_pPlaceHolder->AddHolder(letterPlaceListLeft);
				m_pPlaceHolder->isPointPlaceholder = false;
			}
			else
			{
				m_pPlaceHolder->isPointPlaceholder = false;
				break;
			}

			int nTotalSlash1 = 0;
			posLetterBottom.dX = 0;
			posLetterBottom.dY = 0;
			nextLineLetter1 = 0;

			for(int i = 0; i < nNumLetter; i++ )
			{
				char letter = sLable.GetAt(i);
				if( '/' == letter )
				{
					hasTwoLines     = true;
					nextLineLetter1 = 0;
					nTotalSlash1++;
					continue;
				}
				if( !hasTwoLines )////only has one line point
				{
					pPointShape->getPosTangentRadianByDistance(
						nStartDistance + i*nLetterRoom, &posLetterBottom, &dRadianLetter );
				}
				else////go to the next line
				{
					pPointShape->getPosTangentRadianByDistance(
						nStartDistance + nextLineLetter1*nLetterRoom, &posLetterBottom, &dRadianLetter );
					posLetterBottom.dY = posLetterBottom.dY + nFontHeight*nTotalSlash1;
					nextLineLetter1++;
				}

				CFont font;
				VERIFY(font.CreateFont(
					nFontHeight,               // nHeight
					0,                         // nWidth
					0,                         // nEscapement unit in 0.1
					0,                         // nOrientation
					FW_BOLD,//                  // nWeight
					FALSE,                     // bItalic
					FALSE,                     // bUnderline
					0,                         // cStrikeOut
					ANSI_CHARSET,              // nCharSet
					OUT_DEFAULT_PRECIS,        // nOutPrecision
					CLIP_DEFAULT_PRECIS,       // nClipPrecision
					ANTIALIASED_QUALITY,//DEFAULT_QUALITY,           // nQuality
					DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
					"Lucida Console"));  //verdana


				CFont* def_font = pDC->SelectObject(&font);
				COLORREF color;

				//color comes from request
				int nVolumnSize = (int)(pShape->m_aProperties.GetSize());
				CString strLabelColor = pShape->m_aProperties[1];
				CColorManager pointLabelColor;

				pointLabelColor.SetPointLabelColor(colorInfo, strLabelColor, nVolumnSize);
				color = pointLabelColor.GetPointLabelColor();
				color = pointLabelColor.AddShadow( color );
				/*pDC->SetTextColor( color );*/
				//char letter = sLable.GetAt(i);
				//if(letter < 33 || letter > 126)//the scope of ascii
				//{
				//	letter = ' ';
				//}

				CString sLetter( letter );

				//move the location of label down one pictoheight
				posLetterBottom.dY = posLetterBottom.dY+ ( nPictoHeight / 2 );

				posLetterRight.dX = posLetterBottom.dX + ( nLengthLabel / 2 ) + ( nPictoWidth / 2 ); 
				//posLetterRight.dY = posLetterBottom.dY - ( nPictoHeight / 2 );
				posLetterRight.dY = posLetterBottom.dY - nPictoHeight;
				posLetterUp.dX    = posLetterBottom.dX;
				posLetterUp.dY    = posLetterBottom.dY - nPictoHeight - ( nFontHeight / 1 );
				posLetterLeft.dX  = posLetterRight.dX - nPictoWidth - nLengthLabel- (nPictoWidth/2);
				posLetterLeft.dY  = posLetterRight.dY;


				//for label density level
				GetLabelDensityLevel(nldensity, &isPutBottom, &isPutRight, &isPutUp, &isPutLeft);


				if( isPutBottom )
				{
					if( isPNGOutput )
					{
						//pointLabelColor.DrawPointLabelShadow(pDC, posLetterBottom.dX, posLetterBottom.dY, sLetter);
						//Felix
						if (hasShadow)
						{
							pointLabelColor.DrawPNGPointLabelShadow(pDC, posLetterBottom.dX, posLetterBottom.dY, sLetter);
						}
						pDC->SetTextColor( color );
					}
					else if( isGIFOutput )
					{
						if (hasShadow)
						{
							pointLabelColor.DrawPointLabelShadow(pDC, posLetterBottom.dX, posLetterBottom.dY, sLetter);
						}
						pDC->SetTextColor( pointLabelColor.GetGifPaletteColor(bgr(color)) );
					}
					else
					{
						pDC->SetTextColor( color );
					}
					/*pDC->TextOut( (int) posLetterBottom.dX,
						(int) posLetterBottom.dY,
						sLetter );*/

					pDC->TextOut( round( posLetterBottom.dX ),
						round( posLetterBottom.dY ),
						sLetter );
				}
				else if( isPutRight )
				{
					if( isPNGOutput )
					{
						//pointLabelColor.DrawPointLabelShadow(pDC, posLetterRight.dX, posLetterRight.dY, sLetter);
						if (hasShadow)
						{
							pointLabelColor.DrawPNGPointLabelShadow(pDC, posLetterRight.dX, posLetterRight.dY, sLetter);
						}
						pDC->SetTextColor( color );
					}
					else if( isGIFOutput )
					{
						if (hasShadow)
						{
							pointLabelColor.DrawPointLabelShadow(pDC, posLetterRight.dX, posLetterRight.dY, sLetter);
						}
						pDC->SetTextColor( pointLabelColor.GetGifPaletteColor(bgr(color)) );
					}
					else
					{
						pDC->SetTextColor( color );
					}
					/*pDC->TextOut( (int) posLetterRight.dX,
						(int) posLetterRight.dY,
						sLetter );*/
					pDC->TextOut( round( posLetterRight.dX),
						round( posLetterRight.dY),
						sLetter );
				}
				else if( isPutUp )
				{
					if( isPNGOutput )
					{
						//pointLabelColor.DrawPointLabelShadow(pDC, posLetterRight.dX, posLetterRight.dY, sLetter);
						if (hasShadow)
						{
							pointLabelColor.DrawPNGPointLabelShadow(pDC, posLetterUp.dX, posLetterUp.dY, sLetter);
						}
						pDC->SetTextColor( color );
					}
					else if( isGIFOutput )
					{
						if (hasShadow)
						{
							pointLabelColor.DrawPointLabelShadow(pDC, posLetterUp.dX, posLetterUp.dY, sLetter);
						}
						pDC->SetTextColor( pointLabelColor.GetGifPaletteColor(bgr(color)) );
					}
					else
					{
						pDC->SetTextColor( color );
					}
					/*pDC->TextOut( (int) posLetterUp.dX,
					              (int) posLetterUp.dY,
					              sLetter );*/

					pDC->TextOut( round( posLetterUp.dX),
					              round( posLetterUp.dY),
					              sLetter );
				}
				else if( isPutLeft )
				{
					if( isPNGOutput )
					{
						//pointLabelColor.DrawPointLabelShadow(pDC, posLetterRight.dX, posLetterRight.dY, sLetter);
						if (hasShadow)
						{
							pointLabelColor.DrawPNGPointLabelShadow(pDC, posLetterLeft.dX, posLetterLeft.dY, sLetter);
						}
						pDC->SetTextColor( color );
					}
                    else if( isGIFOutput )
					{
						if (hasShadow)
						{
							pointLabelColor.DrawPointLabelShadow(pDC, posLetterLeft.dX, posLetterLeft.dY, sLetter);
						}
						pDC->SetTextColor( pointLabelColor.GetGifPaletteColor(bgr(color)) );
					}
					else
					{
						pDC->SetTextColor( color );
					}
					/*pDC->TextOut( (int) posLetterLeft.dX,
						(int) posLetterLeft.dY,
						sLetter );*/

					pDC->TextOut( round( posLetterLeft.dX),
						round( posLetterLeft.dY),
						sLetter );
				}
				else
				{
					LOG("isPutBottom and isPutRight are not TRUE!!!");
					break;
				}
				pDC->SelectObject(def_font);
				// Done with the font.  Delete the font object.
				font.DeleteObject(); 
			}
		}

		break;

	case ShapeUtility::arc:
		{
			ASSERT( pShape->GetType() == ShapeUtility::arc );
			CLineShape* pLineShape = (CLineShape*) pShape;
			double dLengthLine = pLineShape->GetLength();
			//int nFontWidth = (int) (7 * dEnlargeRatio);// use for letter room
			//int nFontHeight = (int) (12 * dEnlargeRatio) ;//used
			//int nIndexLabel = 0, nLetterSpaceWidth = 4;
			int nFontWidth = (int) (6 * dEnlargeRatio);// use for letter room
			int nFontHeight = (int) (11 * dEnlargeRatio) ;//used
			int nIndexLabel = 0, nLetterSpaceWidth = 0;
			CLabelManager arcLabel;
			CString sLable = arcLabel.ExtractArcLabel( pLineShape );
			/////////////////////////////////////////////////////////
			//SM
			m_pPlaceHolder->hasBigSpaceForLabelConflict = false;
			if( (1 == (int)dEnlargeRatio ) || (0 == (int)dEnlargeRatio) )
			{
				nLetterSpaceWidth = 4;
			}
			else
			{
				m_pPlaceHolder->hasBigSpaceForLabelConflict = true;
			}
			//////////////////////////////////////////////////////////
			if( "RAMP" == sLable.Trim() )
			{
				break;
			}
			int nLengthLine = (int) pLineShape->GetLength();
			int nNumLetter = (int) sLable.GetLength();
			int nLengthLabel = nFontWidth * nNumLetter + nLetterSpaceWidth * (nNumLetter -1);
			int nLetterRoom = nFontWidth + nLetterSpaceWidth;
			//////////////////////////////////////////////////////
			int nEndPointBuffer = 10;
			int nRestTryLength = nLengthLine - nLengthLabel;

			if(nRestTryLength <=  nEndPointBuffer )
			{
				break;
			}
			int nStartDistance = (nLengthLine - nLengthLabel) / 2;
			///////////////////////////////////////////////////////
			//CDoublePoint posLetter;
			double dRadianLetter = 0; 
			double degree = 0;
			//double nFontBufferWidthRatio = 1;//1.249999;
			int nFontBufferWidth;
			if(m_pPlaceHolder->hasBigSpaceForLabelConflict)
			{
				nFontBufferWidth = (int)(1 * nFontWidth/2);
			}
			else
			{
				nFontBufferWidth = (int)(nFontBufferWidthRatio * nFontWidth/2);
			}
			//
			
			//if (( nLengthLine ) < nLengthLabel )
			//{
			//	break;// break to swith; end draw
			//}
			int nCount = (int) pLineShape->m_aPointArray.GetCount();
			if (pLineShape->m_aPointArray[0].dX > pLineShape->m_aPointArray[nCount-1].dX )
			{
				pLineShape->Rewind();
			}

			CCircleList letterPlaceList, letterPlaceListInOneThird;
			CDoublePoint posLetter, posLetterOneThird;
			//////////////////////
			CList<double> firstLetterDegreeList;
			CList<double> secondLetterDegreeList;
			double firstLastLetterDegree = 0.0, degreeOffset = 0.0, secondLastLetterDegree = 0.0;
			double dFirstLetterX = 0.0, dFirstLetterY = 0.0, dFirstDegree  = 0.0;			
			bool   isFirstDegreeOffsetOK  = true, isSecondDegreeOffsetOK = true;
			bool   isFirstDegreeOK = true, isSecondDegreeOK = true;
			int    nFirstReverseDegree = 0, nFirstDegree = 0;
			int    nSecondReverseDegree = 0, nSecondDegree = 0;
			int    nFirstLetterX = 0, nFirstLetterY = 0, nFirstLetterR = 0;
			//////////////////////
			for(int i = 0; i < nNumLetter; i++ )
			{
				pLineShape->getPosTangentRadianByDistance(
					nStartDistance + i*nLetterRoom, &posLetter, &dRadianLetter );
				//posLetter.dX 
				double dAngleCos = cos( dRadianLetter );
				double dAngleSin = sin( dRadianLetter );//2*PI -
				//int nXCenter = (int) (posLetter.dX )+ ((nFontWidth * (dAngleCos) + (nFontHeight) * (-dAngleSin)) / 2);
				//int nYCenter = (int) (posLetter.dY )+ ((nFontWidth * (dAngleSin) + (nFontHeight) * ( dAngleCos)) / 2);
				/*int nXCenter = (int) ( (posLetter.dX )+ ((nFontWidth * (dAngleCos) + (nFontHeight) * (-dAngleSin)) / 2) );
				int nYCenter = (int) ( (posLetter.dY )+ ((nFontWidth * (dAngleSin) + (nFontHeight) * ( dAngleCos)) / 2) );*/

				int nXCenter = round ( (posLetter.dX )+ ((nFontWidth * (dAngleCos) + (nFontHeight) * (-dAngleSin)) / 2) );
				int nYCenter = round ( (posLetter.dY )+ ((nFontWidth * (dAngleSin) + (nFontHeight) * ( dAngleCos)) / 2) );
				letterPlaceList.AddHead(CCircle(nXCenter, nYCenter, nFontBufferWidth ));
				/////////////////////////////////////////////////////////
				double currentDegree = 360 - dRadianLetter*180/PI;
				if( (currentDegree >90)&&(currentDegree <270) )
				{
					nFirstReverseDegree++;
				}
				else
				{
					nFirstDegree++;
				}

				if( firstLetterDegreeList.IsEmpty() )
				{
					firstLastLetterDegree = 0;
				}
				else
				{
					firstLastLetterDegree = firstLetterDegreeList.GetHead();
				}
				firstLetterDegreeList.AddHead( currentDegree );
				degreeOffset = abs(currentDegree - firstLastLetterDegree);

				if( (degreeOffset > 150) && (degreeOffset < 210) )
				{
					isFirstDegreeOffsetOK = false;
				}
				///////////////////////////////////////////////////////////
				if( 0 == i )
				{
					dFirstLetterX = posLetter.dX;
					dFirstLetterY = posLetter.dY;
					dFirstDegree  = currentDegree;//rotation degree
					nFirstLetterX = nXCenter;
					nFirstLetterY = nYCenter;
					nFirstLetterR = nFontBufferWidth;//radium
				}
			}
			//////////////////////////////////////////////
			if( nFirstDegree < nFirstReverseDegree )
			{
				isFirstDegreeOK = false;
			}
			//////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////
			//for XML
			/*if( (dFirstLetterX > m_pPlaceHolder->m_rectBoundary.dX1) &&
			(dFirstLetterX < m_pPlaceHolder->m_rectBoundary.dX2) && 
			(dFirstLetterY > m_pPlaceHolder->m_rectBoundary.dY1) && 
			(dFirstLetterY < m_pPlaceHolder->m_rectBoundary.dY2))*/
			//double dTempX1 = m_pTransformer->m_pLinearTransformer->MapToDate_X( 
			//	dFirstLetterX );
			//double dTempY1 = m_pTransformer->m_pLinearTransformer->MapToDate_Y( 
			//	dFirstLetterY );

			//if( m_pPlaceHolder->IsCircleInBoundary( 
			//	CCircle(nFirstLetterX, nFirstLetterY, nFirstLetterR)) )
			//{
			//	( m_pCanvas->nTotalArc ) ++;   
			//	m_pCanvas->AddXMLData(CXMLData(ARC_TYPE, sLable.GetString(), 
			//		dTempX1, dTempY1, dFirstDegree));
			//}
			//////////////////////////////////////////////////////////////////
			ASSERT( m_pPlaceHolder != NULL);
			bool isPutHalf = false, isPutOneThird = false;
			////////////////////////////////////////////////////////////////////////////
			double nOneThirdStartDistance = ( nLengthLine / 3 ) - ( nLengthLabel / 2 );
			double dOneThirdRadianLetter  = 0; 

			if (m_pPlaceHolder->IsPlaceAvailable(letterPlaceList) 
				&& isFirstDegreeOffsetOK 
				&& isFirstDegreeOK)
			{
				isPutHalf = true;
				m_pPlaceHolder->AddHolder(letterPlaceList);
			}
			else
			{
				////////////////////////////////////
				if ( nOneThirdStartDistance <= nEndPointBuffer )
				{
					break;// break to swith; end draw
				}
				/////////////////////////////////////
				for(int i = 0; i < nNumLetter; i++ )
				{
					pLineShape->getPosTangentRadianByDistance(
						nOneThirdStartDistance + i*nLetterRoom, &posLetterOneThird, &dOneThirdRadianLetter );

					double dAngleCos = cos( dOneThirdRadianLetter );
					double dAngleSin = sin( dOneThirdRadianLetter );

					//int nXCenter = (int) (posLetterOneThird.dX )+ ((nFontWidth * (dAngleCos) + (nFontHeight) * (-dAngleSin)) / 2);
					//int nYCenter = (int) (posLetterOneThird.dY )+ ((nFontWidth * (dAngleSin) + (nFontHeight) * ( dAngleCos)) / 2);
					/*int nXCenter = (int) ( (posLetterOneThird.dX )+ ((nFontWidth * (dAngleCos) + (nFontHeight) * (-dAngleSin)) / 2) );
					int nYCenter = (int) ( (posLetterOneThird.dY )+ ((nFontWidth * (dAngleSin) + (nFontHeight) * ( dAngleCos)) / 2) );*/

					int nXCenter = round ( (posLetterOneThird.dX )+ ((nFontWidth * (dAngleCos) + (nFontHeight) * (-dAngleSin)) / 2) );
					int nYCenter = round ( (posLetterOneThird.dY )+ ((nFontWidth * (dAngleSin) + (nFontHeight) * ( dAngleCos)) / 2) );
					letterPlaceListInOneThird.AddHead(CCircle(nXCenter, nYCenter, nFontBufferWidth ));

					/////////////////////////////////////////////////////////
					double currentDegree = 360 - dOneThirdRadianLetter*180/PI;

					if( (currentDegree >90)&&(currentDegree <270) )
					{
						nSecondReverseDegree++;
					}
					else
					{
						nSecondDegree++;
					}

					if( secondLetterDegreeList.IsEmpty() )
					{
						secondLastLetterDegree = 0;
					}
					else
					{
						secondLastLetterDegree = secondLetterDegreeList.GetHead();
					}
					secondLetterDegreeList.AddHead( currentDegree );
					degreeOffset = abs(currentDegree - secondLastLetterDegree);
					if( (degreeOffset > 150) && (degreeOffset < 210) )
					{
						isSecondDegreeOffsetOK = false;
					}
					///////////////////////////////////////////////////////////
				}
				//////////////////////////////////////////////
				if( nSecondDegree < nSecondReverseDegree )
				{
					isSecondDegreeOK = false;
				}
				//////////////////////////////////////////////
				if (m_pPlaceHolder->IsPlaceAvailable(letterPlaceListInOneThird) 
					&& isSecondDegreeOffsetOK 
					&& isSecondDegreeOK)
				{
					isPutOneThird = true;
					m_pPlaceHolder->AddHolder(letterPlaceListInOneThird);
				}
				else
				{
					break;
				}
			}
			////////////////////////////////////////////////////////////////////
			for(int i = 0; i < nNumLetter; i++ )
			{
				if( isPutHalf )
				{
					pLineShape->getPosTangentRadianByDistance(
						nStartDistance + i*nLetterRoom, &posLetter, &dRadianLetter );
				}
				else if( isPutOneThird )
				{
					pLineShape->getPosTangentRadianByDistance(
						nOneThirdStartDistance + i*nLetterRoom, &posLetter, &dRadianLetter );
				}
				else
				{
					break;
				}
				
				degree = dRadianLetter*180/PI;
				int nDgree = (int)((360-degree)*10);
				nDgree = arcLabel.ExtractAntiAliasingDegree(&nDgree);


				CFont font;
				VERIFY(font.CreateFont(
					nFontHeight,                        // nHeight
					0,                         // nWidth
					nDgree /* temp*10*/,                 // nEscapement unit in 0.1
					0,                         // nOrientation
					600,//FW_NORMAL,//FW_BOLD,                 // nWeight
					FALSE,                     // bItalic
					FALSE,                     // bUnderline
					0,                         // cStrikeOut
					ANSI_CHARSET,              // nCharSet
					OUT_DEFAULT_PRECIS,        // nOutPrecision
					CLIP_DEFAULT_PRECIS,       // nClipPrecision
					ANTIALIASED_QUALITY,//DEFAULT_QUALITY,           // nQuality
					DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
					"Lucida Console"));    
					//"verdana"));

				CFont* def_font = pDC->SelectObject(&font);
				COLORREF color;
				//if(pShape->m_aProperties.GetSize() >= 3)
				CColorManager arcColorManager;
				arcColorManager.SetArcLabelColor(colorInfo, hasStreets, hasSechwys, 
					hasMajHwys, hasDetailroads, hasMainroads);
				color = arcColorManager.GetArcLabelColor();
				color = arcColorManager.AddShadow( color );
				//pDC->SetTextColor( color );
				/*if(color == 0x61F061)
				{
					int x = 0;
				}*/
				char letter = sLable.GetAt(i);

				if(letter < 33 || letter > 126)//the scope of ascii
				{
					letter = ' ';
				}

				CString sLetter( letter );

				if( isPNGOutput )
				{
					//arcColorManager.DrawLineLabelShadow(pDC, posLetter.dX, posLetter.dY, sLetter);
					if (hasShadow)
					{
						arcColorManager.DrawPNGLineLabelShadow(pDC, posLetter.dX, posLetter.dY, sLetter, font, nFontHeight);
					}
					pDC->SetTextColor( color );
				}
				else if( isGIFOutput )
				{
					if (hasShadow)
					{
						arcColorManager.DrawLineLabelShadow(pDC, posLetter.dX, posLetter.dY, sLetter);
					}
					pDC->SetTextColor( arcColorManager.GetGifPaletteColor(bgr(color)) );
				}
				else
				{
					//color = arcColorManager.AddShadow( color );
					pDC->SetTextColor( arcColorManager.AddShadow( color ) );
				}
				pDC->TextOut( (int) posLetter.dX,
					(int) posLetter.dY,
					sLetter);
				pDC->SelectObject(def_font);
			    // Done with the font.  Delete the font object.
				font.DeleteObject(); 
			}

			firstLetterDegreeList.RemoveAll();
			secondLetterDegreeList.RemoveAll();
		}
		break;

	case ShapeUtility::polygon:

		{ 
			ASSERT( pShape->GetType() == ShapeUtility::polygon );
			CPolygonShape* pPolygonShape = (CPolygonShape*) pShape;

			
			if(hasTransInfo)	
			{
				int trans = transInfo.values[2];
				if( 0 == trans )// .xx0 means label invisible in gif version
				{
					break;
				}
			}

			double dLengthPolygon = pPolygonShape->GetLength();//double type length of street
			int nFontWidth = 7;
			int nFontHeight = (int)(12 * dEnlargeRatio);
			int nIndexLabel = 0;//get name of street index from properties
			int nLetterSpaceWidth = 2;
			/////////////////////////////////////////////////////
			//SM
			m_pPlaceHolder->hasBigSpaceForLabelConflict = false;//just for lines, coz lines ask bigger space for looking nice
			if( (1 == (int)dEnlargeRatio ) || (0 == (int)dEnlargeRatio) )
			{
				nLetterSpaceWidth = 2;
			}
			else
			{
				nLetterSpaceWidth = (int)(2 * 10 * dEnlargeRatio);
			}
			//////////////////////////////////////////////////////
			CLabelManager areaManager;
			CString sLable = areaManager.ExtractAreaLabel( pPolygonShape );
			//double dLengthPolygon = pPolygonShape->GetLength();//integer type length of street
			int nNumLetter = (int) sLable.GetLength();
			int nLengthLabel = nFontWidth * nNumLetter + nLetterSpaceWidth * (nNumLetter -1);
			int nLetterRoom = nFontWidth + nLetterSpaceWidth;

			int nPolygonParts = 10;//define 10 parts to calculate the mid of area
			double dStartDistance = dLengthPolygon / nPolygonParts;//draw label on 1/10 of length

			CDoublePoint posLetter, posPolygonLabelLetter;
			CDoublePoint posPolygonLetterTotalLength, posPolygonLabel;
			posPolygonLetterTotalLength.dX = 0, posPolygonLetterTotalLength.dY = 0;
			posPolygonLabel.dX = 0, posPolygonLabel.dY = 0;

			int nPolygonLabelStartPos = - ( nLengthLabel / 2 );
			double dRadianLetter = 0; 
			double degree = 0;
			//double nFontBufferWidthRatio = 1.24999;//width ratio, 1.5 is pretty good
			int nFontBufferWidth = (int)(nFontBufferWidthRatio * nFontWidth)/2;
			//Important!!
			//if ( nLengthPolygon < ( 4 * nLengthLabel )  )
			//{
			//	//tip: comes from (PI * D = Circumference), 4 is same to PI
			//	break;// break to swith; end draw
			//}
			for(int i = 0; i < nPolygonParts; i++ )
			{
				pPolygonShape->getPosTangentRadianByDistance(
					(i+1)* dStartDistance * (99.0/100.0), &posLetter, &dRadianLetter );

				posPolygonLetterTotalLength.dX = posPolygonLetterTotalLength.dX + posLetter.dX;
				posPolygonLetterTotalLength.dY = posPolygonLetterTotalLength.dY + posLetter.dY;

			}
			//polygon label's start pos
			posPolygonLabel.dX = posPolygonLetterTotalLength.dX / nPolygonParts;
			posPolygonLabel.dY = posPolygonLetterTotalLength.dY / nPolygonParts;
			//////////////////////////////////////////////////////////////////
			//for XML
			if( isXMLOutput )
			{
				double dTempX1 = m_pTransformer->m_pLinearTransformer->MapToDate_X( 
					posPolygonLabel.dX );
				double dTempY1 = m_pTransformer->m_pLinearTransformer->MapToDate_Y( 
					posPolygonLabel.dY );

				if( isXMLUTM )
				{
					m_pTransformer->m_pLinearTransformer->GDCToUTM(dTempX1, dTempY1);
				}	

				if( m_pPlaceHolder->IsCircleInBoundary( 
					CCircle(round(posPolygonLabel.dX), 
					round(posPolygonLabel.dY), (nFontWidth/2))) )
				{
					//dTempX1 = m_pTransformer->m_pLinearTransformer->MapToDate_X(posPolygonLabel.dX);
					//dTempY1 = m_pTransformer->m_pLinearTransformer->MapToDate_Y(posPolygonLabel.dY);


					( m_pCanvas->nTotalArea ) ++;   
					m_pCanvas->AddXMLData(CXMLData(AREA_TYPE, sLable.GetString(), 
						dTempX1, dTempY1));

					break;
				}
			}

			int nPictoWidth  = 16, nPictoHeight = 16;			
			CCircleList letterPlaceListBottom, letterPlaceListRight, letterPlaceListUp, letterPlaceListLeft;
			CDoublePoint posLetterBottom, posLetterRight, posLetterUp, posLetterLeft;

			for(int i = 0; i < nNumLetter; i++ )
			{
				pPolygonShape->getPolygonLetterPos(
					nPolygonLabelStartPos + i*nLetterRoom, posPolygonLabel.dX, posPolygonLabel.dY, &posPolygonLabelLetter, &dRadianLetter );

				posLetterBottom.dX = posPolygonLabelLetter.dX;
				posLetterBottom.dY = posPolygonLabelLetter.dY;

				posLetterRight.dX = posLetterBottom.dX + ( nLengthLabel / 2 ) + ( nPictoWidth / 2 ); 
				posLetterRight.dY = posLetterBottom.dY - ( nPictoHeight / 2 );
				//posLetterUp.dX    = posLetterBottom.dX;
				//posLetterUp.dY    = posLetterBottom.dY - nPictoHeight - nFontHeight;
				posLetterLeft.dX  = posLetterRight.dX - nPictoWidth - nLengthLabel;
				posLetterLeft.dY  = posLetterRight.dY;
				/////////////////////////////
				double dAngleCos = 1;
				double dAngleSin = 0;
				///////////////////////////////
				//int nXCenterBottom = (int)( (posLetterBottom.dX )+ ((nFontWidth * (dAngleCos) + (nFontHeight) * (-dAngleSin)) / 2) );
				//int nYCenterBottom = (int)( (posLetterBottom.dY )+ ((nFontWidth * (dAngleSin) + (nFontHeight) * ( dAngleCos)) / 2) );

				//int nXCenterRight  = (int)( (posLetterRight.dX )+ ((nFontWidth * (dAngleCos) + (nFontHeight) * (-dAngleSin)) / 2) );
				//int nYCenterRight  = (int)( (posLetterRight.dY )+ ((nFontWidth * (dAngleSin) + (nFontHeight) * ( dAngleCos)) / 2) );

				////int nXCenterUp     = (int)( (posLetterUp.dX )+ ((nFontWidth * (dAngleCos) + (nFontHeight) * (-dAngleSin)) / 2) );
				////int nYCenterUp     = (int)( (posLetterUp.dY )+ ((nFontWidth * (dAngleSin) + (nFontHeight) * ( dAngleCos)) / 2) );

				//int nXCenterLeft   = (int)( (posLetterLeft.dX )+ ((nFontWidth * (dAngleCos) + (nFontHeight) * (-dAngleSin)) / 2) );
				//int nYCenterLeft   = (int)( (posLetterLeft.dY )+ ((nFontWidth * (dAngleSin) + (nFontHeight) * ( dAngleCos)) / 2) );



				int nXCenterBottom = round( (posLetterBottom.dX )+ ((nFontWidth * (dAngleCos) + (nFontHeight) * (-dAngleSin)) / 2) );
				int nYCenterBottom = round( (posLetterBottom.dY )+ ((nFontWidth * (dAngleSin) + (nFontHeight) * ( dAngleCos)) / 2) );

				int nXCenterRight  = round( (posLetterRight.dX )+ ((nFontWidth * (dAngleCos) + (nFontHeight) * (-dAngleSin)) / 2) );
				int nYCenterRight  = round( (posLetterRight.dY )+ ((nFontWidth * (dAngleSin) + (nFontHeight) * ( dAngleCos)) / 2) );

				//int nXCenterUp     = (int)( (posLetterUp.dX )+ ((nFontWidth * (dAngleCos) + (nFontHeight) * (-dAngleSin)) / 2) );
				//int nYCenterUp     = (int)( (posLetterUp.dY )+ ((nFontWidth * (dAngleSin) + (nFontHeight) * ( dAngleCos)) / 2) );

				int nXCenterLeft   = round( (posLetterLeft.dX )+ ((nFontWidth * (dAngleCos) + (nFontHeight) * (-dAngleSin)) / 2) );
				int nYCenterLeft   = round( (posLetterLeft.dY )+ ((nFontWidth * (dAngleSin) + (nFontHeight) * ( dAngleCos)) / 2) );


				letterPlaceListBottom.AddHead(CCircle(nXCenterBottom, nYCenterBottom, nFontBufferWidth ));
				letterPlaceListRight.AddHead(CCircle(nXCenterRight, nYCenterRight, nFontBufferWidth ));
				//letterPlaceListUp.AddHead(CCircle(nXCenterUp, nYCenterUp, nFontBufferWidth ));
				letterPlaceListLeft.AddHead(CCircle(nXCenterLeft, nYCenterLeft, nFontBufferWidth ));
			}

			bool isPutBottom = false, isPutRight = false, isPutUp = false, isPutLeft = false;

			if ( m_pPlaceHolder->IsPlaceAvailable(letterPlaceListBottom) )
			{

				isPutBottom = true;
				m_pPlaceHolder->AddHolder(letterPlaceListBottom);

			}
			else if( m_pPlaceHolder->IsPlaceAvailable(letterPlaceListRight) )
			{

				isPutRight = true;
				m_pPlaceHolder->AddHolder(letterPlaceListRight);

			}
			//else if( m_pPlaceHolder->IsPlaceAvailable(letterPlaceListUp) )
			//{
			//	isPutUp = true;

			//	m_pPlaceHolder->AddHolder(letterPlaceListUp);
			//}
			else if( m_pPlaceHolder->IsPlaceAvailable(letterPlaceListLeft) )
			{
				isPutLeft = true;
				m_pPlaceHolder->AddHolder(letterPlaceListLeft);
			}
			else
			{
				break;
			}

			for(int i = 0; i < nNumLetter; i++ )
			{
				pPolygonShape->getPolygonLetterPos(
					nPolygonLabelStartPos + i*nLetterRoom, posPolygonLabel.dX, posPolygonLabel.dY, &posPolygonLabelLetter, &dRadianLetter );

				CFont font;
				VERIFY(font.CreateFont(
					nFontHeight,               // nHeight
					0,                         // nWidth
					0,                         // nEscapement unit in 0.1
					0,                         // nOrientation
					FW_BOLD,                 // nWeight
					FALSE,                     // bItalic
					FALSE,                     // bUnderline
					0,                         // cStrikeOut
					ANSI_CHARSET,              // nCharSet
					OUT_DEFAULT_PRECIS,        // nOutPrecision
					CLIP_DEFAULT_PRECIS,       // nClipPrecision
					ANTIALIASED_QUALITY,//DEFAULT_QUALITY,           // nQuality
					DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
					"Lucida Console"));    

				CFont* def_font = pDC->SelectObject(&font);
				COLORREF color;


				//CString strAreaColor = pShape->m_aProperties[1];
				CColorManager AreaColorManager;
				CString strAreaColor;

				if( "" != colorInfo.label)
				{
					strAreaColor = colorInfo.label.c_str();
				}
				else 
				{
					strAreaColor = pShape->m_aProperties[1];// area label color from dbf
				}	


				int nColumnSize = (int)(pShape->m_aProperties.GetSize());
				CColorManager areaLabelColor;
                areaLabelColor.SetAreaLabelColor(colorInfo, strAreaColor, nColumnSize);
				color = areaLabelColor.GetAreaLabelColor();
				//encode transparency
				CTransInfoManager tranManager;
				//encode transparency info from request
				if(hasTransInfo)	
				{
					int trans = transInfo.values[2];
					if( 0 == trans )// .xx0 means label invisible in gif version
					{
						break;
					}
					tranManager.SetTransInfo( trans, color );
					color = tranManager.GetTransInfo();
				}
				//pDC->SetTextColor(color);
				char letter = sLable.GetAt(i);
				if(letter < 33 || letter > 126)//the scope of ascii
				{
					letter = ' ';
				}
				CString sLetter( letter );
				/////////////////////////////
				posLetterBottom.dX = posPolygonLabelLetter.dX;
				posLetterBottom.dY = posPolygonLabelLetter.dY;

				posLetterRight.dX = posLetterBottom.dX + ( nLengthLabel / 2 ) + ( nPictoWidth / 2 ); 
				posLetterRight.dY = posLetterBottom.dY - ( nPictoHeight / 2 );
				//posLetterUp.dX    = posLetterBottom.dX;
				//posLetterUp.dY    = posLetterBottom.dY - nPictoHeight - ( nFontHeight / 1 );
				posLetterLeft.dX  = posLetterRight.dX - nPictoWidth - nLengthLabel;
				posLetterLeft.dY  = posLetterRight.dY;



				//for label density level
				GetLabelDensityLevel(nldensity, &isPutBottom, &isPutRight, &isPutUp, &isPutLeft);


				if( isPutBottom )
				{
					if( isPNGOutput )
					{
						//areaLabelColor.DrawAreaLabelShadow(pDC, posLetterBottom.dX, posLetterBottom.dY, sLetter);
						//pDC->SetTextColor( AreaColorManager.GetGifPaletteColor(bgr(color)) );
						if (hasShadow)
						{
							areaLabelColor.DrawPNGPointLabelShadow(pDC, posLetterBottom.dX, posLetterBottom.dY, sLetter);
						}
						pDC->SetTextColor( color );
						//pDC->SetTextColor(RGB(55,55,3));
					}
					else if( isGIFOutput )
					{
						if (hasShadow)
						{
							areaLabelColor.DrawAreaLabelShadow(pDC, posLetterBottom.dX, posLetterBottom.dY, sLetter);
						}
						pDC->SetTextColor( AreaColorManager.GetGifPaletteColor(bgr(color)) );
					}
					else
					{
						pDC->SetTextColor( color );
					}
					pDC->TextOut( round( posLetterBottom.dX),
						round( posLetterBottom.dY),
						sLetter );
				}
				else if( isPutRight )
				{
					if( isPNGOutput )
					{
						//areaLabelColor.DrawAreaLabelShadow(pDC, posLetterBottom.dX, posLetterBottom.dY, sLetter);
						//pDC->SetTextColor( AreaColorManager.GetGifPaletteColor(bgr(color)) );
						if (hasShadow)
						{
							areaLabelColor.DrawPNGPointLabelShadow(pDC, posLetterRight.dX, posLetterRight.dY, sLetter);
						}
						pDC->SetTextColor( color );
						//pDC->SetTextColor(RGB(55,55,3));
					}
					else if( isGIFOutput )
					{
						if (hasShadow)
						{
							areaLabelColor.DrawAreaLabelShadow(pDC, posLetterRight.dX, posLetterRight.dY, sLetter);
						}
						pDC->SetTextColor( AreaColorManager.GetGifPaletteColor(bgr(color)) );
					}
					else
					{
						pDC->SetTextColor( color );
					}
					pDC->TextOut( round( posLetterRight.dX),
						round( posLetterRight.dY),
						sLetter );
				}
				//else if( isPutUp )
				//{
				//	pDC->TextOut( (int) posLetterUp.dX,
				//	              (int) posLetterUp.dY,
				//	              sLetter );
				//}
				else if( isPutLeft )
				{
					if( isPNGOutput )
					{
						//areaLabelColor.DrawAreaLabelShadow(pDC, posLetterBottom.dX, posLetterBottom.dY, sLetter);
						//pDC->SetTextColor( AreaColorManager.GetGifPaletteColor(bgr(color)) );
						if (hasShadow)
						{
							areaLabelColor.DrawPNGPointLabelShadow(pDC, posLetterLeft.dX, posLetterLeft.dY, sLetter);
						}
						pDC->SetTextColor( color );
						//pDC->SetTextColor(RGB(55,55,3));
					}
					else if( isGIFOutput )
					{
						if (hasShadow)
						{
							areaLabelColor.DrawAreaLabelShadow(pDC, posLetterLeft.dX, posLetterLeft.dY, sLetter);
						}
						pDC->SetTextColor( AreaColorManager.GetGifPaletteColor(bgr(color)) );
					}
					else
					{
						pDC->SetTextColor( color );
					}
					pDC->TextOut( round( posLetterLeft.dX),
						round( posLetterLeft.dY),
						sLetter );
				}
				else
				{
					LOG("isPutBottom and isPutRight are not TRUE!!!");
					break;
				}
				/////////////////////////////
				pDC->SelectObject(def_font);
				// Done with the font.  Delete the font object.
				font.DeleteObject();
			}//endfor
		}//endcase
		break;
	default:
		VERIFY(false);
		//LOG ..... impossible!!!
		break;
	}
	return true;
}
bool CFormatPaintTool::SetCanvas(CMapCanvas* pCanvas)
{
	ASSERT( pCanvas != NULL );
	m_pCanvas = pCanvas;
	return true;
}
bool CFormatPaintTool::SetTransformer(CCoordinateTransformer* pTransformer)
{
	ASSERT( pTransformer != NULL );
	m_pTransformer = pTransformer;
	return true;
}
