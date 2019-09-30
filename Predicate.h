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

using namespace std;

class Predicate{
public:
    bool isPositive;
    string predname;
    int predseq;
    int argsnum;
    vector <string> args;
    vector <bool> isvar;
    bool isequal(Predicate &p2);
};


