#include <PushRelabel.h>
#include "StereoUsingGraphCut.h"

PushRelabel::PushRelabel(int numberOfNodes):count(2*(numberOfNodes+2))
{
	N = numberOfNodes +2;
}

PushRelabel::~PushRelabel()
{
}

void PushRelabel::Enqueue(GCVtx* v)
{
	 if (!v->active && v->excess > 0) 
		{
			v->active = 1;
			Q.push(v);
	    }
}

void PushRelabel::Push(int edgeIndex, GCVtx* from) 
{
	int amt;
	//amt = int (min((int)e.from->excess, (e.capacity - e.flow)));
    amt = int (min((int)from->excess, (m_Ebuf[edgeIndex].weight - m_Ebuf[edgeIndex].flow)));
	if ((from->prdist <= m_Ebuf[edgeIndex].dst->prdist) || amt == 0) 
		return;
    m_Ebuf[edgeIndex].flow += amt;
    //G[e.to][e.index].flow -= amt;
	m_Ebuf[edgeIndex^1].flow -= amt;
    m_Ebuf[edgeIndex].dst->excess += amt;    
    from->excess -= amt;
    Enqueue(m_Ebuf[edgeIndex].dst);
}

void PushRelabel::Relabel(GCVtx* v) 
{
    count[v->prdist]--;
    v->prdist = 2*N;
	int next = v->first;
	while(next != 0)
	{
		//if (G[v][i].cap - G[v][i].flow > 0)
		if(m_Ebuf[next].weight - m_Ebuf[next].flow > 0)
		{
			//prdist[v] = min(prdist[v], prdist[G[v][i].to] + 1);
			v->prdist = min(v->prdist, m_Ebuf[next].dst->prdist + 1);
		}	
		next = m_Ebuf[next].next;
	}
    count[v->prdist]++;
    Enqueue(v);
  }

void PushRelabel::Discharge(GCVtx* v) 
{
	if(v->excess > 0)
	{
		int next = v->first;
		while(next !=0)
		{
			Push(next, v);
			next = m_Ebuf[next].next;
		}
	}
    if (v->excess > 0) 
	{
      if (count[v->prdist] == 1) 
		Gap(v->prdist); 
      else
		Relabel(v);
    }
}
void PushRelabel::CreateSourceAndSinkEdges(GCVtx* vbuf, 
											GCEdge* edgeBuf, 
											int noOfVertices, 
											int noOfEdges, 
											int s, 
											int t)
{
	GCVtx* source = &vbuf[s];
	GCVtx* sink = &vbuf[t];
	source->first = 0;
	sink->excess = 0;
	sink->visit = 0;
	source->visit = 0;
	//To be checked
	sink->first = 0;
	sink->t = 1;
	//Add Source & sink Edges
	unsigned int j=0;
	for(int i=0; i<noOfVertices; i++)
	{
		vbuf[i].active = 0;
		vbuf[i].excess = 0;
		vbuf[i].prdist = 0;
		vbuf[i].visit = 0;
		if(vbuf[i].weight ==0)
		{
			vbuf[i].prdist = 0;
			vbuf[i].t = 0;
			continue;
		}
		else if(vbuf[i].weight>0)
		{
			vbuf[i].prdist = 0;
			vbuf[i].t = 0;
			//Add source edge
			edgeBuf[noOfEdges+j].next = source->first;
			edgeBuf[noOfEdges+j].dst = &vbuf[i];
			edgeBuf[noOfEdges+j].weight = vbuf[i].weight;
			edgeBuf[noOfEdges+j].flow = 0;
			source->first = noOfEdges + j;

			edgeBuf[noOfEdges+j+1].weight = 0;
			edgeBuf[noOfEdges+j+1].dst = source;
			edgeBuf[noOfEdges+j+1].flow = 0;
			edgeBuf[noOfEdges+j+1].next = vbuf[i].first;
			vbuf[i].first = noOfEdges+j+1;
			j+=2;
		}
		else 
		{
			vbuf[i].t = 1;
			vbuf[i].prdist = 0;
			//Add Sink Edge
			edgeBuf[noOfEdges+j].next = vbuf[i].first;
			edgeBuf[noOfEdges+j].dst = sink;
			edgeBuf[noOfEdges+j].weight = -(vbuf[i].weight);
			edgeBuf[noOfEdges+j].flow = 0;
			vbuf[i].first = noOfEdges + j;

			edgeBuf[noOfEdges+j+1].next = sink->first;
			edgeBuf[noOfEdges+j+1].dst = &vbuf[i];
			edgeBuf[noOfEdges+j+1].weight = 0;
			edgeBuf[noOfEdges+j+1].flow = 0;
			sink->first = noOfEdges+j+1;
			j+=2;
		}
	}
}


