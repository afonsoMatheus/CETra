#!/usr/bin/env python
# coding: utf-8

from sklearn.datasets import make_blobs
import transitions as trans
import copy, csv
import numpy as np
from matplotlib import pyplot as plt
from random import randint
from math import floor


'''
The purpose of this algorithm is to enable the simulation of different types of group transitions, both internal and external, between multiple clusterings in a n-dimensional space.
'''

class Evocluster:

    #Args n_samples l_centers num_f r_state, list 2dlist int int
    #Ret None; makes the initial clustering 
    def __init__(self, n_samples, l_centers, num_f, r_state):
    
        self.evo_clustering = {}
        self.centers = {}
        self.num_f = num_f

        X, y = make_blobs(n_samples, centers = l_centers, n_features=self.num_f, random_state = r_state)
        colors = att_colors(y)

        self.evo_clustering[0] = X,y,colors
        self.centers[0] = find_centroids(X,y, num_f)
        
        
    #Args transitions, list
    #Ret None; makes the next clustering with the chosen transictions 
    def new_clustering(self, transitions):
        
        for tr in transitions:
                        
            next_clustering = copy.deepcopy(self.evo_clustering)    
            last_t = list(self.evo_clustering)[-1]

            X, y, colors = self.evo_clustering[last_t]

            X, y, colors, new_c = ch_transition(tr, X, y, colors,  self.centers[last_t], self.num_f)
            next_clustering[last_t+1] = X, y, colors

            self.evo_clustering = next_clustering
            self.centers[last_t+1] = new_c
                
    #Args None, None
    #Ret None; plots graphs for all clusterings
    def plot_view_all(self):
        for j in self.evo_clustering.keys():
            X, y, colors = self.evo_clustering[j]
            n_c = 0
            centers_c = self.centers[j]
            for i in np.unique(y):
                plt.scatter(X[y == i, 0], X[y == i, 1], color=colors[i], marker = 'o', s=20, label="Cluster "+str(n_c))
                plt.scatter(centers_c[i][0], centers_c[i][1], color='black', marker = 'h', s=20)
                plt.title("T_" + str(j))
                plt.xlabel("x")
                plt.ylabel("y")
                plt.xlim(0, 40)
                plt.ylim(0, 50)
                plt.legend()
                n_c = n_c + 1
            plt.show()
            
    #Args None, None
    #Ret None; export the clustering samples and labels in a csv format        
    def export_csv(self):
        
        t = 0
        for i in self.evo_clustering.keys():
            
            X, y, colors = self.evo_clustering[i]
                        
            with open("evo_cluster_"+ str(t) +".csv", mode='w') as csv_file:
                writer = csv.writer(csv_file)
                                                
                for j in range(X.shape[0]):
                    row = np.concatenate((X[j], y[j]), axis = None)
                    writer.writerow(row)
                    
                csv_file.close()
                
            t = t + 1      
            
#Args tr X y colors centers num_f, list list list dict dict int
#Ret list list dict dict; returns the new data for the next clustering 
def ch_transition(tr, X, y, colors, centers, num_f):

    new_c = 0
    
    #internal transitions

    if("int_den_dif" in tr):
        X, y = trans.int_den_dif(X, y, centers, [1], num_f)
        new_c = find_centroids(X, y, num_f)

    if("int_den_comp" in tr):
        X, y = trans.int_den_comp(X, y, centers, [2], num_f)
        new_c = find_centroids(X, y, num_f)

    if("int_size_grow" in tr):
        X, y = trans.int_size_grow(X, y, centers, [1], num_f, 30)
        new_c = find_centroids(X, y, num_f)

    if("int_size_reduc" in tr):
        X, y = trans.int_size_reduc(X, y, [1], 80)
        new_c = find_centroids(X, y, num_f)

    if("int_local" in tr):   
        X, y = trans.int_local(X, y, [2], num_f, (-8,8))
        new_c = find_centroids(X, y, num_f)

    #external transitions

    if("ext_death" in tr):
        X, y, colors = trans.ext_death(X, y, [3], colors)
        new_c = find_centroids(X, y, num_f)

    if("ext_birth" in tr):
        X, y, colors = trans.ext_birth(X, y, colors, num_f, 15)
        new_c = find_centroids(X, y, num_f)

    if("ext_union" in tr):
        X, y, colors = trans.ext_union(X, y, colors, [0,2], [10,40,25], num_f)
        new_c = find_centroids(X, y, num_f)

    if("ext_div" in tr):
        X, y, colors = trans.ext_div(X, y, colors, [0], [[30,25,20],[20,40,25],[30,10,20]], [0.3,0.3,0.4], num_f)
        new_c = find_centroids(X, y, num_f)

    return X, y, colors, new_c


#Args y, list
#Ret dict; returns the associated colors for the first clustering 
def att_colors(y):
    colors = {}
    for i in np.unique(y):
        colors[i] = '#%06X' % randint(0, 0xFFFFFF)   
    return colors

#Args X y num_f, list list int
#Ret dicts; returns the updated centers 
def find_centroids(X, y, num_f):
        
    c = list(np.unique(y))
    
    clusters = {}
    sum_axis = {}
    len_axis = {}
    
    for i in c:
        clusters[i] = []
        sum_axis[i] = []
        len_axis[i] = []
        
    for i in range(X.shape[0]):
        clusters[y[i]].append(X[i])
            
    for k in clusters.keys():
        c_axis = list(zip(*clusters[k]))
        for i in range(num_f):
            sum_axis[k].append(sum(c_axis[i]))
            len_axis[k].append(len(c_axis[i]))
                
    centers = {}
    for k in clusters.keys():
        centers[k] = []
        for i in range(num_f):
            centers[k].append(sum_axis[k][i]/len_axis[k][i])
            
    return centers
   