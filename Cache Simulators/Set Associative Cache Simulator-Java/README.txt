# Cache Simulation Program
This program simulates a cache memory system with a specified cache size, associativity, and block size. It reads a list of hexadecimal addresses from a file, converts them to decimal, and simulates their access to the cache. The program reports the total number of cache hits and misses, as well as the hits and misses for each set in the cache.

### Compilation:
First, compile the Java program using the following command:
javac main.java

### Execution:
java main <file_path> <cache_size> <associativity>

 `<file_path>`: Path to the input file containing hexadecimal addresses.
 `<cache_size>`: Size of the cache in kilobytes (KB).
 `<associativity>`: Number of ways (lines) per set in the cache (associativity).

## Input File Format
The input file should contain hexadecimal addresses, each separated by whitespace or a newline character. Each address should be in the format `0x...` (e.g., `0x1A2B3C4D`).

## Output
The program prints the following to the console:
- Total Misses: The total number of cache misses.
- Total Hits: The total number of cache hits.
- Set-wise Misses: The number of misses for each set in the cache.
- Set-wise Hits: The number of hits for each set in the cache.

