#include "StdAfx.h"
#include "lineshape.h"
#include "LineLayer.h"

CLineLayer::CLineLayer(void)
{
}

CLineLayer::~CLineLayer(void)
{
}

/*
	virtual bool DrawLabel(CMapCanvas* canvas) = 0;
	// Drew shape (point, line, polygon) on canvas
	virtual bool DrawShape(CMapCanvas* canvas) = 0;

*/

// Draw the Labels of shape layer 
bool CLineLayer::DrawTypeLabel( )
{
	
	int nCount = (int) getShapeObjectCount();
	for( int i = 0; i< nCount; i++ )
	{
		m_formatPaintTool.DrawLabel( getShapeObject( i ), ShapeUtility::arc );
	}
	

	return false;
}

// Drew shape (point, line, polygon) on canvas
bool CLineLayer::DrawTypeShape( )
{

	int nCount = (int) getShapeObjectCount();
	for( int i = 0; i< nCount; i++ )
	{
		m_formatPaintTool.DrawShape( getShapeObject( i ), ShapeUtility::arc, "", 0 );
	}
	
	return false;
}



bool CLineLayer::MergePartsTo(CShapeLayer* pShapeLayer)
{

	//CTypedPtrList<CPtrList, LineGroup*> groups;

	typedef CList< CShape* > CShapeList;
	CShapeList* pShapeExistGroupList = NULL;
	CShapeList* pShapeNewGroupList = NULL;

	CMapStringToPtr groupsStreetByName;

	CString sFieldValue; // field value 
	POSITION posPreForward, posCurrent;

	int nConnectSum = 0;// connect count
	
	//int nIndexGroupField = GetLabelFieldIndex();

	//
	int nCount = getShapeObjectCount();
	for( int i = 0; i < nCount; i++ )
	{
		
		/* get the shape to be be cloned here ????!!! do not use pointer!!! new */ 
		ASSERT(getShapeObject(i)->GetType() == ShapeUtility::arc);
		CShape* pShapeToAdd = new CLineShape(*getShapeObject(i)); // (*pShapeToAddTemp).Clone(); //&shapeToAdd;

		ASSERT( pShapeToAdd->GetType() == ShapeUtility::arc);

		//get the map group key value
		//sFieldValue = pShapeToAdd->m_aProperties[nIndexGroupField];
		//new codes for name of street
		CLabelManager lineLabel;
		sFieldValue = lineLabel.ExtractArcLabel( (CLineShape*) pShapeToAdd );

				
		// if the field value exists in the mapGroups
		if (  groupsStreetByName.Lookup(sFieldValue, (void*&) pShapeExistGroupList )  == TRUE )
		{

			//for perfomence only connect twice for lines, it is the fact!!!!
			nConnectSum = 0;

			/* found the value in the groups then begin to connect shape objects
				in this group	*/
			for( posPreForward = pShapeExistGroupList->GetHeadPosition(); ( posCurrent = posPreForward ) != NULL; )
			{
				// forward the Position of list first and get the current element;
				CShape* pCurrentShapeInList = pShapeExistGroupList->GetNext( posPreForward ); 

				// try to connect the shape object of loop and the shape object in the current group list.

				CLineShape* pConnectedShape = new CLineShape();
				if ( pConnectedShape == NULL )
				{
					LOG("Fatal ERROR: insufficent memory");
					exit(300);
				}

				bool bConnected = CLineShape::Connect( (CLineShape*) pCurrentShapeInList, (CLineShape*) pShapeToAdd, pConnectedShape ); 

				if ( bConnected == true )   // if could connect; just remove the connect group object from the group list;
				{
					
					CShape* pShapeToBeCombine = pShapeExistGroupList->GetAt( posCurrent );
					pShapeExistGroupList->RemoveAt( posCurrent );

					/*	deleted the object that was combined and 
						the same time delete the old shapeToAdd element ,
						because it and element in list are connect together, 
						old should not exist any more!!!*/
					delete pShapeToBeCombine;
					pShapeToBeCombine = NULL;
				
					delete pShapeToAdd;


					// change the connected shape as the shape object to add;
					pShapeToAdd = pConnectedShape; 
					

					//for perfomence only connect twice for lines, it is the fact!!!!
					nConnectSum++;
					if (nConnectSum >= 2)
					{
						break; // break the loop for the list in the same name;
					}

				}
				else
				{	
					delete pConnectedShape;
				}
				// then just continue, if could not connect to current record in the list				
			
			}// end  loop  in a list;

			 // add the merged destination objects to list after it merged all elements (if did )
			//pShapeExistGroupList->AddTail( pShapeToAdd );
			pShapeExistGroupList->AddHead( pShapeToAdd );
		}// if does not exists in the map group
		else
		{
			
			/* not found the value in the map groups then add a new group */

			pShapeNewGroupList = new CShapeList; //will this cause memory leak?!!!???
			pShapeNewGroupList->AddTail( pShapeToAdd );//???/*should clone here!!!*/
			groupsStreetByName.SetAt( sFieldValue, pShapeNewGroupList ); // add new group list to map groups
		};

	} // end loop for all elements to coonnect.


	/***********************************************************
		begin to dump all data in map lists to a Layer object.
	*************************************************************/

	POSITION posMapGroups = NULL;
	POSITION posListInMap = NULL;
	CString sKeyField;
	CShapeList* pShapeListInMap;
	for( posMapGroups = groupsStreetByName.GetStartPosition(); posMapGroups != NULL; )
	{
		groupsStreetByName.GetNextAssoc( posMapGroups, sKeyField, (void*&)pShapeListInMap );
		/*#ifdef _DEBUG
			afxDump << sKeyField << " : " << pShapeListInMap << "\n";
		#endif*/
		ASSERT( pShapeListInMap != NULL );
		for(posListInMap = pShapeListInMap->GetHeadPosition(); posListInMap != NULL; )
		{
			CShape* pShape = pShapeListInMap->GetNext( posListInMap );
			ASSERT( pShape != NULL );
			pShapeLayer->AddShapeObject(pShape);
		}
		delete pShapeListInMap;
		pShapeListInMap = NULL;
	}	


	return true;
}

/*
// example for CMapStringToOb::GetNextAssoc and CMapStringToOb::GetStartPosition
   CMapStringToOb map;
   POSITION pos;
   CString key;
   CAge* pa;
   
   map.SetAt( "Bart", new CAge( 13 ) );
   map.SetAt( "Lisa", new CAge( 11 ) );
   map.SetAt( "Homer", new CAge( 36 ) );
   map.SetAt( "Marge", new CAge( 35 ) );
   // Iterate through the entire map, dumping both name and age.
   for( pos = map.GetStartPosition(); pos != NULL; )
   {
   map.GetNextAssoc( pos, key, (CObject*&)pa );
#ifdef _DEBUG
      afxDump << key << " : " << pa << "\n";
#endif
   }

*/

	/*
	 list.AddHead( new CAge( 21 ) );
   list.AddHead( new CAge( 40 ) );
   list.AddHead( new CAge( 65 ) ); // List now contains (65 40, 21).
   for( pos1 = list.GetHeadPosition(); ( pos2 = pos1 ) != NULL; )
   {
       if( *(CAge*) list.GetNext( pos1 ) == CAge( 40 ) )
       {
           pa = list.GetAt( pos2 ); // Save the old pointer for
                                  //deletion.
           list.RemoveAt( pos2 );
           delete pa; // Deletion avoids memory leak.
       }
	 }

	*/