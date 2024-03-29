#include<bits/stdc++.h>
#include "Sequence.h"
#include "AntColonyOptimization.h"
#include "pickyAnt.h"
#include "chaoticAnt.h"
#include <filesystem>
#include "Colony.h"
#include <glog/logging.h>


namespace fs = std::filesystem;

using namespace std;

static int NUMBER_OF_TEST_ITERATIONS = 2;
static fs::path TESTS_PATH = "tests";
static fs::path LOGS_PATH = "logs";

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

int dumpCSV(string testName, string headerName, vector<pair<int, tuple<double, double, double>>> results){
	std::ofstream file;
    file.open ("tests/"+testName + ".csv");
	file << headerName << endl;
	file << "number;chaoticAnt;pickyAnt;random" << endl;
	for(auto it:results){
		file << it.first<< ";"<< get<0>(it.second)<<";"<< get<1>(it.second)<<";"<< get<2>(it.second)<<endl;
	}
    file.close();
	return 1;
}

void runTest(int seqLen, Sequence &a, AntColonyOptimization algo, vector<pair<auto, tuple<double, double, double>>> &results,  auto numberOfSomething,bool withRandom = false){
	double resForPicky = 0;
	double resForChaotic = 0;
	double resForRandom = 0;
	for(int i = 0; i<NUMBER_OF_TEST_ITERATIONS; i++){
		LOG(INFO)<<"Starting test loop";
		a.genNewSeq(200*(seqLen+1));
		LOG(INFO)<<"generated seq";
		a.shredSequence();
		LOG(INFO)<<"shredded seq";
		a.createGraphWithFixedCover();
		LOG(INFO)<<"created graph";
		algo.attunePheromones();
		LOG(INFO)<<"attuned pheromones";
		std::ofstream file;
		file.open ("tests/seq" + to_string(seqLen) + "_" + to_string(i) + ".txt");
		file << a.seq <<  endl;
		file << "Błędy pozytywne:" << a.falsePositives << " Błędy negatywne:" << a.falseNegatives << " próg błędów pozytywnych:" << a.falsePositiveThreshold << " próg błędów negatywnych:"<<a.falsePositiveThreshold << endl;
		file.close();
		cout<<"Seqence: "<<endl<<a.seq<<endl;

		LOG(INFO)<<"Went past the sequence creation";
		
		vector<pair<double,string>> pathsForChaotic = algo.commenceACO<chaoticAnt>();
		LOG(INFO)<<"finished chaotic";
		algo.resetEssentialParts();
		LOG(INFO)<<"Reset carried out succesfully";
		vector<pair<double,string>> pathsForPicky = algo.commenceACO<pickyAnt>();
		LOG(INFO)<<"finnished picky";
		resForChaotic += levenshteinFullMatrix(a.seq,pathsForChaotic[0].second);
		resForPicky += levenshteinFullMatrix(a.seq,pathsForPicky[0].second);
		if(withRandom){
			int prevNumOfAnts = algo.getNumOfAnts();
			algo.setNumOfAnts(1);
			vector<pair<double,string>> pathsForRandom = algo.commenceACO<pickyAnt>();
			resForRandom+=levenshteinFullMatrix(a.seq,pathsForRandom[0].second);
			algo.setNumOfAnts(prevNumOfAnts);
		}
	}
	LOG(INFO)<<"after test loop";

	if(withRandom){
		results.push_back(*new pair<int, tuple<double, double, double>>(numberOfSomething, *new tuple<double, double, double>(
			resForChaotic/NUMBER_OF_TEST_ITERATIONS,
			resForPicky/NUMBER_OF_TEST_ITERATIONS,
			resForRandom/NUMBER_OF_TEST_ITERATIONS
		)));
	} else {
		LOG(INFO)<<"pushing results";
		results.push_back(*new pair<int, tuple<double, double, double>>(numberOfSomething, *new tuple<double, double, double>(
			resForChaotic/NUMBER_OF_TEST_ITERATIONS,
			resForPicky/NUMBER_OF_TEST_ITERATIONS,
			-1
		)));
	}
	LOG(INFO)<<"after writing results";
}

