from sklearn.datasets import make_blobs
import transitions as trans
import util, copy

def init_clustering():
    
    evo_clustering = {}
    centers = {}
    num_f = 3
    
    X, y = make_blobs(n_samples=[10,10,10,10], centers=[(30,40, 35),(20,25,22),(10,12,11),(10,30,15)], n_features=num_f,random_state=1)
    colors = util.att_colors(y)
                    
    evo_clustering[0] = X,y,colors
    centers[0] = util.find_centroids(X,y, num_f)
            
    return evo_clustering, centers

def new_clustering(evo_clusters, centers, tr):
    
    num_f = 3
    
    next_clustering = copy.deepcopy(evo_clusters)    
    last_t = list(evo_clusters)[-1]
    
    X, y, colors = evo_clusters[last_t]
    evo_clusters.clear()
    
    #transitions
    
    X, y, colors, new_c = ch_transition(tr, X, y, colors,  centers[last_t], num_f)
    
    #updating clustering
                
    next_clustering[last_t+1] = X, y, colors
    
    centers[last_t+1] = new_c
    
    return next_clustering, centers


def ch_transition(tr, X, y, colors, centers, num_f):
    
    #internal transitions
    
    new_c = 0
            
    if("int_den_dif" in tr):
        X, y = trans.int_den_dif(X, y, centers, 3, num_f, 1)
        new_c = util.find_centroids(X, y, num_f)
    
    if("int_den_comp" in tr):
        X, y = trans.int_den_comp(X, y, centers, 2, num_f, 1)
        new_c = util.find_centroids(X, y, num_f)
        
    if("int_size_grow" in tr):
        X, y = trans.int_size_grow(X, y, centers, 0, num_f, 30)
        new_c = util.find_centroids(X, y, num_f)
        
    if("int_size_reduc" in tr):
        X, y = trans.int_size_reduc(X, y, 2, 6)
        new_c = util.find_centroids(X, y, num_f)
                
    if("int_local" in tr):   
        X, y = trans.int_local(X, y, 2, num_f, (-8,8))
        new_c = util.find_centroids(X, y, num_f)
        
    #external transitions
                
    return X, y, colors, new_c
    