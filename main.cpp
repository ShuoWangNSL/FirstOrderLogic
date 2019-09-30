#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <queue>
#include <stack>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>
#include<cmath>
#include <random>
#include <time.h>
#include<sstream>
#include <cctype>
#include <algorithm>
#include "Predicate.h"
#include "Clause.h"

using namespace std;

class Solution{

public:
    int clausenum = 0;
    int queriesnum = 0;
    vector <Clause> clauses;
    vector <Clause> queries;
    int predtotalnum;

    map <string, int> predname2seq;
    set <string> clausesset;

    bool check(int queryseq);
    void parse();
};

bool Solution::check(int queryseq) {
    bool res = false;
    if (queries[queryseq].preds[0].predseq < 0){
        return false;
    }
    Clause query = queries[queryseq];
    query.preds[0].isPositive = !query.preds[0].isPositive;
    query.normalize(true);
    vector <Clause> clauses = this->clauses;
    clauses.push_back(query);
    for(auto &p : clauses){
        p.printstr();
        clausesset.insert(p.clausestr);
    }
    int loop = max(10000, 10 * clausenum);
    int cursize;
    int presize = 0;
    while (true){
        cursize = clauses.size();
        for (int i = 0; i < cursize; i++){
            for (int j = max(i + 1, presize); j < cursize; j++){
//                cout<<"Clause"<<i<<" resolution with Clause"<<j<<endl;
                clauses[i].resolution(clauses[j], clauses, clausesset, res);
                if (res){
                    clauses.resize(clausenum);
                    clausesset.clear();
                    return true;
                }
                if (clauses.size() > loop ){
                    clauses.resize(clausenum);
                    clausesset.clear();
                    return false;
                }
            }
        }



        if (clauses.size() == cursize){
            clauses.resize(clausenum);
            clausesset.clear();
            return false;
        }

        presize = cursize;
    }

    //return true;
}


vector<string> split(const string &s, const string & sepstring, vector<string> &res){

    set <char> sep;
    for (auto c : sepstring){
        sep.insert(c);
    }

    int i = 0;
    while(i != s.size()){
        while (i != s.size() && sep.count(s[i]) == 1){
            i++;
        }

        int j = i;
        while ( j != s.size() && sep.count(s[j]) == 0 ) {
            j++;
        }

        if(i != j){// i ==j means i == s.size() means all the chars are sep
            res.push_back(s.substr(i, j-i));
            i = j;
        }
    }
    return res;
}

