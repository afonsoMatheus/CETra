from random import uniform, sample
import math
import numpy as np

''' internal transitions '''

#localization changes
def int_local(X, y, clu, ax, ay):
    for i in range(X.shape[0]):
        if(y[i] == clu):
            X[i][0] = X[i][0] + ax
            X[i][1] = X[i][1] + ay
    return X, y

#density changes
#--difusion
def int_den_dif(X, y, centers, clu, ax, ay):
    
    for i in range(X.shape[0]):
        if(y[i] == clu):
            X[i] = dist_points(X[i], centers[y[i]], ax, ay, 0)       
    return X, y
    
#--compaction
def int_den_comp(X, y, centers, clu, ax, ay):
    
    for i in range(X.shape[0]):
        if(y[i] == clu):
            X[i] = dist_points(X[i], centers[y[i]], ax, ay, 1)       
    return X, y

#size changes
#--growing
def int_size_grow(X, y, centers, clu, q):
        
    max_d = 0
    max_xy = 0
    for i in range(X.shape[0]):
        if(y[i] == clu):
            if(euc_distance(X[i], centers[y[i]]) > max_d):
                max_xy = X[i]
                max_d = euc_distance(X[i], centers[y[i]])
                
    new_x, new_y = gen_samples(max_xy, max_d, clu, centers[clu], q)
    
    X = np.concatenate((X, new_x), axis = 0)
    y = np.concatenate((y, new_y), axis = 0)
        
    return X, y

#--reduction
def int_size_reduc(X, y, clu, k):
    
    c_index = []
        
    for i in range(X.shape[0]):
        if(y[i] == clu):
            c_index.append(i)
            
    r_index = sample(c_index, k)
                
    X = np.delete(X, r_index, axis = 0)
    y = np.delete(y, r_index, axis = 0)  
          
    return X, y
    
''' external transitions '''







''' other functions '''

def dist_points(x, center, ax, ay, t):
    if(t == 0):
        if(x[0] < center[0]):
            x[0] = x[0] - ax
        else:
            x[0] = x[0] + ax
        if(x[1] < center[1]):
            x[1] = x[1] - ax
        else:
            x[1] = x[1] + ax
    
    if(t == 1):
        if(x[0] < center[0]):
            x[0] = x[0] + ax
        else:
            x[0] = x[0] - ax
        if(x[1] < center[1]):
            x[1] = x[1] + ax
        else:
            x[1] = x[1] - ax

    return x


def gen_samples(d, m, clu, c, q):
    
    new_samples_X = []
    new_samples_Y = []
        
    for i in range(q):
        new_samples_X.append([uniform(c[0] - m, d[0] + m), uniform(c[1] - m, d[1] + m)])
        new_samples_Y.append(clu)
    
    return new_samples_X, new_samples_Y
    
def euc_distance(d1, d2):
    return math.sqrt(sum([(a - b) ** 2 for a, b in zip(d1, d2)]))