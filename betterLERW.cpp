///////////////////////////////////////////////////////////////////////////
//    Upgrade to the LoopErasure function to run LERW on generic graphs: //
//     we optimize the RW such that we don't need to produce the RW,     //
//           then loop erase it and finally check if it correct          //
///////////////////////////////////////////////////////////////////////////

// Include a header file containing all the necessary libraries and function prototypes
#include "/mnt/d/Offline_Documents/University/PhD_Paris/PhD_work/Simulations/Simple_Graph/my_UpgradedHeader.h" 

#define PRINT 0 // 1: print everything exept for the RW steps; 2: print everything; 0: don't print anything

// Counters for the total number of attempts 
int g_totalAttemptsCounter = 0;
int g_successfulAttemptsCounter = 0;

/******************************************************************
//                Initialization Function                        //
******************************************************************/
void LoopErasedRandomWalk( Graph  /*Provide a graph to run the random walk on*/,
                          int*  /*Provide the array containing the desired trail of the Laplacian RW*/,
                          int    /*as well as its size*/,
                          int  nSteps=1e6  /*(OPTIONAL) the number of steps (just to make sure the loop ends)*/);

////////////////////////////////////////////
//                MAIN                    //
////////////////////////////////////////////

int main()
{
  cout << setprecision(4); 

  srand48(time(NULL)); // Initialize the sequence
  srand(time(NULL)); // Initialize the sequence

  // We want to compute a mean of the probabilities, so we need a variable to store the numbers of runs
  int nRun = 1;

  // Variable to store the partial sum of the probabilities to compute their mean
  double partProbSum = 0.;

  // Number of iterations of the LoopErasedRandomWalk function
  int N= 2e6;

  // Typical dimension of the graph
  int nbase=4;

  // Shape of the tiles (4=square)
  int v=6;

  // Absorption weight
  double q = 0;

  // Define the condition, i.e. what LERW we want
  int condition[] = {0,1,5,6};

///////////////////////////////////////////////////////////////////

  // Let's print out that this is the LERW
  cout << "\n LOOP-ERASED RANDOM WALK" << endl;

  // Let's print out the condition
  cout << "\n We want to obtain the path: (" << condition[0] << ", ";
  for (int i = 1; i < sizeof(condition)/sizeof(condition[0]) -1 ; i++)
    cout << condition[i] << ", " ;
  cout << condition[sizeof(condition)/sizeof(condition[0]) -1] << ")" << endl;

  Graph g=RegularGraph(nbase,v, q);

  // Retreive the graph dimension
  int n=g.graph_size();

  // Compute the mean of the probabilities
  for (int j = 0; j < nRun; j++)
  {
    for(int i=0; i<N; i++)
    {   
      // Run a LERW
      LoopErasedRandomWalk(g, condition,sizeof(condition)/sizeof(condition[0]));

      #if PRINT==1 || PRINT==2
        cout << "****************************************************************************************" << endl;
      #endif
    }

    partProbSum += (double) g_successfulAttemptsCounter/(g_totalAttemptsCounter);

    /*
    cout << "we have obtained " << g_totalAttemptsCounter << " LERWs,"
       << " of which "  << g_successfulAttemptsCounter << " are the desired ones," 
       << " so the conditioned probability is " << (double) g_successfulAttemptsCounter/(g_totalAttemptsCounter) *100 <<  "%" << endl;
    */

    g_successfulAttemptsCounter = 0;
    g_totalAttemptsCounter = 0;
  }

  // Print out the numeber of correct LERW
  cout << "\n In " << nRun << " runs of " << N << " attempts each, the mean probability to get the LERW (" << condition[0] << ", ";
  for (int i = 1; i < sizeof(condition)/sizeof(condition[0]) -1 ; i++)
    cout << condition[i] << ", " ;
  cout << condition[sizeof(condition)/sizeof(condition[0]) -1] << ") is " << partProbSum/nRun * 100 << "%" << endl;


  return 0;

} // End of main()




////////////////////////////
//  Function definitions  //
////////////////////////////

