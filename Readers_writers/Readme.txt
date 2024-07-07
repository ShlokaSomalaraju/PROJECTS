Solution yto the famous Readers-Writers Problem in Operating Systems.

Input file : (inp-params.txt)
	     nw, nr, kw, kr, muCS, muRem values each line by line in the input file.
	     
Output files: 
		Writers prefernce
		RW-log.txt (to store the timestamps of each thread at requesting, entry, exit 												sections)
		RW-Average.txt (to store the logs of average times of each thread)
		
		Fair Readers-Writers		
		FairRW-log.txt
		FairRW-Average.txt
		
Compilation command: (In linux)		
		g++ fair_writers.cpp
		g++ fair_rw.cpp
		
Execution command: ./a.out (unless object file is named)
