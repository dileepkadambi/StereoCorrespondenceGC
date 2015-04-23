#pragma once
#include <vector>
#include "sources\modules\legacy\src\precomp.hpp"


struct GCPREdge;
typedef long long LL;

typedef struct GCVtx
{
    GCVtx *next;
    int parent;
    int first;
    int ts;
    int dist;
    short weight;
	int visit;
    uchar t; 

	LL excess;
	int active;
	int prdist;
}
GCVtx;

typedef struct GCEdge
{
    GCVtx* dst;
    int next;
    int weight;

	//Added by Team 033
	int flow;
}
GCEdge;
