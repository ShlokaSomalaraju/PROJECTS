#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unistd.h>
#include <pthread.h>
#include <atomic>
#include <chrono>

using namespace std;

int N; //Size of Array
int K; //No.of threads
int rowInc;
std::atomic<int> counter(0);

struct threadArgs{
	int threadNumber;
	int** inputArray;
	int** outputArray;
};

int dotProduct(int** inputArray,int row,int column,int size){
	int result=0;

	for(int i=0; i<size; i++){
		result+= (inputArray[row][i]) * (inputArray[i][column]);
	}

	//cout << result << " ";
	return result;
}

void* threadFunc(void* arg){

	threadArgs* args = static_cast<threadArgs*>(arg);

	int threadNumber = args->threadNumber;
	int** inputArray = args->inputArray;
	int** outputArray = args->outputArray;
	int temp;

	while(true){
		
		// Critical section

		int temporaryCount = counter.fetch_add(rowInc,std::memory_order_relaxed);
		if(temporaryCount >= N){
			break;
		}
		else{
			temp = temporaryCount;
		}

		// Critical section until here
		
		/*remainder section*/

		if( (temp + rowInc) < N){
			for(int i = temp; i < (temp+rowInc); i++){
				for(int j=0; j<N; j++){
					outputArray[i][j]=dotProduct(inputArray,i,j,N);
				}
			}
		}
		else{
			for(int i = temp; i < N; i++){
				for(int j=0; j<N; j++){
					outputArray[i][j]=dotProduct(inputArray,i,j,N);
				}
			}
		}

	}


	return NULL;
}

int main(){

	// Recording the start time
	chrono::high_resolution_clock::time_point startTime = chrono::high_resolution_clock::now();

	ifstream inputFile;
	inputFile.open("inp.txt");

	if(inputFile.is_open()==0){
		cerr << "Error in opening the input file" << endl;
		return 1;
	}	

	string line;
	getline(inputFile, line);
	
	istringstream iss(line);
	iss >> N >> K >> rowInc;

	//cout << rowInc << endl;

	int** inputArray = new int*[N];
    for(int i=0; i<N; i++){
        inputArray[i] = new int[N];
    }

	//Storing the values of the input array
	for(int i=0; getline(inputFile,line); i++){
		istringstream iss(line);
		int number;
		for(int j=0;iss >> number;j++){
			inputArray[i][j]=number;
		}
	}

	inputFile.close();

	/*for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			cout << inputArray[i][j] << " ";
		}
		cout << endl;
	}*/

	//Thread declaration
	pthread_t thread[K];
	threadArgs args[K];


	int** outputArray = new int*[N];
	for(int i=0; i<N; i++){
        outputArray[i] = new int[N];
    }

	for(int i=0; i<K; i++){
		args[i].threadNumber = i;
        args[i].inputArray = inputArray;
        args[i].outputArray = outputArray;
		pthread_create(&thread[i],nullptr,threadFunc,&args[i]);

	}

	for(int i=0; i<K; i++){
        pthread_join(thread[i],nullptr);
    }

    // Writing the output array into out.txt file
    ofstream outputFile("out_Atomic.txt");

    if(outputFile.is_open()){
    	for(int i=0; i<N; i++){
    		for(int j=0; j<N; j++){
    			outputFile << outputArray[i][j] << " ";
    		}
    		outputFile << endl;
    	}
    }

    // Recording the End time
    chrono::high_resolution_clock::time_point endTime = chrono::high_resolution_clock::now();

    chrono::microseconds time = chrono::duration_cast<chrono::microseconds>(endTime - startTime);

    outputFile << "Time taken is: " << time.count() / 1e6 << " seconds.";
    
    //atomic_flag_destroy(&lock);

    outputFile.close();

	// Deleting inputArray
	for(int i=0; i<N; i++){
    	delete[] inputArray[i];
	}
	delete[] inputArray;

	// Deleting outputArray
	for(int i=0; i<N; i++){
    	delete[] outputArray[i];
	}
	delete[] outputArray;

	return 0;

}