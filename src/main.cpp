#include<bits/stdc++.h>
#include "Sequence.h"
#include "AntColonyOptimization.h"
#include "pickyAnt.h"
#include "chaoticAnt.h"
#include "Colony.h"
#include <glog/logging.h>

using namespace std;

static int NUM_OF_ANTS = 50;
static int SMOOTHING_LOG_BASE = 3;
static int SMOOTHING_LOWEST =  0.01;
static int EVAPORATION_RATE = 50;

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

void runTest(Sequence &a, AntColonyOptimization algo, vector<pair<int, tuple<double, double, double>>> &results,  bool withRandom = false){
	double resForPicky = 0;
	double resForChaotic = 0;
	double resForRandom = 0;

	for(int i = 0; i<100; i++){
		vector<pair<double,string>> pathsForChaotic = algo.commenceACO<chaoticAnt>();
		algo.resetEssentialParts();
		vector<pair<double,string>> pathsForPicky = algo.commenceACO<pickyAnt>();
		resForPicky+=levenshteinFullMatrix(a.seq,pathsForChaotic[0].second);
		resForPicky+=levenshteinFullMatrix(a.seq,pathsForPicky[0].second);
		if(withRandom){
			int prevNumOfAnts = algo.getNumOfAnts();
			algo.setNumOfAnts(1);
			vector<pair<double,string>> pathsForRandom = algo.commenceACO<pickyAnt>();
			resForRandom+=levenshteinFullMatrix(a.seq,pathsForRandom[0].second);
			algo.setNumOfAnts(prevNumOfAnts);
		}
	}

	if(withRandom){
		results.push_back(*new pair<int, tuple<double, double, double>>(25, *new tuple<double, double, double>(
			resForChaotic/100,
			resForPicky/100,
			resForRandom/100
		)));
	} else {
		results.push_back(*new pair<int, tuple<double, double, double>>(25, *new tuple<double, double, double>(
			resForChaotic/100,
			resForPicky/100,
			-1
		)));
	}
}

int main(int argc, char * argv[])
{
	FLAGS_log_dir = "/home/myc0plasmus/Documents/C/genome_assembly_project/logs/";
	FLAGS_logbufsecs = 0;
	FLAGS_logbuflevel = -1;
	google::InitGoogleLogging(argv[0]);
	srand(time(0));
	Sequence a = Sequence("ATTCAGAAGTATGGCACCCACTTCTGCCTACGTGAGTAGCTAGCGCCATTAGCTAGCCAATCGAAGGTGGGTGTGTGCGTGGCATTGGGGGCATTACCTCACGGATTGGCCGAGGTCGTATCTGAAGCCTTTGCCGAGGGAATCGTGACCCGGGTGGTAAAGTGAAGAGTAATTCTAATCTGCCTGACCATCGACAAAAA");
	// Sequence a = Sequence(1000);
	cout<<"Seqence: "<<endl<<a.seq<<endl;
	a.shredSequence();
	a.createGraphWithFixedCover();
	AntColonyOptimization algo(a);
	// algo.numOfAnts=1;
	// algo.stopTime = 0.01;
	vector<pair<double,string>> paths = algo.commenceACO<chaoticAnt>();
	cout<<"Seqence: "<<endl<<a.seq<<endl;
	cout<<"results:"<<endl;
	for(auto it : paths ){
		cout<<"solution distance: "<<levenshteinFullMatrix(a.seq,it.second)<<endl;
		cout<<it.first<<"\t"<<it.second<<endl;
	}
	return 0;
	// tests for changed number of ants
	vector<vector<pair<int, tuple<double, double, double>>>> overallResults;
	vector<pair<int, tuple<double, double, double>>> results;

	algo.setNumOfAnts(25);
	runTest(a, algo, results);
	algo.setNumOfAnts(50);
	runTest(a, algo, results);
	algo.setNumOfAnts(75);
	runTest(a, algo, results);
	algo.setNumOfAnts(100);
	runTest(a, algo, results);
	algo.setNumOfAnts(125);
	runTest(a, algo, results);
	algo.setNumOfAnts(150);
	runTest(a, algo, results);
	algo.setNumOfAnts(175);
	runTest(a, algo, results);
	algo.setNumOfAnts(200);
	runTest(a, algo, results);
	algo.resetNumOfAnts();
	algo.resetEssentialParts();

	overallResults.push_back(results);
	results.clear();
	//tests for changed evaporation rate

	algo.setEvaporationRate(0);
	runTest(a, algo, results);
	algo.setEvaporationRate(0.01);
	runTest(a, algo, results);
	algo.setEvaporationRate(0.02);
	runTest(a, algo, results);
	algo.setEvaporationRate(0.03);
	runTest(a, algo, results);
	algo.setEvaporationRate(0.04);
	runTest(a, algo, results);
	algo.setEvaporationRate(0.05);
	runTest(a, algo, results);
	algo.setEvaporationRate(0.06);
	runTest(a, algo, results);
	algo.setEvaporationRate(0.08);
	runTest(a, algo, results);
	algo.resetEvaporationRate();
	algo.resetEssentialParts();

	overallResults.push_back(results);
	results.clear();

	//tests for changed smoothing lowest

	algo.setSmoothingLowest(0);
	runTest(a, algo, results);
	algo.setSmoothingLowest(0.01);
	runTest(a, algo, results);
	algo.setSmoothingLowest(0.02);
	runTest(a, algo, results);
	algo.setSmoothingLowest(0.03);
	runTest(a, algo, results);
	algo.setSmoothingLowest(0.04);
	runTest(a, algo, results);
	algo.setSmoothingLowest(0.05);
	runTest(a, algo, results);
	algo.setSmoothingLowest(0.06);
	runTest(a, algo, results);
	algo.setSmoothingLowest(0.08);
	runTest(a, algo, results);
	algo.resetSmoothingLowest();
	algo.resetEssentialParts();

	overallResults.push_back(results);
	results.clear();
	//tests for changed smoothing log based

	algo.setSmoothingLogBase(0);
	runTest(a, algo, results);
	algo.setSmoothingLogBase(1);
	runTest(a, algo, results);
	algo.setSmoothingLogBase(2);
	runTest(a, algo, results);
	algo.setSmoothingLogBase(3);
	runTest(a, algo, results);
	algo.setSmoothingLogBase(4);
	runTest(a, algo, results);
	algo.setSmoothingLogBase(5);
	runTest(a, algo, results);
	algo.setSmoothingLogBase(6);
	runTest(a, algo, results);
	algo.setSmoothingLogBase(7);
	runTest(a, algo, results);
	algo.resetSmoothingLogBase();
	algo.resetEssentialParts();

	overallResults.push_back(results);
	results.clear();
	//test
	// cout<<"solution distance: "<<levenshteinFullMatrix(a.seq,it.second)<<endl;
 
}
