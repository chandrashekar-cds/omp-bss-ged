#pragma once
#ifndef _GRAPH_H
#define _GRAPH_H
#include "common.h"

struct BTuple
{
	int first;
	int second;
	BTuple(){}
	BTuple(int f, int s) :first(f), second(s){}
	bool operator < (const BTuple &t) const
	{ return first < t.first || (first == t.first && second < t.second);}
};

//extra variable used for DFS traversal 
static BTuple order_tmp[2048];
static BTuple order_vertex[2048];
static int vertex_degree[2048];

class graph
{
public:
	int v;
	vector<int> V;
//	vector<vector<int > > E;
	int graph_id;
	int e;

	vector <int> A;   //we can have float values too
    vector <int> JA,IA; // IA stores no of nnz elements till previous row.. JA stores column indices
    int nnz;
    int crow; // keeps the current source vertex info

public:
	graph()
	:v(0),V(5,0),graph_id(0),e(0),A(5,0),JA(5,0),IA(5,0),nnz(0),crow(0)
	{}
	~graph(){}
	/*
	inline int edgeinfo(int from, int to)
	{
		assert(from < v && to < v);
		return E[from][to];
	}*/

	inline int edgeinfo(int from, int to)
	{
		assert(from < v || to < v);
		//return E[from][to];
		int source=from,dest=to;
			if(from > v) {source = to; dest = from;}
		int r,k = IA[source+1]-IA[source];
		int flag=0;
        if (k==0)
			flag = 0;
		else{
			for(r = 0;r < k;r++){
            	if(JA[IA[source]+r]==dest) return A[IA[source]+r]; // comparing column index; if a hit,then return element
          	}
		    if(r == k) flag = 0;
        }

        if (flag == 0) 
		{
			if(from < v && to < v)
			{   
			   source = to; dest = from;
			   k = IA[source+1]-IA[source];
			   for(r = 0;r < k;r++){
                  if(JA[IA[source]+r]==dest) return A[IA[source]+r]; 
			      }  
			}
		}
		return 255; // entry not found in the array storing nz elements

	}

