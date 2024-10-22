////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// We want to run a simulation of a random walk on a simple graph
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Include a header file containing all the necessary libraries and function prototypes
#include "/mnt/d/Offline_Documents/University/PhD_Paris/PhD_work/Simulations/Simple_Graph/my_header.h" 

#define SESSION 2  // To do slightly different things: 0: directed graph; 2: undirected graph;
#define PRINT 0     // 1: print all the RandomWalk cout; 0: don't

// Global variables
int nedges=1;         // Edges counter
int selfLoop=0;      // Initially set the selfLoop to 

bool LR=0;              // Boolean to distinguish if it went Left or Right
int countR=0;           // A variable to count whenever the path went right
int countL=0;           // A variable to count whenever the path went left
int countA=0;           // A variable to count whenever the path is absorbed before making it to end

class Graph {
    // Adjacency matrix to store graph edges
    vector<vector<double> > adj_matrix;

public:
    // Constructor to initialize the graph with 'n' vertices
    Graph(int n)
    {
        adj_matrix
            = vector<vector<double> >(n, vector<double>(n, 0));
    }

    // Function to add an edge between vertices 'u' and 'v' of the graph using a weight 'q' ('u' and 'v' could be the same vertex, which means that a self loop is allowed). If 'ud' is true, then it is an undirected graph, and the edge going backwards is filled as well
    void add_edge(int u, int v, double q, bool ud)
    {
        // Set edge from u to v
        adj_matrix[u][v] = q;
        // Set edge from v to u (for undirected graph)
        if (ud) adj_matrix[v][u] = q;
    }


    // Function to print the adjacency matrix representation of the graph
    void print()
    {
      // Get the number of vertices
      cout << "Adjacency Matrix for the Graph: " << endl;
      int n = adj_matrix.size();
      for (int i = 0; i < n; i++) {
          for (int j = 0; j < n; j++) {
              cout << adj_matrix[i][j] << "\t";
          }
          cout << endl;
      }
      cout << endl;
    }


    // Function to compute the total sum of the edges weights at vertex 'u'
    /*double total_sum_edge(int u)
    {
      // Define total sum variable
      double totSum=0.;

      // Get the number of vertices
      int n = adj_matrix.size();
      for (int i = 0; i < n; i++) 
        totSum += adj_matrix[u][i];
      
      return totSum;
    }*/


    // Function to compute the total rate of moving from a vertex 'u'. It also counts the number of edges 'nedges'.
    double total_rate_vertex(int u)
    {
      // Define total probability variable
      double totRate=0.;
      // Set the counter 'nedges' to zero
      nedges = 0;


      // Get the number of vertices
      int n = adj_matrix.size();

      for (int i = 0; i < n; i++) 
        if (adj_matrix[u][i] != 0) 
        {
          totRate += adj_matrix[u][i];
          nedges++;
        }
      if(nedges==0)  return 0;

      return totRate;
    }


    // Function to retrive the size of a graph
    int graph_size()
    {
      return adj_matrix.size();
    }

    // Function to retrieve the edge between 'u' and 'v' of the graph
    double graph_edge(int u, int v)
    {
      return adj_matrix[u][v];
    }
     
}; // End of class Graph

  
// Function declaration
void RandomWalk (Graph );


////////////////////////////////////////////
//                MAIN                    //
////////////////////////////////////////////



