/*
 * Array2DTemplate:
 *	A template of a 2d array.  Could return any data type.
 *
 * Array2D:
 *	Our 2d array object which can be instantiated as a Sim Object via torque script.
 *	Currently this is only a int 2d array, which suits our purposes.  However, the
 *	point of using the template was so that it could be any data type.  Clearly there
 *	is future work planed to complete this.
 *
 *	@author:	Jean-Pierre C.
 */
#pragma once

#ifndef _PS_2DARRAY_H_
#define _PS_2DARRAY_H_

#include <iostream>

#ifndef _SCENE_OBJECT_H_
#include "2d/sceneobject/SceneObject.h"
#endif

#include <iostream>
using namespace std;

/* The template of the array for us to use
 *
 * This template will allow use to declare the size (width and height at runtime)
 */

template<typename T>
class Array2DTemplate
{
private:
	const int width;
	T * data;
public:
	T& operator( ) ( int x, int y ) { return data[y*width + x]; }
	Array2DTemplate( const int w, const int h ) : width(w) { data = new T[w*h]; }
	~Array2DTemplate( ) { delete [] data; }
};

class Array2D: public SimObject
{
private:
	typedef SimObject Parent;
	Array2DTemplate<int> * m_matrix;
	// for depth we should use depth.

public:
	/* ctor and dtors */
	Array2D( ) { m_matrix = new Array2DTemplate<int>( 0,0 ); }
	Array2D( int p_width, int p_height ) { m_matrix = new Array2DTemplate<int>( p_width, p_height ); }
	Array2D( int p_width, int p_height, int p_initializationValue );
	void initArray( int p_width, int p_height, int p_initializationValue );
	virtual ~Array2D( ) { delete m_matrix; }

	/* Getters and Setters */
	int getValueAt( int p_x, int p_y ){ return (* m_matrix)(p_x,p_y); }
	void setValueAt( int p_x, int p_y,int value ){ (* m_matrix)( p_x, p_y ) = value; }

	DECLARE_CONOBJECT( Array2D );
};

#endif _PS_2DARRAY_H_