// We make a slight change to the RandomWalk (RW) function in order to make it stop after a fixed number of steps 'nSteps'
void LoopErasedRandomWalk( Graph g /*Provide a graph to run the random walk on*/,
                          int* condition /*Provide the array containing the desired trail of the Laplacian RW*/,
                          int conditionSize    /*as well as its size*/,
                          int  nSteps  /*(OPTIONAL) the number of steps (just to make sure the loop ends)*/)
{  
  // Set the starting and ending verteces of the RWs from the condition[]
  int v = condition[0];
  int end = condition[conditionSize-1];

  // Define the number of steps left and initialize it
  int nStepsLeft = nSteps;

  // Define the random variable to decide where to move
  double x; 

  // Retrieve the size of the graph
  int n = g.graph_size();

  // Total rate buffer (the total rate information is stored in the n+1 column of the adjacency matrix)
  double totR= g.graph_edge(v,n);

  // Total probability buffer
  double totP = 1.;

  // Buffer for the probability to move towards a specific vertex
  double* moveP = new double[n];
  // And set its index variable
  int k = 0;

  // Define the index that will run on the condition array 
  int j=0;

  // Create an vector (NOT an ordered map) where the LERW will be stored. Start by adding the initial point
  vector<int> mp{v};

  // Define an iterator variable. It will run on the map and assign the new entries as it goes
  auto it=mp.begin(); 

  
  #if PRINT==1 ||  PRINT==2
    cout << "\n From the initial vertex " << v << ", we want to arrive at vertex " << end << endl ;
  #endif 

  
  while(find(mp.begin(), mp.end(), end) == mp.end() && totR * nStepsLeft !=0) // Runs until the LERW reaches the end point
  //OR we can no longer move from a vertex, i.e. when its not connected to any other vertex (and hence the total rate is zero), 
  // OR when it has performed all the steps
  {
    // Resets the total probability to 1 at the beginning of each 'while' loop
    totP = 1.;                
    
    // Retrieve the move probabilities as the edge weight divided by the total weight 
    for ( int i=0; i<n; i++)
      moveP[i] = g.graph_edge(v,i)/totR;  

    // Now it initializes the random variable and assign the running index 'k' to start from the end (the last vertex is the absorption one)
    x = drand48();   // Assign a random variable
    k = n;           // Assign the index to start from the end (the last vertex is the absorption one)

    #if PRINT==2
      cout << "\n(The random number is: " << x << ")" << endl ;
    #endif

    // This loop checks in which probability range one finds itself: when it breaks out, the index k represents the edge one moves to
    while ( x<= totP)               
    {
      k--;
      totP -= moveP[k];
    }
    
    if(k==(n-1))        // i.e. if it gets absorbed (the absorption vertex is the last one)
    {
    
      #if PRINT==2
        cout << "Absorbed at vertex " << v  << endl << endl; // We get absorbed at that vertex    
      #endif
    }
    
    #if PRINT==2
      else
        cout << "We move to vertex " << k << " (probability interval: [" << totP << ", " << totP+moveP[k] << "] )";    // We move to the new vertex j
    #endif

    if (v==k) 
    {
      
      #if PRINT==2
        cout << "\nA SELF-LOOP!!\n";
      #endif
    }

    // Search for the element k and store its position into the iterator 'it'
    it = find(mp.begin(), mp.end(), k);

    // Now erase from position 'it' till the end. If k was not present (and therefore it=mp.end()), this erases nothing. If it was present, then erase all the elements following k (k included)   
    mp.erase(it, mp.end() );

    // Then insert it at the end
    mp.insert(mp.end(), k);

    #if PRINT==2
      cout << "\n After erasure (";

      for(auto itr : mp)
        cout << itr << ", ";
      cout << ")" << endl;
    #endif

    // Finally, we can move on with the RW, so reduce the number of steps left..
    nStepsLeft--;

    //...update the starting vertex...
    v=k;      

    // ...and the starting rate...
    totR = g.graph_edge(v,n); 
  }

  // If everything worked fine, at this point we should have a LERW.
  // Let's print it out to check that it is indeed the case
  #if PRINT==1 || PRINT ==2
    cout << "\n\n We have obtained the following LERW of lenght " << mp.size() << ": (";

    for(auto itr : mp)
      cout << itr << ", ";
    cout << ")" << endl;
  #endif

  // Finally, check if the trail is the same as the condition: first check their size...
  if (conditionSize == mp.size())
  {
    // ...then check whether all the elements of condition[] are in the LERW just created
    for(int i=0; i<conditionSize; i++ )
    {
      // If the element 'i' is not found, break the 'for' and go directly to the total attempt counter increment
      if(find(mp.begin(), mp.end(), condition[i]) == mp.end()) 
      {
        goto jump2;
      }
    }

    // Otherwise, if all the elements are present, count it as a succesful attempt      
    g_successfulAttemptsCounter++;
  }

  jump2:
    g_totalAttemptsCounter++;

  delete[] moveP;

} // End of function LoopErasedRandomWalk