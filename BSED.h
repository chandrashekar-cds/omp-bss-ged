#ifndef _BSED_H
#define _BSED_H
#include "stdafx.h"
#include "treeNode.h"
#include <omp.h>

typedef treeNode * PNode;
struct cmpPNode
{
	bool operator() (const PNode& lhs, const PNode& rhs) const
	{
		return  *rhs < *lhs;
	}
};

typedef priority_queue<PNode, vector<PNode >, cmpPNode > PQueue; // have a look at PQ definition of C++
//define a data structure here
typedef std::vector<PNode > TNV; 
extern PNode succ[20000]; // do I need 20k?? each entry is a pointer to a treenode
static int initFilter = 0;

//typedef 
struct beamItem
{
	PNode left, right;
	int lower, upper;
	beamItem(int l, int u, PNode lf, PNode rg) : lower(l), upper(u), left(lf), right(rg){}
	beamItem()
	{
		left = right = NULL;
		lower = -1, upper = 0xffff;
	}
};

class BSEditDistance
{
public:
	//vector<PQueue> closed; // closed a vector of priority queues - size no of layers
    //use another definition according to some other data structure
	vector<TNV > closed; 
	stack<beamItem>  bs;
	int w;
	int ged;
public:
	BSEditDistance(int width) { w = width; ged = 0xFFFF; }
	BSEditDistance(){ w = 50; ged = 0xFFFF; }
	~BSEditDistance()
	{

	}
private:
	inline void swap(PNode &a, PNode &b)
	{
		PNode c = a;
		a = b;
		b = c;
	}
	inline int median5(PNode *a)
	{
		PNode a0 = a[0];
		PNode a1 = a[1];
		PNode a2 = a[2];
		PNode a3 = a[3];
		PNode a4 = a[4];
		if (*a1 < *a0)
			swap(a0, a1);
		if (*a2 < *a0)
			swap(a0, a2);
		if (*a3 < *a0)
			swap(a0, a3);
		if (*a4 < *a0)
			swap(a0, a4);

		if (*a2 < *a1)
			swap(a1, a2);
		if (*a3 < *a1)
			swap(a1, a3);
		if (*a4 < *a1)
			swap(a1, a4);

		if (*a3 < *a2)
			swap(a2, a3);
		if (*a4 < *a2)
			swap(a2, a4);

		if (a2 == a[0]) return 0;
		if (a2 == a[1]) return 1;
		if (a2 == a[2]) return 2;
		if (a2 == a[3]) return 3;
		return 4;
	}
	inline int partition(PNode *a, int size, int pivot)
	{
		PNode pivotValue = a[pivot];
		swap(a[pivot], a[size - 1]);
		int storePos = 0;
		for (int loadPos = 0; loadPos < size - 1; loadPos++)
		{
			if (*a[loadPos] < *pivotValue)
			{
				swap(a[loadPos], a[storePos]);
				storePos++;
			}
		}
		swap(a[storePos], a[size - 1]);
		return storePos;
	}

	void freeNode(PNode &root)
	{
		if (root == 0) return;
		int size = root->childs.size();
		for (int i = 0; i < size; i++)
			freeNode(root->childs[i]);
		if (root)  delete root; root = 0;
	}

public:
	void select(PNode *a, int size, int k)
	{
		if (size < 5)
		{
			for (int i = 0; i<size; i++)
				for (int j = i + 1; j<size; j++)
					if (*a[j] < *a[i])
						swap(a[i], a[j]);
			return;
		}
		int groupNum = 0;
		PNode *group = &a[0];
		for (; groupNum * 5 <= size - 5; group += 5, groupNum++)
		{
			swap(group[median5(group)], a[groupNum]);
		}
		int numMedians = size / 5;
		int MOMIdx = numMedians / 2;
		select(a, numMedians, MOMIdx);
		int newMOMIdx = partition(a, size, MOMIdx);
		if (k != newMOMIdx)
		{
			if (k < newMOMIdx)
			{
				select(a, newMOMIdx, k);
			}
			else /* if (k > newMOMIdx) */
			{
				select(a + newMOMIdx + 1, size - newMOMIdx - 1, k - newMOMIdx - 1);
			}
		}
	}
	//here: we need to compute the w in O(n) time
	//first, determine the value of w
	int determine_width(PNode succ[], int &succ_size)
	{
		assert(succ_size != 0);

		int min_cost = succ[0]->ECost + succ[0]->deep;
		int count = 1, tmp;
		for (int i = 1; i < succ_size; i++)
		{
			int tmp = succ[i]->ECost + succ[i]->deep;
			if (tmp == min_cost){ count++; continue; }
			if (tmp < min_cost)
			{
				count = 1;
				min_cost = tmp;
			}
		}
		return count;
	}

