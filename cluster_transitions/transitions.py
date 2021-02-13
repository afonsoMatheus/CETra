from random import uniform, sample, randint, shuffle
from sklearn.datasets import make_blobs
import math
import numpy as np

''' internal transitions '''

#localization changes
def int_local(X, y, clu, num_f, rang):
    
    d = {}
    for i in range(num_f):
        d[i] = randint(rang[0],rang[1])
    
    clu_index = cluster_index(X, y, clu)
     
    for i in clu_index:
        for j in range(num_f):
            X[i][j] = X[i][j] + d[j]
            
    return X, y

#density changes
#--difusion
def int_den_dif(X, y, centers, clu, num_f, d):
    
    clu_index = cluster_index(X, y, clu)
        
    for i in clu_index:
        X[i] = dist_points_den(X[i], centers[clu[0]], d, num_f, 0)  
        
    return X, y
    
#--compaction
def int_den_comp(X, y, centers, clu, num_f, d):
    
    clu_index = cluster_index(X, y, clu)
    
    for i in clu_index:
        X[i] = dist_points_den(X[i], centers[clu[0]], d, num_f, 1)
        
    return X, y

#size changes
#--growing
def int_size_grow(X, y, centers, clu, num_f, q):
        
    max_d = 0
    max_point = 0
    
    clu_index = cluster_index(X, y, clu)
    
    for i in clu_index:
        if(euc_distance(X[i], centers[clu]) > max_d):
            max_d = euc_distance(X[i], centers[clu])
            
    new_samples_X, new_samples_y = gen_samples(max_d, clu, centers[clu], num_f, q)
    
    X = np.concatenate((X, new_samples_X), axis = 0)
    y = np.concatenate((y, new_samples_y), axis = 0)
        
    return X, y

#--reduction
def int_size_reduc(X, y, clu, k):
    
    clu_index = cluster_index(X, y, clu)
            
    r_index = sample(clu_index, k)
                
    X = np.delete(X, r_index, axis = 0)
    y = np.delete(y, r_index, axis = 0)  
          
    return X, y
    
''' external transitions '''

#birth
def ext_birth(X, y, colors, num_f, q):
    
    X_new, y_new = make_blobs(n_samples=[q], centers=[(25,10,20)], n_features=num_f ,random_state=1)
            
    for i in range(len(y_new)):
        y_new[i] = max(y) + 1
            
    X = np.concatenate((X, X_new), axis = 0)
    y = np.concatenate((y, y_new), axis = 0)
            
    colors[max(y)] = '#%06X' % randint(0, 0xFFFFFF)
    
    return X, y, colors

#death
def ext_death(X, y, clu, colors):

    clu_index = cluster_index(X, y, clu)
    
    X = np.delete(X, clu_index, axis = 0)
    y = np.delete(y, clu_index, axis = 0)
    
    del colors[clu[0]]
    
    return X, y, colors
    
#union
def ext_union(X, y, colors, clusters, n_center, num_f):
    
    clu_index = cluster_index(X, y, clusters)
    
    X = approaching_center(X, y, clusters, n_center, clu_index, num_f)
    
    num_y = max(y) + 1
    
    for i in clu_index:
        y[i] = num_y
                        
    colors[num_y] = '#%06X' % randint(0, 0xFFFFFF)
    
    return X, y, colors

#division
def ext_div(X, y, colors, clu, n_centers, ratio, num_f):
    
    clu_index = cluster_index(X, y, clu)
    
    new_clusters = {}
    
    ys = {}
    
    aux = clu_index
    shuffle(aux)
    for i in range(len(ratio)):
        len_ratio = int(len(clu_index)*ratio[i])
        new_clusters[i] = aux[:len_ratio]
        aux = aux[len_ratio:]
        ys[i] = {}
        
    ys[0] = max(y) + 1
    colors[ys[0]] = '#%06X' % randint(0, 0xFFFFFF)
    for i in range(1,len(ratio)):
        ys[i] = ys[i-1]+1
        colors[ys[i]] = '#%06X' % randint(0, 0xFFFFFF)
    print(ys)
        
    for i in new_clusters.keys():
        for j in new_clusters[i]:
            y[j] = ys[i]
    
    for i in new_clusters.keys():                
        X = approaching_center(X, y, [ys[i]], n_centers[i], new_clusters[i], num_f)
        
    return X, y, colors
    


''' other functions '''

def cluster_index(X, y, clusters):
    clu_index = []
    for i in range(X.shape[0]):
        if(y[i] in clusters):
            clu_index.append(i)
    return clu_index
    

def dist_points_den(x, center, d, num_f, t):
    
    if(t == 0):    
        for i in range(num_f):
            if(x[i] < center[i]):
                x[i] = x[i] - d
            else:
                x[i] = x[i] + d
    
    if(t == 1):
         for i in range(num_f):
            if(x[i] < center[i]):
                x[i] = x[i] + d
            else:
                x[i] = x[i] - d

    return x


def gen_samples(max_d, clu, center, num_f, q):
    
    new_samples_X = []
    new_samples_Y = []
        
    for i in range(q):
        new_samples_X.append([])
        for j in range(num_f):
            new_samples_X[i].append(uniform(center[j] - max_d, center[j] + max_d))
        new_samples_Y.append(clu)
        
    return new_samples_X, new_samples_Y

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
                            
    
def euc_distance(d1, d2):
    return math.sqrt(sum([(a - b) ** 2 for a, b in zip(d1, d2)]))