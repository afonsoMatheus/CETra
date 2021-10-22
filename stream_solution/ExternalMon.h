#ifndef EXTMON_H
#define EXTMON_H

#include <iostream>
#include "HashSol.h"
#include "Transitions.h"
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <set>

using namespace std;
using statistics = unordered_map<int ,tuple<float, float, float>>;

/////////////////////////////////////////////////////////////////

Transitions extTransitions(const overlaping &, const vector<int> &);

float sumSplits(const vector<float> &, const vector<int> &);

unordered_map<int,int> hashLabels(const vector<int> &);

statistics buildStatistics(const clustering &);

#endif