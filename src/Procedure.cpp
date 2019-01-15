#include <sstream>
#include <stdlib.h>
#include <string>
#include <fstream>

using namespace std;

#include "Generic.h"
#include "BuchiSet.h"
#include "TreeNode.h"
#include "Tree.h"
#include "my_utility.h"
#include "Procedure.h"

sp<Tree> initial_creation(size_t n, BuchiSet& I, BuchiSet& F) {
    bool cond1 = (*I.intersect(F)).is_empty();
    bool cond2 = (*I.unionof(F)) == F;
    vec v = [] (size_t n) {vec temp; 
        for (size_t i = 1; i < 2*n; i++) temp.push_back(i+1); 
        return temp; } (n);
    sp<pq> name_giver(new pq(v.begin(), v.end()));
    sp<BuchiSet> I_ (new BuchiSet(I));
    sp<TreeNode> root;
    if (cond1) root.reset(new TreeNode(n, 1, I_, false));
    else {
        if (cond2) root.reset(new TreeNode(n, 1, I_, true));
        else {
            root.reset(new TreeNode(n, 1, I_, false));
            (*root).create(F, name_giver); // name_giver modified
        }
    }
    sp<Tree> T0(new Tree(n, root, name_giver));
    return T0;
}

std::tuple<sp<std::vector<Tree>>,sp<dtransition>>
tree_closure(size_t n, size_t sig, const ftransition& tau,
    BuchiSet& I, BuchiSet& F) {
    sp<dtransition> delta(new dtransition);
    sp<Tree> T0 = initial_creation(n, I, F);
    std::unordered_set<Tree> visited;
    std::queue<sp<Tree>> curr;
    (*T0).set_num(1);
    curr.push(T0);
    visited.insert(*T0);
    int num = 2;
    int a;
    typename std::unordered_set<Tree>::iterator refer;
    while (curr.size() > 0)
    {
        sp<Tree> cT = curr.front(); curr.pop();
        for (size_t i = 0; i < sig; i++) {
            a = i+1;
            sp<Tree> nT = (*cT).transition(F, a, tau);
            refer = visited.find(*nT);
            if (refer == visited.end()) {
                (*nT).set_num(num);
                visited.insert(*nT);
                curr.push(nT);
                (*delta).insert(std::pair<fpair, int>
                    (fpair((*cT).get_num(), a), num)); 
                num += 1;
            }
            else {
                (*delta).insert(std::pair<fpair, int>
                    (fpair((*cT).get_num(), a), 
                    (*refer).get_num())); 
            }
        }
    }
    std::vector<Tree> trees;
    trees.insert(trees.end(), visited.begin(), visited.end());
    auto cmp = [] (Tree A, Tree B) 
        { return A.get_num() < B.get_num(); };
    std::sort(trees.begin(), trees.end(), cmp); 
    sp<std::vector<Tree>> trees_ (new std::vector<Tree>(trees));
    return std::make_tuple(trees_, delta);
}

sp<std::vector<rabin_pair>> rabin_pairs(size_t n, 
    std::vector<Tree>& trees) {
    std::vector<vec> Lset(2*n, vec());
    std::vector<vec> Rset(2*n, vec());
    int tree_ind;
    for (size_t t = 0; t < trees.size(); t++) {
        tree_ind = t+1;
        vec usage = *trees.at(t).get_name_usage();
        for (size_t i = 0; i < 2*n; i++) {
            if (usage.at(i) == 0) Lset.at(i).push_back(tree_ind); 
            if (usage.at(i) == 2) Rset.at(i).push_back(tree_ind);
        }
    }
    std::vector<rabin_pair> pairs;
    for (size_t i = 0; i < 2*n; i++) {
        if (Rset.at(i).size() > 0) {
            rabin_pair p = rabin_pair(Lset.at(i), Rset.at(i));
            if (std::find(pairs.begin(), pairs.end(), p) == pairs.end()) {
                pairs.push_back(p);
            }
        }
    }
    sp<std::vector<rabin_pair>> pairs_ (new std::vector<rabin_pair>(pairs));
    return pairs_;
}

std::tuple<size_t, size_t, sp<dtransition>, sp<std::vector<rabin_pair>>,
    sp<std::vector<Tree>>> 
    Safra(size_t n, size_t sig, 
    const ftransition& tau, BuchiSet I, BuchiSet F) {
    auto tup = tree_closure(n, sig, tau, I, F);
    sp<std::vector<Tree>> trees = std::get<0>(tup);
    sp<dtransition> delta = std::get<1>(tup);
    sp<std::vector<rabin_pair>> pairs = rabin_pairs(n, *trees);
    size_t rabin_n = (*trees).size();
    size_t rabin_sig = sig;
    return std::make_tuple(rabin_n, rabin_sig, delta, pairs, trees);
}

// destructive split
sp<std::vector<std::string>> split (std::string& s, std::string delim) {
    std::vector<std::string> v;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delim)) != std::string::npos) {
        token = s.substr(0, pos);
        v.push_back(token);
        s.erase(0, pos + delim.length());
    }
    v.push_back(s);
    sp<std::vector<std::string>> v_(new std::vector<std::string>(v));
    return v_;
}

