//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    We want to define a better Lampacian RW function. To do so, we modify the RandomWalk function to make it respect the Laplacian rules  // 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Include a header file containing all the necessary libraries and function prototypes
#include "/mnt/d/Offline_Documents/University/PhD_Paris/PhD_work/Simulations/Simple_Graph/my_UpgradedHeader.h" 

#define PRINT 0 // 1: print everything exept for the RW steps; 2: print everything; 0: don't print anything

int g_notArrivedCounter = 0;
// Counters for the total number of attempts 
int g_totalAttemptsCounter = 0;
int g_successfulAttemptsCounter = 0;


/******************************************************************
//                Initialization Function                        //
******************************************************************/
void LaplacianRandomWalk( Graph  /*Provide a graph to run the random walk on*/,
                          //int* trail  /*Provide a pointer to an array to store the trail of the RW*/,
                          int*  /*Provide the array containing the desired trail of the Laplacian RW*/,
                          int    /*as well as its size*/,
                          int  nSteps=1e6  /*(OPTIONAL) the number of steps (just to make sure the loop ends)*/);

bool checkArrays(int* /*Provide a pointer to an array*/, 
                 int* /*Provide a pointer to a second array*/,
                 int  /*Size of arr1*/,
                 int  /*Size of arr2*/);

int intersectArrays(int * /*Provide a pointer to an array*/, 
                     int* /*Provide a pointer to a second array*/, 
                     int /*Size of arr1*/,  
                     int /*Size of arr2*/);


////////////////////////////////////////////
//                MAIN                    //
////////////////////////////////////////////

int main()
{
  cout << setprecision(4); 

  srand48(time(NULL)); // Initialize the sequence

  // We want to compute a mean of the probabilities, so we need a variable to store the numbers of runs
  int nRun = 10;

  // Variable to store the partial sum of the probabilities to compute their mean
  double partProbSum = 0.;

  // Number of iterations
  int N= 2e6;

  // Typical dimension of the graph
  int nbase=3;

  // Shape of the tiles (4=square)
  int v=4;

  // Absorption weight
  double q = 0;

  // Define the condition, i.e. what Laplacian RW we want
  int condition[] = {0,1,4,7,8};


  ///////////////////////////////////////////////////////////////////

  // Let's print out that this is the Laplacian RW
  cout << "\n LAPLACIAN RANDOM WALK" << endl;

  // Let's print out the condition
  cout << "\n We want to obtain the path: (" << condition[0] << ", ";
  for (int i = 1; i < sizeof(condition)/sizeof(condition[0]) -1 ; i++)
    cout << condition[i] << ", " ;
  cout << condition[sizeof(condition)/sizeof(condition[0]) -1] << ")" << endl;
    

  // Create the graph
  Graph g=RegularGraph(nbase,v, q);

  // Retreive the graph dimension
  int n=g.graph_size();

  // Compute the mean of the probabilities
  for (int j = 0; j < nRun; j++)
  {
    for(int i=0; i<N; i++)
    {
      LaplacianRandomWalk(g, condition, sizeof(condition)/sizeof(condition[0]) );

      #if PRINT==1 ||  PRINT==2
      cout << "\n******************************************************" << endl;
      #endif

    /*#if PRINT==1
      // Print the LERW
      cout << "\n->LERW: (";

      for(int i=0; i<(SizeLERW-1); i++)
        cout << LERW[i] << ", ";

      cout << LERW[SizeLERW-1] << ")" << endl;
    #endif*/
    }

    partProbSum += (double) g_successfulAttemptsCounter/(g_totalAttemptsCounter);

    // Print out the numebr of correct LERW
    /*cout << "\n Out of the " << N << " attempts, we have obtained " << g_totalAttemptsCounter << " Laplacian RWs,"
        << " of which " << g_successfulAttemptsCounter << " are the desired ones" 
        << ", so the probability is " << (double) g_successfulAttemptsCounter/(g_totalAttemptsCounter) *100 <<  "%" << endl;
    */
    g_successfulAttemptsCounter = 0;
    g_totalAttemptsCounter = 0;
  }
  
  // Print out the numeber of correct Laplacian RW
  cout << "\n In " << nRun << " runs of " << N << " attempts each, the mean probability to get the Laplacian RW (" << condition[0] << ", ";
  for (int i = 1; i < sizeof(condition)/sizeof(condition[0]) -1 ; i++)
    cout << condition[i] << ", " ;
  cout << condition[sizeof(condition)/sizeof(condition[0]) -1] << ") is " << partProbSum/nRun * 100 << "%" << endl;

  return 0;

} // End of main()




