#include "BSED.h"
#include "omp.h"
int main(int argc, char *argv[])
{
	string db ; 
	int totalGraph =  1;
	string query ;
	int totalQuery = 1;
	int ub = -1;
	int width = 10;
    int thread = 1;
	const int sorted = 0;
    h = 1;
    

	int cmd_flag;
    while ((cmd_flag = getopt(argc, argv, "i:n:q:m:b:w:t:h:")) != -1) {
        switch (cmd_flag) {
            case 'i':
                db.assign(optarg);
                break;
			case 'n':
                totalGraph = std::stoi(std::string(optarg));
                break;
            case 'q':
                query.assign(optarg);
                break;
			case 'm':
                totalQuery = std::stoi(std::string(optarg));
                break;
            case 'b':
                ub = std::stoi(std::string(optarg));
                break;
            case 'w':
                width = std::stoi(std::string(optarg));
                break;
			case 't':
                thread = std::stoi(std::string(optarg));
                break;
			case 'h':
                h = std::stoi(std::string(optarg));
                break;
            default:
                std::cerr << "main() : Invalid command line argument"
                          << std::endl;
                exit(1);
        }
    }
    
	  
    omp_set_num_threads(thread); 
    MAX_THREADS = thread;
    cout<<"NO of THREADS  =  "<<omp_get_num_threads()<<endl;        
	const char *db_out = sorted ? "./ged_db" : db.c_str(); 
	const char *query_out = sorted ? "./ged_query" : query.c_str();


	if(sorted){
			graph::reOrderGraphs(db.c_str(), db_out, totalGraph);
			graph::reOrderGraphs(query.c_str(), query_out, totalQuery);
			cout << "preprocessing graph order done" << endl;
	}
	
	vector<graph> queryDB = graph::readGraphMemory(query_out, totalQuery);
	vector<graph> graphDB = graph::readGraphMemory(db_out, totalGraph);
	if(sorted){ std::remove(db_out); std::remove(query_out);}

	//graph::print_mat(queryDB[0],"first_query.txt");
	//graph::print_mat(graphDB[2],"third graph.txt");

	int i = 0, j = 0, bound;	
	int sum = 0; 
	struct timeval start,end; 
	float timeuse; 
	gettimeofday(&start, NULL); 

	i = 0;
	for(; i < graphDB.size();i++)
	{
		graph g = graphDB[i];
		for(j = 0; j < queryDB.size(); j++)
		{
			graph q = queryDB[j]; 
			if(ub == -1)  bound = max(g.v, q.v) + g.e + q.e;
			else bound = ub;
			BSEditDistance ed(width);
			int ged = ed.getEditDistance(q, g, bound);
			if(ged > -1) 
			{
				cout << "GED(" << i << "," << j << ")" << " = " << ged << endl;
                                cout<<"Graph size: "<<g.v<<endl; 
				sum++;
			}
		}
		FLAG = true;
	}
	cout << "total expand node " << totalExpandNode << endl;
	cout << "total no of entries used by maps " << memfootprint << endl;
	cout << "initFilter and total results " << initFilter  << " " << sum << endl;
	gettimeofday(&end, NULL); 
	timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec; 
	timeuse = timeuse * 1.0 / 1000000; 
        if(h == 1)
		cout << "Total graph edit distance computation time(no heuristic) " << timeuse << endl;
        else
        	cout << "Total graph edit distance computation time(with heuristic) " << timeuse << endl;

	return 0;
}
