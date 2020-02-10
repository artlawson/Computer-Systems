
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

////////tested function below//////////
//std::default_random_engine generator;
//std::uniform_real_distribution<double> distribution(-10.0,10.0);

 // for (int i=0; i< 10; ++i) {
   // double number = distribution(generator);
    //std::cout<<number<<std::endl;
   


/////////


// Update position by velocity, one time-step
void update_coords(std::vector<double> xs, 
                   std::vector<double> ys, 
                   std::vector<double> zs, 
                   std::vector<double> vx, 
                   std::vector<double> vy, 
                   std::vector<double> vz) {
   
    for (unsigned i = 0; i < xs.size(); i++){
        xs[i] = xs[i] + vx[i];
        ys[i] = ys[i] + vy[i];
        zs[i] = zs[i] + vz[i];

        }

    }
void run(int itter,std::vector<double> xs, 
                   std::vector<double> ys, 
                   std::vector<double> zs, 
                   std::vector<double> vx, 
                   std::vector<double> vy, 
                   std::vector<double> vz ){
  for (int i = 0; i<itter; i++){
    update_coords(xs, ys, zs, vx, vy, vz);}


  }
int main () {
//if (argc!= 3){


 //   std::cout<<"Required arguments: vector_length(N) and iterations_num(M)"<<std::endl;
 //   exit (EXIT_FAILURE);

 //   }

//int size = static_cast<unsigned>(std::string(argv[1]));
//int iters =static_cast<unsigned>(std::string(argv[2]));

//std::string arg = argv[1];
int size = 256;

//std::string arg_1 = argv[2];
int itters = 1000000;



   


////////////////////////////////////////random.seed(size)
//AJ3 currently working here

constexpr float bound = 1000.0;


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

duration = ((duration * 1000000) * 1000000) / size * itters;


double checksum = accumulate(xs.begin(),xs.end(),0) +
                  accumulate(ys.begin(),ys.end(),0) +
                  accumulate(zs.begin(),zs.end(),0);

std::cout<<"mean time per coordinate is: "<<duration<< "us"<<std::endl;
std::cout<<"Checksum: "<<checksum<<std::endl;

return 0;

    }