////////////////////////////
//  Function definitions  //
////////////////////////////

// We make a slight change to the RandomWalk (RW) function in order to make it stop after a fixed number of steps 'nSteps'
void LaplacianRandomWalk( Graph g /*Provide a graph to run the random walk on*/,
                          //int* trail  /*Provide a pointer to an array to store the trail of the RW*/,
                          int* condition /*Provide the array containing the desired trail of the Laplacian RW*/,
                          int conditionSize    /*as well as its size*/,
                          int  nSteps  /*(OPTIONAL) the number of steps (just to make sure the loop ends)*/)
{  
  // Set the starting vertex of the RWs as the last vertex of the condition
  int v = condition[conditionSize-1];

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

  // Start by storing the first vertex in the array 'trail'. It will be useful to then performe the loop erasure. 
  //trail[nSteps-nStepsLeft] = v;
  
  // Define the index that will run on the condition array 
  int j=0;
  
  // Define the growth vertex buffer, where the position of the Laplacian RW is at a given moment, initialized as the first vertex of the condition array
  int tip = condition[j];  

  // Define a local attempt counter
  int attemptsCounter = 0;

  // Create a map where the Laplacian RW will be stored. Start by adding the initial point, which must be the 'tip'
  unordered_map<int, bool> mp;

  mp[tip] = 1;

  while(mp.find(condition[conditionSize-1]) == mp.end()) // Runs until the Laplacian RW reaches the source
  {
    // Reset the starting vertex...
    v = condition[conditionSize-1]; 
    
    // ...and the starting rate...
    totR = g.graph_edge(v,n); 

    #if PRINT==1 ||  PRINT==2
      cout << "\n From the initial vertex " << v << endl ;
    #endif 

    while( totR * nStepsLeft !=0) // The loop stops when we can no longer move from a vertex, i.e. when its not connected to any other vertex (and hence the total rate is zero), 
                                  // OR when it has performed all the steps OR when it hits the trail of the Laplacian RW
    {

      // Resets the total probability to 1 at the beginning of each 'while' loop
      totP = 1.;                
      
      // Retrieve the move probabilities as the edge weight divided by the total weight 
      for ( int i=0; i<n; i++)
        moveP[i] = g.graph_edge(v,i)/totR;  

      // Now it initializes the random variable and assign the running index 'j' to start from the end (the last vertex is the absorption one)
      x = drand48();   // Assign a random variable
      k = n;           // Assign the index to start from the end (the last vertex is the absorption one)

      #if PRINT==2
        cout << "\n (The random number is: " << x << ")" << endl ;
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
          cout << " We move to vertex " << k << " (probability interval: [" << totP << ", " << totP+moveP[k] << "] )";    // We move to the new vertex k
      #endif

      if (v==k) 
      {
        
        #if PRINT==2
          cout << "\nA SELF-LOOP!!\n";
        #endif
      }

      // If k is not present in condition[], good, go ahed with the RW
      if (mp.find(k) == mp.end())
      {
        #if PRINT==1 ||  PRINT==2
          cout << "\n Vertex " << k << " not in the trail :)";
        #endif

        // Finally, reduce the number of steps left..
        nStepsLeft--;   

        //...update the starting vertex...
        v=k;

        // ...and the starting rate...
        totR = g.graph_edge(v,n); 
              
        // ... and record the starting vertex in the trail array
        //trail[nSteps-nStepsLeft] = v;
      }
      // If instead k is present, i.e. the RW hit the trail of the Laplacian RW, report it by return it braking the loop
      else
        goto jump1;
    }
      
    // Check if there is any intersection between the trail and the condition. intersactionArrays provides the first intersection
    //intersect = intersectArrays( condition, trail, j, nSteps);

    // If we jump here is because we hit the trail of the Laplacian RW. 
    jump1:
      // Check if k is at the tip, and if so add the last step to the Laplacian RW
      if (k == tip)
      {
        #if PRINT==1 ||  PRINT==2
          cout << "\n Vertex " << k << " is the tip!!";
        #endif

        // Store the previous vertex in the map
        mp[v] = 1;

        // And update the 'tip'        
        tip = v;
      }
      // Else, if it has hit a part of the trail, simply do the RW again
      else
      {
        #if PRINT==1 ||  PRINT==2
          cout << "\n Vertex " << k << " in the trail :(";
          cout << "\n So let's start again the RW without counting it as an attempt";
        #endif
      }

      #if PRINT == 1 ||  PRINT==2
        cout << " (after " << attemptsCounter << " failed attempts)" << endl;
      #endif
  }

  // Finally, check if the trail is the same as the condition: first check their size...
  if (conditionSize == mp.size())
  {
    // ...then check whether all the elements of condition[] are in the Laplacian RW just created
    for(int i=0; i<conditionSize; i++ )
    {
      // If the element 'i' is not found, break the 'for' and go directly to the total attempt counter increment
      if(mp.find(condition[i]) == mp.end()) 
      {
        goto jump2;
      }
    }

    // Otherwise, if all the elemnts are present, count it as a succesful attempt      
    g_successfulAttemptsCounter++;
  }

  jump2:
    g_totalAttemptsCounter++;

  
  #if PRINT == 1 ||  PRINT==2  
    cout << "\n Desired Laplacian RW obtained in " << attemptsCounter << " attempts" << endl;
  #endif

  /*
  #if PRINT==1
    cout << "\n\nAfter " << nSteps << " steps, the RW trail is: (";

    for(int i=0; i<nSteps; i++)
      cout << trail[i] << ", ";

    cout << trail[nSteps] << ")" << endl << endl;
  #endif*/

  delete[] moveP;

} // End of function LaplacianRandomWalk


