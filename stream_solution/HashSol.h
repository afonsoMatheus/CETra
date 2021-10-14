#ifndef HASHSOL_H
#define HASHSOL_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <tuple>
#include <algorithm>

using namespace std;

typedef unordered_map<int, vector<int>> clustering;
typedef unordered_map<int, vector<float>> overlaping;


template <typename T, typename U >
unordered_map<T, U> makeHash(vector<T>, vector<U>);

template <typename T, typename U , typename V>
unordered_map<T, tuple<U, V> > makeHash(vector<T>, vector<U>, vector<V>);

template <typename T, typename U >
unordered_map<T,U> clusterWeights(vector<T>, vector<U>, clustering);

clustering storeClusters(vector<int>, vector<int>);

overlaping clusterOverlap(clustering, unordered_map<int, tuple<int,float>>, unordered_map<int,float>, vector<int>);

unordered_map<int,int> useLabels(vector<int>);

#endif