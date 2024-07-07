#include <iostream>
#include <thread>
#include <fstream>
#include <chrono>
#include <random>
#include <string>
#include <semaphore.h>
#include <iomanip> // For std::put_time
#include <sstream>
#include <vector> // For storing durations
 
using namespace std;

int nw, nr, kr, kw;
double muCS, muRem;

sem_t lock1; // to ensure consistency while updating writers count
sem_t lock2; // to ensure consistency while updating readers count
sem_t writerPrefer; // to ensure writer priority, allowing writers to access the critical section when no readers are present
sem_t readerhandle; // to manage reader access to the critical section
sem_t file;		// to synchronize log file writing operations
sem_t timeFile;  // to synchronize Average time file writing operations

// To store the average time taken by each writer and reader thread
vector <long long> reader_threads;
vector <long long> writer_threads;

int readers = 0;
int writers = 0;

ofstream output_file("RW-log.txt"); 
ofstream output_average("RW-Average.txt");

default_random_engine generator(chrono::system_clock::now().time_since_epoch().count()); // Global random number generator

// Function To extract present time
chrono::time_point<chrono::system_clock> getSysTime() {
    return chrono::system_clock::now();
}


// Function to format time as string including microseconds
std::string format_time(const std::chrono::time_point<std::chrono::system_clock>& time_point) {
    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(time_point.time_since_epoch()) % 1000000;
    auto time = std::chrono::system_clock::to_time_t(time_point);
    std::tm tm = *std::localtime(&time);
    std::stringstream ss;
    ss << std::put_time(&tm, "%H:%M:%S") << "." << std::setw(6) << std::setfill('0') << microseconds.count();
    return ss.str();
}

// Function on which every writer thread executes
void writer(int threadNumber, exponential_distribution<double>& expDistCS, exponential_distribution<double>& expDistRem){
    
    thread::id id = this_thread::get_id();
    long long sum_of_times = 0;

    for( int i=0; i<kw; i++){
        
        
        auto reqTime = getSysTime();
        sem_wait(&file);
        output_file << i+1 <<"th CS request by Writer Thread "<< id <<" at "<<format_time(reqTime)<<endl ;
        sem_post(&file);

        sem_wait(&lock1);
        writers++;
        if(writers == 1){
            sem_wait(&readerhandle);
        }
        sem_post(&lock1);
        
        sem_wait(&writerPrefer);


        /*CS*/
        sem_wait(&file);
        auto enterTime = getSysTime();
        output_file << i+1 <<"th CS Entry by Writer Thread "<< id <<" at "<< (format_time(enterTime)) << endl ;
        sem_post(&file);
        
        sum_of_times += (chrono::duration_cast<chrono::microseconds>(enterTime - reqTime)).count();
        
        double randCSTime = expDistCS(generator);
        this_thread::sleep_for(chrono::milliseconds(static_cast<long long>(randCSTime)));
        /*CS*/

        sem_post(&writerPrefer);
        
        sem_wait(&lock1);
        writers--;
        if(writers == 0){
            sem_post(&readerhandle);
        }
        sem_post(&lock1);


        /*Remainder Section*/
        
        auto exitTime = getSysTime();
        sem_wait(&file);
        output_file << i+1 <<"th CS Exit by Writer Thread "<< id <<" at "<< (format_time(exitTime)) << endl ;
        sem_post(&file);
        
        double randRemTime = expDistRem(generator);
        this_thread::sleep_for(chrono::milliseconds(static_cast<long long>(randRemTime)));
        /*Rem*/

    }
    
    sum_of_times/=kw;
    sem_wait(&timeFile);
    writer_threads.push_back(sum_of_times);
    output_average <<"Average time took by writer thread "<< id << " is " << sum_of_times <<" microseconds."<< endl;
    sem_post(&timeFile);
       
    return;
}