int main()
{
  cout << setprecision(4); 

  srand48(time(NULL)); // Initialize the sequence

  // Number of iterations
  int N = 1.e7;

  // Number of vertices (I consider the absorption as one of the vertices: it is that '+1')
  int n = (3+1);

  // Weight of ea ch edge 'beta' and of being absorbed 'q'
  double beta =  0.5 ;
  double beta1 = 12 ;
  double beta2 = 11. ;
  double beta3 = 1. ;
  double q    =  1. ;

  // Boolean for undirected (1) or directed (0) graph
  bool ud;

  #if SESSION==0
  ud = 0;
  #endif

  #if SESSION==2
  ud = 1;
  #endif

  // Let's print out our choice
  ud ? cout << "Undirected graph" << endl << endl : cout << "Directed graph" << endl << endl ; 

  /*// Weight of the random walks
  double w;             // Temporary buffer
  double wLeft=0.;      // Path toward the Left, i.e. no possible self loops
  double wRight=0.;     // Path toward the Right, i.e. possible self loops
  double wAbsorebd=0.;     // Path absorbed before reaching the end*/

  // Probabilities of left, right and absorbed paths
  double pLeft;
  double pRight;
  double pAbsorbed;

  // Create a graph with n vertices
  Graph g(n);

#if SESSION==0
  // Adding the specified edges in the graph 
  g.add_edge(0, 1, beta, ud);
  g.add_edge(0, 2, beta2, ud);
  g.add_edge(0, 3, q, ud);      // Absorption

  g.add_edge(1, 1, beta1, ud);
  g.add_edge(1, 2, beta3, ud);
  g.add_edge(1, 3, q, ud);      // Absorption

#endif 

#if SESSION==2
  // Adding the specified edges in the graph
  g.add_edge(0, 1, beta, ud);
  g.add_edge(0, 2, beta2, 0);
  g.add_edge(0, 3, q, 0);      // Absorption

  g.add_edge(1, 1, beta1, ud);
  g.add_edge(1, 2, beta3, 0);
  g.add_edge(1, 3, q, 0);      // Absorption
#endif 

  // Print the adjacency matrix representation of the graph
  g.print();

  /*// Test total probability again
  for(int i =0; i<n; i++)
  cout << "Total prob for edge " << i << ": " << g.total_rate_vertex(i) << "\t(" << nedges << " edge(s))" << endl;

  // Test the access to the vector memory and the retrieval of the graph size
  cout << "\nVector element (0,1): " << g.graph_edge(0,1) << endl;
  cout << "\nVector element (1,0): " << g.graph_edge(1,0) << endl;
  cout << "\nVector element (3,3): " << g.graph_edge(3,3) << endl;
  cout << "\nGraph size: " << g.graph_size() << endl << endl;
  
  */

  /////////////////////////////////////////////////////
  //        Test the RandomWalk function             //
  /////////////////////////////////////////////////////

  for(int i=0; i<N; i++)
  {
    // w=RandomWalk(g);
    RandomWalk(g);

    // LR ? wLeft += w : wRight += w; // if it went left, sum p to pLeft, otherwise sum it to pRight
  }

  // Simple test: check that it goes as much to the left as to the right (same weight on both edges)
  pRight = (double) countR/(countR+countL);     // This...
  pLeft = (double) countL/(countR+countL);      // ...and this are conditioned probabilities: the prob to reach the end provided it didn't get absorbed
  pAbsorbed = (double) countA/N;

  cout << "\nOf the total " << N << " paths: "
    << "\nit got absorbed " << countA << " times \t=>\t empirical probability: " << pAbsorbed*100 << "%"
    << ";\nit went LEFT " << countL << " times \t=>\t empirical probability: " << pLeft*100 << "%" 
    << ";\nit went RIGHT " << countR << " times \t=>\t empirical probability: " << pRight*100 << "%."
    << endl << endl;
  cout << "The ration between the two is (pL/pR) " << pLeft/pRight << " (expected to be equal to countL/countR = " << (double) countL/countR << ")" << endl << endl;

  /*// WHAT WE ARE REALLY INTERESTED IN: we want the weight of the left paths compared to those on the right.  
  wLeft /= countL;
  wRight /= countR;

  cout << "Weight on the LEFT " << wLeft << endl;
  cout << "Weight on the RIGHT " << wRight  << " (a total of " << selfLoop << " self-loops)" << endl ;
  
#if SESSION==0
  cout << "Ratio of them ( 1 - Weight on the LEFT/Weight on the RIGHT=" << q << " expected) " << (1 - wLeft/wRight) << endl << endl;
#endif 

#if SESSION==2
  cout << "Ratio of them ( Weight on the LEFT/Weight on the RIGHT= 1 expected) " << (wLeft/wRight) << endl << endl;
#endif */


  return 0;

} // End of main()



/******************************************************************
//                           Functions                           //
******************************************************************/


// Define all the functions 
void RandomWalk( Graph g)
{
  // Define the random variable To decide where to move
  double x; 

  // Retrieve the size of the graph
  int n = g.graph_size();

  // Set an index variable
  int j = 0;

  // Define the initial vertex position: we start from vertex 0
  int v = 0;

  // Total rate buffer
  double totR= g.total_rate_vertex(v);

  //Total probability buffer
  double totP = 1.;

  // Buffer for the probability to move twards a specific vertex
  double* moveP = new double[n];


  while(nedges !=0) // The loop stops when we can no longer move from a vertex, i.e. when its not connected to any other vertex
  {
    totP = 1.;                // Resets the total probability to 1
    for ( int i=0; i<n; i++)
    {
      moveP[i] = g.graph_edge(v,i)/totR;  
    }
    
    x = drand48();   // Assign a random variable
    j = n;           // Assign the index to start from the end (the last vertex is the absorption one)

#if PRINT==1
    cout << "(The random number is: " << x << ")" << endl ;
#endif

    while ( x<= totP)               
    {
      j--;
      totP -= moveP[j];
    }

    // w*= g.graph_edge(v,j);
    
    if(j==(n-1))        // i.e. if it gets absorbed (the absorption vertex is the last one)
    {
      countA++;
    
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
      selfLoop++;
      
      #if PRINT==1
        cout << "\nA SELF-LOOP!!\n";
      #endif
    }

    #if PRINT==1
      cout << endl; 
    #endif

    if (j==2)     // i.e. if it reaches the destination
    {
      if (v==1)   // i.e. from the vertex on the right
      {
        LR = 0;
        countR++;

        #if PRINT==1
          cout << "(It went from RIGHT " << countR << " times)" << endl << endl;
        #endif
      }

      else 
      {
        LR = 1;
        countL++;

        #if PRINT==1
          cout << "(It went from LEFT " << countL << " times)" << endl << endl;
        #endif
      }
    }

    v=j;      // Now it updates the starting vertex...

    totR = g.total_rate_vertex(v);      // ...and starting rate and 'nedges' (which is done inside total_rate_vertex)
  }

  delete[] moveP;

  #if PRINT == 1
    //cout << "\nThe weight of the walk is: " << w << endl;
    cout << "---------------------------------------" << endl;
  #endif

  // return w;

} // End of function RandomWalk