std::tuple<size_t, size_t, sp<ftransition>, sp<vec>, sp<vec>> 
    ingest(const char* filename) {
    ifstream file;
    std::string line;
    file.open(filename);
    size_t n, sig, tau_size;
    ftransition tau_;
    vec I_, F_;
    if (file.is_open()) {
        std::getline(file, line); // BUECHI
        std::getline(file, line); // Rabin Size: #
        std::getline(file, line); // Rabin transitions: #
        std::getline(file, line); // Number of states
        std::getline(file, line); // !
        n = atoi(line.c_str()); 
        std::getline(file, line); // Alphabet size
        std::getline(file, line); // !
        sig = atoi(line.c_str()); 
        std::getline(file, line); // Number of transitions
        std::getline(file, line); // !
        tau_size = atoi(line.c_str());
        std::getline(file, line); // begin transitions
        std::vector<std::string> tokens;
        for (size_t t = 0; t < tau_size; t++) {
            std::getline(file, line); // !
            tokens = *split(line, "  ");
            int state = atoi(tokens.at(0).c_str()); 
            int ch = atoi(tokens.at(1).c_str());
            int nstate = atoi(tokens.at(2).c_str());
            fpair temp = fpair(state, ch);
            if (tau_.find(temp) == tau_.end())
                tau_.insert(std::pair<fpair,vec>(temp, vec({ nstate })));
            else
                tau_.at(temp).push_back(nstate);
        }
        std::getline(file, line); // end transitions
        std::getline(file, line); // Buechi initial
        std::getline(file, line); // !
        tokens = *split(line, " ");
        int e;
        for (size_t i = 0; i < tokens.size(); i++) { 
            e = atoi(tokens.at(i).c_str());
            if (e != 0) I_.push_back(e);
        }
        std::getline(file, line); // Buechi final
        std::getline(file, line); // !
        tokens = *split(line, " ");
        for (size_t i = 0; i < tokens.size(); i++) { 
            e = atoi(tokens.at(i).c_str());
            if (e != 0) F_.push_back(e);
        }
        std::getline(file, line); // Buechi eof
    }
    file.close();
    sp<ftransition> tau (new ftransition(tau_));
    sp<vec> I (new vec(I_));
    sp<vec> F (new vec(F_));
    return std::make_tuple(n, sig, tau, I, F);
}

void output (const char* outname, const char* inpname, bool tree_view,
        stringstream& buffer, size_t rabin_n, size_t rabin_sig, 
        dtransition& rabin_delta, std::vector<rabin_pair>& rabin_ps, 
        std::vector<Tree>& trees) {
    ofstream file(outname);
    file << "RABIN\n";
    file << "# Buechi File: " << inpname << "\n";
    file << "# Number of states\n" << rabin_n << "\n";
    file << "# Alphabet size\n" << rabin_sig << "\n";
    size_t delta_size = rabin_delta.size();
    file << "# Number of transitions\n" << delta_size << "\n";
    file << "# begin transitions\n";
    for (std::pair<fpair, int> e : rabin_delta) {
        int state = e.first.first;
        int ch = e.first.second;
        int nstate = e.second;
        file << state << " " << ch << " " << nstate << "\n";
    }
    file << "# end transitions\n";
    size_t pairs_size = rabin_ps.size();
    file << "# Number of Rabin pairs\n" << pairs_size << "\n";
    file << "# Rabin pairs\n";
    vec L, R;
    for (size_t i = 0; i < pairs_size; i++) {
        L = rabin_ps.at(i).first;
        R = rabin_ps.at(i).second;
        for (size_t j = 0; j < L.size(); j++) {
            file << L.at(j) << " ";
        }
        file << "| ";
        for (size_t j = 0; j < R.size()-1; j++) {
            file << R.at(j) << " ";
        }
        file << R.at(R.size()-1) << "\n";
    }
    file << "# Rabin eof\n";
    file.close();

    if (tree_view) {
        buffer << "Printing Trees...\n";
        for (size_t t = 0; t < rabin_n; t++) {
            buffer << "NUM: " << t << "\n";
            print_Tree(trees.at(t), buffer);
            buffer << "\n";
        }
        buffer << "Printing Trees Finished\n\n";
    }
}

void Procedure::Buchi_Rabin(std::stringstream& buffer,
        const char*& inpname, const char*& outname, bool tree_view) {
    auto buchi_tup = ingest(inpname);
    size_t buchi_n = std::get<0>(buchi_tup);
    size_t buchi_sig = std::get<1>(buchi_tup);
    ftransition buchi_tau = *std::get<2>(buchi_tup);
    BuchiSet buchi_I = BuchiSet(buchi_n, *std::get<3>(buchi_tup));
    BuchiSet buchi_F = BuchiSet(buchi_n, *std::get<4>(buchi_tup));
    auto rabin_tup = Safra(buchi_n, buchi_sig, buchi_tau, buchi_I, buchi_F);
    size_t rabin_n = std::get<0>(rabin_tup); 
    size_t rabin_sig = std::get<1>(rabin_tup); 
    dtransition rabin_delta = *std::get<2>(rabin_tup); 
    std::vector<rabin_pair> rabin_ps = *std::get<3>(rabin_tup);
    std::vector<Tree> trees = *std::get<4>(rabin_tup);
    output(outname, inpname, tree_view, buffer,
        rabin_n, rabin_sig, rabin_delta, rabin_ps, trees);
}

