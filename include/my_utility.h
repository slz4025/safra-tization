#ifndef my_utility_H
#define my_utility_H

#include "Generic.h"
#include "BuchiSet.h"
#include "TreeNode.h"
#include "Tree.h"

void print_set(set<int>& s, std::stringstream& o) {
    o << "{ ";
    for (int e : s) {
        o << e << " ";
    }
    o << "}";
}
void print_set(const set<int>& s, std::stringstream& o) {
    o << "{ ";
    for (int e : s) {
        o << e << " ";
    }
    o << "}";
}

void print_vector(vec& v, std::stringstream& o) {
    size_t n = v.size();
    o << "< ";
    for (size_t i = 0; i < n; ++i) {
        o << v.at(i) << " ";
    }
    o << ">";
}
void print_vector(const vec& v, std::stringstream& o) {
    size_t n = v.size();
    o << "< ";
    for (size_t i = 0; i < n; ++i) {
        o << v.at(i) << " ";
    }
    o << ">";
}

void print_pair(fpair& p, std::stringstream& o) {
    o << "( state: ";
    o << p.first;
    o << " , char: ";
    o << p.second;
    o << ")";
}
void print_map(const ftransition& map, std::stringstream& o) {
    o << "{ ";
    for (std::pair<fpair, vec> e : map) {
        o << "\t";
        print_pair(e.first, o);
        o << " : ";
        print_vector(e.second, o); 
        o << "\n";
    } 
    o << "}";
}
void print_map2(const dtransition& map, std::stringstream& o) {
    o << "{ ";
    for (std::pair<fpair, int> e : map) {
        o << "\t";
        print_pair(e.first, o);
        o << " : " << e.second << "\n";
    } 
    o << "}";
}

void print_pq(std::priority_queue<int, vec, std::greater<int>>& pq, 
        std::stringstream& o) {
    o << "PQ: < ";
    std::priority_queue<int> temp;
    while (!pq.empty()) {
        int e = pq.top();
        pq.pop();
        o << e << " ";
        temp.push(e);
    }
    // refill
    while (!temp.empty()) {
        pq.push(temp.top());
        temp.pop();
    }
    o << ">";
}

void print_rp(rabin_pair p, std::stringstream &o) {
    o << "( left: ";
    print_vector(p.first, o);
    o << " , right: ";
    print_vector(p.second, o);
    o << ")";
}

void print_rps(std::vector<rabin_pair>& pairs, std::stringstream &o) {
    o << "<\n";
    for (size_t i = 0; i < pairs.size(); i++) {
        o << "\t";
        print_rp(pairs.at(i), o);
        o << "\n";
    }
    o << ">";
}

void print_BuchiSet (BuchiSet& B, std::stringstream &out) {
    out << "BuchiSet | " << "\tn: " << B.get_size() << "\tr: " << B.get_r(); 
    out << "\ts: ";
    print_vector(B.get_set(), out);
}
void print_BuchiSet (const BuchiSet& B, std::stringstream &out) {
    out << "BuchiSet | " << "\tn: " << B.get_size() << "\tr: " << B.get_r(); 
    out << "\ts: ";
    print_vector(B.get_set(), out);
}

void print_TreeNode_helper (TreeNode& TN, 
                            std::stringstream &out, 
                            std::string indent) {
    out << indent << "TreeNode | " << "\tn: " << TN.get_n() 
                                   << "\tname: " << TN.get_name()
                                   << "\tmark: " << TN.get_mark()
                                   << "\n" << indent << "set: ";
    print_BuchiSet(*TN.get_set(), out);
    out << "\n" << indent << "children: \n";
    std::vector<sp<TreeNode>> children = TN.get_children();
    for (size_t i = 0; i < children.size(); i++) {
        print_TreeNode_helper(*children.at(i), out, indent + "\t\t");
        out << "\n";
    }
}
void print_TreeNode (TreeNode& TN, std::stringstream& out) {
    print_TreeNode_helper (TN, out, "");
}
void print_TreeNode_helper (const TreeNode& TN, 
                            std::stringstream &out, 
                            std::string indent) {
    out << indent << "TreeNode | " << "\tn: " << TN.get_n() 
                                   << "\tname: " << TN.get_name()
                                   << "\tmark: " << TN.get_mark()
                                   << "\n" << indent << "set: ";
    print_BuchiSet(*TN.get_set(), out);
    out << "\n" << indent << "children: \n";
    std::vector<sp<TreeNode>> children = TN.get_children();
    for (size_t i = 0; i < children.size(); i++) {
        print_TreeNode_helper(*children.at(i), out, indent + "\t\t");
        out << "\n";
    }
}
void print_TreeNode (const TreeNode& TN, std::stringstream& out) {
    print_TreeNode_helper (TN, out, "");
}

void print_Tree (Tree& T, std::stringstream& out) {
    out << "Tree | " << "\tn: " << T.get_n()
                     << "\tname giver: ";
    print_pq(*T.get_name_giver(), out);
    out << "\n" << "tree: \n";
    print_TreeNode_helper(*T.get_root(), out, "\t"); // one-layer-depth
}
void print_Tree (const Tree& T, std::stringstream& out) {
    out << "Tree | " << "\tn: " << T.get_n()
                     << "\tname giver: ";
    print_pq(*T.get_name_giver(), out);
    out << "\n" << "tree: \n";
    print_TreeNode_helper(*T.get_root(), out, "\t"); // one-layer-depth
}

namespace std {
    template <>
    struct hash<BuchiSet> {
        size_t operator() (const BuchiSet& B) const {
            stringstream buffer;
            print_BuchiSet(B, buffer);
            return hash<string> () (buffer.str());
        }
    };
}

namespace std {
    template <>
    struct hash<TreeNode> {
        size_t operator() (const TreeNode& TN) const {
            stringstream buffer;
            print_TreeNode(TN, buffer);
            return hash<string> () (buffer.str());
        }
    };
}

namespace std {
    template <>
    struct hash<Tree> {
        size_t operator() (const Tree& T) const {
            stringstream buffer;
            print_Tree(T, buffer);
            return hash<string> () (buffer.str());
        }
    };
}

#endif
