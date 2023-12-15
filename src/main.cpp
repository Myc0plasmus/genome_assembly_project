#include<bits/stdc++.h>
#include "Sequence.h"
#include "AntColonyOptimization.h"
#include "pickyAnt.h"
#include "Colony.h"

using namespace std;

int levenshteinFullMatrix(const string& str1,
                        const string& str2)
{
    int m = str1.length();
    int n = str2.length();
 
    vector<vector<int> > dp(m + 1, vector<int>(n + 1, 0));
 
    for (int i = 0; i <= m; i++) {
        dp[i][0] = i;
    }
 
    for (int j = 0; j <= n; j++) {
        dp[0][j] = j;
    }
 
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (str1[i - 1] == str2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            }
            else {
                dp[i][j] = 1
                        + min(
 
                            // Insert
                            dp[i][j - 1],
                            min(
 
                                // Remove
                                dp[i - 1][j],
 
                                // Replace
                                dp[i - 1][j - 1]));
            }
        }
    }
 
    return dp[m][n];
}

int main()
{
	srand(time(0));
	// Sequence a = Sequence("GCTCGGCTACATGATCCTTACCACCACCGAGTTCACACGATGTCGATAGAAATACGCGCAGATCTTTTGCGCACTGTAGCCGCGATTCCGCCAGTTTCAC");
	Sequence a = Sequence(1000);
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
	AntColonyOptimization algoFinal(a);
	// vector<pair<double,string>> paths = algoFinal.commenceACO(bind(&AntColonyOptimization::ant,&algoFinal));
	vector<pair<double,string>> paths = algoFinal.commenceACO<pickyAnt>();
	cout<<"Seqence: "<<endl<<a.seq<<endl;
	cout<<"results:"<<endl;
	for(auto it : paths ){
		cout<<"solution distance: "<<levenshteinFullMatrix(a.seq,it.second)<<endl;
		cout<<it.first<<"\t"<<it.second<<endl;
	}
}
