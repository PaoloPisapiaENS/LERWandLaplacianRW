/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    We want to define a LoopErasure function to run LERW on generic graphs (but with fixed number of steps for simplicity)   //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Include a header file containing all the necessary libraries and function prototypes
#include "/mnt/d/Offline_Documents/University/PhD_Paris/PhD_work/Simulations/Simple_Graph/my_UpgradedHeader.h" 

#define PRINT 0 // 1: print all the RandomWalk cout; 0: don't

int g_notArrivedCounter = 0;


/******************************************************************
//                Initialization Function                        //
******************************************************************/

void RandomWalk( Graph /*Provide a graph to run the random walk on*/,
                int*   /*Provide a pointer to an array to store the trail of the RW*/,
                int    /*as well as its dimension, i.e. the number of steps*/,
                int v=0   /*(OPTIONAL)Provide the starting vertex*/);

int LoopErasure( int*  /*Provide a pointer the array where the trail of the RW is stored*/,
                 int  /*as well as the number of steps (WARNING: its dimension is actually nSteps+1)*/,
                 int*& /*Provide the pointer to the array where the LERW is to be stored*/);

bool checkArrays(int* /*Provide a pointer to an array*/, 
                 int* /*Provide a pointer to a second array*/,
                 int  /*Size of arr1*/,
                 int  /*Size of arr2*/);


////////////////////////////////////////////
//                MAIN                    //
////////////////////////////////////////////

int main()
{
  cout << setprecision(4); 

  srand48(time(NULL)); // Initialize the sequence
  srand(time(NULL)); // Initialize the sequence

  // Number of iterations
  int N= 3e6;

  // Typical dimension of the graph
  int nbase=3;

  // Shape of the tiles (4=square)
  int v=4;

  // Absorption weight
  double q = 0;

  // Define the counter if a certain condition is met (e.g. we could want the LERW to be (0,1,3))
  int counter = 0;
  int condition[] = {0,1,4,7,8};

  // Define the array to store the 'trail' of the RW:

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // WARNING: DEPENDING ON THE NUMBER OF STEPS A PARTICULAR PATH MAY OR MAY NOT BE PRODUCED!!!  **SOLVED** : random number of steps //
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  int nSteps;        // First its size (actually its size-1)
  int* trail = NULL;

  // Define an arry that will store the LERW and an int to store its size
  int *LERW = NULL;
  int SizeLERW;
  
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

  //cout << "Size of the graph: " << n << endl;

  for(int i=0; i<N; i++)
  {
    nSteps = rand()%150+51; // Generate a random number in a given interval (in this way we are quite confident of getting RWs of any length and that has enough time to form loops)
    trail = new int[nSteps+1];
    
    // Run a random walk
    RandomWalk(g, trail, nSteps);

    // Loop erase the RW
    SizeLERW = LoopErasure(trail, nSteps, LERW);

    #if PRINT==1
      // Print the LERW
      cout << "\n->LERW: (";

      for(int i=0; i<(SizeLERW-1); i++)
        cout << LERW[i] << ", ";

      cout << LERW[SizeLERW-1] << ")" << endl;
    #endif

    // Check if the LERW is the same as the condition
    if(checkArrays(condition, LERW, sizeof(condition)/sizeof(condition[0]) ,SizeLERW))
      counter++;

    #if PRINT==1
      cout << "****************************************************************************************" << endl;
    #endif

    delete[] LERW;
    delete[] trail;
  }

  // Print out the numebr of correct LERW
  cout << "\n Of the total " << N << " RWs," << endl
       << " the number of LERWs that arrived at the correct end point is " << N-g_notArrivedCounter << ", and the number of correct LERWs is " << counter << "," << endl
       << " with a conditioned probability of " << (double) counter/(N-g_notArrivedCounter) *100 <<  "%" << endl;


  return 0;

} // End of main()




////////////////////////////
//  Function definitions  //
////////////////////////////

// We make a slight change to the RandomWalk (RW) function in order to make it stop after a fixed number of steps 'nSteps'
void RandomWalk( Graph g /*Provide a graph to run the random walk on*/,
                 int* trail  /*Provide a pointer to an array to store the trail of the RW*/,
                 int  nSteps  /*as well as its dimension, i.e. the number of steps*/,
                 int v      /*(OPTIONAL)Provide the starting vertex*/)
{  
  // Define the number of steps left and initialize it
  int nStepsLeft = nSteps;

  // Define the random variable to decide where to move
  double x; 

  // Retrieve the size of the graph
  int n = g.graph_size();

  // Set an index variable
  int j = 0;

  // Total rate buffer (the total rate information is stored in the n+1 column of the adjacency matrix)
  double totR= g.graph_edge(v,n);

  #if PRINT==1
    cout << "The total weight of the initial vertex is: " << totR << endl;
  #endif

  // Total probability buffer
  double totP = 1.;

  // Buffer for the probability to move towards a specific vertex
  double* moveP = new double[n];

  // Start by storing the first vertex in the array 'trail'. It will be useful to then performe the loop erasure. 
  trail[nSteps-nStepsLeft] = v;

  while( totR * nStepsLeft !=0) // The loop stops when we can no longer move from a vertex, i.e. when its not connected to any other vertex (and hence the total rate is zero), 
                            // OR when it has performed all the steps
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

    // Finally, it reduce the number of steps left..
    nStepsLeft--;

    //...it updates the starting vertex...
    v=j;      

    // ...and the starting rate...
    totR = g.graph_edge(v,n); 
         
    // ... and it records the starting vertex in the trail array
    trail[nSteps-nStepsLeft] = v;
  }

  #if PRINT==1
    cout << "\n\nAfter " << nSteps << " steps, the RW trail is: (";

    for(int i=0; i<nSteps; i++)
      cout << trail[i] << ", ";

    cout << trail[nSteps] << ")" << endl;
  #endif

  delete[] moveP;

} // End of function RandomWalk