    // modify this function
	void pruneLayer(PNode succ[], TNV &pq, int &succ_size)
	{
		if (!succ_size) return;
		int i = 0, size = succ_size;
		//int width = determine_width(succ, succ_size);
		//int width = 500000;
		//width = min(w, width);
		int width = w;
		if (size <= width)
		{
			for (; i < size; i++)
			{
				pq.push_back(succ[i]);
			}
			return;
		}
		
		select(&succ[0], size, width);
		for (; i < width; i++)
		{
			pq.push_back(succ[i]);
		}
		int minValue = succ[width]->deep + succ[width]->ECost;
		bs.top().upper = minValue;
		bs.top().right = succ[width]; //convert [0, u)->[L, u)
	}


	void expandSuccNode(TNV &node, PNode succ[], int &succ_size)
	{
		int size = node.size();
		for (int i = 0; i < size; i++)
		{
			if (!node[i]) continue;
			if (node[i]->visited == true)
			{
				freeNode(node[i]);
				continue;
			}
			int cost = node[i]->deep + node[i]->ECost;
			if (cost >= ged)
			{
				freeNode(node[i]);
				continue;
			}
			if (cost >= bs.top().lower && cost < bs.top().upper)
			{
				succ[succ_size++] = node[i];
			}
		}
	}

