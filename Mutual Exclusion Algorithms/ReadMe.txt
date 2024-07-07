Input file: inp.txt

			N K rowInc
			A11 A12 A13 ... A1N
			A21 A22 ....... A2N
			.
			.
			AN1 AN2 ....... ANN

			The values N,K,rowInc should be seperated by a space and the entries of the matrix A should be given from a new line as shown above.

Output file : out_TAS.txt
			  out_CAS.txt
			  out_Bounded_CAS.txt
			  out_Atomic.txt


Compilation command : g++ -std=c++11 Assgn3_TAS_Src-CS22BTECH11056.cpp -pthread
					  g++ -std=c++11 Assgn3_CAS_Src-CS22BTECH11056.cpp -pthread
					  g++ -std=c++11 Assgn3_Bounded_CAS_Src-CS22BTECH11056.cpp -pthread
					  g++ -std=c++11 Assgn3_Atomic_Src-CS22BTECH11056.cpp -pthread


Execution command : For all the source code files it is ./a.out 