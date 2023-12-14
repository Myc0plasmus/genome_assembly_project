#include<bits/stdc++.h>
#include "Sequence.h"
#include "AntColonyOptimization.h"

using namespace std;

int main()
{
	srand(time(0));
	// Sequence a = Sequence("GCTCGGCTACATGATCCTTACCACCACCGAGTTCACACGATGTCGATAGAAATACGCGCAGATCTTTTGCGCACTGTAGCCGCGATTCCGCCAGTTTCAC");
	Sequence a = Sequence(100);
	cout<<"Seqence: "<<endl<<a.seq<<endl;
	a.shredSequence({});
	a.createDefaultGraph();
	for(int i=0;i<a.graphSize;i++){
		cout<<"i: "<<i<<endl;
		cout<<"label: "<<a.graph[i].label<<endl;
		cout<<"neighbours: ";
		for(auto node : a.graph[i].edges){
			cout<<node.neighbour<<" ";
		}
		cout<<endl;
		cout<<"values    : ";
		for(auto node : a.graph[i].edges){
			cout<<node.val<<" ";
		}
		cout<<endl;
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
	cout<<"before colony"<<endl;
	// AntColonyOptimization algo(&a);	
	// cout<<"evaporationRate: "<<algo.evaporationRate<<endl;
	// cout<<"initialized colony"<<endl;
	// cout<<"first ant:"<<endl;
	// algo.ant();
	// cout<<"second ant:"<<endl;
	// algo.ant();
	// algo.simplePath();
	AntColonyOptimization algoFinal(&a);
	vector<pair<double,string>> paths = algoFinal.commenceACO(bind(&AntColonyOptimization::ant,&algoFinal));
	cout<<"Seqence: "<<endl<<a.seq<<endl;
	cout<<"results:"<<endl;
	for(auto it : paths ){
		cout<<it.first<<"\t"<<it.second<<endl;
	}
}
