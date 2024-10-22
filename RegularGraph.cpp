//////////////////////////////////////////////////////////////////////
//    DEFINITION OF A FUNCTION THAT DEFINES REGULAR GRAPHS IN 2D    //
//////////////////////////////////////////////////////////////////////

// Include a header file containing all the necessary libraries and function prototypes
#include "/mnt/d/Offline_Documents/University/PhD_Paris/PhD_work/Simulations/Simple_Graph/my_header.h" 

#define DELTA(x,y) (x==y ? 1 : 0) // Defines a MACRO that is the Kroneker delta


class Graph {
    // Adjacency matrix to store graph edges
    vector<vector<double> > adj_matrix;

public:
    // Constructor to initialize the graph with 'n' vertices
    Graph(int n)
    {
        adj_matrix
            = vector<vector<double> >(n, vector<double>(n+1, 0));       // NB: we have added an extra column which will contain the sum of all rates in each row. This will come in handy as it allows
                                                                        // us to retreive this information much more quickly
    }

    // Function to add an edge between vertices 'u' and 'v' of the graph using a weight 'q' ('u' and 'v' could be the same vertex, which means that a self loop is allowed). If 'undirected' is true, then it is an undirected graph, and the edge going backwards is filled as well (with the same weight. If another weight is in order, one has to consider it as a directed graph and manually insert the new backward weight)
    void add_edge(int u, int v, bool undirected, double q=1.)
    {
        // Set edge from u to v
        adj_matrix[u][v] = q;
        // Set edge from v to u (for undirected graph)
        if (undirected) adj_matrix[v][u] = q;
    }


    // Function to print the adjacency matrix representation of the graph
    void print()
    {
      // Get the number of vertices
      cout << "Adjacency Matrix for the Graph: " << endl;
      int n = adj_matrix.size();
      

      cout << "\t  ";
      for (int i = 0; i < n; i++) 
        cout << i << "\t";

      cout << endl;

      cout << "\t  ";
      for (int i = 0; i < n-1; i++) 
        cout <<  "________";

      cout << endl;

      for (int i = 0; i < n; i++) 
      {
        cout << i << "\t| ";
        for (int j = 0; j < n; j++) 
        {
          cout << adj_matrix[i][j] << "\t";
        }
          cout << endl;
      }
      cout << endl;
    }


    // Function to compute the total rate of moving from a vertex 'u'.
    double total_rate_vertex(int u)
    {
      // Define total probability variable
      double totRate=0.;


      // Get the number of vertices
      int n = adj_matrix.size();

      for (int i = 0; i < n; i++) 
        totRate += adj_matrix[u][i];
        
      // We both return totRate and store this information in the extra column of the adj_matrix
      adj_matrix[u][n] = totRate;

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


Graph RegularGraph( int nbase /*number verteces in each row*/, 
                    int nv /*number of verteces of each 'tile': 4->square, 6->honeycomb, etc.*/,
                    bool undirected=1 /*boolean for undirected (1,TRUE) or directed (0,FALSE) graphs (optional, but almost always true)*/,
                    double w=1. /*weight of each link (optional)*/)
{
  // Define some variables by taking the information from the input
  int n = nbase;
  int v = nv;

  // Create the graph
  Graph g(n*n);

  // Let's print out our choice
  undirected ? cout << "Undirected graph" << endl  : cout << "Directed graph" << endl; 


  // Then, depending on the number of vertices in each tile (i.e. shape of the tile), construct the graph
  if(v==3)
  {
    /*TO BE COMPLETED*/
  }

  else if(v==4)
  {
    cout << "\nSquare lattice" << endl;
    for(int i=0; i<n*n; i++)
      for(int j=i+1;j<n*n; j++)
       g.add_edge(i,j,undirected, w* (( DELTA(i,(j+1)) + DELTA(i,(j-1)) ) * DELTA((i/n),(j/n)) + DELTA(i,(j+n)) + DELTA(i,(j-n))));
  } 

  else if(v==6)
  {
    cout << "\nHoneycomb lattice" << endl;
    for(int i=0; i<n*n; i++)
      for(int j=i+1;j<n*n; j++)
       g.add_edge(i,j,undirected, w * ( DELTA(i,(j-1)) * DELTA((i/n),(j/n)) * ( DELTA( ((n + i + i/n)%2), 0) + DELTA( ((n )%2), 1) * DELTA( ((i )%2), 0) ) + DELTA(i,(j+n)) + DELTA(i,(j-n))) );
  } 


  g.print();

  return g;
} // End of RegularGraph function

  
////////////////////////////////////////////
//                MAIN                    //
////////////////////////////////////////////



int main()
{
  cout << setprecision(4); 

  srand48(time(NULL)); // Initialize the sequence

  // Number of vertices for each tile (i.e. shape of the tiles)
  int v = 6;      // square lattice
  // Number of vertices in each row
  int n = 3;

  Graph g = RegularGraph(n,v);

  return 0;

} // End of main()