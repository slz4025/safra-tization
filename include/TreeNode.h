#ifndef TreeNode_H
#define TreeNode_H

#include "Generic.h"
#include "BuchiSet.h"

class TreeNode {
    size_t self_n;
    int self_name;
    sp<BuchiSet> self_B;
    bool self_mark;
    std::vector<sp<TreeNode>> children;
    public:
        TreeNode (size_t n, int name, sp<BuchiSet> B, bool m);
        TreeNode (const TreeNode& TN);
        size_t get_n ();
        int get_name ();
        sp<BuchiSet> get_set ();
        bool get_mark ();
        std::vector<sp<TreeNode>>& get_children ();
        void set_set (sp<BuchiSet> b);
        void set_children(std::vector<sp<TreeNode>> c);
        bool operator== (TreeNode& other); 
        bool is_empty (); 
        void unmark (); 
        void mark (); 
        void update (char a, const ftransition& tau); 
        void create (BuchiSet& F, sp<pq> namegiver); 
        void horizontal_merge (sp<BuchiSet> above); 
        void kill_empty (); 
        void vertical_merge (); 
        sp<vec> get_names (); 
        void get_name_usage (sp<vec> L);

        size_t get_n () const;
        int get_name () const;
        sp<BuchiSet> get_set () const;
        bool get_mark () const;
        std::vector<sp<TreeNode>> get_children () const;
};

TreeNode::TreeNode (size_t n, int name, 
    sp<BuchiSet> B, bool m): self_n(n), self_name(name), self_B(B) {
    self_mark = m; 
}
TreeNode::TreeNode (const TreeNode& TN): self_n(TN.get_n()), 
    self_name(TN.get_name()) {
    self_B = TN.get_set(); // changed pointer
    self_mark = TN.get_mark();
    // we want a deep copy
    std::vector<sp<TreeNode>> temp;
    std::vector<sp<TreeNode>> C = TN.get_children();
    for (const sp<TreeNode>& c : C) {
        sp<TreeNode> c_ (new TreeNode(*c));
        temp.push_back(c_);
    }
    children = temp;
}
size_t TreeNode::get_n () { return self_n; }
int TreeNode::get_name () { return self_name; }
sp<BuchiSet> TreeNode::get_set () { return self_B; }
bool TreeNode::get_mark () { return self_mark; }
std::vector<sp<TreeNode>>& TreeNode::get_children () { return children; }
void TreeNode::set_set(sp<BuchiSet> b) { self_B = b; }
void TreeNode::set_children(std::vector<sp<TreeNode>> c) { children = c; }
bool TreeNode::operator== (TreeNode& other) {
    std::vector<sp<TreeNode>>& oc = other.get_children();
    if (children.size() != oc.size()) return false;
    for (size_t i = 0; i < children.size(); i++) {
        if (!(*children.at(i) == *oc.at(i))) return false;
    }
    // basecase check
    return (self_n == other.get_n())
        && (self_name == other.get_name())
        && (*self_B == *other.get_set()); // done through BuchiSet, no pointers
}
bool TreeNode::is_empty () {
    return (*self_B).is_empty();
}
void TreeNode::unmark () {
    self_mark = false;
}
void TreeNode::mark () {
    self_mark = true;
}

// transformative
void TreeNode::update (char a, const ftransition& tau) {
    self_B = (*self_B).transition(a, tau); // pointer
    for (size_t i = 0; i < children.size(); i++) 
        (*children.at(i)).update(a, tau);
}
void TreeNode::create (BuchiSet& F, 
                          sp<pq> name_giver) {
    sp<TreeNode> TN;
    sp<BuchiSet> inter = (*self_B).intersect(F);
    if (!(*inter).is_empty()) {
        int name = (*name_giver).top(); (*name_giver).pop();
        // new right child
        TN.reset(new TreeNode(self_n, name, inter, true));
    }
    for (size_t i = 0; i < children.size(); i++) {
        (*children.at(i)).create(F, name_giver); // name_giver changes
    }
    if (!(*inter).is_empty())
        children.push_back(TN); // with contents
}
void TreeNode::horizontal_merge (sp<BuchiSet> above) {
    auto scour = [] (sp<BuchiSet> A, TreeNode TN) {
        sp<BuchiSet> inter = (*TN.get_set()).intersect(*A);
        sp<BuchiSet> B_ = (*TN.get_set()).diff(*inter);
        return std::make_tuple((*A).unionof(*B_), inter, B_);
    };
    std::vector<sp<BuchiSet>> aboves;
    for ( const sp<TreeNode>& c : children) { // above will change
       auto tup = scour(above, *c);
       above = std::get<0>(tup); // A
       aboves.push_back(std::get<1>(tup)); // using current above
       (*c).set_set(std::get<2>(tup));
    }
    for (size_t i = 0; i < children.size(); i++) {
        (*children.at(i)).horizontal_merge(aboves.at(i));
    }
}
void TreeNode::kill_empty () {
    std::vector<sp<TreeNode>> temp;
    for (const sp<TreeNode>& c : children) {
        if (!(*c).is_empty())
            temp.push_back(c);
    }
    children.swap(temp); // change
    for (size_t i = 0; i < children.size(); i++) {
        (*children.at(i)).kill_empty();
    }
}
void TreeNode::vertical_merge () {
    if (children.size() > 0) {
        sp<BuchiSet> un (new BuchiSet(self_n, {}));  // empty
        for (const sp<TreeNode>& c : children) {
            un = (*un).unionof(*(*c).get_set());
        }
        if (*self_B == *un) { // merge
            children.clear(); // make empty
            mark(); // set true
        }
        for (size_t i = 0; i < children.size(); i++) {
            (*children.at(i)).vertical_merge();
        }
    }
}
sp<vec> TreeNode::get_names () {
    sp<vec> temp (new vec({ self_name })); // include self
    for (const sp<TreeNode>& c : children) {
        sp<vec> tc = (*c).get_names();
        (*temp).insert((*temp).end(), (*tc).begin(), (*tc).end());
    }
    return temp;
}
void TreeNode::get_name_usage (sp<vec> L) { // in array, off by one
    (*L).at(self_name - 1) = self_mark ? 2 : 1;
    for (const sp<TreeNode>& c : children)
        (*c).get_name_usage(L);
} 

size_t TreeNode::get_n () const { return self_n; }
int TreeNode::get_name () const { return self_name; }
sp<BuchiSet> TreeNode::get_set () const { return self_B; }
bool TreeNode::get_mark () const { return self_mark; }
std::vector<sp<TreeNode>> TreeNode::get_children () const 
{ return children; }

#endif
