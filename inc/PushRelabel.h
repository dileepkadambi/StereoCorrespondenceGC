// Adjacency list implementation of FIFO push relabel maximum flow
// with the gap relabeling heuristic.  This implementation is
// significantly faster than straight Ford-Fulkerson.  It solves
// random problems with 10000 vertices and 1000000 edges in a few
// seconds, though it is possible to construct test cases that
// achieve the worst-case.
//
// Running time:
//     O(|V|^3)
//
// INPUT: 
//     - graph, constructed using AddEdge()
//     - source
//     - sink
//
// OUTPUT:
//     - maximum flow value
//     - To obtain the actual flow values, look at all edges with
//       capacity > 0 (zero capacity edges are residual edges).
#pragma once
#include <cmath>
#include <vector>
#include <iostream>
#include <queue>
#include "opencv2/core/core.hpp"
#include "opencv2/core/core_c.h"

using namespace std;
struct GCEdge;
struct GCVtx;

typedef long long LL;

class PushRelabel
{
public:
	PushRelabel(int N);
	~PushRelabel();
	LL GetMaxFlow(GCVtx* vbuf, GCEdge* edgeBuf, int NoOfVertices, int NoOfEdges, int s, int t);	
private:
	void FindMinCut(int numVertices);
	void Enqueue(GCVtx *v);
	void Push(int edgeIndex, GCVtx* from);
	void Gap(int k);
	void Relabel(GCVtx* v);
	void Discharge(GCVtx* v);
	void CreateSourceAndSinkEdges(GCVtx* vbuf, 
									GCEdge* edgeBuf, 
									int noOfVertices, 
									int noOfEdges, 
									int s, 
									int t);
	void FindMinCut(GCVtx* pSink);
	//void AddTWeights(GCNode* vertex);

private:
	GCVtx* m_G;
	GCEdge* m_Ebuf;
	int N;
	vector<int> count;
	queue<GCVtx*> Q;
};

