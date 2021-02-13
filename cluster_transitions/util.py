import numpy as np
from random import randint
from matplotlib import pyplot as plt
from math import floor
import numpy as np


def plot_view(clustering, centers, j):
    X, y, colors = clustering
    for i in np.unique(y):
        plt.scatter(X[y == i, 0], X[y == i, 1], color=colors[i], marker = 'o', s=20, label="Cluster "+str(i))
        plt.scatter(centers[i][0], centers[i][0], color='black', marker = 'h', s=20, label="centers")
        plt.title("T_" + str(j))
        plt.xlabel("x")
        plt.ylabel("y")
        plt.xlim(0, 40)
        plt.ylim(0, 50)
        
        
def plot_view_all(clustering, centers):
    for j in clustering.keys():
        X, y, colors = clustering[j]
        n_c = 0
        centers_c = centers[j]
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

def att_colors(y):
    colors = {}
    for i in np.unique(y):
        colors[i] = '#%06X' % randint(0, 0xFFFFFF)   
    return colors

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


    
    