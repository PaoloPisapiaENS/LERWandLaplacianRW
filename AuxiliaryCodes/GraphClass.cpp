//////////////////////////////////////////////////////////////////////
// DEFINITION OF A CLASS THAT DEFINES GRAPHS AS AN ADJACECY MATRIX  //
//////////////////////////////////////////////////////////////////////

// Include a header file containing all the necessary libraries and function prototypes
#include "/mnt/d/Offline_Documents/University/PhD_Paris/PhD_work/Simulations/Simple_Graph/my_header.h" 

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
      for (int i = 0; i < n; i++) {
          for (int j = 0; j < n; j++) {
              cout << adj_matrix[i][j] << "\t";
          }
          cout << endl;
      }
      cout << endl;
    }


    // Function to compute the total rate of moving from a vertex 'u'. It also counts the number of edges 'nedges'.
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

  
////////////////////////////////////////////
//                MAIN                    //
////////////////////////////////////////////



int main()
{
  cout << setprecision(4); 

  srand48(time(NULL)); // Initialize the sequence

  // Number of vertices 
  int n = 3;

  // Boolean for undirected (1) or directed (0) graph
  bool undirected=0;

  // Let's print out our choice
  undirected ? cout << "Undirected graph" << endl << endl : cout << "Directed graph" << endl << endl ; 

  
  // Create a graph with n vertices
  Graph g(n);

  // Adding the specified edges in the graph 
  g.add_edge(0, 1, undirected);
  g.add_edge(0, 2, undirected);

  g.add_edge(1, 1, undirected, 10);
  g.add_edge(1, 2, undirected, 3);

  // Compute the total rate of moving from each vertex
  for(int i=0; i<n; i++)
    g.total_rate_vertex(i);

  // Print the adjacency matrix representation of the graph
  g.print();


  return 0;

} // End of main()