	void BeamSearchOnce(int &l, int &bound, vector<int> &group_1, vector<int> &group_2)
	{// inputs l = yet to know, bound is clear.. group1 & group2 -- vector of vertices with their classes
		TNV PQL = closed[l], PQLL, nullPQL;
		// change definitions here
		PNode node; int size;
		//double elapsed_secs=0;
		int getout = 0;
		cout<<"Inside BeamSearchOnce.."<<endl;
    //#pragma omp parallel 
	//change the while loop - conditions etc
		while ((!PQL.empty() || !PQLL.empty()) && !getout)
		{
			cout<<"Hi"<<endl;
			size = 0;
			int s = PQL.size();
			cout<<"\n No of nodes to be processed = "<<s<<endl;
            omp_set_num_threads(min(s,MAX_THREADS));
			/*
			int size1 = 0, max1 = 0;     
			memset(tmpDegree1, 0, max_d_1 * sizeof(u16));
		 	memset(tmpDegree2, 0, max_d_2 * sizeof(u16));
			for (int i = 0; i < closed[0][0]->uG1.gs; i++)
			{
				if (max1 < succ_degree_1[i])
					max1 = succ_degree_1[i];
				tmpDegree1[succ_degree_1[i]]++;
			}
			for (int i = max1; i >= 0; i--)
			{
				int len = tmpDegree1[i]; //chongdu 
				for (int l = 0; l < len; l++)
					ds1[size1++] = i;
			}
			cout<<"\nContents of ds1 - "<<endl;
			//int s = *(&ds1 + 1) - ds1;
			for(int k=0;k<size1;k++)
				cout<<" "<<ds1[k];
			ds1_size = size1;*/    
		#pragma omp parallel for private(node,ds1,ds2,ds1_size)
			//for (int i = 0;!PQL.empty() && !getout;i++)
			for (int i = 0;i<s;i++)
			{
				//node = PQL.top(); PQL.pop();
				//node = PQL.back(); PQL.pop_back();
                cout<<"Inside for.. thread   "<<omp_get_thread_num()<<endl; 
				node = PQL[i];
				
			int size1 = 0, max1 = 0;     
			memset(tmpDegree1, 0, max_d_1 * sizeof(u16));
		 	memset(tmpDegree2, 0, max_d_2 * sizeof(u16));
			for (int i = 0; i < node->uG1.gs; i++)
			{
				if (max1 < succ_degree_1[i])
					max1 = succ_degree_1[i];
				tmpDegree1[succ_degree_1[i]]++;
			}
			for (int i = max1; i >= 0; i--)
			{
				int len = tmpDegree1[i]; //chongdu 
				for (int l = 0; l < len; l++)
					ds1[size1++] = i;
			}
			cout<<"\nContents of ds1 - "<<endl;
			//int s = *(&ds1 + 1) - ds1;
			for(int k=0;k<size1;k++)
				cout<<" "<<ds1[k];
			ds1_size = size1;
				if (node->allverifyGraphNodesUsed())
				{
					//assert(node->deep < bound);
					bound = node->deep;
					//return;
					getout = 1;
				}
				//#pragma omp critical
				if (!node->visited && !getout)
				{
					clock_t begin = clock();
                    	node->generateSuccessors(bound, group_1, group_2);       //imp 
            		clock_t end = clock();
            		double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
            		cout<<" generateSuccessors:"<<'\t'<<"Time taken(ms) = "<<elapsed_secs*1000<<endl;
					lvlcounter++;
					node->visited = true;
					#pragma omp critical
           					totalExpandNode += node->childs.size();
	                    				
	                /*
	                cout<<"###################################################################### "<<endl; 
        	        cout<<"*****************Contents of nodes at level "<<lvlcounter<<"***********************"<<endl;	
		        	cout<<"###################################################################### "<<endl;
					for(auto i = 0;i < node->childs.size(); i++)
					{
						
						//Dictmapu16 myMap = node->childs[i]->matching;
						//cout<<"Contents of matching map"<<endl;
						//printmap(myMap);
						//myMap = node->childs[i]->inverseMatching;
						//cout<<"Contents of inverse matching map"<<endl;
						//printmap(myMap);
						Dictmap mymap = node->childs[i]->group;
						cout<<"Contents of group map"<<endl;
						printmap(mymap);
						//mymap = node->childs[i]->cost;
						//cout<<"Contents of cost map"<<endl;
						//printmap(mymap);
						cout<<"---------------------------------------------------------------------- "<<endl;

					}
					cout<<"---------------------------------------------------------------------- "<<endl;
					//cout<<" totalExpandNode - "<<totalExpandNode<<endl;*/
					
				}
				//#pragma omp critical
					expandSuccNode(node->childs, succ, size);                   // imp
		   	} // end of for loop
			
			// need to modify this section too.. first need is to get it running for sequential
				cout<<"Number of children spawned = "<<size<<endl;
				pruneLayer(succ, PQLL, size);
			PQL = PQLL; PQLL = nullPQL;
			if (PQL.empty() && PQLL.empty())
			{ 
				cout<<"Both queues empty.."<<endl;
				return;
			}
				
			if(!getout)
			{
				l = l + 1;
				closed[l] = PQL;
				bs.push(beamItem(-1, bound, NULL, NULL));
			}
		}
     return;
	}

	int getEditDistance(graph &ga, graph &gb, int bound, vector<int> &group_1, vector<int> &group_2)