	static void reOrderGraphs(const char *in, const char *out, int total)
	{	
		FILE *fr = fopen(in, "r"); assert(fr);
		FILE *fw = fopen(out, "w"); assert(fw);

		int gid, v, e;
		int f, t, l;
		int count = 0;

		while (!feof(fr))
		{
			fscanf(fr, "%d\n", &gid);
			fscanf(fr, "%d %d\n", &v, &e);
			fprintf(fw, "%d\n", gid);
			fprintf(fw, "%d %d\n", v, e);

			graph g;
			g.graph_id = gid; g.v = v; g.e = e;
			g.V.resize(g.v, 0); //vector<int> tmp(g.v, 255);
			g.nnz = 2*e;
			g.A.resize(g.nnz);
            g.JA.resize(g.nnz);
            g.IA.resize(g.v + 1);
		    g.crow = 0; 
			//ADDRESS THIS//g.E.resize(g.v, tmp);
			vector<int> from, to, label; from.resize(g.e, -1); 
			to.resize(g.e, -1); label.resize(g.e, -1);

			for (int i = 0; i < v; i++)
				fscanf(fr, "%d\n", &g.V[i]);
			vector<int> vertex_tmp = g.V;
            int cre = 0;
			for (int i = 0; i < g.nnz; i++)
			{
				fscanf(fr, "%d %d %d\n", &f, &t, &l);
				if(g.crow != f) {
					g.IA[g.crow+1]=g.IA[g.crow]+cre;
					for(int j = g.crow+1; j<f; j++)
					  	g.IA[j+1]=g.IA[j];
					g.crow=f;
					cre = 0;
                }
				cre++;
				g.JA[i] = t;
				g.A[i] = l;

				//g.E[f][t] = l;   //ADDRESS THIS
				//g.E[t][f] = l;   //ADDRESS THIS 
				from[i] = f;
				to[i] = t;
				label[i] = l;
			}		
			vector<int> vertex_order;
			g.DFSTraverse(vertex_order);

			for (int i = 0; i < v; i++)
			{
				int rank = vertex_order[i];
				vertex_tmp[rank] = g.V[i];
			}
			for (int i = 0; i < v; i++)
			{
				fprintf(fw, "%d\n", vertex_tmp[i]);
			}
			for (int i = 0; i < e; i++)
			{
				int rankf = vertex_order[from[i]];
				int rankt = vertex_order[to[i]];
				if (rankt < rankf)
				{
					int tmp = rankt;
					rankt = rankf;
					rankf = tmp;
				}
				fprintf(fw, "%d %d %d\n", rankf, rankt, label[i]);
			}
			count++;
			if (count >= total) break;
		}
		if (fr) fclose(fr);
		if (fw) fclose(fw);
	}
/*
	static vector<graph> readGraphMemory(const char *db, int total)
	{
		vector<graph> vg;
		FILE *fr = fopen(db, "r+"); assert(fr);
		int v, e;
		int gid;
		int f, t, l;
		int count = 0;

		while (!feof(fr))
		{
			fscanf(fr, "%d\n", &gid);
			fscanf(fr, "%d %d\n", &v, &e);
			graph g;
			g.graph_id = gid; g.v = v; g.e = e;
			g.V.resize(g.v, 0); vector<int> tmp(g.v, 255); g.E.resize(g.v, tmp);

			for (int i = 0; i < v; i++)
				fscanf(fr, "%d\n", &g.V[i]);

			for (int i = 0; i < e; i++)
			{
				fscanf(fr, "%d %d %d\n", &f, &t, &l);
				g.E[f][t] = l;
				g.E[t][f] = l;
			}
			vg.push_back(g);
			count++;
			if (count >= total)
				break;
		}
		if (fr) fclose(fr);
		return vg;
	}
	graph(const graph &g)
	{
		this->graph_id = g.graph_id;
		this->v = g.v;
		this->e = g.e;
		this->V = g.V;
		this->E = g.E;
	}
*/

static vector<graph> readGraphMemory(const char *db, int total)
	{
		vector<graph> vg;
		vg.resize(total);
		ifstream indata;
		 indata.open(db);

		if(!indata) { // file couldn't be opened
        cerr << "Error: file could not be opened" << endl;
        exit(1);
		}
	
		int v, e;
		int gid;
		int f, t, l;
		int count = 0;
        indata >> gid; 
		while (!indata.eof())
		{
			//fscanf(fr, "%d\n", &gid);
             //indata >> gid;

			//fscanf(fr, "%d %d\n", &v, &e);
			indata >> v >> e;
			graph g;
			g.graph_id = gid; g.v = v; g.e = e;
			g.V.resize(g.v, 0); //vector<int> tmp(g.v, 255);
		    g.nnz = e;
			g.A.resize(g.nnz);
            g.JA.resize(g.nnz);
            g.IA.resize(g.v+1);
		    g.crow = 0;
			//ADDRESS THIS// g.E.resize(g.v, tmp);

                       //resize(new size, initial value)
					   //vector<int> var(size, init values)
			for (int i = 0; i < v; i++)
				//fscanf(fr, "%d\n", &g.V[i]);
				  indata >> g.V[i];
                     // label of each vertex
			int cre = 0; // current row edges
			for (int i = 0; i < g.nnz; i++)  // replace e with nnz
			{
				//fscanf(fr, "%d %d %d\n", &f, &t, &l);
				indata >>f >>t >>l;
                if(g.crow != f) {
					g.IA[g.crow+1]=g.IA[g.crow]+cre;
					for(int j = g.crow+1; j<f; j++)
					  	g.IA[j+1]=g.IA[j];
					g.crow=f;
					cre = 0;
                }

				cre++;
				g.JA[i] = t;
				g.A[i] = l;

				//g.E[f][t] = l;
				//g.E[t][f] = l;
			}     // edge denoted by vertices and a label
			if(g.crow <= g.v-1) {
					g.IA[g.crow+1]=g.IA[g.crow]+cre;
					for(int j = g.crow+1; j<g.v; j++)
					  	g.IA[j+1]=g.IA[j];
					g.crow=g.v-1;
            }


			vg[count] = g;
			count++;
			if (count >= total)
				break;
			indata >> gid;	
		}
		if (indata.eof()) indata.close();
		return vg;
	}
	static void print_mat(graph g, const char* output_file) 
       {
	
		ofstream fileout(output_file,ofstream::out);
   		int i,j;
   		for(i=0;i<g.v;i++)
   		{
      		for(j=0;j<g.v;j++)
      		{
          
          		fileout << g.edgeinfo(i,j);
          		if(j<g.v-1)fileout<<'\t';
      		}
      		fileout<<'\n';
   		}
       }

public:
	map<int, int> vertexLabel(int &max_vertex);
	map<int, int> edgeLabel(int &max_edge);
	void vertexDegree(int &vertex, u16 &degree);
	void vertexDegree(int &vertex, int &degree);
	void editDistanceInduced(graph &g, graph &h, vector<vector<int > >&assignment, int &value);
	inline void degreeSet(int* vd, int &max_d)
	{
		max_d = 0;
		for (int i = 0; i < this->v; i++)
		{
			this->vertexDegree(i, vd[i]);
			if (max_d < vd[i])
				max_d = vd[i];
		}
	}
	inline void degreeSet(u16 *vd, int &max_d)
	{
		max_d = 0;
		for (int i = 0; i < this->v; i++)
		{
			this->vertexDegree(i, vd[i]);
			if (max_d < vd[i])
				max_d = vd[i];
		}
	}	
public:
	inline vector<BTuple > neibhoor(int &id);
	bool equal(int &f, int &t);
	vector<BTuple> equalSet();
	void mergeEqualSet(vector<BTuple> &vb, vector<vector<int> > &vertexEqual);
	u64  divideGroup(vector<int> &vs, int &group_number);
public:
	void DFS(int &i, int &order_count, vector<bool> &visited, vector<int> &vertex_order, int *degree);
	void DFSTraverse(vector<int> &vertex_order);
};
#endif
