#include<bits/stdc++.h>
#include "Sequence.h"

using namespace std;

int main()
{
	Sequence a = Sequence(100);
	cout<<"Seqence: "<<endl<<a.seq<<endl;
	a.shredSequence({});
	a.createDefaultGraph();
	for(int i=0;i<=a.graphSize;i++){
		cout<<"label: "<<a.graph[i].label<<endl;
		cout<<"values"<<endl;
		for(auto node : a.graph[i].edges){
			cout<<node<<endl;
		}	
	}
	
}
