#ifndef HASHSOL_H
#define HASHSOL_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <tuple>

using namespace std;

typedef unordered_map<int, vector<int>> clustering;
typedef unordered_map<int, vector<float>> overlaping;


template <typename T, typename U >
unordered_map<T, U> makeHash(vector<T> v1, vector<U> v2);

template <typename T, typename U , typename V>
unordered_map<T, tuple<U, V> > makeHash(vector<T> v1, vector<U> v2, vector<V> v3);

template <typename T, typename U >
unordered_map<T,U> clusterWeights(vector<T> sensors, vector<U> weights, clustering clus);

clustering storeClusters(vector<int> clu, vector<int> sen);

overlaping clusterOverlap(clustering clus, unordered_map<int, tuple<int,float>> clusE, unordered_map<int,float> cluW, int size);


#endif