#ifndef Tree_H
#define Tree_H

#include "Generic.h"
#include "BuchiSet.h"
#include "TreeNode.h"

class Tree {
    size_t self_n;
    sp<TreeNode> self_root; // same object
    sp<pq> self_name_giver;
    int self_num;
    public:
        Tree(size_t n, sp<TreeNode> root, sp<pq> ng); 
        Tree(const Tree& T);
        void operator= (const Tree& T); 
        size_t get_n ();
        sp<TreeNode> get_root ();
        sp<pq> get_name_giver ();
        int get_num();
        void set_num(int i);
        bool operator== (Tree& other);
        //bool operator< (Tree& other);
        sp<Tree> transition (BuchiSet& F, char a, const ftransition& tau);
        sp<vec> get_name_usage (); 
        size_t get_n () const;
        sp<TreeNode> get_root () const;
        sp<pq> get_name_giver () const;
        int get_num () const;
        bool operator== (const Tree& other) const;
};


Tree::Tree(size_t n, sp<TreeNode> root, sp<pq> ng):  
    self_n(n), self_root(root), self_name_giver(ng) {
        self_num = -1; // unset
}
Tree::Tree(const Tree& T): 
    self_n(T.get_n()), self_root(T.get_root()), 
    self_name_giver(T.get_name_giver()) {
    self_num = T.get_num();
}
void Tree::operator=(const Tree& T) { 
    // n should be taken care of, can do an assert
    self_root = T.get_root(); 
    self_name_giver = T.get_name_giver();
    self_num = T.get_num();
}
size_t Tree::get_n () { return self_n; }
sp<TreeNode> Tree::get_root () { return self_root; }
sp<pq> Tree::get_name_giver () { return self_name_giver; } // pointer not ref
int Tree::get_num() { return self_num; }
void Tree::set_num(int i) { self_num = i; }
bool Tree::operator== (Tree& other) {
    return *self_root == *other.get_root();
}

/**
bool Tree::operator< (Tree& other) { // diff criteria
    return self_num < other.get_num();
}
**/
// F, tau stays fixed
sp<Tree> Tree::transition (BuchiSet& F, char a, const ftransition& tau) {
    sp<TreeNode> next (new TreeNode(*self_root)); 
    (*next).unmark();
    (*next).update(a, tau);
    sp<pq> name_giver (new pq(*self_name_giver)); // copy
    (*next).create(F, name_giver); // name_giver changed
    sp<BuchiSet> emp (new BuchiSet(self_n, {}));
    (*next).horizontal_merge(emp);
    (*next).kill_empty();
    (*next).vertical_merge();
    // untaken names
    vec v_ = [] (vec taken, size_t n) {vec temp; 
        for (size_t i = 0; i < 2*n; i++) 
            if (std::find(taken.begin(), taken.end(), i+1) 
                    == taken.end()) // taken
                temp.push_back(i+1); 
        return temp; } (*(*next).get_names(), self_n);
    sp<pq> next_name_giver (new pq(v_.begin(), v_.end()));
    sp<Tree> T (new Tree(self_n, next, next_name_giver)); // give name_giver
    return T;
}
sp<vec> Tree::get_name_usage () {
    sp<vec> name_status(new vec(2*self_n));
    (*self_root).get_name_usage(name_status);
    return name_status;
}
size_t Tree::get_n () const { return self_n; }
sp<TreeNode> Tree::get_root () const { return self_root; }
sp<pq> Tree::get_name_giver () const { return self_name_giver; }
int Tree::get_num () const { return self_num; }
bool Tree::operator== (const Tree& other) const 
    { return *self_root == *other.get_root(); }

#endif
