
//Clang -Wall -Wextra -pedantic -std=c++17
#include <cassert>
#include <random>
#include <memory>
#include <cmath>
#include <vector>
#include <iostream>
#include <chrono>
#include <algorithm>
#include <string>


int iters = 1000;
int n = 1024;

int main(){
	 
	std::default_random_engine generator;

		while (n < pow(2,27))
		{
			 int x = 0;
			double min = 318.0; // initializes minimum
			std::vector<int> indicies(n); //initialize vector to be n sized with 0's
			std::iota(indicies.begin(), indicies.end(), 0);
			std::shuffle(std::begin(indicies), std::end(indicies), generator);
			std::vector<int> buffer(n); // creates a buffer of size n

		    for (int i=0; i < iters; i++)
		    {   
		        

		        auto t1 = std::chrono::high_resolution_clock::now();
		        for (int j = 0; j < n ; j++)
		        { 
		            x = buffer[indicies[j]];
		            
		        }
		        auto t2 = std::chrono::high_resolution_clock::now();
		        double duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
		        duration*=pow(10,3);
		        
		        duration = duration / (n * sizeof(int));
		        if (duration < min) {min = duration;} 

		    }
		    std::cout<<min<<" " <<n <<" "<< x<< std::endl; //lets us see the output of average of n runs
		    n*=2;
		}

return 0;
}

//https://stackoverflow.com/questions/14246976/c-how-to-force-prefetch-data-to-cache-array-loop