# bss-ged-gpu
BSS_GED with GPU execution enabled ( Due credits to the original authors of BSS_GED; its code can be accessed @ https://github.com/Hongweihuo-Lab/BSS-GED/ )

BSS_GED is a vertex-mapping based method for graph edit distance (GED) computation. It employs the beam-stack
search paradigm, combined with two specially designed heuristics to improve the GED computation, achieving a trade-off between memory utilization and expensive backtracking calls. Also, a novel strategy is proposed to identify invalid and redundant mappings, leading to a reduced search space. 

## How to use it?   
### Step I. Install
   1. download (or clone) the source code from https://github.com/chandrashekar-cds/omp-bss-ged.git
   2. Compile the source code using make. 
### Step II. run omp-bss-ged
   1. run the shell command: "./ged -i graph200v.txt -n 1 -q test200v.txt -m 1 -b -1 -w 10 -t 1 -h 2", where    
       (1) -i input graphs file.       
       (2) -n is the number of graphs in the database.        
       (3) -q is the set of query graphs file. 
       (4) -m is the number of graphs in the query.     
       (5) -b is the GED threshold. Specifically, -1 means that we perform the exact 
           GED computation; and otherwise, we perform the threshold-based graph similarity search.        
       (6) -w is the beam width.      
       (7) -t is the no of threads
       (8) -h is the heuristic value     
      
---------------------------------------------------------------------------------------------------------------------------
Test-case

./ged -i graph200v.txt -n 1 -q test200v.txt -m 1 -b -1 -w 10 -t 1 -h 2

---------------------------------------------------------------------------------------------------------------------------
