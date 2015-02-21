/**
A Star Path Finding
psPaths.h

@brief: Finds the easiest path to the target.  Easiest is not always the shortest path.
Easiest can be the path of least resistance as well.

Notes:	Initially I set out to use Phil Shenk's resource and did not have much success.
		The resource seems to be much more robust, but this does everything we need it to do.
		Most of the logic was taken from: 
		
		http://code.activestate.com/recipes/577457-a-star-shortest-path-algorithm/

		^ I've made some minor improvements and implemented T2D tie ins and made it more readable IMO.

@authors:	Jean-Pierre C.
			Jonathan B.
*/
#pragma once

#ifndef _PS_PATHS_H_
#define _PS_PATHS_H_

#ifndef _VECTOR2_H_
#include "2d/core/Vector2.h"
#endif

#ifndef _PS_2DARRAY_H_
#include "math/Array2D.h"
#endif

#include <iostream>
#include <iomanip>
#include <queue>
#include <string>
#include <math.h>
#include <ctime>

#define NUMBER_OF_DIRECTIONS 4 // use 8 if we are to use diagonal paths
const string SPACE = " ";

static int dx[NUMBER_OF_DIRECTIONS]= {1, 0, -1, 0};
static int dy[NUMBER_OF_DIRECTIONS]= {0, 1, 0, -1};
//static int dx[NUMBER_OF_DIRECTIONS]={1, 1, 0, -1, -1, -1, 0, 1};
//static int dy[NUMBER_OF_DIRECTIONS]={0, 1, 1, 1, 0, -1, -1, -1};

class PathNode
{
	int mX, mY;				// current position
	int mLevel;				// total distance already traveled to reach the node
	int mPriority;			// priority = level + remaining distance estimate. Smaller = higher priority

public:
	/* ctor */
	PathNode( int pX, int pY, int pD, int pP ) { mX = pX; mY = pY; mLevel = pD; mPriority = pP; }

	/* Getters and setters */
	int getX( ) const { return mX; }
	int getY( ) const { return mY; }
	int getLevel( ) const { return mLevel; }
	int getPriority( ) const { return mPriority; }
	void setLevel( int pLevel ){ mLevel = pLevel; }


	void updatePriority( const int & pDestinationX, const int & pDestinationY, const int & pPassibility )
	{
		mPriority = mLevel + estimate( pDestinationX, pDestinationY ) * 10 + pPassibility; //A*
	}

	// give better priority to going strait instead of diagonally
	void nextLevel( const int & pDirection ) // i: direction
	{
		mLevel += 10;/*( NUMBER_OF_DIRECTIONS == 8 ? ( pDirection % 2 == 0 ? 10 : 14 ) : 10 )*/;
	}

	// Estimation function for the remaining distance to the goal.
	const int & estimate( const int & pDestinationX, const int & pDestinationY ) const
	{
		int xDistance, yDistance, distance;
		xDistance = pDestinationX-mX;
		yDistance = pDestinationY-mY;         

		// Euclidean Distance
		//distance = static_cast<int> ( sqrt ( ( double ) xDistance * xDistance + yDistance * yDistance ) );

		// Manhattan distance
		//distance = abs ( xDistance ) + abs ( yDistance );

		// Chebyshev distance
		distance = max ( abs ( xDistance ), abs ( yDistance ) );

		return distance;
	}
};

// Determine priority (in the priority queue)
bool operator< ( const PathNode & pNodeA, const PathNode & pNodeB )
{
	return pNodeA.getPriority( ) > pNodeB.getPriority( );
}

class PathSolver : public SimObject
{
	typedef SimObject	Parent;

private:
	Vector2			mStart;
	Vector2			mDest;
	F32				mPathDistance;
	Array2D*		mTileMap;
	Array2D*		mClosedNodesMap;
	Array2D*		mOpenNodesMap;
	Array2D*		directionsMap;
	bool			mNoDiagonal;

	int				mMaxPassability;
	int				mMaxPathability;


public:	
	/* ctors */
	PathSolver( );
	
	/* dtor */
	virtual ~PathSolver( );		

	void init( Array2D* pTileMap, int pWidth, int pHeight, int pMaxPathable, int pMaxPassable );			

	// calculates a path (erases old path)
	char* calculatePath( 
		const Vector2 pStart,
		const Vector2 pDest,
		const int pMaxPassability,
		const int pMinPassability
		);

	void updatePassability( int pX, int pY, int pValue );
	int getPassability( int pX, int pY);
	void setMaxPassability( int pPassibility );
	void setMaxPathability( int pPathibility );
	void GridToFile();

	int				mTileMapWidth;
	int				mTileMapHeight;
	int getMaxPassability( );
	int getMaxPathability( );

	DECLARE_CONOBJECT( PathSolver );
};

#endif _PS_PATHS_H_