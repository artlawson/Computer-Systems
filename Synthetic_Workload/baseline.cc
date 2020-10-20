//baseline

#include "benchmarks.cc" 
#include <fstream>
#include <iostream>
#include <vector>



std::vector<double> baseline_latencies(int nreq){
	Cache* cache = new Cache("127.0.0.1","6969");
    warm_cache(nreq, cache);
    std::vector<double> output_vec;
    //time for part 3
    auto t1 = std::chrono::high_resolution_clock::now();
    for (int j = 0; j<nreq; j++){

    //get random request
    double work = generate(1, cache); //genorate takes n_req, which would be one at a time 
    output_vec.push_back(work);
    
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    double duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
    duration*=pow(10,-6); //converts to seconds
    std::cout<<duration<<"<- duration"<<std::endl;

    delete cache;
    return output_vec;
}

std::pair<double,double> baseline_performance(int num){

    //if we change the vector type for baseline_lat... make sure to change this one too :)
    std::vector<double> vec = baseline_latencies(num);
    sort(vec.begin(),vec.end());
    double total;
    for (int i =0; i < num; i++){total+=vec[i];} 
    double req_per = (total * 0.001) / num; //average of values in vector

    //perc is the 95th percentile barrier in the vector
    int perc = num * 0.95;
    double ret = vec[perc]; //ret is the 95th percentile of vec

    std::pair<double, double> output;
    output.first = ret;
    output.second = req_per;
    
    return output;

}

int main() {

	auto output = baseline_performance(5000);
	std::cout<<output.first<<"<- 95th percentile;9"<< output.second<<"<-req/per     l"<<std::endl;
	
	return 1;
}