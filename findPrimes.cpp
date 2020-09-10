#include <iostream>
#include <chrono> // capture execution time
#include <vector>
#include <algorithm> //sort vector to print top 10 primes
#include <thread>
#include <atomic> // atomic counter
#include <ctime> // calculate CPU time
#include <mutex> // lock vector when storing primes
#include <math.h> // sqrt when finding prime number

int main()
{
    const unsigned int MAX = 100000000;
	
	std::mutex vectLock;
    std::vector<char> isPrime(MAX);
	std::vector<unsigned int> listOfPrimes;
    
	
    std::atomic_uint counter{0}; // atomic variable 
    std::vector<std::thread> threads; 
    
	unsigned int cpuStartTime = clock(); //capture cpu start time
	auto wallStartTime = std::chrono::high_resolution_clock::now(); //capture wall clock start time
 	
	
    for (unsigned int i = 0; i < 8; ++i) {
        threads.emplace_back(
            [&](){
                bool flag; // to determine prime number
                for (unsigned int num; (num = counter.fetch_add(1)) < MAX; ) { 
                    flag = true;
				// skip 0 and 1				
				if(num == 0 || num == 1 || ((num % 2 == 0) && (num > 2))){
					continue;	
				}
				
				
                    for (unsigned int k = 2; k <= sqrt(num); ++k) {
						if(num == 2){
							break;					
						}
                        if (num % k == 0) {
                            flag = false;
                            break;
                        }
                    }
                    isPrime[num] = flag;
                    
					if(flag == true){
						 vectLock.lock();
                		listOfPrimes.push_back(num);
                		vectLock.unlock();
					}
                }
            }
        );
    }
	
    for (auto & t : threads) {
        t.join();
    }
	unsigned int cpuEndTime = clock(); //capture cpu end time
	auto wallEndTime = std::chrono::high_resolution_clock::now(); //capture wall clock end time

	//print CPU time
	std::cout << "CPU Time: " << 
            (cpuEndTime - cpuStartTime)/double(CLOCKS_PER_SEC) << "\n" << std::endl;

	// print wall clock time
	auto wallClock = std::chrono::duration_cast<std::chrono::seconds>( wallEndTime - wallStartTime ).count();
	std::cout << "Wall clock Time: "<< wallClock << "\n" << std::endl;

	//print total number of primes
	unsigned int totalPrimes = 0;
	for (int i = 0; i < isPrime.size(); i++){
		if(isPrime[i] == true){
			totalPrimes += 1;		
		}
	}
    std::cout << "number of primes found: " << totalPrimes << "\n" << std::endl;
	
	// sum of all the primes in vector	
	unsigned int sumOfPrimes = 0;
	for (int i = 0; i < listOfPrimes.size(); i++)
    sumOfPrimes += listOfPrimes[i];
	std::cout << "Sum of Prime numbers : " << sumOfPrimes << "\n" << std::endl;

	//sort vector and print top 10 max primes
	std::cout << "Top 10 prime numbers:" << "\n" << std::endl;
	sort(listOfPrimes.begin(), listOfPrimes.end());
	for (unsigned int i = listOfPrimes.size()-10; i < listOfPrimes.size(); i++) {
        std::cout << listOfPrimes[i] << "\t";
        }
    std::cout << "\n";

    return 0;
}


