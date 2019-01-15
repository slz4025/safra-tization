#ifndef Generic_H
#define Generic_H

#include <iostream>
#include <array>
#include <algorithm>
#include <sstream>
#include <string>
#include <map>
#include <set>
#include <unordered_set>
#include <vector>
#include <queue>
#include <utility>
#include <memory>
#include <assert.h>

typedef std::vector<int> vec;
typedef std::pair<int, int> fpair;
typedef std::map<fpair, int> dtransition;
typedef std::map<fpair, vec> ftransition;
typedef std::pair<vec, vec> rabin_pair;
typedef std::priority_queue<int, std::vector<int>, std::greater<int> > pq;
template <typename T>
using up = std::unique_ptr<T>;
template <typename T>
using sp = std::shared_ptr<T>;

#endif
