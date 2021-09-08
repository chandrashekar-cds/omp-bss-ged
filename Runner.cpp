#include <iostream>
#include <string.h>
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>
#include <ctime>
#include <fstream>
#include <vector>
#include "MatrixImpl.cpp"

//using namespace std;

//double elapsed_secs;// = double(end - begin) / CLOCKS_PER_SEC;
/**
  * Runner code that serves as harness for invoking various functions required 
  * to be implemented for this assignment.
  * You may modify this code, but need to retain the input parameter signature and 
  * output requirements specified in the assignment.
  */  
  
/////////////////////////////////////////////////////////////////////////////

IMatrix* load_arr(char* input_file) 
{
	// TODO: Code for loading matrix from input_file into array impl and returning object
   char *file = input_file;
   ifstream infile(file);
   if(!infile) throw std::invalid_argument(" Can't load input file");
   
   
   int i,j;
   
   unsigned row,col=0;
   
    ArrayMatrix* mat = new ArrayMatrix();
   
    infile>>row;
    infile>>col;
    infile.get();
    
    float *rowval = new float[col];
     
    mat->ArrayMatrix::init(row,col);
    
    //read the values and fill it into a local float array to pass to append function
    for(i = 0; i < row; i++) {  // stop loops if nothing to read
        for(j = 0; j < col; j++){
           infile >> rowval[j];
           if ( ! infile ) {
              throw std::invalid_argument("Error reading file for element ");
           }
        }
        mat->append(rowval);
    }
   return mat;   
}

IMatrix* load_csr(char* input_file) 
{
	// TODO: Code for loading matrix from input_file into CSR impl and returning object
    
   char *file = input_file;
   ifstream infile(file);
    
   if(!infile) throw std::invalid_argument(" Can't load input file");

   
   int i,j;
   
   unsigned row,col=0;
   
   CSRMatrix* mat= new CSRMatrix();
   
   infile>>row;
   infile>>col;
   infile.get();
    
   float *rowval = new float[col];
   mat->CSRMatrix::init(row,col);
  
   //read the values and fill it into a local float array to pass to append function
   for(i = 0; i < row; i++) {  // stop loops if nothing to read
      for(j = 0; j < col; j++){
        infile >> rowval[j];
        if ( ! infile ) {
           throw std::invalid_argument("Error reading file for element ");
        }
      }
      mat->append(rowval);
   }
 /*// to check what values are filled in A[] vector
    cout<<"After append: "<<endl;
   for(i=0;i<mat->CSRMatrix::nnz_count();i++)
   {
       cout<<" "<<mat->CSRMatrix::get_nz(i)<<'\t';
   }*/
   return mat;
}

IMatrix* init_arr(int rows, int cols) 
{
	// TODO: Code for initializing an empty matrix using array impl with rows and cols as 
	// dimensions, and returning the object
    ArrayMatrix *mat = new ArrayMatrix();   
    mat->ArrayMatrix::init(rows,cols);    

    return mat;   
}


IMatrix* init_csr(int rows, int cols) 
{
	// TODO: Code for initializing an empty matrix using CSR impl with rows and cols as 
	// dimensions, and returning the object
    CSRMatrix *mat = new CSRMatrix();   
    mat->CSRMatrix::init(rows,cols);    

    return mat;
}


void print_mat(IMatrix* mat, char* output_file) 
{
	// TODO: print matrix as TSV to output_file
   ofstream fileout(output_file,ofstream::out);
   int i,j;
   for(i=0;i<mat->row_count();i++)
   {
      for(j=0;j<mat->col_count();j++)
      {
          
          fileout << mat->get(i,j);
          if(j<mat->col_count()-1)fileout<<'\t';
      }
      fileout<<'\n';
   }
    
}

/////////////////////////////////////////////////////////////////////////////

