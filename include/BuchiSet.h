#ifndef BuchiSet_H
#define BuchiSet_H

#include "Generic.h"

class BuchiSet {
    size_t self_n;
    vec self_s;
    int self_r;
    public: 
        BuchiSet (size_t n, vec s); // get actual vector
        BuchiSet (const BuchiSet& B);
        ~BuchiSet();
        size_t get_size ();
        vec& get_set ();
        int get_r ();
        bool operator== (BuchiSet& other);
        sp<BuchiSet> copy ();
        bool is_empty ();
        int convert_s2r (vec& s);
        sp<BuchiSet> transition(int a, const ftransition& tau);
        sp<BuchiSet> intersect(BuchiSet& B);
        sp<BuchiSet> unionof(BuchiSet& B);
        sp<BuchiSet> diff(BuchiSet& B);
        // const versions 
        size_t get_size () const;
        vec get_set () const;
        int get_r () const;
};

BuchiSet::BuchiSet (size_t n, vec s): self_n(n) {
    self_s = s;
    self_r = convert_s2r(self_s);
}
BuchiSet::BuchiSet (const BuchiSet& B): self_n(B.get_size()) {
    self_s = vec(B.get_set()); // make copy
    self_r = convert_s2r(self_s);
}
BuchiSet::~BuchiSet(void) {}
size_t BuchiSet::get_size () { return self_n; }
vec& BuchiSet::get_set () { 
    return self_s; 
}
int BuchiSet::get_r () { return self_r; }
bool BuchiSet::operator== (BuchiSet& other) {
    return (self_n == other.get_size()) && (self_r == other.get_r());
}
bool BuchiSet::is_empty () {
    return self_r == 0;
}
// off by one!
int BuchiSet::convert_s2r (vec& s) {
    int r = 0;
    int m = 1;
    for (size_t i = 0; i < self_n; i++) {
        if (std::find(s.begin(), s.end(), i+1)!= s.end()) r += m;
        m *= 2;
    }
    return r;
}
sp<BuchiSet> BuchiSet::transition(int a, const ftransition& tau) {
    auto make_trans = [] (vec& v, int a, const ftransition& tau) {
        vec trans;
        fpair temp;
        vec res;
        for (size_t i = 0; i < v.size(); i++) {
            temp = fpair(v.at(i), a);
            if (tau.find(temp) != tau.end()) {// append
                res = tau.at(temp);
                trans.insert(trans.end(), res.begin(), res.end());
            }
        }
        sort(trans.begin(), trans.end()); // for erasing
        trans.erase(unique(trans.begin(),trans.end()),trans.end());
        return trans;
    };
    sp<BuchiSet> C (new BuchiSet(self_n, make_trans(self_s, a, tau)));
    return C;
}
sp<BuchiSet> BuchiSet::intersect(BuchiSet& B) {
    assert(self_n == B.get_size());
    auto make_inter = [] (vec& a, vec& b) {
        vec inter;
        std::set_intersection(
                a.begin(), a.end(), 
                b.begin(), b.end(),
                std::inserter(inter, inter.end()));
        return inter;
    };
    sp<BuchiSet> C (new BuchiSet(self_n, make_inter(self_s, B.get_set())));
    return C;
}
sp<BuchiSet> BuchiSet::unionof(BuchiSet& B) {
    assert(self_n == B.get_size());
    auto make_union = [] (vec& a, vec& b) {
        vec un;
        std::set_union(
                a.begin(), a.end(), 
                b.begin(), b.end(),
                std::inserter(un, un.end()));
        return un;
    };
    sp<BuchiSet> C (new BuchiSet(self_n, make_union(self_s, B.get_set())));
    return C;
}
sp<BuchiSet> BuchiSet::diff(BuchiSet& B) {
    assert(self_n == B.get_size());
    auto make_diff = [] (vec& a, vec& b) {
        vec df;
        std::set_difference(
                a.begin(), a.end(), 
                b.begin(), b.end(),
                std::inserter(df, df.end()));
        return df;
    };
    sp<BuchiSet> C (new BuchiSet(self_n, make_diff(self_s, B.get_set())));
    return C;
}

size_t BuchiSet::get_size () const { return self_n; }
vec BuchiSet::get_set () const { return self_s; }
int BuchiSet::get_r () const { return self_r; }

#endif
