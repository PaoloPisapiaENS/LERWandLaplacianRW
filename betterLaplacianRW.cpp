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

#if PRINT == -1

void RandomWalk( Graph /*Provide a graph to run the random walk on*/,
                int*   /*Provide a pointer to an array to store the trail of the RW*/,
                int    /*as well as its dimension, i.e. the number of steps*/,
                int v=0   /*(OPTIONAL)Provide the starting vertex*/);

int LaplacianRW(  Graph  /*Provide a graph to run the random walk on*/,
                  int   /*Provide the number of steps desired for the RW*/,
                  int*  /*Provide the array containing the desired trail of the Laplacian RW*/,
                  int /*as well as its size*/);
#endif

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


  // Number of iterations
  int N= 1e6;

  // Typical dimension of the graph
  int nbase=3;

  // Shape of the tiles (4=square)
  int v=4;

  // Absorption weight
  double q = 0;

  // Define the condition, i.e. what Laplacian RW we want
  int condition[] = {0,1,4};

  // Define the desired length of the RW:
  //int nSteps = 250;        // First its size (actually its size-1)

  // Define the variable that store the number of total attempts
  //int totAttempts = 0;

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

  //cout << "Size of the graph: " << n << endl;

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

  // Print out the numebr of correct LERW
  cout << "\n We have obtained " << N << " Laplacian RWs, in a total of " << g_totalAttemptsCounter << " attempts" 
       << ", so the probability is " << (double) N/(g_totalAttemptsCounter) *100 <<  "%" << endl;

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
  // And set an index variable
  int k = 0;

  // Start by storing the first vertex in the array 'trail'. It will be useful to then performe the loop erasure. 
  //trail[nSteps-nStepsLeft] = v;
  
  // Define the index that will run on the condition array 
  int j=0;
  
  // Define the growth vertex buffer, where the position of the Laplacian RW is at a given moment, initialized as the first vertex of the condition array
  int tip = condition[j];  
  
  // Define also the growth vertex, where the condition wants the Laplacian RW to growt to. If the RW arrives here without arriving at a point in the past first, then the point is added to the Laplacian RW
  int growth_v = condition[++j];

  // Define a local attempt counter
  int attemptsCounter = 0;

  // Index of the first (possible) intersection
  //int intersect;

  // Store the first elements of condition[] in hash map
  unordered_map<int, bool> mp;
  
  mp[condition[0]] = 1;


  while(j<conditionSize) // Runs until the Laplacian RW has covered the whole condition array
  {
    // Reset the starting vertex
    v = condition[conditionSize-1]; 

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
          cout << " We move to vertex " << k;    // We move to the new vertex k
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
        goto jump;
    }
      
    // Check if there is any intersection between the trail and the condition. intersactionArrays provides the first intersection
    //intersect = intersectArrays( condition, trail, j, nSteps);

    // If we jump here is because we hit the trail of the Laplacian RW. 
    jump:
      // Check if k is at the tip, and count it as an attempt(we want a CONDITIONED PROBABILITY!!). 
      // But if it hits FROM the growt_v, then it is good, and so remove it from the count and upgrade the variables. 
      if (k == tip)
      {
        #if PRINT==1 ||  PRINT==2
          cout << "\n Vertex " << k << " is the tip!!";
        #endif

        g_totalAttemptsCounter++;
        attemptsCounter++;

        if ( v == growth_v )
        {
          // If the intersection is at the tip FROM the growt_v, then it is a good attempt
          g_successfulAttemptsCounter++;
          g_totalAttemptsCounter--;

          // Store the next elements of condition[] in hash map
          mp[condition[j]] = 1;
          
          //And update the growth_v and tip and advances the index over the condition array
          j++;
          tip = growth_v;
          if(j<conditionSize) growth_v = condition[j];
        }
        else
        {
          #if PRINT==1 ||  PRINT==2
            cout << "\n But it arrives from the wrong direction :(";
            cout << "\n So let's start again the RW" << endl;
          #endif
        }
      }
      else
      {
        #if PRINT==1 ||  PRINT==2
          cout << "\n Vertex " << k << " in the trail :(";
          cout << "\n So let's start again the RW" << endl;
        #endif
      }

      #if PRINT == 1   
        cout << " (after " << attemptsCounter << " total attempts)" << endl;
      #endif
  }
  g_totalAttemptsCounter++;

  
  /*
  #if PRINT==1
    cout << "\n\nAfter " << nSteps << " steps, the RW trail is: (";

    for(int i=0; i<nSteps; i++)
      cout << trail[i] << ", ";

    cout << trail[nSteps] << ")" << endl << endl;
  #endif*/

  delete[] moveP;

} // End of function LaplacianRandomWalk