	{
		int T = bound + 1, ged = bound + 1;
		int count = 0, l = 0;
		double elapsed_secs;
		treeNode *start = new treeNode();
		start->init(ga, gb);            // treeNode initialized with graphs
		int tmp_wdith = w;
		bool flg = false, flag = false;

		TNV startPQL;
		closed.resize(max(ga.v, gb.v) + 2, startPQL);
		bs.push(beamItem(-1, ged, NULL, NULL));

		startPQL.push_back(start);
		closed[0] = startPQL;  // initializing a vector of priority queue entries with startPQL

        cout<<"No of nodes = "<<start->uG1.gs;
		

		while (!(bs.empty() || flg))
		{
			//if (!flag) { w = 500; flag = !flag; }
			{
			clock_t begin = clock();
                    BeamSearchOnce(l, ged, group_1, group_2);
            clock_t end = clock();
            elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
            //cout<<" BeamSearchOnce:"<<'\t'<<"Time taken(ms) = "<<elapsed_secs*1000<<endl;
		    }
			while (!bs.empty() && bs.top().upper >= ged)
			{
				bs.pop();
				l--;
			}
			if (bs.empty())
			{
				/*if (start) freeNode(start);
				if (T == ged) return -1;
				else return ged;*/
				//#pragma omp atomic update
					flg = true;
				continue;
			}
			bs.top().lower = bs.top().upper; bs.top().upper = ged; //
			bs.top().left = bs.top().right; bs.top().right = NULL;
			count++;
#if 0
			if (flag)
			{
				w = tmp_wdith;
			}
#endif
		} // end of WHILE
	   
		if (start) freeNode(start);
		cout<<"T = "<<T<<"GED = "<<ged<<endl;
		if (T == ged) return -1;
		else return ged;
	}
public:
	inline int getEditDistance(graph &g1, graph &g2, int bound)
	{

		u16 lv_1[256], lv_2[256], le_1[64], le_2[64];
		double elapsed_secs;
		memset(lv_1, 0, 256 * sizeof(u16)); memset(lv_2, 0, 256 * sizeof(u16));
		memset(le_1, 0, 64 * sizeof(u16)); memset(le_2, 0, 64 * sizeof(u16));
		cout<<"Hi..inside get-edit-dist "<<endl;
		max_v_1 = max_v_2 = max_e_1 = max_e_2 = 0;

// try modifying this to fit for CSR representation
		for (int i = 0; i < g1.v; i++)
		{
			for (int j = 0; j < g1.v; j++)
			{
				if (g1.edgeinfo(i,j) != 0xff)
				{
					if (i < j)
					{
						le_1[g1.edgeinfo(i,j)]++;
						if (max_e_1 < g1.edgeinfo(i,j))
							max_e_1 = g1.edgeinfo(i,j);
					}
				}
			}
			if (max_v_1 < g1.V[i]) max_v_1 = g1.V[i];
			lv_1[g1.V[i]]++;
		}
		cout<<"g1.edgeinfo over..."<<endl;
		max_v_1++; max_e_1++;

		for (int i = 0; i < g2.v; i++)
		{
			for (int j = 0; j < g2.v; j++)
			{
				if (g2.edgeinfo(i,j) != 0xff)
				{
					if (i < j)
					{
						le_2[g2.edgeinfo(i,j)]++;
						if (max_e_2 < g2.edgeinfo(i,j))
							max_e_2 = g2.edgeinfo(i,j);
					}
				}

			}
			if (max_v_2 < g2.V[i]) max_v_2 = g2.V[i];
			lv_2[g2.V[i]]++;
		}
		max_v_2++; max_e_2++;
        cout<<"g2.edgeinfo over..."<<endl;
		int commonVertex = common::initCommonLabel(lv_1, lv_2, max_v_1, max_v_2);
		int commonEdge = common::initCommonLabel(le_1, le_2, max_e_1, max_e_2);
		cout<<"init common labels done"<<endl;
		int lower_bound = max(g1.v, g2.v) - commonVertex + max(g1.e, g2.e) - commonEdge;
		if (lower_bound > bound) return -1;
        cout<<"lower_bound = "<<lower_bound<<" bound = "<<bound<<endl;

		int degree_1[2048], degree_2[2048];
		g1.degreeSet(degree_1, max_d_1); max_d_1++;
		g2.degreeSet(degree_2, max_d_2); max_d_2++;
		//cout<<"entering memset"<<endl;
		//memset(tmpDegree1, 0, max_d_1 * sizeof(u16));
		//memset(tmpDegree2, 0, max_d_2 * sizeof(u16));
		//cout<<"memset done"<<endl;
		int i = 0, max1 = 0, max2 = 0, size1 = 0, size2 = 0, ie = 0, de = 0,
			edge1 = g1.e, edge2 = g2.e;
         cout<<"some initializations done"<<endl;
		for (int i = 0; i < g1.v; i++)
		{
			if (max1 < degree_1[i])              // is this not a rework
				max1 = degree_1[i];
			tmpDegree1[degree_1[i]]++;
		}
		for (i = max1; i >= 0; i--)
		{
			int len = tmpDegree1[i]; //chongdu   // len - no of vertices of degree i
			for (int l = 0; l < len; l++)
				ds1[size1++] = i;
		}
		for (int i = 0; i < g2.v; i++)
		{
			if (max2 < degree_2[i])
				max2 = degree_2[i];
			tmpDegree2[degree_2[i]]++;
		}
		for (i = max2; i >= 0; i--)
		{
			int len = tmpDegree2[i];
			//cout<<"no of degree "<<i<<" vertices = "<<len<<endl;
			for (int l = 0; l < len; l++)
				ds2[size2++] = i;
		}

		cout<<"size1 = "<<size1<<"size2 = "<<size2<<endl;
		clock_t begin = clock();
                    common::degreeEditDistance(ds1, size1, ds2, size2, ie, de); //time this
        clock_t end = clock();
        elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        cout<<" common::degreeEditDistance:"<<'\t'<<"Time taken(ms) = "<<elapsed_secs*1000<<endl;
		
		cout<<" safely back from degree edit dist"<<endl;
		int tmp = max(2 * ie + edge1 - edge2, 2 * de + edge2 - edge1);
		cout<<"tmp = "<<tmp<<" ie = "<<ie<<" de = "<<de<<endl;
		tmp = max(tmp, edge2 - commonEdge + de);
		cout<<"tmp = "<<tmp<<" commonVertex = "<<commonVertex<<endl;
		lower_bound = max(g1.v, g2.v) - commonVertex + tmp;
		cout<<"lower_bound = "<<lower_bound<<" bound = "<<bound<<endl;
		if (lower_bound > bound) return -1;
		else  //normal execution brings it here
		{
			initFilter++;
			u64 s1, s2;
			int group1_number, group2_number;
			if (FLAG)
			{
				group2.clear();
				
				clock_t begin = clock();
                    s2 = g2.divideGroup(group2, group2_number); //here ?
                clock_t end = clock();
                elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
                cout<<" g2.divideGroup:"<<'\t'<<"Time taken(ms) = "<<elapsed_secs*1000<<endl;
			
				FLAG = false;
			}
			// time this dividegroup function... esp for larger graphs..
			//s1 - product of the sizes of each eq class
			//group_number = total number of Eq classes
    		
			clock_t begin = clock();
                    group1.clear(); s1 = g1.divideGroup(group1, group2_number); // group1 - each vertex and its group..
            clock_t end = clock();
            elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
            cout<<" g1.divideGroup:"<<'\t'<<"Time taken(ms) = "<<elapsed_secs*1000<<endl;
			
			if (s1 > 1) VERTEXFLAG1 = true; else VERTEXFLAG1 = false;
			if (s2 > 1) VERTEXFLAG2 = true; else VERTEXFLAG2 = false;
			if(VERTEXFLAG1) cout<<"VERTEXFLAG1"<<endl;
                        if(VERTEXFLAG2) cout<<"VERTEXFLAG2"<<endl;
			if (s1 < s2)
			{
				cout<<"s1 < s2"<<endl;
				return getEditDistance(g1, g2, bound, group1, group2);
			}
			else
			{
				cout<<"s1 >= s2"<<endl;
				return getEditDistance(g2, g1, bound, group2, group1);
			}
		}
	}
};

#endif

