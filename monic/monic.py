#!/usr/bin/env python
# coding: utf-8

import numpy as np

def execute(clust_i, clust_j, age_j):
    
    X_1, y_1= clust_i
    X_2, y_2= clust_j
    
    clu_index_1 = {}
    for clu in np.unique(y_1):
        if(clu != -1):
            clu_index_1[clu] = cluster_index(X_1, y_1, clu)
        
    clu_index_2 = {}
    for clu in np.unique(y_2):
        clu_index_2[clu] = cluster_index(X_2, y_2, clu)
        
    overlap_m = overlap_matrix(clu_index_1, clu_index_2, age_j)
    overlap_i = overlap_matrix(clu_index_2, clu_index_1, age_j)
    
    surv_list, absor_list, split_list, deaths, tracked = external_transitions(clu_index_1, clu_index_2, overlap_m, overlap_i, age_j)
    
    show_trans(surv_list, absor_list, split_list, deaths, tracked, list(clu_index_2))
    
''' MONIC External Transitions'''
    
def external_transitions(clu_index_1, clu_index_2, overlap_m, overlap_i, age_j):
    
    C_1 = list(clu_index_1)
    C_2 = list(clu_index_2)

    deaths, split_list, absor_sur, tracked = [], [], [], [-1]
    for i in range(len(C_1)):
        split_cand, split_union = [], []
        surv_cand = -1

        for j in range(len(C_2)):

            mcell = overlap_m[C_1[i]][j]

            if(mcell > 0.5):
                if(mcell > surv_cand):
                    surv_cand = C_2[j]
                elif(mcell == surv_cand):
                    if(overlap_i[C_2[j][i]] > overlap_i[surv_cand][i]):
                        surv_cand = C_2[j]   

            elif(mcell > 0.25):
                split_cand.append(C_2[j])
                split_union = split_union + clu_index_2[C_2[j]]

        if(surv_cand == -1 and split_cand == []):
            deaths.append(C_1[i])
        elif(split_cand != []):
            if (overlap(clu_index_1[C_1[i]], split_union, age_j) > 0.5):
                for j in range(len(split_cand)):
                    split_list.append([C_1[i],split_cand[j]])
                    tracked.append(split_cand[j])
            else:
                deaths.append(C_1[i])
        else:
            absor_sur.append([C_1[i], surv_cand])

    absor_list, surv_list = [], []

    for j in range(len(C_2)):
        absor_cand = get_candidates(absor_sur, C_2[j])

        if(len(absor_cand) > 1): 
            for i in absor_cand:
                absor_list.append([i,C_2[j]])
                tracked.append(C_2[j])

        elif(absor_cand == C_2[j]):
            surv_list.append([absor_cand[0],C_2[j]])
            tracked.append(C_2[j])

    return surv_list, absor_list, split_list, deaths, tracked

    
'''functions to make the overlap matrix'''

def overlap_matrix(clu_index_1, clu_index_2, age_y):
    
    overlaps = {}
    for i in clu_index_1.keys():
        overlaps[i] = []
        
        for j in clu_index_2.keys():
            if(j != -1):
                overlaps[i].append(overlap(clu_index_1[i], clu_index_2[j], age_y))
            else:
                overlaps[i].append(0)
            
    return overlaps
    
            
def overlap(c1, c2, age_j):
    
    inters_index = set(c1).intersection(c2)
    
    int_sum = 0
    for i in inters_index:
        int_sum = int_sum + age_j[i]
    
    int_clus = 0
    for i in c1:
        int_clus = int_clus + age_j[i]    
    
    return int_sum/int_clus


'''Functions to assign weights'''

def weight_age(clustering):
    
    age_w = {}
    
    X, y = clustering[0]
    age_w[0] = [1 for x in range(len(X))]
    
    for i in range(1, len(clustering.keys())):
        X, y = clustering[i]
        age_w[i] = age(X, i, age_w[i-1])
                
    return age_w

def age(X, time, last_w):
    
    weights = []        

    aux = 0
    for i in range(len(last_w)):
        weights.append(last_w[i]*0.75)
        aux = i

    for i in range(aux+1, len(X)):
        weights.append(1)

    return weights


'''Other functions'''

def cluster_index(X, y, clusters):
    clu_index = []
    for i in range(X.shape[0]):
        if(y[i] == clusters):
            clu_index.append(i)
    return clu_index

def get_candidates(absor_sur, j):
    absortion_cand = []
    for i in range(len(absor_sur)):
        if(absor_sur[i][1] == j):
            absortion_cand.append(absor_sur[i][0])
    return absortion_cand

def show_trans(surv_list, absor_list, split_list, deaths, tracked, C_2):
    
    print("Survivals")
    if(surv_list != []):
        for i in surv_list:
            print("C_" + str(i[0]) + " -> " + "C_" + str(i[1]))
    else:
        print("-")
        
    print()

    print("Splits")
    if(split_list != []):    
        for i in split_list:
            print("C_" + str(i[0]) + " -> // " + "C_" + str(i[1]))
    else:
        print("-")
        
    print()
        
    print("Absorptions")
    if(absor_list != []):
        for i in absor_list:
            print("C_" + str(i[0]) + " -> U " + "C_" + str(i[1]))
    else:
        print("-")
        
    print()
        
    print("Deaths")
    if(deaths != []):
        for i in deaths:
            print("C_" + str(i) + " -> DEATH")
    else:
        print("-")
        
    print()
        
    print("Births")
    if(len(set(tracked).intersection(C_2)) != len(C_2)):
        for i in C_2:
            if(i not in tracked and i != -1):
                print("*" + " -> " + "C_" + str(i))
    else:
        print("-")