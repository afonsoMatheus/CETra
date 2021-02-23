import numpy as np

def execute(clust_i, clust_j, age_j):
    
    X_1, y_1, c1 = clust_i
    X_2, y_2, c2 = clust_j
    
    clu_index_1 = {}
    for clu in np.unique(y_1):
        clu_index_1[clu] = cluster_index(X_1, y_1, clu)
        
    clu_index_2 = {}
    for clu in np.unique(y_2):
        clu_index_2[clu] = cluster_index(X_2, y_2, clu)
        
    overlap_m =  overlap_matrix(clu_index_1, clu_index_2, age_j)
    
    print(overlap_m)
        
    deaths, split_list, absor_sur = [], [], []
    for i in clu_index_1.keys():
        split_cand, split_union = [], []
        surv_cand = -1
        
        for j in clu_index_2.keys():
            
            mcell = overlap_m[i][j]
            
            if(mcell > 0.5):
                if(mcell > surv_cand):
                    surv_cand = j
            elif(mcell > 0.25):
                split_cand.append(j)
                split_union.append(clu_index_2[j])
                        
        if(surv_cand == -1 and split_cand == []):
            deaths.append(i)
        elif(split_cand != []):
            if (overlap(clu_index_1[i], split_union) > 0.5):
                for j in split_cand:
                    split_list.append([i,j])
            else:
                deaths.append(i)
        else:
            
            absor_sur.append([i, surv_cand])
            
    absor_list, surv_list = [], []
                
    for j in clu_index_2.keys():
        absor_cand = get_candidates(absor_sur, j)
        
        print(absor_cand)
        
        if(len(absor_cand) > 1): 
            for i in absor_cand:
                absor_list.append([i,j])
        
        elif(absor_cand[0] == j):
            surv_list.append([absor_cand[0],j])
    
    print("----------------")
    print("Absorvidos: " + str(absor_list))
    print("Sobrevivência: " + str(surv_list))
    print("Separações: " + str(split_list))
    print("Mortes: " + str(deaths))
            
        
def get_candidates(absor_sur, j):
        
    absortion_cand = []
    
    for i in range(len(absor_sur)):
        if(absor_sur[i][1] == j):
            absortion_cand.append(absor_sur[i][0])
    return absortion_cand
    
    


def overlap_matrix(clu_index_1, clu_index_2, age_y):
    
    size = {}
    overlaps = {}
    for i in clu_index_1.keys():
        overlaps[i] = []
        #size[i] = []
        #size[i].append(len(clu_index_1[i]))
        
        for j in clu_index_1.keys():
            
            if(j not in clu_index_2.keys()):
                overlaps[i].append(0) 
            else:
                overlaps[i].append(overlap(clu_index_1[i], clu_index_2[j], age_y))
            #size[i].append(len(clu_index_2[j]))'
            
    return overlaps
    
            

def overlap(c1, c2, age_j):
    
    #print(c1)
    #print(c2)
    
    inters_index = set(c1).intersection(c2)
    #print(inters_index)
    
    int_sum = 0
    for i in inters_index:
        int_sum = int_sum + age_j[i]
    
    int_clus = 0
    for i in c1:
        int_clus = int_clus + age_j[i]    
    
    #print(int_sum/int_clus)
    
    #print("-------------")
    
    
    return int_sum/int_clus

def match(overlaps, size):
                           
    matchs = {}
    for i in overlaps.keys():
        if(max(overlaps[i]) < 0.5):
            matchs[i] = None
        elif(max(overlaps[i]) == 0.5 and 0.5 in repeat(overlaps[i])):
            matchs[i] = size[i][1:].index(min(size[i][1:], key=lambda x:abs(x-size[i][0])))
        else:
            matchs[i] = overlaps[i].index(max(overlaps[i]))
            
    return matchs


def weight_age(clustering):
    
    age_w = {}
    for i in clustering.keys():
        age_w[i] = []
    
    X, y, c = clustering[0]
    age_w[0] = [1 for x in range(len(X))]
    
    for i in range(1, len(clustering.keys())):
        X, y, c = clustering[i]
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


def cluster_index(X, y, clusters):
    clu_index = []
    for i in range(X.shape[0]):
        if(y[i] == clusters):
            clu_index.append(i)
    return clu_index

def repeat(x): 
    _size = len(x) 
    repeated = [] 
    for i in range(_size): 
        k = i + 1
        for j in range(k, _size): 
            if x[i] == x[j] and x[i] not in repeated: 
                repeated.append(x[i]) 
    return repeated 