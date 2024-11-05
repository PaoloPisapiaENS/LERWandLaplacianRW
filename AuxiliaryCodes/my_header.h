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
#include <ctime>         // To use time(NULL) as the seed in the pseudo-random number generator
#include <cstdlib>       // To use random numbers
#include <vector>
using namespace std;
