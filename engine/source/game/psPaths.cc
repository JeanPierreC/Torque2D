/**
A Star Path Finding
psPaths.h

@brief: Finds the easiest path to the target.  Easiest is not always the shortest path.
Easiest can be the path of least resistance as well.

Notes:	Initially I set out to use Phil Shenk's resource and did not have much success.
		That resource seems to be much more robust, but this does everything we need it to do.
		Most of the logic was taken from: 
		
		http://code.activestate.com/recipes/577457-a-star-shortest-path-algorithm/

		^ I've made some minor improvements and implemented T2D tie ins and made it more readable IMO.

@author:	Jean-Pierre C., Jonathan B.
*/

#include "./psPaths.h"
#include <fstream>   // file I/O

IMPLEMENT_CONOBJECT( PathSolver );

PathSolver::PathSolver( )
{
	mStart.x = 0;
	mStart.y = 0;
	mDest.x = 0;
	mDest.y = 0;

	mNoDiagonal = true;

	mTileMapWidth = 0;
	mTileMapHeight = 0;
}

PathSolver::~PathSolver( )
{
	// left empty
}

/* init
 *
 * Constructors are implemented in a slightly different manner for objects which 
 * are to be instantiated in (Torque) script.
 * 
 * This really should have been a ctor, or (truthfully) I should be using Persisted Fields.
 *
 * Notes: Likely some optimization can be done. I've initialized all three 2d arrays
 * (Array2D objects) to be the size of the tileMap.  This is only true in the worst 
 * case (memory usage).
 */
void PathSolver::init( 
	Array2D* pTileMap,
	int pWidth,
	int pHeight,
	int pMaxPathable,
	int pMaxPassable
	)
{
	mTileMap = pTileMap;
	mTileMapWidth = pWidth;
	mTileMapHeight = pHeight;
	mMaxPathability = pMaxPathable;
	mMaxPassability = pMaxPassable;

	mClosedNodesMap = new Array2D( );
	mClosedNodesMap->initArray( mTileMapWidth, mTileMapHeight, 0 );
	mOpenNodesMap = new Array2D( );
	mOpenNodesMap->initArray( mTileMapWidth, mTileMapHeight, 0 );
	directionsMap = new Array2D( );
	directionsMap->initArray( mTileMapWidth, mTileMapHeight, 0 );
}

