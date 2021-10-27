#ifndef _GLOBAL_H
#define _GLOBAL_H
#include "bitmap.h"
#include "verifyGraph.h"

extern int max_v_1;
extern int max_e_1;
extern int max_v_2;
extern int max_e_2;
extern int max_d_1;
extern int max_d_2;
extern int MAX_THREADS;
extern int h;
extern int lvlcounter;
extern int memfootprint;
extern int ds1_size;
typedef std::map<int, int> Dictmap;
typedef std::map<u16, u16> Dictmapu16;

template <typename T> int mapfind(T mapping, int key, int defalt)
{
    typename T::iterator itr;
    itr = mapping.find(key);
    //cout<<"itr = "<<itr<<endl;
    if (itr == mapping.end())  
        return defalt;
    else return itr->second;
}
//void mapinsert(T mymap, int key, int value);
template <typename T>
void printmap(T myMap)
{
    typename T::iterator it;
    for(auto it = myMap.begin(); it != myMap.end(); ++it)
    {
        std::cout << it->first << "\t" << it->second << "\n";
        memfootprint += 2;
    }
}


auto mfind = [] (vector<u16> m, int i)->u16 { if (i < m.size()) 
                                              return m[i];
                                              else return 0; };

void minsert (vector<u16> &m, int ind, u16 val);
/*auto minsert = [&] (vector<u16> &m, int ind, u16 val) {  if (ind < m.size())
                                                            m[ind] = val;
                                                        else if(ind == m.size())
                                                             m.push_back(val);
                                                        else  
                                                            {  m.resize(ind+1,0xff);
                                                               m[ind] = val;                                                            
                                                            }    
                                                    };
*/
/*
auto cfind = [] (vector<int> m, int i) { if (i < m.size()) 
                                              return m[i];
                                              else return 0; };

auto cinsert = [&] (vector<int> &m, int ind, int val) {  if (ind >= m.size())
                                                            m.resize(ind+1,0);
                                                        m[ind] = val;    
                                                    };

*/

extern vector<vector<int > > adjList1;
extern vector<vector<int > > adjList2;
extern verifyGraphNode gn1[2048];
extern verifyGraphNode gn2[2048];
extern u16 a1[2048][2048]; //
extern u16 a2[2048][2048];
extern vector<int> vs1; //size of vertices - set in undealVertexSet
extern vector<int> vs2; //size of vertices 
extern vector<int> group1; // size of vertices - set in divideGroup
extern vector<int> group2; // size of vertices - set in divideGroup
extern u16 start_deleted[256];
extern u16 end_deleted[256];
extern u16 tmpDegree1[2048];
extern u16 tmpDegree2[2048];
extern u16 Degree1[2048];
extern u16 Degree2[2048];
extern u16 edgeList1[256];
extern u16 edgeList2[256];
extern u16 edge_set_1[2048];
extern u16 edge_set_2[2048];
extern u16 succ_degree_1[2048]; 
extern u16 succ_degree_2[2048];

extern int *Group;
extern int ds1[2048];
extern int ds2[2048];
extern bool FLAG;
extern bool VERTEXFLAG1;
extern bool VERTEXFLAG2;
extern bool groupFlag1[2048];
extern bool groupFlag2[2048];
extern int groupNumber;
extern bool unionFlag1[2048];
extern bool unionFlag2[2048];


extern u64 total_1;
extern u64 total_2;
extern u64 totalExpandNode; 
extern u64 total_search_node;
#endif
