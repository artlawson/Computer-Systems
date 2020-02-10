
#include <algorithm>
#include <cassert>
#include <random>
#include <memory>
#include <vector>
#include <iostream>
#include <chrono>
#include <string>

//Create a list of 'size' floating point numbers in the range [-bound, bound]

std::default_random_engine generator;

std::vector<double> generate_random_vector(int size, float bound) //make size and bound into pointers / reference
    
    {
        std::vector<double> vect(size, 0.0);
        std::uniform_real_distribution<double> distribution(-bound,bound);

        for (int j = 0; j < size; j++)
        {
            vect[j] = distribution(generator);
        }
        return vect;
    }


// Update position by velocity, one time-step
void update_coords( std::vector<double> &xs, 
                   std::vector<double> &ys, 
                   std::vector<double> &zs, 
                   const std::vector<double> &vx, 
                   const std::vector<double> &vy, 
                   const std::vector<double> &vz) {
   
    for (unsigned i = 0; i < xs.size(); i++){
        xs[i] = xs[i] + vx[i];
        ys[i] = ys[i] + vy[i];
        zs[i] = zs[i] + vz[i];

        }

    }
void run(int itter,std::vector<double> &xs, 
                   std::vector<double> &ys, 
                   std::vector<double> &zs, 
                   const std::vector<double> &vx, 
                   const std::vector<double> &vy, 
                   const std::vector<double> &vz ){
  for (int i = 0; i<itter; i++){
    update_coords(xs, ys, zs, vx, vy, vz);}


  }
int main () {


const int size = 256; //CHANGE SIZE HERE
std::cout<<size<<std::endl;
const int itters = 100000; //CHANGE ITERATIONS HERE



const float bound = 1000.0;


std::vector<double> xs = generate_random_vector(size, bound);
std::vector<double> ys = generate_random_vector(size, bound);
std::vector<double> zs = generate_random_vector(size, bound);
std::vector<double> vx = generate_random_vector(size, 1.0);
std::vector<double> vy = generate_random_vector(size, 1.0);
std::vector<double> vz = generate_random_vector(size, 1.0);

auto t1 = std::chrono::high_resolution_clock::now();
run(itters, xs, ys, zs, vx, vy, vz);
auto t2 = std::chrono::high_resolution_clock::now();

double duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();

duration = duration / (size * itters);

double checksum = accumulate(xs.begin(),xs.end(),0) +
                  accumulate(ys.begin(),ys.end(),0) +
                  accumulate(zs.begin(),zs.end(),0);

std::cout<<"mean time per coordinate is: "<<duration<< "us"<<std::endl;
std::cout<<"Checksum: "<<checksum<<std::endl;

return 0;

    }