// calculates a path (erases old path)
char* PathSolver::calculatePath( 
	const Vector2 pStart,
	const Vector2 pDest,
	const int pMaxPathability,
	const int pMinPathability
	)
{
	mStart = pStart;
	mDest = pDest;
	static std::priority_queue<PathNode> pq[2]; // list of open (not-yet-tried) nodes
	static int pqi; // pq index
	static PathNode* n0;
	static PathNode* m0;
	static int i, j, x, y, xdx, ydy;
	static char c;
	pqi=0;

	// reset the node maps
	/*	Note1: I could have used:
		mClosedNodesMap->initArray( mTileMapWidth, mTileMapHeight, 0 );
		mOpenNodesMap->initArray( mTileMapWidth, mTileMapHeight, 0 );

		However, I'd have to iterate through both.  Meaning two sets of loops.

		This way we only use one set of loops, and update both 2d arrays.

		Note2: If the 2d arrays are different sizes, this will no longer work 
		(goes with note from init (...) )
	*/
	for( x=0; x < mTileMapWidth; x++ )
	{
		for( y=0; y < mTileMapHeight; y++ )
		{
			mClosedNodesMap->setValueAt( x, y, 0 );
			mOpenNodesMap->setValueAt( x, y, 0 );
		}
	}

	// create the start node and push into list of open nodes
	n0 = new PathNode( mStart.x, mStart.y, 0, 0 );
	n0->updatePriority( mDest.x, mDest.y, mTileMap->getValueAt( mDest.x, mDest.y) );
	pq[pqi].push( *n0 );
	mOpenNodesMap->setValueAt( mStart.x, mStart.y, n0->getPriority( ) ); // mark it on the open nodes map

	// A* search
	while( !pq[pqi].empty( ) )
	{
		// get the current node w/ the highest priority
		// from the list of open nodes
		n0 = new PathNode( 
			pq[pqi].top( ).getX( ), 
			pq[pqi].top( ).getY( ), 
			pq[pqi].top( ).getLevel( ), 
			pq[pqi].top( ).getPriority( ) );

		x = n0->getX( ); y = n0->getY( );

		pq[pqi].pop( ); // remove the node from the open list
		mOpenNodesMap->setValueAt( x, y, 0 );
		// mark it on the closed nodes map
		mClosedNodesMap->setValueAt( x, y, mMaxPassability );

		// quit searching when the goal state is reached
		//if((*n0).estimate(xFinish, yFinish) == 0)
		if( x == mDest.x && y == mDest.y ) 
		{
			// generate the path from finish to start
			// by following the directions
			string path = "";

			while(! ( x == mStart.x && y == mStart.y ) )
			{

				if( path == "" ) // first time no trailing whitespace.
					path = to_string( static_cast<long long> ( x ) ) 
					+ "," 
					+ to_string( static_cast<long long> ( y ) );
				else
					path = to_string( static_cast<long long> ( x ) ) 
					+ "," 
					+ to_string( static_cast<long long> ( y ) ) 
					+ SPACE 
					+ path;
				j = directionsMap->getValueAt( x, y );
				c = '0' + ( j + NUMBER_OF_DIRECTIONS / 2 ) % NUMBER_OF_DIRECTIONS;
				x += dx[j];
				y += dy[j];
			}

			// garbage collection
			delete n0;

			// empty the leftover nodes
			while( !pq[pqi].empty( ) ) pq[pqi].pop( );

			char *pathToReturn = new char[path.size( )+1];
			strcpy ( pathToReturn,path.c_str( ) );

			return pathToReturn;
		}

		// generate moves (child nodes) in all possible directions
		for( i = 0; i < NUMBER_OF_DIRECTIONS; i++ )
		{
			xdx = x+dx[i];
			ydy = y+dy[i];

			if( ! ( xdx < 0 || xdx > mTileMapWidth - 1 || ydy < 0 || ydy > mTileMapHeight - 1 // out of the map bounds
				|| mTileMap->getValueAt( xdx, ydy ) == mMaxPassability 
				|| mTileMap->getValueAt( xdx, ydy ) > pMaxPathability
				|| mTileMap->getValueAt( xdx, ydy ) < pMinPathability
				|| mClosedNodesMap->getValueAt( xdx, ydy ) == mMaxPassability
				|| mClosedNodesMap->getValueAt( xdx, ydy ) > pMaxPathability
				|| mClosedNodesMap->getValueAt( xdx, ydy ) < pMinPathability ) )
			{
				// generate a child node
				m0 = new PathNode( xdx, ydy, n0->getLevel( ), n0->getPriority( ) );
				m0->nextLevel( i );
				m0->updatePriority( mDest.x, mDest.y, mTileMap->getValueAt( xdx, ydy ) );

				// if it is not in the open list then add into that
				if( mOpenNodesMap->getValueAt( xdx, ydy )==0 )
				{
					mOpenNodesMap->setValueAt( xdx,ydy,m0->getPriority( ) );
					pq[pqi].push( *m0 );
					// mark its parent node direction
					directionsMap->setValueAt( xdx, ydy, ( i + NUMBER_OF_DIRECTIONS / 2 ) % NUMBER_OF_DIRECTIONS );
				}
				else if( mOpenNodesMap->getValueAt( xdx, ydy )>m0->getPriority( ) )
				{
					// update the priority info
					mOpenNodesMap->setValueAt( xdx, ydy, m0->getPriority( ) );
					// update the parent direction info
					directionsMap->setValueAt( xdx, ydy, ( i + NUMBER_OF_DIRECTIONS / 2 ) % NUMBER_OF_DIRECTIONS );

					// replace the node
					// by emptying one pq to the other one
					// except the node to be replaced will be ignored
					// and the new node will be pushed in instead
					while( ! ( pq[pqi].top( ).getX( ) == xdx 
						&& pq[pqi].top( ).getY( ) == ydy ) )
					{                
						pq[1-pqi].push( pq[pqi].top( ) );
						pq[pqi].pop( );       
					}
					pq[pqi].pop( ); // remove the wanted node

					// empty the larger size pq to the smaller one
					if( pq[pqi].size( ) > pq[1-pqi].size( ) ) 
						pqi=1-pqi;

					while( ! pq[pqi].empty( ) )
					{                
						pq[1-pqi].push(pq[pqi].top( ) );
						pq[pqi].pop( );       
					}
					pqi=1-pqi;
					pq[pqi].push( *m0 ); // add the better node instead
				}
				delete m0; // garbage collection
			}
		}
		delete n0; // garbage collection
	}
	return ""; // no route found
}

void PathSolver::GridToFile()
{
	ofstream fp_out;
	fp_out.open("gridout.txt");

	for( int x=0; x < mTileMapWidth; x++ )
	{
		for( int y=0; y < mTileMapHeight; y++ )
		{
			fp_out << mTileMap->getValueAt( x, y ) << " ";
		}
		fp_out << "\n";
	}

	fp_out.close();
}

/* Accessors and Modifiers */
void PathSolver::setMaxPassability( int pPassability )
{
	mMaxPassability = pPassability;
}

void PathSolver::setMaxPathability( int pPathability )
{
	mMaxPathability = pPathability;
}

int PathSolver::getMaxPassability( )
{
	return mMaxPassability;
}

int PathSolver::getMaxPathability( )
{
	return mMaxPathability;
}

void PathSolver::updatePassability( int pX, int pY, int pValue )
{
	mTileMap->setValueAt(pX, pY, pValue);
}

int PathSolver::getPassability( int pX, int pY)
{
	return mTileMap->getValueAt(pX, pY);
}


