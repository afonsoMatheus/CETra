from sklearn.datasets import make_blobs
import transitions as trans
import util, copy

def init_clustering():
    
    evo_clustering = {}
    centers = {}
    num_f = 3
    
    X, y = make_blobs(n_samples=[100,100,100,100], centers=[(30,40, 35),(20,25,22),(10,12,11),(10,30,15)], n_features=num_f, random_state=1)
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
        X, y = trans.int_den_dif(X, y, centers, [3], num_f, 0.5)
        new_c = util.find_centroids(X, y, num_f)
    
    if("int_den_comp" in tr):
        X, y = trans.int_den_comp(X, y, centers, [2], num_f, 1)
        new_c = util.find_centroids(X, y, num_f)
        
    if("int_size_grow" in tr):
        X, y = trans.int_size_grow(X, y, centers, [0], num_f, 30)
        new_c = util.find_centroids(X, y, num_f)
        
    if("int_size_reduc" in tr):
        X, y = trans.int_size_reduc(X, y, [1], 80)
        new_c = util.find_centroids(X, y, num_f)
                
    if("int_local" in tr):   
        X, y = trans.int_local(X, y, [2], num_f, (-8,8))
        new_c = util.find_centroids(X, y, num_f)
        
    #external transitions
    
    if("ext_death" in tr):
        X, y, colors = trans.ext_death(X, y, [3], colors)
        new_c = util.find_centroids(X, y, num_f)
        
    if("ext_birth" in tr):
        X, y, colors = trans.ext_birth(X, y, colors, num_f, 15)
        new_c = util.find_centroids(X, y, num_f)
        
    if("ext_union" in tr):
        X, y, colors = trans.ext_union(X, y, colors, [1,5,6], [30,40,20], num_f)
        new_c =util.find_centroids(X, y, num_f)
        
    if("ext_div" in tr):
        X, y, colors = trans.ext_div(X, y, colors, [0], [[30,25,20],[20,40,25],[30,10,20]], [0.3,0.3,0.4], num_f)
        new_c = util.find_centroids(X, y, num_f)
        
                
    return X, y, colors, new_c
    