LL PushRelabel::GetMaxFlow(GCVtx* vbuf, 
							GCEdge* edgeBuf, 
							int noOfVertices, 
							int noOfEdges, 
							int s, 
							int t) 
{
	N = noOfVertices + 2;
	
	//Store the pointer in member
	m_G = vbuf;
	m_Ebuf = edgeBuf;
	//Create Source and Sink Edges for all vertices
	CreateSourceAndSinkEdges(vbuf, edgeBuf, noOfVertices, noOfEdges, s, t);
	//Dileep: Have to check if count should be for Source and Sink 

	vbuf[s].excess = 0;
	vbuf[t].excess = 0;

	count[0] = N-1;
    count[N] = 1;
    vbuf[s].prdist = N;
	vbuf[t].prdist = 0;
	vbuf[s].active = vbuf[t].active = true;
    
	int next = vbuf[s].first;
	//Push Flow from All edges of the source
	while(next != 0)
	{
		//excess[s] += G[s][i].cap;
		vbuf[s].excess += edgeBuf[next].weight; 
        //Push(G[0].pLsEdges->at(i));
		Push(next, &vbuf[s]);
		next = m_Ebuf[next].next;
	}
    
    while (!Q.empty()) 
	{
      GCVtx* v = Q.front();
      Q.pop();
	  v->active = false;
      Discharge(v);
    }
    
    int totflow = 0;
	int totcap = 0, totsink = 0;
	next = vbuf[s].first;
	while(next != 0)
	{
		//if(m_Ebuf[next].flow > 0)
		totflow += m_Ebuf[next].flow;
		if(m_Ebuf[next].weight > 0)
		totcap += m_Ebuf[next].weight;
		//printf("sub %d %d\n", m_Ebuf[next].weight, m_Ebuf[next].flow);
		next = m_Ebuf[next].next;
	}
	next = vbuf[t].first;
	while(next) {
		totsink += m_Ebuf[next ^ 1].weight;
		next = m_Ebuf[next].next;
	}
	printf("Team033_maxFlow %d \n", totflow);
	//Find Min Cut
	
	FindMinCut(N-2);
	//FindMinCut(&vbuf[t]);
    return totflow;
}

void PushRelabel::Gap(int k) 
{
    for (int v = 0; v < N; v++) 
	{
      if (m_G[v].prdist < k) 
		  continue;
	  count[m_G[v].prdist]--;
      m_G[v].prdist = max(m_G[v].prdist, N+1);
      count[m_G[v].prdist]++;
      Enqueue(&m_G[v]);
    }
}

void PushRelabel::FindMinCut(GCVtx* pSink)
{
	//Do a BFS from the Sink
	//Get all edges from sink Whose sister edge has weight - flow > 0. if t value is 0 then change 
	while(!Q.empty()) Q.pop();

	int nextEdge = pSink->first;
	while(nextEdge)
	{
		GCVtx* dstNode = m_Ebuf[nextEdge].dst;
		if(dstNode->visit == 0 && m_Ebuf[nextEdge^1].weight - m_Ebuf[nextEdge^1].flow > 0)
		{
			//Implies the dstNode belongs to sink
			dstNode->visit = 1;
			dstNode->t = 1;
			dstNode->parent = 1;
			Q.push(dstNode);
		}
		nextEdge = m_Ebuf[nextEdge].next;
	}

	while(!Q.empty())
	{
		GCVtx* v = Q.front();
		Q.pop();
		nextEdge = v->first;
		while(nextEdge)
		{
			GCVtx* dstNode = m_Ebuf[nextEdge].dst;
			if(dstNode->visit == 0 && m_Ebuf[nextEdge^1].weight - m_Ebuf[nextEdge^1].flow > 0)
			{
				dstNode->visit = 1;
				//Implies the dstNode belongs to sink
				if(dstNode->t == 0)
				{
					dstNode->t = 1;
					dstNode->parent = 1;
					Q.push(dstNode);
				}
			}
			nextEdge = m_Ebuf[nextEdge].next;
		}
	}
}

void PushRelabel::FindMinCut(int numVertices)
{
	//Algorithm: Do a BFS from the Sink 
	//All Edges from Sink Whose Sister Edge has Capacity - flow > 0 set the value of t to 1 i.e they belong to the sink.
	//All vertices with negative weights are connected to sink. Push them all  to the queue and do bfs from there.. 
	while(!Q.empty())
		Q.pop();
	for(int i=0; i<numVertices; i++)
	{
		m_G[i].t = 1;
		m_G[i].parent = 1;
	}
	Q.push(&m_G[numVertices]);
	while(!Q.empty())
	{
		GCVtx* v = Q.front();
		Q.pop();

		for(int edgeIndex = v->first; edgeIndex != 0; edgeIndex = m_Ebuf[edgeIndex].next) 
		{
            //int sisterEdgeIndex = edgeIndex ^ 1;
            GCVtx * nxt = m_Ebuf[edgeIndex].dst;

            if(nxt->t == 1 && (m_Ebuf[edgeIndex].weight - m_Ebuf[edgeIndex].flow) > 0) 
			{
				nxt->visit = 1;
                nxt->t = 0;
                nxt->parent = 0;
                Q.push(nxt);
            }
		}
	}
}
