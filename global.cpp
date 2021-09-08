#include "global.h"

int max_v_1 = 0;
int max_e_1 = 0;   //max label of edge
int max_v_2 = 0;
int max_e_2 = 0;
int max_d_1 = 0;
int max_d_2 = 0; 
int MAX_THREADS = 0;
int h = 0;
int lvlcounter = 0;
int memfootprint = 0;

//typedef std::map<int, int> Dictmap;
/*template <typename T>
int mapfind(T mapping, int key, int defalt)
{
    typename T::iterator itr;
    itr = mapping.find(key);
    //cout<<"itr = "<<itr<<endl;
    if (itr == mapping.end())  
        return defalt;
    else return itr->second;
}*/
/*void mapinsert(T mymap, int key, int value)
{
    typename T::iterator itr;	
    itr = mymap.find(key);
    if(itr == mymap.end()) 
        { 
            mymap[key] = value; 
            //cout<<"new element inserted"<<endl;
        }
    else itr->second = value;
   
   // std::pair<std::map<int,int>::iterator,bool> ret;
   // ret = mymap.insert ( std::pair<int,int>(key,value) );
   // if (ret.second==false) ret.first->second = value;
   
    return;
}*/
/*template <typename T>
void printmap(T myMap)
{
    typename T::iterator it;
    for(auto it = myMap.begin(); it != myMap.end(); ++it)
    {
        std::cout << it->first << "\t" << it->second << "\n";
        memfootprint += 2;
    }
}*/

vector<vector<int > > adjList1(0, vector<int>());
vector<vector<int > > adjList2(0, vector<int>());
verifyGraphNode gn1[2048] = { verifyGraphNode() };
verifyGraphNode gn2[2048] = { verifyGraphNode() };
u16 a1[2048][2048] = {0}; //
u16 a2[2048][2048] = {0};
vector<int> vs1(0, 0);
vector<int> vs2(0, 0);
vector<int> group1(0, 0);
vector<int> group2(0, 0);

u16 start_deleted[256] = {0};    // same as max_e_1
u16 end_deleted[256] = {0};      // same as max_e_1
u16 tmpDegree1[2048] ={0};      // max length of max degree of any vertex
u16 tmpDegree2[2048] = {0};
u16 Degree1[2048] = {0};
u16 Degree2[2048] = {0};
u16 edgeList1[256] = {0};     //max length of max label size - max_e_1
u16 edgeList2[256] = {0};
u16 edge_set_1[2048] = {0};   //  
u16 edge_set_2[2048] = {0};
u16 succ_degree_1[2048] = {0};    // max size - no of vertices
u16 succ_degree_2[2048] = {0};

int *Group;
int ds1[2048] = {0};            // max size - no of vertices
int ds2[2048] = {0};

bool groupFlag1[2048] = {0};       // max size - no of vertices
bool groupFlag2[2048] = {0};
int groupNumber = 0;
bool unionFlag1[2048] = {0};      // max size - no of vertices
bool unionFlag2[2048] = {0};

bool FLAG = true;
bool VERTEXFLAG1 = false;
bool VERTEXFLAG2 = false;


u64 total_1 = 0;
u64 total_2 = 0;
u64 totalExpandNode = 0;
u64 total_search_node = 0;
