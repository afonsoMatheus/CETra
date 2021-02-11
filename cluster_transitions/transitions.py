from random import uniform, sample, randint
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
        X[i] = dist_points_den(X[i], centers[clu], d, num_f, 0)  
        
    return X, y
    
#--compaction
def int_den_comp(X, y, centers, clu, num_f, d):
    
    clu_index = cluster_index(X, y, clu)
    
    for i in clu_index:
        X[i] = dist_points_den(X[i], centers[clu], d, num_f, 1)
        
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







''' other functions '''

def cluster_index(X, y, clu):
    clu_index = []
    for i in range(X.shape[0]):
        if(y[i] == clu):
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
    
def euc_distance(d1, d2):
    return math.sqrt(sum([(a - b) ** 2 for a, b in zip(d1, d2)]))