void load(char* mat_type, char* input_file, char* output_file)
{
	// TODO: any other validation?
	IMatrix* mat1;
    
    double elapsed_secs;
	if (strcmp("array", mat_type)==0) // DONE: time this region and print "load,array,output_file,time_millisec"
        { 
                clock_t begin = clock();  
		            mat1 = load_arr(input_file);
                clock_t end = clock();
                elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
                cout<<" load, array. Output file:"<<output_file<<'\t'<<"Time taken(ms) = "<<elapsed_secs*1000<<endl;
        }  
	else if (strcmp("csr", mat_type)==0) // DONE: time this region and print "load,csr,output_file,time_millisec"
	{           clock_t begin = clock();  
		            mat1 = load_csr(input_file);
                clock_t end = clock();
                elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
                cout<<" load, csr. Output file:"<<output_file<<'\t'<<"Time taken(ms) = "<<elapsed_secs*1000<<endl;
                
	}
	else
		cout<<"[load] invalid matrix type input seen: "<<mat_type<<endl;

	// store the loaded matrix mat1 in output file given by output_file
	print_mat(mat1, output_file);
	

	return;
}

/////////////////////////////////////////////////////////////////////////////
void Add(IMatrix* mat1, IMatrix* mat2, IMatrix* mat3) 
{	
	// TODO: Code for adding the mat1 and mat2 and storing in a third matrix mat3
	  int i,j,m=mat1->row_count(),n=mat1->col_count();
          float sum[n] = {0};
 
          for ( i=0;i<m;i++)
          {       
             for ( j=0;j<n;j++)
             {
                sum[j] = 0.0;
                sum[j] = mat1->get(i,j)+mat2->get(i,j);
             }
             mat3->append(sum);
          }
	return;
}

void add(char* mat_type, char* input_file1, char* input_file2, char* output_file)
{ 
	// TODO: any other validation?
	
	IMatrix *mat1, *mat2, *mat3;
	double elapsed_secs;
	if (strcmp("array", mat_type)==0) 
    { 
		mat1 = load_arr(input_file1);
		mat2 = load_arr(input_file2);
		
		// TODO: any other validation?
		if( mat1->row_count() != mat2->row_count() || mat1->col_count() != mat2->col_count())
                        throw std::invalid_argument("add_array: dimensions of matrices don't match");
                else 
		        mat3 = init_arr(mat1->row_count(), mat1->col_count());
    }  
	else if (strcmp("csr", mat_type)==0)
	{
		mat1 = load_csr(input_file1);
		mat2 = load_csr(input_file2);

		// TODO: any other validation?
		if( mat1->row_count() != mat2->row_count() || mat1->col_count() != mat2->col_count())
                        throw std::invalid_argument("add_csr: dimensions of matrices don't match");
                else
		        mat3 = init_csr(mat1->row_count(), mat1->col_count());
	}
	else {
		cout<<"[add] invalid matrix type input seen: "<<mat_type<<endl;
		return;
	}
	
	// TODO: time this method and print "add,mat_type,output_file,time_millisec"
	
	  clock_t begin = clock();  
	       Add(mat1, mat2, mat3);
      clock_t end = clock();
      elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
      cout<<"add:"<<mat_type<<"Output file:"<<output_file<<'\t'<<"Time taken(ms) = "<<elapsed_secs*1000<<endl;
	
        // store output matrix mat3 in file given by output_file
	print_mat(mat3, output_file);

	return;
}



/////////////////////////////////////////////////////////////////////////////
void Multiply(IMatrix* mat1, IMatrix* mat2, IMatrix* mat3) 
{	
	
	// TODO: Code for adding the mat1 and mat2 and storing in a third matrix mat3
	int i,j,k,m=mat1->row_count(),n=mat1->col_count(), p=mat2->col_count();
        float r,prod[p] = {0};
 
        for ( i=0;i<m;i++)
        {  
           for( k=0;k<p;k++)
           {    
             prod[k] = 0.0; 
             r = mat1->get(i,k);
             for ( j=0;j<n;j++)
             {
                prod[k] += r*mat2->get(k,j);
             }
           }
           mat3->append(prod);
        }
	
	return;
}


