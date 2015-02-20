#ifndef _PS_2DARRAY_H_
#include "math/Array2D.h"
#endif

IMPLEMENT_CONOBJECT( Array2D );

void Array2D::initArray( int p_width, int p_height, int p_initializationValue)
{ 
	m_matrix = new Array2DTemplate<int>(p_width,p_height); 
	for(int x = 0; x < p_width; x++) 
		for(int y = 0; y < p_height; y++) 
			(* m_matrix) (x,y) = p_initializationValue;  
}

/* Console Methods */
/*! Getter for the 2d Array
    @param pX location along the x
    @param pY location along the y
*/
ConsoleMethodWithDocs( Array2D, getValue, ConsoleInt, 4,4,( int pX, int pY ) )
{
	const int p_x = dAtoi(argv[2]);
	const int p_y = dAtoi(argv[3]);
	return object->getValueAt(p_x,p_y);
}

/*! Setter for the 2d Array
	@param pX location along the x
	@param pY location along the y
	@param pValue Value to set the at (pX, pY)
*/
ConsoleMethodWithDocs( Array2D, setValue, ConsoleVoid, 5,5,( int pX, int pY, int pValue ) )
{
	const int p_x = dAtoi(argv[2]);
	const int p_y = dAtoi(argv[3]);
	const int p_value = dAtoi(argv[4]);
	object->setValueAt(p_x,p_y,p_value);
}

/*! Initialization method
	@param pX location along the x
	@param pY location along the y
	@param pValue Value to initialize each node int he 2d array to.
*/
ConsoleMethodWithDocs( Array2D, initializeArray, ConsoleVoid, 5, 5, ( int pWidth, int pHeight, int pValue ) )
{
	const int pWidth = dAtoi( argv[2] );
	const int pHeight = dAtoi( argv[3] );
	const int pInitValue = dAtoi( argv[4] );
	object->initArray( pWidth, pHeight, pInitValue );
}