#if PRINT == -1
// Now we define a loopErasing function
int LaplacianRW(  Graph g /*Provide a graph to run the random walk on*/,
                  int  nSteps /*Provide the number of steps desired for the RW*/,
                  int* condition /*Provide the array containing the desired trail of the Laplacian RW*/,
                  int conditionSize    /*as well as its size*/)
{
  // Define the index that will run on the condition array 
  int j=0;
  // Define the growth vertex buffer, where the position of the Laplacian RW is at a given moment, initialized as the first vertex of the condition array
   int tip = condition[j];  
  // Define also the growth vertex, where the condition wants the Laplacian RW to growt to. If the RW arrives here without arriving at a point in the past first, then the point is added to the Laplacian RW
  int growth_v = condition[++j];

  // Counter for the total number of attempts 
  int attemptsCounter = 0;

  // Index of the first (possible) intersection
  int intersect;

  // Define the Trail array, where the RW trail will be stored
  int *trail = new int[nSteps+1];

  while(j<conditionSize) // Runs until the Laplacian RW has covered the whole condition array
  {
    // Run a RW from the end vertex
    RandomWalk(g, trail, nSteps, condition[conditionSize-1]);
    
    // Check if there is any intersection between the trail and the condition. intersactionArrays provides the first intersection
    intersect = intersectArrays( condition, trail, j, nSteps);

    // If the intersection is at the tip, count it as an attempt(we want a CONDITIONED PROBABILITY!!). 
    // But if it hits FROM the growt_v, then it is good, and so remove it from the count and upgrade the variables. If there is no intersection, just generate a new RW
    if (trail[intersect] == tip)
    {
      attemptsCounter++;

      if ( trail[intersect-1] == growth_v )
      {
        // If the intersection is at the tip FROM the growt_v, then it is a good attempt, and we will only count it at the end. So remove it for now from the counter
        attemptsCounter--;
        
        //And update the growth_v and tip and advances the index over the condition array
        j++;
        tip = growth_v;
        if(j<conditionSize) growth_v = condition[j];
      }
    }
    
    /*
    // Now check if the RW hits the tip and growth vertex of the Laplacian RW, and if so, also check if it did not hit a piece of the Laplacian RW already there 
    for(int i=1; i<=nSteps; i++ )
    {
      if( trail[i] == tip )
      {
        #if PRINT == 1
          cout << "\n Tip hit!";
        #endif

        //////////////////////////////////////////////
        // DOUBLE CHECK THAT THE COUNTING IS RIGHT  //
        //////////////////////////////////////////////


        if( intersect== true )
        {
          #if PRINT == 1
            cout << " However, it hit the trail before :( ";
          #endif
          attemptsCounter++;
          //attemptsCounter--;
        }

        else if(trail[i-1] == growth_v)
        {       
          #if PRINT == 1   
            cout << "\n Vertex number " << tip << " succesfully hit coming from " << growth_v;
          #endif

          // Updates the growth_v and tip and advances the index over the condition array
          j++;
          tip = growth_v;
          if(j<conditionSize) growth_v = condition[j];
        }

        else
        {
          #if PRINT == 1   
            cout << "\n But a vertex in the trail of the Laplacian RW was hit :(";
          #endif
          attemptsCounter++;
        }

        break;
      }    
    }*/

    
    #if PRINT == 1   
      cout << " (after " << attemptsCounter << " total attempts)" << endl;
    #endif
  }
  attemptsCounter++;

  delete[] trail;

  // Finally returns the total number of attempt to get that Laplacian RW //as (no.attempts - (lenght of the Laplacian RW-1) because we need at least a number equal to (lenght of the Laplacian RW)+1 of attempts

  #if PRINT == 1   
    cout << "\n We get the desired Laplacian RW after: " << (attemptsCounter - (conditionSize-1)) << " attempts" << endl
        << "*****************************************************************************************" << endl; 
  #endif

  return (attemptsCounter);// - (conditionSize-1));

} // End of function LaplacianRW 

#endif

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