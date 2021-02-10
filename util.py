import numpy as np
from random import randint
from matplotlib import pyplot as plt
from math import floor
import numpy as np


def plot_view(clustering, centers, j):
    X, y, colors = clustering
    for i in np.unique(y):
        plt.scatter(X[y == i, 0], X[y == i, 1], color=colors[i], marker = 'o', s=20, label="Cluster "+str(i))
        plt.scatter(centers[i][0], centers[i][1], color='black', marker = 'h', s=20, label="centers")
        plt.title("T_" + str(j))
        plt.xlabel("x")
        plt.ylabel("y")
        plt.xlim(0, 40)
        plt.ylim(0, 50)
        
        
def plot_view_all(clustering, centers):
    for j in clustering.keys():
        X, y, colors = clustering[j]
        centers_c = centers[j]
        for i in np.unique(y):
            plt.scatter(X[y == i, 0], X[y == i, 1], color=colors[i], marker = 'o', s=20, label="Cluster "+str(i))
            plt.scatter(centers_c[i][0], centers_c[i][1], color='black', marker = 'h', s=20)
            plt.title("T_" + str(j))
            plt.xlabel("x")
            plt.ylabel("y")
            plt.xlim(0, 40)
            plt.ylim(0, 50)
            plt.legend()
        plt.show()

def att_colors(y):
    colors = {}
    for i in np.unique(y):
        colors[i] = '#%06X' % randint(0, 0xFFFFFF)   
    return colors

def find_centroids(clustering):
    
    X, y, colors = clustering
    
    c = list(np.unique(y))
    
    clusters_x = {}
    clusters_y = {}
    for i in c:
        clusters_x[i] = []
        clusters_y[i] = []
        
    for i in range(len(y)):
        clusters_x[y[i]] = np.concatenate((clusters_x[y[i]], X[i][0]), axis = None)
        clusters_y[y[i]] = np.concatenate((clusters_y[y[i]], X[i][1]), axis = None)
                
    centers = []
    for i in c:
        sum_x = sum(clusters_x[i])
        sum_y = sum(clusters_y[i])
        centers.append([sum_x/len(clusters_x[i]), sum_y/len(clusters_y[i])])
            
    return centers


    
    