#ifndef HASHSOL_H
#define HASHSOL_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <tuple>
#include <algorithm>

using namespace std;

using clustering = unordered_map<int, vector<int>>;
using overlaping = unordered_map<int, vector<float>>;


template <typename T, typename U >
unordered_map<T, U> makeHash(const vector<T>&, const vector<U>&);

template <typename T, typename U , typename V>
unordered_map<T, tuple<U, V> > makeHash(const vector<T>&, const vector<U>&, const vector<V>&);

template <typename T, typename U >
unordered_map<T,U> clusterWeights(const vector<T>&, const vector<U>&, const clustering&);

clustering storeClusters(const vector<int>&, const vector<int>&);

overlaping clusterOverlap(const clustering&, unordered_map<int, tuple<int,float>>&, unordered_map<int,float>&, vector<int>&);

unordered_map<int,int> useLabels(vector<int>&);

#endif