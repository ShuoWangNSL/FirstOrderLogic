#include "Clause.h"
using namespace std;

bool comp(const Predicate &p1, const Predicate &p2){
    if (p1.predseq != p2.predseq){
        return p1.predseq < p2.predseq;
    }
    if (p1.isPositive != p2.isPositive){
        return p2.isPositive;
    }

    for (int j = 0; j < p1.argsnum; j++){
        if (p1.isvar[j] != p2.isvar[j]){
            return p2.isvar[j];
        }
    }

    for (int j = 0; j < p1.argsnum; j++){
        if (p1.args[j] != p2.args[j]){
            return p1.args[j] < p2.args[j];
        }
    }
}

void Clause::normalize(bool isQuery){
    if (isQuery) {
        clausestr = "";
        for(int i = 0; i < prednum; i++){
            if (preds[i].isPositive == false){
                clausestr = clausestr + "~";
            }
            clausestr = clausestr + preds[i].predname + "(";
            for (int j = 0; j < preds[i].argsnum; j++){
                clausestr = clausestr + preds[i].args[j] + ",";
            }
            clausestr = clausestr + ")|";
        }
        return;
    }
    sort(preds.begin(),preds.end(),comp);

    vector <int> duppred;
    int i = 0;
    while (i < prednum - 1){
        int j = i + 1;
        while (j < prednum && preds[i].isequal(preds[j]) ){
            duppred.push_back(j);
            j++;
        }
        i = j;
    }
    if (duppred.size() != 0 ){
        auto it = preds.begin();
        it = it + duppred[0];
        for (int i = 0; i < duppred.size(); i++){
            it = preds.erase(it);
            if (i < duppred.size() -  1){
                it = it + duppred[i + 1] - duppred[i] - 1;
            }
        }
        prednum = preds.size();
    }


    int varcnt = 0;
    map <string,string> varseq;
    for (int i = 0; i < prednum; i++){
        for (int j = 0; j< preds[i].argsnum; j++){
            if (preds[i].isvar[j] == true){
                if ( varseq.count(preds[i].args[j]) == 1){
                    preds[i].args[j] = varseq[preds[i].args[j]];
                } else{
                    varcnt++;
                    stringstream ss;
                    ss<<varcnt;
                    varseq.insert(make_pair(preds[i].args[j],ss.str()));
                    preds[i].args[j] = ss.str();
                }
            }
        }
    }
    varrange = varcnt;

    clausestr = "";
    for(int i = 0; i < prednum; i++){
        if (preds[i].isPositive == false){
            clausestr = clausestr + "~";
        }
        clausestr = clausestr + preds[i].predname + "(";
        for (int j = 0; j < preds[i].argsnum; j++){
            clausestr = clausestr + preds[i].args[j] + ",";
        }
        clausestr = clausestr + ")|";
    }

}

void Clause::printstr(){

    cout<<"this clausestr: "<<clausestr<<endl;

}

bool Clause::isequal(Clause &C2) {
    return this->clausestr == C2.clausestr;
}

bool Clause::resolution(Clause &C2, vector <Clause> &clauses, set <string> &clausesset, bool &res) {
    //Predicate p1,p2;
    Clause &C1 = *this;
    vector < pair<int,int> > pairs;
    //int s,t;
    for (int i = 0; i < C1.prednum; i++){
        Predicate &pred1 = C1.preds[i];
        for (int j = 0; j < C2.prednum; j++){
            Predicate &pred2 = C2.preds[j];
            if ( pred1.predseq == pred2.predseq && pred1.isPositive != pred2.isPositive ){
                pairs.push_back(make_pair(i,j));
            }
        }
    }
    int cnt = pairs.size();
    if (cnt == 0) {
        return false;
    }
    Clause * Cs = new Clause[cnt];
    bool *flag = new bool[cnt];
    for (int i = 0; i < cnt; i++){
        flag[i] = unify_pred(pairs[i].first, pairs[i].second, C1, C2, Cs[i], res);
        if (res){
            return true;
        }
    }
    for (int i = 0; i < cnt; i++) {
        if ( flag[i] && clausesset.count(Cs[i].clausestr) == 0 /*Cs[i].prednum != 0*/) {
            //cout<<"clauses"<<i<<"predum is "<<Cs[i].prednum<<endl;
            cout<<"Clause No."<<clauses.size()<<endl;
            Cs[i].printstr();
            clauses.push_back(Cs[i]);
            clausesset.insert(Cs[i].clausestr);
        }
    }
    delete [] Cs;
    delete [] flag;
}

bool Clause::unify_pred(int s, int t, Clause C1, Clause C2, Clause &C, bool &res){
    Predicate &p1 = C1.preds[s];
    Predicate &p2 = C2.preds[t] ;
    unordered_map <string,string> subst1;
    unordered_map <string,string> subst2;
    for (int i = 0; i < p1.argsnum; i++ ){
        if (p1.isvar[i] && p2.isvar[i]){
            return false;
        }
        if(!p1.isvar[i] && !p2.isvar[i]){
            if (p1.args[i] != p2.args[i]){
                return false;
            }
        } else if(p1.isvar[i]){
            if ( subst1.count(p1.args[i]) == 1 && subst1[p1.args[i]] != p2.args[i]){
                return  false;
            }
            subst1.insert(make_pair(p1.args[i],p2.args[i]));
        } else { //p2.isvar[i]
            if ( subst2.count(p2.args[i]) == 1 && subst2[p2.args[i]] != p1.args[i]){
                return  false;
            }
            subst2.insert(make_pair(p2.args[i],p1.args[i]));
        }
    }
    for (int i = 0; i < C1.prednum; i++){
        Predicate &pred = C1.preds[i];
        for(int j = 0; j < pred.argsnum; j++){
            if(pred.isvar[j] && subst1.count(pred.args[j]) == 1){
                pred.isvar[j] = false;
                pred.args[j] = subst1[pred.args[j]];
            }
        }
    }

    for (int i = 0; i < C2.prednum; i++){
        Predicate &pred = C2.preds[i];
        for(int j = 0; j < pred.argsnum; j++){
            if(pred.isvar[j] && subst2.count(pred.args[j]) == 1){
                pred.isvar[j] = false;
                pred.args[j] = subst2[pred.args[j]];
            } else if(pred.isvar[j]){
                int temp = atoi(pred.args[j].c_str());
                temp = temp + C1.varrange;
                stringstream ss;
                ss<<temp;
                pred.args[j] = ss.str();
            }
        }
    }

    C.varrange = C1.varrange + C2.varrange;
    C.prednum = C1.prednum + C2.prednum - 2;
    if (C.prednum > 10 || C.prednum <0){
        cout<<"prednum is wrong !"<<endl;
    }
    //C2.preds.resize(C.prednum);
    if (C.prednum == 0){
        res = true;

    }

    for (int i = 0; i < s; i++){
        Predicate &pred = C1.preds[i];
        C.preds.push_back(pred);
    }

    for (int i = s + 1; i < C1.prednum; i++){
        Predicate &pred = C1.preds[i];
        C.preds.push_back(pred);
    }

    for (int i = 0; i < t; i++){
        Predicate &pred = C2.preds[i];
        C.preds.push_back(pred);
    }

    for (int i = t + 1; i < C2.prednum; i++){
        Predicate &pred = C2.preds[i];
        C.preds.push_back(pred);
    }
    C.normalize(false);
    return true;
}