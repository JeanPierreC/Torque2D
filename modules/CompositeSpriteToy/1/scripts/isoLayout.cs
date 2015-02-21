//-----------------------------------------------------------------------------
// Copyright (c) 2013 GarageGames, LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

function CompositeSpriteToy::createIsoLayout( %this )
{       
    // Set the layer #0 sort mode to be depth.
    SandboxScene.setLayerSortMode( 0, "-y" );
    
    // Create the composite sprite.   
	%composite = new CompositeSprite();
	
    // Set the default sprite stride.
    // This is used in isometrics layout mode to scale the specified logical position arguments.	
    %composite.setDefaultSpriteStride( 4, 2 );
    
    // Set the default sprite size used.
    %composite.setDefaultSpriteSize( 8 );    
    
	// Set the batch layout mode.  We must do this before we add any sprites.
    %composite.SetBatchLayout( "iso" );

    // Set the batch sort mode for when we're render isolated.
    %composite.SetBatchSortMode( "-y" );
    
    // Set the batch render isolation.
    %composite.SetBatchIsolated( CompositeSpriteToy.RenderIsolated );
    
	// Calculate a range.
	%range = mSqrt( CompositeSpriteToy.SpriteCount ) * 0.5;
	if ( %range < 1 ) %range = 1;

	// Path Finding
	// Added to output debug msgs Path Finding
	$DebugOut = true;

   // setup the grid representation of the tiles

   $READY_TO_PLACE_STARTING_LOCATION = false;       // These bools are really just hacks to tie into the composite sprite toy
   $READY_TO_PLACE_GOAL_LOCATION = false;
   $RENDER_PATH = false;

   $goalPoint = "";                                 // target and start position for the path.  Globaled for this demo.
   $startingPoint = "";

   // arrays are zero indexed
   %xx = 0;
   %yy = 0;

	// Add some sprites.
    for ( %y = -%range; %y <= %range; %y++ )        // usually I'd have my second variable initialized and incremented here, but can't in tScript
	{
	    for ( %x = -%range; %x <= %range; %x++ )    // usually I'd have my second variable initialized and incremented here, but can't in tScript
        {
            // Add a sprite with the specified logical position.
	        // In isometric layout this two-part position is scaled by the default sprite-stride.            
            %composite.addSprite( %x SPC %y );
            
            // every cell is pathable.
            $NAVMESH.setValue( %xx, %yy, 0 );         // All locations are fair play because the iso grid doesn't have holes at this point.

            // Set the sprite image with a random frame.
            // We could also use an animation here.            
            %composite.setSpriteImage( "CompositeSpriteToy:isoTiles", getRandom(0,4) );
            %xx++;
        }
        %yy++;
        %xx=0;
	}
	
	// Add to the scene.
	SandboxScene.add( %composite );
	
	// Set the composite sprite toy.
	CompositeSpriteToy.CompositeSprite = %composite;

    //GUI
    PathFinding_Calculate_Path.visible = 1;
    PathFinding_Place_Start.visible = 1;
    PathFinding_Place_Target.visible = 1;
}
