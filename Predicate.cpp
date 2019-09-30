#include "Predicate.h"

bool Predicate::isequal(Predicate &p2) {
    Predicate &p1 = *this;
    if (p1.predseq != p2.predseq){
        return false;
    }
    if (p1.isPositive != p2.isPositive){
        return false;
    }
    for (int i = 0; i < p1.argsnum; i++){
        if (p1.args[i] != p2.args[i]){
            return false;
        }
    }
    return true;
}
