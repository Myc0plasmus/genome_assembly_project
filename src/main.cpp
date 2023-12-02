#include<bits/stdc++.h>
#include "Sequence.h"

using namespace std;

int main()
{
	srand(time(0));
	Sequence a = Sequence(100);
	cout<<"Seqence: "<<endl<<a.seq<<endl;
	a.shredSequence({});
	a.createDefaultGraph();
	for(int i=0;i<a.graphSize;i++){
		cout<<"label: "<<a.graph[i].label<<endl;
		cout<<"values"<<endl;
		for(auto node : a.graph[i].edges){
			cout<<node.neighbour<<endl;
		}	
	}
	cout<<"0 label: "<<a.graph[0].label<<endl;
	cout<<"last but one label: "<<a.graph[a.graphSize -1].label<<endl;
	cout<<"last label: "<<a.graph[a.graphSize].label<<endl;
	cout<<"one past label: "<<a.graph[a.graphSize+1].label<<endl;
	cout<<"First element has index: "<<a.firstElemIdx<<endl;
	cout<<"label: "<<a.graph[a.firstElemIdx].label<<endl;
	cout<<"values"<<endl;
	for(auto node : a.graph[a.firstElemIdx].edges){
		cout<<node.neighbour<<endl;
	}	
}