void generateTests(){
for(int i = 0; i<5; i++){
		Sequence a = Sequence();
		

		AntColonyOptimization algo(a);

		//tests for changed number of ants
		vector<pair<int, tuple<double, double, double>>> results;
		cout<<"test 1: "<<endl;
		for(int j=1; j<=8; j++){
			LOG(INFO)<<"before setting number of ants";
			algo.setNumOfAnts(25*j);	
			LOG(INFO)<<"after setting number of ants";
			runTest(i,a, algo, results, algo.getNumOfAnts());
			LOG(INFO)<<"after running test";
		}
		LOG(INFO)<<"finished first testing loop";
		algo.resetNumOfAnts();
		algo.resetEssentialParts();
		LOG(INFO)<<"seq reset";

		cout<<"file has been generated"<<endl;
		dumpCSV("NumOfAnts" + to_string(i), "NumOfAnts", results);
		results.clear();
		//tests for changed evaporation rate
		cout<<"test 2: "<<endl;
		for(int j=0; j<9; j++){
			if(i==0)
				algo.setEvaporationRate(0);
			else
				algo.setEvaporationRate(0.01*j);
			runTest(i,a, algo, results, algo.getEvaporationRate());
		}
		algo.resetEvaporationRate();
		algo.resetEssentialParts();
		cout<<"file has been generated"<<endl;

		dumpCSV("EvaporationRate" + to_string(i), "EvaporationRate", results);
		results.clear();
		
		//tests for changed smoothing lowest
		cout<<"test 3: "<<endl;
		for(int j=0; j<9; j++){
			if(i==0) algo.setSmoothingLowest(0);
			else algo.setSmoothingLowest(0.01*j);
			runTest(i,a, algo, results, algo.getSmoothingLowest());
		}
		algo.resetSmoothingLowest();
		algo.resetEssentialParts();
		cout<<"file has been generated"<<endl;

		dumpCSV("SmoothingLowest"+to_string(i), "SmoothingLowest", results);
		results.clear();
		//tests for changed smoothing log based
		cout<<"test 4: "<<endl;
		for(int j=1; j<=8; j++){
			algo.setSmoothingLogBase(j);	
			runTest(i,a, algo, results, algo.getSmoothingLogBase());
		}
		algo.resetSmoothingLogBase();
		algo.resetEssentialParts();
		cout<<"file has been generated"<<endl;

		dumpCSV("SmoothingLogBase"+to_string(i), "SmoothingLogBase", results);
		results.clear();

	  	for(int j=0; j < 9; j++){
			if(j == 0) a.falseNegativeThreshold = 0.01;
			else a.falseNegativeThreshold = 0.01*(j+1);
			cout<<j<<endl;
			runTest(i,a, algo, results,a.falseNegativeThreshold, true);
		}
		algo.resetEvaporationRate();
		algo.resetEssentialParts();
		cout<<"file has been generated"<<endl;

		dumpCSV("falseNegatives" + to_string(i), "falseNegatives", results);
		results.clear();
		for(int j=0; j < 9; j++){
			if(j == 0) a.falsePositiveThreshold = 0.01;
			else a.falsePositiveThreshold = 0.01*(j+1);
			runTest(i,a, algo, results,a.falsePositiveThreshold, true);
		}
		algo.resetEvaporationRate();
		algo.resetEssentialParts();
		cout<<"file has been generated"<<endl;
		dumpCSV("falsePositives" + to_string(i), "falsePositives", results);
		results.clear();
		
		dumpCSV("falseNegatives" + to_string(i), "falseNegatives", results);
		results.clear();
		for(int j=0; j < 9; j++){
			algo.stopTime = (double)(j+1);
			runTest(i,a, algo, results,a.falsePositiveThreshold, true);
		}
		algo.stopTime = (double)(5);
		algo.resetEvaporationRate();
		algo.resetEssentialParts();
		cout<<"file has been generated"<<endl;
		dumpCSV("stopTime" + to_string(i), "stopTime", results);
		results.clear();
	}
}

void testSingleChaoticInstance(int n){
	// Sequence a = Sequence("ATTCAGAAGTATGGCACCCACTTCTGCCTACGTGAGTAGCTAGCGCCATTAGCTAGCCAATCGAAGGTGGGTGTGTGCGTGGCATTGGGGGCATTACCTCACGGATTGGCCGAGGTCGTATCTGAAGCCTTTGCCGAGGGAATCGTGACCCGGGTGGTAAAGTGAAGAGTAATTCTAATCTGCCTGACCATCGACAAAAA");
	Sequence a = Sequence(n);
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

}

void testSinglePickyInstance(int n){
	// Sequence a = Sequence("ATTCAGAAGTATGGCACCCACTTCTGCCTACGTGAGTAGCTAGCGCCATTAGCTAGCCAATCGAAGGTGGGTGTGTGCGTGGCATTGGGGGCATTACCTCACGGATTGGCCGAGGTCGTATCTGAAGCCTTTGCCGAGGGAATCGTGACCCGGGTGGTAAAGTGAAGAGTAATTCTAATCTGCCTGACCATCGACAAAAA");
	Sequence a = Sequence(n);
	cout<<"Seqence: "<<endl<<a.seq<<endl;
	a.shredSequence();
	a.createGraphWithFixedCover();
	AntColonyOptimization algo(a);
	// algo.numOfAnts=1;
	// algo.stopTime = 0.01;
	vector<pair<double,string>> paths = algo.commenceACO<pickyAnt>();
	cout<<"Seqence: "<<endl<<a.seq<<endl;
	cout<<"results:"<<endl;
	for(auto it : paths ){
		cout<<"solution distance: "<<levenshteinFullMatrix(a.seq,it.second)<<endl;
		cout<<it.first<<"\t"<<it.second<<endl;
	}

}

int main(int argc, char * argv[])
{
	fs::path logsPath = "logs";
	if(!fs::exists(LOGS_PATH))
		fs::create_directory(LOGS_PATH);
	if(!fs::exists(TESTS_PATH))
		fs::create_directory(TESTS_PATH);
	FLAGS_logbufsecs = 0;
	FLAGS_logbuflevel = -1;
	FLAGS_log_dir = filesystem::current_path().append(logsPath.string()).string();
	google::InitGoogleLogging(argv[0]);
	srand(time(0));


	testSinglePickyInstance(1000);
	// testSingleChaoticInstance(1000);
	//assert(argc == 3);
	//int num = atoi(argv[2]);
	//assert((string)(argv[1]) == "c" || (string)(argv[1]) == "p" );
	//if((string)(argv[1]) == "c") testSingleChaoticInstance(num);
	//else testSinglePickyInstance(num);
	
	return 0;
}