// A function to check whether two arrays are equal
bool checkArrays(int * arr1, int * arr2, 
                 int /*Size of arr1*/ n, 
                 int m /*Size of arr2*/)
{
  // If lengths of arrays are not equal
  if (n != m)
  {
    g_notArrivedCounter++;
    return false;
  }
  

  // Store arr1[] elements and their frequencies in hash map
  unordered_map<int, int> mp;
  for (int i = 0; i < n; i++)
    mp[arr1[i]]++;

  // Traverse arr2[] elements and check if all elements of arr2[] are present in the same number of times or not.
  for (int i = 0; i < n; i++) 
  {
    // If there is an element in arr2[], but not in arr1[]
    if (mp.find(arr2[i]) == mp.end())
      return false;

    // If an element of arr2[] appears more times than it appears in arr1[]
    if (mp[arr2[i]] == 0)
      return false;

    // Decrease the count of arr2 elements in the unordered map
    mp[arr2[i]]--;
  }

  return true;
} // End of function checkArrays

// A function to check whether two arrays intersect, i.e. have elements in common 
int intersectArrays(int * arr1, int * arr2, 
                 /*Size of arr1*/ int n,  
                 int m /*Size of arr2*/)
{
  // Store arr1[] elements in hash map
  unordered_map<int, bool> mp;

  for (int i = 0; i < n; i++)
    mp[arr1[i]] = 1;

  // Traverse arr2[] elements and check if any element of arr2[] is also present in arr1[]
  for (int i = 0; i < m; i++) 
  {
    // If the element of arr2[] is not present, good, go ahed with the check
    if (mp.find(arr2[i]) == mp.end());
    // If instead the element is present, report it by return it 
    else
      return i;
  }

  // If no element of arr2[] was found in arr1[], then return -1, which in this circumstance means FALSE 
  return -1;
} // End of function checkArraysSoft