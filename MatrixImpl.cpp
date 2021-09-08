#include<iostream>
#include <vector>
#include <bits/stdc++.h>
using namespace std;


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////



/**
 * 2D Matrix Implementation using CSR
 */
class CSRMatrix 
{  	

     private: 
		
        vector <float> A;
        vector <int> JA,IA;
        int nnz,row,col;
        int crow; // keeps the current row in append function
	
     public:
		
   
   
	/**
         * Default constructor is required.
	 * You may optionally define additional constructors as well.
     */
	CSRMatrix()
	{ 
		
        nnz = 0;// no of non zero elements
        row = 0;
        col = 0;
        A.resize(10);
        JA.resize(10);
        IA.resize(10);
        crow = 0;
        IA[0]=0;
	}
 

	void init (int N, int M)
 	{ 
		
        
        row = N;
        col = M;
        std::fill(A.begin(),A.end(),0);
        std::fill(JA.begin(),JA.end(),0);
        IA.resize(row+1,0);
        
	}
	

        void append (float* row_vals)
	{ 
	    
        int j;
        int count = 0; // stores no of nz elements in current row
        if ( crow == row ) throw std::invalid_argument("[append]:no. of rows exceeded");
        
        
        for(j=0;j<col;j++){
            if( row_vals[j] == 0) continue;
            if(A.capacity()==nnz){
                A.resize(2*nnz);
                JA.resize(2*nnz);
            }
            A[nnz]=row_vals[j];
            JA[nnz++] = j;
            count++;
        }    
        IA[crow+1]=IA[crow]+count;
        crow++;
	}

	
	
	float get(int i, int j)
	{ 
	  
        int r,k = IA[i+1]-IA[i];
        if (k==0) return 0; // no non-zero entry in row i
        else{
           for(r = 0;r < k;r++){
            if(JA[IA[i]+r]==j) return A[IA[i]+r]; // comparing column index; if a hit,then return element
          }
        }
        return 0; // entry not found in the array storing nz elements
            
	}

	/**
	  * This returns the number of rows in the matrix based on init()
	  */
	int row_count()
	{
		
        return row;
	}
	
	/**
	  * This returns the number of columns in the matrix based on init()
	  */
	int col_count() 
	{
		
        return col;
	}

	
	/**
	  * This returns the number of non zero values in the matrix
	  */
	int nnz_count()
	{
		

        return nnz;
       }
       
       int* row_offsets()
       {
           int *pos = IA.data();
           return pos;
       }
       
       int *col_indices()
       {
           int *pos = JA.data();
           return pos;
       }
       
       float* values()
       {
           float *pos = A.data();
           return pos;
       }

       
       

	
};