// Function on which every reader thread executes
void reader(int threadNumber, exponential_distribution<double>& expDistCS, exponential_distribution<double>& expDistRem){

    thread::id id = this_thread::get_id();
    long long sum_of_times = 0;

    for( int i=0; i<kr; i++ ){
         
        /*Entry Section*/     
        auto reqTime = getSysTime();
        sem_wait(&file);
        output_file << i+1 <<"th CS request by Reader Thread "<< id <<" at "<< (format_time(reqTime)) << endl;
        sem_post(&file);

        sem_wait(&readerhandle);
        sem_wait(&lock2);
        readers++;
        if(readers == 1){
            sem_wait(&writerPrefer);
        }       
        sem_post(&lock2);
        sem_post(&readerhandle);
		

        /*Critical Section*/
        sem_wait(&file);
        auto enterTime = getSysTime();
        output_file << i+1 <<"th CS Entry by Reader Thread "<< id <<" at "<< (format_time(enterTime)) << endl;
        sem_post(&file);
        
        sum_of_times += (chrono::duration_cast<chrono::microseconds>(enterTime - reqTime)).count();
        
        double randCSTime = expDistCS(generator);
        this_thread::sleep_for(chrono::milliseconds(static_cast<long long>(randCSTime)));
        /*CS*/

		// Exit Section
        sem_wait(&lock2);
        readers--;
        if(readers==0){
            sem_post(&writerPrefer);
        }
        sem_post(&lock2);


        /*Remainder Section*/
                
        auto exitTime = getSysTime();
        sem_wait(&file);
        output_file << i+1 <<"th CS Exit by Reader Thread "<< id <<" at "<< (format_time(exitTime)) << endl ;
        sem_post(&file);
        
        double randRemTime = expDistRem(generator);
        this_thread::sleep_for(chrono::milliseconds(static_cast<long long>(randRemTime)));
        
        /*Rem*/

    }
    
    sum_of_times/=kr;
    sem_wait(&timeFile);
    reader_threads.push_back(sum_of_times);
    output_average <<"Average time took by reader thread "<< id << " is " << sum_of_times <<" microseconds"<< endl;
    sem_post(&timeFile);
    return;
}

int main(){

    // Open the input file
    ifstream input_file("inp-params.txt");

    string line;
    getline(input_file, line);
    nw = stoi(line);
    cout << nw << endl;

    getline(input_file, line);
    nr = stoi(line);
    cout << nr << endl;

    getline(input_file, line);
    kw = stoi(line);
    cout << kw << endl;

    getline(input_file, line);
    kr = stoi(line);
    cout << kr << endl;

    getline(input_file, line);
    muCS = stod(line);
    cout << muCS << endl;

    getline(input_file, line);
    muRem = stod(line);
    cout << muRem << endl;

    input_file.close();
    // Closed the input file


    // Intialising all the locks
    sem_init(&lock1, 0, 1);
    sem_init(&lock2, 0, 1);
    sem_init(&writerPrefer, 0, 1);
    sem_init(&readerhandle, 0, 1);
    sem_init(&file, 0, 1);
    sem_init(&timeFile,0,1);
    

    // Intilaising the exponential distributions
    exponential_distribution<double> expDistCS(1.0 / muCS); 
    exponential_distribution<double> expDistRem(1.0 / muRem);    
    
    thread writerThread[nw];
    for(int i = 0; i<nw; i++){
        writerThread[i] = std::thread(writer, i, ref(expDistCS), ref(expDistRem));
    }

    thread readerThread[nr];
    for(int i = 0; i<nr; i++){
        readerThread[i] = std::thread(reader, i, ref(expDistCS), ref(expDistRem));
    }


    // Joining all the reader and writer threads

    for(int i = 0; i<nr; i++){
        readerThread[i].join();
    }
    for(int i = 0; i<nw; i++){
        writerThread[i].join();
    }

    output_file.close();
    output_average.close();
    
    long long writers_average = 0;
    long long readers_average = 0;
    long long writers_worst = 0;
    long long readers_worst = 0;
    
    for(int i=0 ; i<nw; i++){
    	if( writer_threads[i] > writers_worst ){
    		writers_worst = writer_threads[i];
    	}
    	writers_average += writer_threads[i]; 
    }
    
    for(int i=0 ; i<nr; i++){
    	if( reader_threads[i] > readers_worst ){
    		readers_worst = reader_threads[i];
    	}
    	readers_average += reader_threads[i]; 
    }
    
    cout << "Average time took by a writer thread is " << (writers_average/nw) << " microseconds." << endl;
    cout << "Worst time took by a writer thread is " << writers_worst << " microseconds." << endl;
    cout << "Average time took by a reader thread is " << (readers_average/nr) << " microseconds." << endl;
    cout << "Worst time took by a readter thread is " << readers_worst << " microseconds." << endl;
    

    sem_destroy(&lock1);
    sem_destroy(&lock2);
    sem_destroy(&writerPrefer);
    sem_destroy(&readerhandle);
    sem_destroy(&file);
    sem_destroy(&timeFile);

    //#pragma GCC diagnostic pop

    return 0;
}

