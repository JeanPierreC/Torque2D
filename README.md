### Repository based on T2D 3.1.  This is not official, nor have I requested a pull request.

![Path Finding Demo](http://media.tumblr.com/823865910bf1702f7a838ee1e8ebec18/tumblr_inline_nk57u1Aig31rxazf0.jpg)

## A Star Path finding for Torque 2D

Well, it's about time I give back.

This is a path finding resource for T2D.

### What does it do?
A * Path Solving. Admittedly, A * isn't the most robust path finding algorithm to implement, but it is fairly easy to implement and to understand. Hence it's choice.

### How does it do it?
I've introduced two classes. A Array 2D class, which is can be instantiated from script and a PathSolver class. The path solver does the heavy lifting using a priority queue. Classes are fairly well documented.

### This looks a little different, something is up!
True, I've added preferential pathing. What's that? Well, each node has a base cost. For example, roads could be a 2, and grass could be a 3. If you wish, the path solver could have a preference for roads. In this case, for the grass path to be selected over the road path, it would have to be at least half of the distance away when compared to the road path, otherwise the road path is selected.

It isn't necessary to use this resource as indicated in the preceding paragraph. If it's preferable to use 0 as pathable and 1 as the blocker (standard A* stuff) the you can set it up that way as well.

### How to compile it!
Add the Array2D.cc and h to the torque2d/math filter
Add the psPaths.cc and h to the torque2d/game filter
Build.

### How to use it!
Instantiate an Array2D object, this will essentially be your navigational mesh. Set it up at the same time as you would when populating your CompositeSprite. Once level creation is complete, instantiate a PathSolver Object and pass the Array2D object in it's init function.

So, something like this:

    $NAVMESH = new Array2D();  
    $NAVMESH.initializeArray($XSIZE, $YSIZE, 0);  


To set the value of a node/tile/cell use this:

    $NAVMESH.setValue(%x, %y, %cell.obstacleValue);  


where %cell.obstacle could be a 1 if it's a blocker or 0 if it's pathable.

then, after the world is created, something like this:

// init path finding
    $pathSolver = new PathSolver();  
    $pathSolver.initGrid($NAVMESH,$XSIZE,$YSIZE, 0, 1);  


Of note, when initializing the grid the last two variables are used to determine the maximum pathable(walkable) tile/node value. A zero is used if standard A * behaviour is wanted.

To get a path back, you'd do something like this:

    %path = $pathSolver.getPath( %startPos, %endPos );  


the path that is returned is in the format of x,y x2,x2 x3,y3... etc...

Of note, we are using the collision mesh for lighting. Works quite well for pre rendered scenes.

How to debug?
Well, you could step through and check everything. No I didn't add a render. The path is passed back as a string, so you could echo it or do something like this, of note, this part was taken from my friend and colleague LordSharpe:

    %pathShapeVector = new ShapeVector();  
      
    %path = $pathSolver.getPath( %startPos, %endPos );  
      
    if (strlen(%path) > 0)   
    {  
        %formatedPath = "";    // ShapeVectors do not take commas  Must remove.  
        %count = getWordCount( %path);  
        for (%i = 0; %i < %count; %i++)   
        {  
            %pos = getWord(%path, %i);  
            %pos = nextToken( %pos , "x" , "," );  
            %pos = nextToken( %pos , "y" , "," );  
      
            %formatedPath = %formatedPath @ %x SPC %y SPC " ";  
        }  
      
        %pathShapeVector.PolyList = %formatedPath;  
      
        %pathShapeVector.setPosition( %x, %y );  
        %pathShapeVector.setLineColor( %colour );// some colour <- I'm Canadian and so there is a u in the word colour.  
    }  



### Why I didn't extend or use Composite Sprites rather than introducing Array2D?
I could give a few answers for this, the simplest is that we wanted to use Array2D for many purposes. And also, I wanted this to be a drop in resource. No need to worry about me breaking your code, 'cause that sucks!

### Why an init method?
I was lazy, I should have added an initPersistFields method and added the fields. Sorry. Feel free to modify and update this resource :)

### Spelling mistakes!!! UGH
Again, sorry. I try hard to correctly spell everything, but they slip past me on occasion.

Hope it's helpful for someone.
- RT