void Solution::parse() {
    ifstream f("input.txt");
    if (!f){
        cout<<"Fail to read input.txt"<<endl;
        return;
    }
    string size_q, size_c;
    getline(f, size_q);
    queriesnum = atoi(size_q.c_str());
    queries.resize(queriesnum);
    string * querystr = new string[queriesnum];
    for (int i = 0; i < queriesnum; i++){
        getline(f, querystr[i]);
    }
    getline(f, size_c);
    clausenum = atoi(size_c.c_str());
    clauses.resize(clausenum);
    string * clausestr = new string[clausenum];
    for (int i = 0; i < clausenum; i++){
        getline(f, clausestr[i]);
    }
    f.close();
    vector < vector <string> > splited_clauses;
    splited_clauses.resize(clausenum);
    for (int i = 0; i < clausenum; i ++){
        string sepstring = "| ";
        split(clausestr[i], sepstring , splited_clauses[i]);
        for (auto p : splited_clauses[i]){
            cout<<p<<endl;
        }
    }
    delete []clausestr;

    clauses.resize(clausenum);
    int predcnt = 0;
    int varcnt = 0;
    for (int i = 0; i < clausenum; i++ ){
        varcnt = 0;
        map <string, int> varname2seq;
        int curprednum = splited_clauses[i].size();
        clauses[i].prednum = curprednum;
        clauses[i].preds.resize(curprednum);

        for (int j = 0; j < curprednum; j++){
            vector <string> splited_predargs;
            string sep = "(,)";
            split(splited_clauses[i][j], sep, splited_predargs);

            if (splited_clauses[i][j][0] == '~'){
                clauses[i].preds[j].isPositive = false;
                clauses[i].preds[j].predname = splited_predargs[0].substr(1, splited_predargs[0].size()-1);
                if ( predname2seq.count(clauses[i].preds[j].predname) == 0 ){
                    predcnt++;
                    predname2seq.insert( make_pair(clauses[i].preds[j].predname, predcnt));
                    clauses[i].preds[j].predseq = predcnt;
                } else {
                    clauses[i].preds[j].predseq = predname2seq[clauses[i].preds[j].predname];
                }

            } else {
                clauses[i].preds[j].isPositive = true;
                clauses[i].preds[j].predname = splited_predargs[0];
                if ( predname2seq.count(clauses[i].preds[j].predname) == 0 ){
                    predcnt++;
                    predname2seq.insert( make_pair(clauses[i].preds[j].predname, predcnt));
                    clauses[i].preds[j].predseq = predcnt;
                } else {
                    clauses[i].preds[j].predseq = predname2seq[clauses[i].preds[j].predname];
                }
            }

            clauses[i].preds[j].argsnum = splited_predargs.size() - 1;
            clauses[i].preds[j].args.resize(clauses[i].preds[j].argsnum);
            clauses[i].preds[j].isvar.resize(clauses[i].preds[j].argsnum);
            for (int m = 0; m < clauses[i].preds[j].argsnum; m++){
                if ( isupper( splited_predargs[m + 1][0] ) ){
                    clauses[i].preds[j].isvar[m] = false;
                    clauses[i].preds[j].args[m] = splited_predargs[m + 1];
                } else {
                    clauses[i].preds[j].isvar[m] = true;
                    if (varname2seq.count(splited_predargs[m + 1]) == 0) {
                        varcnt++;
                        varname2seq.insert( make_pair( splited_predargs[m + 1], varcnt) );
                        stringstream ss;
                        ss<<varcnt;
                        clauses[i].preds[j].args[m] = ss.str();
                    } else {
                        stringstream ss;
                        ss<<varname2seq[splited_predargs[m + 1]];
                        clauses[i].preds[j].args[m] = ss.str();
                    }
                }
            }
        }
        clauses[i].varrange = varcnt;
    }

    for (int i = 0; i <clausenum; i++){
        clauses[i].normalize(false);
    }

    for (int i = 0; i < queriesnum; i++ ){

        queries[i].prednum = 1;
        queries[i].preds.resize(1);

        vector <string> splited_predargs;
        string sep = "(,)";
        split(querystr[i], sep, splited_predargs);

        if (querystr[i][0] == '~'){
            queries[i].preds[0].isPositive = false;
            queries[i].preds[0].predname = splited_predargs[0].substr(1, splited_predargs[0].size()-1);
            if ( predname2seq.count(queries[i].preds[0].predname) == 0 ){
                cout<<"Undefined Predicate name!"<<endl;
                queries[i].preds[0].predseq = -1;
                return;
            } else {
                queries[i].preds[0].predseq = predname2seq[queries[i].preds[0].predname];
            }

        } else {
            queries[i].preds[0].isPositive = true;
            queries[i].preds[0].predname = splited_predargs[0];
            if ( predname2seq.count(queries[i].preds[0].predname) == 0 ){
                cout<<"Undefined Predicate name!"<<endl;
                queries[i].preds[0].predseq = -1;
                return;
            } else {
                queries[i].preds[0].predseq = predname2seq[queries[i].preds[0].predname];
            }
        }
        queries[i].preds[0].argsnum = splited_predargs.size() - 1;
        queries[i].preds[0].args.resize(queries[i].preds[0].argsnum);
        queries[i].preds[0].isvar.resize(queries[i].preds[0].argsnum);
        for (int m = 0; m < queries[i].preds[0].argsnum; m++){
            if ( isupper( splited_predargs[m + 1][0] ) ){
                queries[i].preds[0].isvar[m] = false;
                queries[i].preds[0].args[m] = splited_predargs[m + 1];
            } else {
                cout<<"Arguments of Quety is not Constant!"<<endl;
                return;
            }
        }

    }
    delete []querystr;

    for (int i = 0; i < queriesnum; i++){
        queries[i].normalize(true);
    }
}


int main() {

    Solution solution;
    solution.parse();

    ofstream f("./output.txt");
    if(!f)
        cout<<"Fail to Create output.txt"<<endl;
    for (int i = 0; i < solution.queriesnum; i++){
        if (solution.check(i)){
            cout<<"TRUE"<<endl;
            f<<"TRUE"<<endl;
        } else{
            cout<<"FALSE"<<endl;
            f<<"FALSE"<<endl;
        };
    }
    f.close();
    return 0;
}