void multiply(char* mat_type, char* input_file1, char* input_file2, char* output_file)
{
	// TODO: any other validation?

	IMatrix *mat1, *mat2, *mat3;
    double elapsed_secs;
	if (strcmp("array", mat_type)==0) 
        { 
		mat1 = load_arr(input_file1);
		mat2 = load_arr(input_file2);

		// TODO: any other validation?
		
		// TODO: init output matrix mat3 with arr impl


               if( mat1->col_count() != mat2->row_count())
                        throw std::invalid_argument("multiply_array: no. of columns of first matrix doesn't match with no. of rows of second matrix.");
                else 
		        mat3 = init_arr(mat1->row_count(), mat2->col_count());

	}  
	else if (strcmp("csr", mat_type)==0)
	{
		mat1 = load_csr(input_file1);
		mat2 = load_csr(input_file2);

		// TODO: any other validation?
		
		// TODO: init output matrix mat3 with csr impl

                if( mat1->col_count() != mat2->row_count())
                        throw std::invalid_argument("multiply_csr: no. of columns of first matrix doesn't match with no. of rows of second matrix.");
                else 
		        mat3 = init_csr(mat1->row_count(), mat2->col_count());
	}
	else {
		cout<<"[multiply] invalid matrix type input seen: "<<mat_type<<endl;
		return;
	}
	
	// TODO: time this method and print "multiply,csr,output_file,time_millisec"
	    
	  clock_t begin = clock();  
	       Multiply(mat1, mat2, mat3);
      clock_t end = clock();
      elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
      cout<<"multiply:"<<mat_type<<"Output file:"<<output_file<<'\t'<<"Time taken(ms) = "<<elapsed_secs*1000<<endl;
      
 	// store output matrix mat3 in file given by output_file
	print_mat(mat3, output_file);

	return;
}


/////////////////////////////////////////////////////////////////////////////

void bfs(char* input_file, char* root_id, char* output_file)
{
 
	// TODO: any validation?
	
	//IMatrix* mat1;
	// TODO: Define a List ADT traverse_list to store output.

	// TODO
	//mat1 = load_csr(input_file);

	//{	
		// TODO: time this region and print "bfs,csr,output_file,time_millisec"
		// TODO: Code for doing BFS on the matrix starting with vertex present in row "root_id"
		// TODO: Add traversed items into traverse_list
	//}
	
	// TODO: store traversal output present in traverse_list into file given by output_file
	
	return;

}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

int main(int n, char *argv[])
{
 
	if(strcmp("load", argv[1])==0)
	{
		load(argv[2], argv[3], argv[4]);                // either array|csr format. save the loaded file to an output file
	}
    else if( strcmp("add", argv[1])==0)
	{
		add(argv[2], argv[3], argv[4], argv[5]);        // accepts files in either array|csr format. adds the matrices given in input files and writes output matrix to output file
	}
    else if( strcmp("multiply", argv[1])==0 )
	{
                multiply(argv[2], argv[3], argv[4], argv[5]);   // accepts files in either array|csr format. multiplies the matrices given in input files and writes output matrix to output file
	}
    else if(strcmp("bfs", argv[1])==0)
	{
                bfs(argv[2], argv[3], argv[4]);                 // takes in input graph as adjacency matrix in tsv format and saves the BFS traversed list of depth and vertices to the output file
	} else 
          {
		cout<<"[main] invalid input parameters. Valid usage is one of the following..."<<endl;
                cout<<"./Runner.o load [array|csr] <input_file> <output_file>"<<endl;
                cout<<"./Runner.o add [array|csr] <input_file_1> <inpout_file_2> <output_file>"<<endl;
                cout<<"./Runner.o multiply [array|csr] <input_file_1> <inpout_file_2> <output_file>"<<endl;
                cout<<"./Runner.o bfs <input_file> <root_row_id> <output_file>"<<endl;
          }
	return 0;
    
}

