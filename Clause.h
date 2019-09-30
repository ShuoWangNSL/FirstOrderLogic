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

using namespace std;

class Clause{
public:
    bool resolution(Clause &C2, vector <Clause> &clauses, set <string> &clausesset, bool &res);
    bool unify_pred(int s, int t, Clause C1, Clause C2, Clause &C, bool &res);
    bool isequal(Clause &C2);
    void printstr();
    void normalize(bool isQuery);

    string clausestr;
    int prednum = 0;
    vector <Predicate> preds; // soretd by predname
    int varrange = 0;
    //int varnum;
    //vector <string> varnames;
    //map <string, int> varname2seq;
};