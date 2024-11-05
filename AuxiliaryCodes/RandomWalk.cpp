////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                   We want to define a RandomWalk function to run random walks on generic graphs                    //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Include a header file containing all the necessary libraries and function prototypes
#include "/mnt/d/Offline_Documents/University/PhD_Paris/PhD_work/Simulations/Simple_Graph/my_UpgradedHeader.h" 



/******************************************************************
//                    RandomWalk Function                        //
******************************************************************/

void RandomWalk( Graph g /*Provide a graph to run the random walk on*/)
{
  #define PRINT 0    // 1: print all the RandomWalk cout; 0: don't
  
  // Define the random variable To decide where to move
  double x; 

  // Retrieve the size of the graph
  int n = g.graph_size();

  // Set an index variable
  int j = 0;

  // Define the initial vertex position: we start from vertex 0
  int v = 0;

  // Total rate buffer (the total rate information is stored in the n+1 column of the adjacency matrix)
  double totR= g.graph_edge(v,n);

  #if PRINT==1
    cout << "The total weight of the initial vertex is: " << totR << endl;
  #endif

  // Total probability buffer
  double totP = 1.;

  // Buffer for the probability to move towards a specific vertex
  double* moveP = new double[n];


  while( totR !=0) // The loop stops when we can no longer move from a vertex, i.e. when its not connected to any other vertex (and hence the total rate is zero)
  {
    // Resets the total probability to 1 at the beginning of each 'while' loop
    totP = 1.;                
    
    // Retrieve the move probabilities as the edge weight divided by the total weight 
    for ( int i=0; i<n; i++)
      moveP[i] = g.graph_edge(v,i)/totR;  

    // Now it initializes the random variable and assign the running index 'j' to start from the end (the last vertex is the absorption one)
    x = drand48();   // Assign a random variable
    j = n;           // Assign the index to start from the end (the last vertex is the absorption one)

    #if PRINT==1
      cout << "\n(The random number is: " << x << ")" << endl ;
    #endif

    // This loop checks in which probability range one finds itself: when it breaks out, the index j represents the edge one moves to
    while ( x<= totP)               
    {
      j--;
      totP -= moveP[j];
    }
    
    if(j==(n-1))        // i.e. if it gets absorbed (the absorption vertex is the last one)
    {
    
      #if PRINT==1
        cout << "Absorbed at vertex " << v  << endl << endl; // We get absorbed at that vertex    
      #endif
    }
    
    #if PRINT==1
      else
        cout << "We move to vertex " << j;    // We move to the new vertex j
    #endif

    if (v==j) 
    {
      
      #if PRINT==1
        cout << "\nA SELF-LOOP!!\n";
      #endif
    }

    v=j;      // Now it updates the starting vertex...

    totR = g.graph_edge(v,n);      // ...and the starting rate 
  }

  delete[] moveP;

  #if PRINT == 1
    //cout << "\nThe weight of the walk is: " << w << endl;
    cout << "---------------------------------------" << endl;
  #endif


} // End of function RandomWalk


////////////////////////////////////////////
//                MAIN                    //
////////////////////////////////////////////

int main()
{
  cout << setprecision(4); 

  srand48(time(NULL)); // Initialize the sequence

  // Typical dimension of the graph
  int nbase=3;

  // Shape of the tiles (4=square)
  int v=4;

  // Absorption weight
  double q = 0.5;

  Graph g=RegularGraph(nbase,v, q);

  // Retreive the graph dimension
  int n=g.graph_size();

  cout << "Size of the graph: " << n << endl;

  RandomWalk(g);

  return 0;

} // End of main()

