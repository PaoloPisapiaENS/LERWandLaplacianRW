//It is better to avoid creating branches with "if"
//e.g. instead of if(i==0 || i==N) we can initalize the sum by summing the endpoints

// To define global variables you have to declare it outside all function (main as well) using "static" when declaring it. It is also better to call it with a name that cannot be misunderstood (e.g. "g_<name>")

//Ternary operator "?": it has the same function of an if but it is a much shorter notation. Syntax: "(condition) ? option1:option2"

//Iterative functions are not as efficient as using a while cycle since you have to call the function many more times


/////////////////////////////////////////////////////////////////////////////////////////////////////
// include all the headers you may need

#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <ctime>          // To use time(NULL) as the seed in the pseudo-random number generator
#include <cstdlib>        // To use random numbers
#include <vector>         // To use vectors
#include <algorithm>      // To use find() for vectors
#include <bits/stdc++.h>  // To use hashing maps
#include <map>            // To use hashing maps (ordered)

using namespace std;

#define DELTA(x,y) (x==y ? 1 : 0) // Defines a MACRO that is the Kroneker delta

/////////////////////////////////////////////////////////////////
/*----------------Definition of the Graph class----------------*/
/////////////////////////////////////////////////////////////////

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

    // Function to add an edge between vertices 'u' and 'v' of the graph using a weight 'w' ('u' and 'v' could be the same vertex, which means that a self loop is allowed). If 'undirected' is true, then it is an undirected graph, and the edge going backwards is filled as well (with the same weight. If another weight is in order, one has to consider it as a directed graph and manually insert the new backward weight)
    void add_edge(int u, int v, bool undirected, double w=1.)
    {
        // Set edge from u to v
        adj_matrix[u][v] = w;
        // Set edge from v to u (for undirected graph)
        if (undirected) adj_matrix[v][u] = w;
    }


    // Function to print the adjacency matrix representation of the graph
    void print()
    {
      // Get the number of vertices
      cout << "Adjacency Matrix for the Graph (the last vertex is the 'Absorption vertex'): " << endl;
      int n = adj_matrix.size();

      cout << "\t  ";
      for (int i = 0; i < n; i++) 
        cout << i << "\t";

      cout << endl;

      cout << "\t ";
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

///////////////////////////////////////////////////////////////////////////
/*----------------Definition of the RegularGraph function----------------*/
///////////////////////////////////////////////////////////////////////////

Graph RegularGraph( int nbase /*number verteces in each row*/, 
                    int nv /*number of verteces of each 'tile': 4->square, 6->honeycomb, etc.*/,
                    double absorptionWeight=1. /*weight of the absorption link (optional)*/,
                    double weight=1. /*weight of each link (optional)*/,
                    bool undirected=1 /*boolean for undirected (1,TRUE) or directed (0,FALSE) graphs (optional, but almost always true)*/)
{
  // Define some variables by taking the information from the input
  int n = nbase;
  int v = nv;

  // Define the dimension of the graph (in any case the regular graph can be seen as a sqaure grid)
  int N = n*n;

  // Create the graph (The '+1' represents the absorption vertex)
  Graph g(N + 1);

  // Let's print out our choice
  undirected ? cout << "\nUndirected graph" << endl  : cout << "\nDirected graph" << endl; 


  // Then, depending on the number of vertices in each tile (i.e. shape of the tile), construct the graph
  if(v==3)
  {
    /*TO BE COMPLETED*/
  }

  else if(v==4)
  {
    cout << "\nSquare lattice " << n << "x" << n << endl;

    for(int i=0; i<N; i++)
    {
      for(int j=i+1;j<N; j++) // It adds the edges using kroneker delta to check that they are next-neighbours
       g.add_edge(i,j,undirected, weight * (( DELTA(i,(j+1)) + DELTA(i,(j-1)) ) * DELTA((i/n),(j/n)) + DELTA(i,(j+n)) + DELTA(i,(j-n))));
      
      // Then finally add the absorption rate
      g.add_edge(i, N, 0, absorptionWeight);

      // And compute the totatl rate of moving from the vertex (this information is stored in the extra column of the adjacency matrix)
      g.total_rate_vertex(i);
    }

  } 

  else if(v==6)
  {
    cout << "\nHoneycomb lattice " << n << "x" << n << endl;

    for(int i=0; i<n*n; i++)
    {
      for(int j=i+1;j<n*n; j++)
       g.add_edge(i,j,undirected, weight * ( DELTA((i/n),(j/n)) * ( DELTA(i,(j+1)) + DELTA(i,(j-1)) ) * ( DELTA((n%2), 0 ) * (DELTA( ((i+i/n)%2),0)) + DELTA((n%2), 1 ) * (DELTA((i%2),0)))/*on the same rows, if next-neighbours: if n is odd, then all the even verteces; if n is even then all the odd verteces on odd rows or even veretces on even rows*/ + (DELTA(i,(j+n)) + DELTA(i,(j-n)))/*i.e. all the vertex on the same column*/) );
      
      g.total_rate_vertex(i);
    }
  } 


  g.print();

  return g;
} // End of RegularGraph function