// Now we define a loopErasing function
int LoopErasure( int* trail  /*Provide a pointer the array where the trail of the RW is stored*/,
                  int  nSteps /*as well as the number of steps (WARNING: its dimension is actually nSteps+1)*/,
                  int*& LERW   /*Provide the pointer to the array where the LERW is to be stored*/)
{
  // Buffer for the current vertex, initialized as the first vertex in the 'trail' array
  int v;

  // Running indeces initialized from the end and the beginning
  int i=nSteps;
  int j=0;

  // Dynamical Size of the new array
  int dynSize = 0;

  // Auxiliary arrays
  int* auxiliary1 = new int[dynSize+1];
  int* auxiliary2 = NULL;

  
  while(i>=j) // Runs from the end of the RW and stops if it reaches the 'starting' vertex (i.e. the one stored in 'trial[j]')
  {
    // Initializes the 'starting' vertex
    v = trail[j];              

    // Store the 'starting' vertex in the auxiliary array1 in position auxiliary1[dynSize]
    auxiliary1[dynSize] = v;         

    // And, free the memory of auxiliary2
    delete[] auxiliary2;  

    while((v-trail[i])!=0)  // Checks if the starting vertex 'v' is the same as another vertex, beginning the check from the end of the RW 
      i--;
    
    // If it finds a vertex equal to 'v', the while loop stops...
    // ...the 'starting' index is updated...
    j=i+1;

    // ...the dynamical size of the auxiliary array is increased...
    dynSize++;

    // ...and the auxiliary2 array is created and filled by copying the auxiliary1 memory
    auxiliary2 = new int[dynSize];

    for(int k=0; k<dynSize; k++)
      auxiliary2[k] = auxiliary1[k];

    // Then the memory of auxiliary1 is freed and reallocated..
    delete[] auxiliary1;
    auxiliary1 = new int[dynSize+1];
    // ...and it is updated by copying from auxiliary2 + the new vertex(done in the next cycle)
    for(int k=0; k<dynSize; k++)
      auxiliary1[k]=auxiliary2[k];
    
    // Finally, 'i' is reinitialized
    i=nSteps;
  }

  // After all the loops are erased, copy the information of auxiliary2 into the input array 'LERW'
  LERW = auxiliary2;

  delete[] auxiliary1;
  //delete[] auxiliary2; NO NEED TO DELETE IT SINCE WE NEED IT FOR LERW!!!

  return dynSize;

} // End of function LoopErasure 


// A function to check whether two arrays are equal
bool checkArrays(int * arr1, int * arr2, 
                 int n /*Size of arr1*/, 
                 int m /*Size of arr2*/)
{
  // Store arr1[] elements and their frequencies in hash map
  unordered_map<int, int> mp;
  for (int i = 0; i < n; i++)
    mp[arr1[i]]++;

  // Check if the last element of arr2[] is not the same as that of arr1[]
  if (arr2[m-1] != arr1[n-1])
  {
    #if PRINT == 1
      cout << "\n Not arrived at the correct end-point :(" << endl;
    #endif

    g_notArrivedCounter++;
    return false;
  }
  
  // If lengths of arrays are not equal
  if (n != m)
  {
    #if PRINT == 1
      cout << "\n Wrong path :( " << endl;
    #endif

    return false;
  }

  // Traverse arr2[] elements and check if all elements of arr2[] are present in the same number of times or not.
  for (int i = 0; i < n-1; i++) 
  {
    // If there is an element in arr2[], but not in arr1[]
    if (mp.find(arr2[i]) == mp.end())
    {
      #if PRINT == 1
        cout << "\n Wrong path :( " << endl;
      #endif

      return false;
    }

    // If an element of arr2[] appears more times than it appears in arr1[]
    if (mp[arr2[i]] == 0)
      return false;

    // Decrease the count of arr2 elements in the unordered map
    mp[arr2[i]]--;
  }

  #if PRINT == 1
      cout << "\n Correct path! :) " << endl;
  #endif

  return true;
} // End of function checkArrays