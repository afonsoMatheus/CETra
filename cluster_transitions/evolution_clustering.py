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
The purpose of this algorithm is to enable the simulation of different types of group 
transitions, both internal and external, between multiple clusterings in a n-dimensional 
space.
'''

class Evocluster:

    #Args n_samples l_centers num_f r_state, list 2dlist int int
    #Ret None; makes the initial clustering 
    def __init__(self, n_samples, l_centers, stdev, num_f, r_state):
    
        self.evo_clustering = {}
        self.centers = {}
        self.num_f = num_f
        
        centers = []
        samples = []
        for i in range(len(l_centers)):
            samples = [n_samples] * len(l_centers)
            centers.append([l_centers[i][0]] + [l_centers[i][1]])
            centers[i] = l_centers[i] + ([l_centers[i][1]] * (num_f - 2))

        X, y = make_blobs(samples, centers = centers, cluster_std = stdev, n_features=self.num_f, random_state = r_state)
        colors = att_colors(y)

        self.evo_clustering[0] = X,y,colors
        self.centers[0] = find_centroids(X,y, self.num_f)
        
        self.next_clustering = {}
    
    #Args None, None
    #Ret None; starts to assign a new clustering
    def new_clustering(self):
        self.next_clustering = self.Newcluster(self.evo_clustering, self.centers, self.num_f)
    
    #Args None, None
    #Ret None; adds the new clustering with all transitions previously allocated
    def make_clustering(self):
        self.evo_clustering = self.next_clustering.next_clustering
        del self.next_clustering.next_clustering
    
    class Newcluster:
        
        #Args evo_clustering centers num_f, dict dict int
        #Ret None; makes a copy of the lattest clustering
        def __init__(self, evo_clustering, centers, num_f):
            self.next_clustering = copy.deepcopy(evo_clustering)
            self.next_centers = centers
            self.num_f = num_f
            
            self.last_t = list(evo_clustering)[-1]
            self.X, self.y, self.colors = evo_clustering[self.last_t]
            
        '''External Transitions'''
        
        #Args clu, int
        #Ret None; makes a death transitons and updates the newest clustering
        def death_transition(self, clu):
            self.X, self.y, self.colors = trans.ext_death(self.X, self.y, [clu], self.colors)
            
            self.next_clustering[self.last_t+1] = self.X, self.y, self.colors
            self.next_centers[self.last_t+1] = find_centroids(self.X, self.y, self.num_f)
        
        #Args qtd, int
        #Ret None; makes a cluster birth and updates the newest clustering
        def birth_transition(self, qtd, new_c, std):
            self.X, self.y, self.colors = trans.ext_birth(self.X, self.y, self.colors, self.num_f, qtd, [new_c], std)
            
            self.next_clustering[self.last_t+1] = self.X, self.y, self.colors
            self.next_centers[self.last_t+1] = find_centroids(self.X, self.y, self.num_f)
        
        #Args clus union_c, int*array int*array int
        #Ret None; makes a union transiton and updates the newest clustering
        def absor_transition(self, clus, union_c, exp):
            self.X, self.y, self.colors = trans.ext_union(self.X, self.y, self.colors, self.next_centers[self.last_t], clus, union_c, exp, self.num_f)
            
            self.next_clustering[self.last_t+1] = self.X, self.y, self.colors
            self.next_centers[self.last_t+1] = find_centroids(self.X, self.y, self.num_f)
        
        #Args clu split_cs ratio, int int*matrix float*array int
        #Ret None; makes a split transiton and updates the newest clustering
        def split_transition(self, clu, split_cs, ratio, exp):
            self.X, self.y, self.colors = trans.ext_div(self.X, self.y, self.colors, self.next_centers[self.last_t], [clu], split_cs, ratio, exp, self.num_f)
            
            self.next_clustering[self.last_t+1] = self.X, self.y, self.colors
            self.next_centers[self.last_t+1] = find_centroids(self.X, self.y, self.num_f)
            
        '''Internal Transitions'''

        def density_difusion(self, clu, exp):

            self.X = trans.int_den_dif(self.X, self.y, self.next_centers[self.last_t], [clu], exp, self.num_f)

            self.next_clustering[self.last_t+1] = self.X, self.y, self.colors
            self.next_centers[self.last_t+1] = find_centroids(self.X, self.y, self.num_f)

        def density_compactation(self, clu, exp):

            self.X = trans.int_den_comp(self.X, self.y, self.next_centers[self.last_t], [clu], exp, self.num_f)

            self.next_clustering[self.last_t+1] = self.X, self.y, self.colors
            self.next_centers[self.last_t+1] = find_centroids(self.X, self.y, self.num_f)

        def size_grow(self, clu, qtd):

            self.X, self.y = trans.int_size_grow(self.X, self.y, self.next_centers[self.last_t], [clu], qtd, self.num_f)

            self.next_clustering[self.last_t+1] = self.X, self.y, self.colors
            self.next_centers[self.last_t+1] = find_centroids(self.X, self.y, self.num_f)

        def size_reduction(self, clu, qtd):

            self.y = trans.int_size_reduc(self.X, self.y, [clu], qtd)

            self.next_clustering[self.last_t+1] = self.X, self.y, self.colors
            self.next_centers[self.last_t+1] = find_centroids(self.X, self.y, self.num_f)

        def localization_change(self, clu, n_center):

            self.X = trans.int_local(self.X, self.y, [clu], n_center, self.num_f)

            self.next_clustering[self.last_t+1] = self.X, self.y, self.colors
            self.next_centers[self.last_t+1] = find_centroids(self.X, self.y, self.num_f)
        
        
              
    #Args None, None
    #Ret None; plots graphs for all clusterings
    def plot_view_all(self):
        for j in self.evo_clustering.keys():
            X, y, colors = self.evo_clustering[j]
            n_c = 0
            centers_c = self.centers[j]
            
            clus = np.unique(y)
            index = np.argwhere(clus==-1)
            clus = np.delete(clus, index, 0)
                        
            for i in clus:
                plt.scatter(X[y == i, 0], X[y == i, 1], color=colors[i], marker = 'o', s=20, label="Cluster "+str(i))
                plt.scatter(centers_c[i][0], centers_c[i][1], color='black', marker = 'h', s=20)
                plt.title("T_" + str(j))
                plt.xlabel("x")
                plt.ylabel("y")
                plt.xlim(0, 1000)
                plt.ylim(0, 1000)
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
                    row = np.concatenate((y[j], X[j]), axis = None)
                    writer.writerow(row)
                    
                csv_file.close()
                
            t = t + 1
            
    #Args None, None
    #Ret dict; return the clusterings with only the X and y values
    def get_samples(self):
        
        clusterings = {}
        
        for i in self.evo_clustering.keys():
            X, y, c = self.evo_clustering[i]
            clusterings[i] = X, y
            
        return clusterings
        
        
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
   