/* Console Methods */
/*! Initialize the Grid
    @param pGrid The instantiated Array2D Object (TileMap)
    @param pWidth Width of the tile map.
    @param pHeight Height of the tile map.
    @param pMaxPathable The maximum value for that is pathable.  Anything over this value is a blocker.
    @param pMaxPassable The maximum value for that is passable.
*/
ConsoleMethodWithDocs( PathSolver, initGrid, ConsoleVoid, 7, 7, ( Array2D pGrid, int pWidth, int pHeight, int pMaxPathable, string pMaxPassable ) )
{

	// Find the Array2D Object.
	Array2D* aStarGrid = dynamic_cast<Array2D*> ( Sim::findObject( argv[2] ) );

	// Validate Object.
	if ( !aStarGrid )
	{
		Con::warnf( "pathAStar2d::Init() - Couldn't find/Invalid object '%s'.", argv[2] );
		return;
	}

	int width = dAtoi( argv[3] );
	int height = dAtoi( argv[4] );
	int maxPathable = dAtoi( argv[5] );
	int maxPassable = dAtoi( argv[6] );
	object->init( aStarGrid, width, height, maxPathable, maxPassable );
}

/*! Get a path
    @param pStartingPosition Starting position as a string ie: "10,20"
    @param pGoal goal position as a string ie: "12,22"
    @param (Not mandatory) pMaxPathability The maximum value for that is pathable.  Anything over this value is a blocker.
    @param (Not mandatory) pMinPathability The minimum value for that is pathable.

	Note: Because each node has a pathability value (making for preferred paths), every time a path is 
	requested you could specify the parameters of the path.  Example:  PathSolver's internal Max Pathable 
	could be set to 10 and it's minimum could be set to 0.  You could pass a max pathable of 5 and min 
	pathable of 3 to find paths that are in between 3 and 5, if any.
*/
ConsoleMethodWithDocs( PathSolver, getPath, ConsoleString, 4, 6, ( string pStartingPosition, string pGoal, int pMaxPathability, int pMinPathability ) )
{
	int minPathability;
	int maxPathability;
	string firstCoord = argv[2];
	string secondCoord = argv[3];
	string::size_type posOfComma = firstCoord.find_first_of( ',' );

	string x = firstCoord.substr( 0, posOfComma );
	string y = firstCoord.substr( posOfComma + 1, firstCoord.length( ) - posOfComma + 1 );
	
	Vector2 *pStart = new Vector2( atoi( x.c_str( ) ), atoi( y.c_str( ) ) );

	posOfComma = secondCoord.find_first_of( ',' );
	x = secondCoord.substr( 0, posOfComma );
	y = secondCoord.substr( posOfComma + 1, firstCoord.length( ) - posOfComma + 1 );

	Vector2 *pEnd = new Vector2( atoi( x.c_str( ) ), atoi( y.c_str( ) ) );

	//
	if (argc > 4)
		maxPathability = dAtoi(argv[4]);
	else
		maxPathability = object->getMaxPassability( );

	if (argc > 5)
		minPathability = dAtoi(argv[5]);
	else
		minPathability = 0;

	return ( object->calculatePath( *pStart, *pEnd, maxPathability, minPathability ) );
}

/*! Update a node's passability.
    @param pNodePosition Position of node in the 2d array, as a string ie: "10,20"
    @param pPassability New passibility value.
*/
ConsoleMethodWithDocs( PathSolver, updateNavGridPassability, ConsoleVoid, 4, 4, ( string pNodePosition, int pPassability) )
{
	string firstCoord = argv[2];
	string::size_type posOfComma = firstCoord.find_first_of( ',' );
	string x = firstCoord.substr( 0, posOfComma );
	string y = firstCoord.substr( posOfComma + 1, firstCoord.length( ) - posOfComma + 1 );

	object->updatePassability( atoi( x.c_str( ) ), atoi( y.c_str( ) ), dAtoi( argv[3] ) );
}

/*! request a nodes passability.
    @param pNodePosition Position of node in the 2d array, as a string ie: "10,20"
	@return Nodes passibility.
*/
ConsoleMethodWithDocs( PathSolver, getNavGridPassability, ConsoleInt, 3, 3, ( string pNodePosition ) )
{
	string coord = argv[2];
	string::size_type posOfComma = coord.find_first_of( ',' );
	string x = coord.substr( 0, posOfComma );
	string y = coord.substr( posOfComma + 1, coord.length( ) - posOfComma + 1 );

	return (object->getPassability( atoi( x.c_str( ) ), atoi( y.c_str( ) ) ));
}

/*! Set the size of the tile map
    @param pSize String of max extents of the 2d array "100,100"
*/
ConsoleMethodWithDocs( PathSolver, setSize, ConsoleVoid, 3, 3, ( ) )
{
	string coord = argv[2];
	string::size_type posOfComma = coord.find_first_of( ',' );
	string x = coord.substr( 0, posOfComma );
	string y = coord.substr( posOfComma + 1, coord.length( ) - posOfComma + 1 );
	object->mTileMapWidth =  atoi( x.c_str( ) );
	object->mTileMapHeight =  atoi( y.c_str( ) );
}

ConsoleMethodWithDocs( PathSolver, GridOutToFile, ConsoleVoid, 2, 2, ( ) )
{
	object->GridToFile();
}