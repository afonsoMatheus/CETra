#!/usr/bin/env python
# coding: utf-8

from random import uniform, sample, randint, shuffle, uniform
from sklearn.datasets import make_blobs
import math
import numpy as np

''' internal transitions '''

#localization changes
def int_local(X, y, clu, n_center, num_f):
    
    clu_index = cluster_index(X, y, clu)
    
    X = approaching_center(X, y, clu, n_center, clu_index, num_f)
            
    return X

#density changes
#--difusion
def int_den_dif(X, y, centers, clu, exp, num_f):
    
    clu_index = cluster_index(X, y, clu)
        
    X = dist_points_den(X, centers[clu[0]], clu_index, num_f, exp, 0) 
        
    return X
    
#--compaction
def int_den_comp(X, y, centers, clu, exp, num_f):
    
    clu_index = cluster_index(X, y, clu)
    
    X = dist_points_den(X, centers[clu[0]], clu_index, num_f, exp, 1) 
    
    return X

#size changes
#--growing
def int_size_grow(X, y, centers, clu, q, num_f):
        
    max_d = 0
    max_point = 0
    
    clu_index = cluster_index(X, y, clu)
    
    for i in clu_index:
        if(euc_distance(X[i], centers[clu[0]]) > max_d):
            max_d = euc_distance(X[i], centers[clu[0]])
            
    new_samples_X, new_samples_y = gen_samples(max_d, clu[0], centers[clu[0]], num_f, q)
    
    X = np.concatenate((X, new_samples_X), axis = 0)
    y = np.concatenate((y, new_samples_y), axis = 0)
        
    return X, y

#--reduction
def int_size_reduc(X, y, clu, k):
    
    clu_index = cluster_index(X, y, clu)
            
    r_index = sample(clu_index, k)
    
    for i in r_index:
        y[i] = -1
          
    return y
    
''' external transitions '''

#birth
def ext_birth(X, y, colors, num_f, q, new_c, std):
    
    new_c = make_centers(new_c, num_f)[0]
        
    X_new, y_new = make_blobs(n_samples=[q], centers = [new_c], cluster_std = std, n_features=num_f ,random_state=1)
            
    for i in range(len(y_new)):
        y_new[i] = max(y) + 1
            
    X = np.concatenate((X, X_new), axis = 0)
    y = np.concatenate((y, y_new), axis = 0)
            
    colors[max(y)] = '#%06X' % randint(0, 0xFFFFFF)
    
    return X, y, colors

#death
def ext_death(X, y, clu, colors):

    clu_index = cluster_index(X, y, clu)
    
    for i in clu_index:
        y[i] = -1
    
    del colors[clu[0]]
        
    return X, y, colors
    
#union
def ext_union(X, y, colors, centers, clusters, n_center, exp, num_f):
    
    n_center = make_centers([n_center], num_f)
    
    clu_index = cluster_index(X, y, clusters)
    
    X = approaching_center(X, y, clusters, n_center[0], clu_index, num_f)
    
    max_d = 0
    for i in clu_index:
        if(euc_distance(X[i], n_center[0]) > max_d):
            max_d = euc_distance(X[i], n_center[0])
    
    num_y = -1
    cen = math.inf
    for i in centers.keys():
        if(euc_distance(centers[i], n_center[0]) < max_d * exp and cen > euc_distance(centers[i], n_center[0])): 
            cen = euc_distance(centers[i], n_center[0])
            num_y = i
            
    if(num_y == -1):
        num_y = max(y) + 1   
        for i in clu_index:
            y[i] = num_y
        colors[num_y] = '#%06X' % randint(0, 0xFFFFFF)
    else:
        for i in clu_index:
            y[i] = num_y
    
    return X, y, colors

#division
def ext_div(X, y, colors, centers, clu, n_centers, ratio, exp, num_f):
    
    n_centers = make_centers(n_centers, num_f)
    
    clu_index = cluster_index(X, y, clu)
    
    new_clusters = {}
    ys = {}
    
    max_d = {}
    
    for i in np.unique(y):
        max_d[i] = 0
    for i in range(len(X)):
        if(euc_distance(X[i], centers[y[i]]) > max_d[y[i]]):
            max_d[y[i]] = euc_distance(X[i], centers[y[i]])
    
    aux = clu_index
    shuffle(aux)
    for i in range(len(ratio)):
        len_ratio = int(len(clu_index)*ratio[i])
        new_clusters[i] = aux[:len_ratio]
        aux = aux[len_ratio:]
        ys[i] = {}
            
    nc = max(y) + 1
    for i in range(len(ratio)):
        ys[i] = None
        for j in centers.keys():
            if(euc_distance(centers[j], n_centers[i]) < max_d[j]*exp and j != clu):
                ys[i] = j
        
        if(ys[i] == None):
            ys[i] = nc
            nc = nc + 1
            colors[ys[i]] = '#%06X' % randint(0, 0xFFFFFF)
        
    for i in new_clusters.keys():
        for j in new_clusters[i]:    
            y[j] = ys[i]
    
    for i in new_clusters.keys():                
        X = approaching_center(X, y, [ys[i]], n_centers[i], new_clusters[i], num_f)
        
    return X, y, colors

def make_centers(centers, num_f):
    
    for i in range(len(centers)):
        centers.append([centers[i][0]] + [centers[i][1]])
        centers[i] = centers[i] + ([centers[i][1]] * (num_f - 2))
        
    return centers



''' other functions '''

def approaching_center(X, y, clusters, n_center, clu_index, num_f):
        
    max_d = {}
    for i in clusters:
        max_d[i] = []
        for j in range(num_f):
            max_d[i].append(0)

    for i in clu_index:
        for j in range(num_f):
            if(abs(X[i][j] - n_center[j]) > max_d[y[i]][j]):
                max_d[y[i]][j] = abs(X[i][j] - n_center[j])

    for i in clu_index:
        for j in range(num_f):
            if(X[i][j] < n_center[j]):
                X[i][j] = X[i][j] + max_d[y[i]][j]
            else:
                X[i][j] = X[i][j] - max_d[y[i]][j]
    return X

def cluster_index(X, y, clusters):
    clu_index = []
    for i in range(X.shape[0]):
        if(y[i] in clusters):
            clu_index.append(i)
    return clu_index
    

def dist_points_den(X, center, clu_index, num_f, exp, t):
        
    std = np.std(X[clu_index])* exp 
                    
    if(t == 0):      
        for i in clu_index:
            for j in range(num_f):
                if(X[i][j] < center[j]):
                    X[i][j] = X[i][j] - abs(uniform(-std,std))
                else:
                    X[i][j] = X[i][j] + abs(uniform(-std,std))
    
    if(t == 1):
         for i in clu_index:
            for j in range(num_f):
                if(X[i][j] < center[j]):
                    X[i][j] = X[i][j] + abs(uniform(-std,std))
                else:
                    X[i][j] = X[i][j] - abs(uniform(-std,std))

    return X


def gen_samples(max_d, clu, center, num_f, q):
    
    new_samples_X = []
    new_samples_Y = []
        
    for i in range(q):
        new_samples_X.append([])
        for j in range(num_f):
            new_samples_X[i].append(uniform(center[j] - max_d, center[j] + max_d))
        new_samples_Y.append(clu)
        
    return new_samples_X, new_samples_Y                
    
def euc_distance(d1, d2):
    return math.sqrt(sum([(a - b) ** 2 for a, b in zip(d1, d2)]))