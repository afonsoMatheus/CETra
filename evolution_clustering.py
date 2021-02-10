from sklearn.datasets import make_blobs
import transitions as trans
import util, copy

def init_clustering():
    
    evo_clustering = {}
    centers = {}
    num_f = 3
    
    X, y = make_blobs(n_samples=[10,10,10,10], centers=[(30,40),(20,25),(10,12),(5,40)], n_features=num_f,random_state=1)
    colors = util.att_colors(y)
                
    evo_clustering[0] = X,y,colors
    centers[0] = util.find_centroids(evo_clustering[0])
            
    return evo_clustering, centers

def new_clustering(evo_clusters, centers, tr):
    
    next_clustering = copy.deepcopy(evo_clusters)
        
    last_t = list(evo_clusters)[-1]
    
    X, y, colors = evo_clusters[last_t]
    evo_clusters.clear()
    
    #transitions
    
    X, y, colors = ch_transition(tr, X, y, colors,  centers[last_t])
    
    #updating clustering
                
    next_clustering[last_t+1] = X, y, colors
    
    centers[last_t+1] = util.find_centroids(next_clustering[last_t+1])
    
    return next_clustering, centers


def ch_transition(tr, X, y, colors, centers):
    
    #internal transitions
            
    if("int_den_dif" in tr):
        X, y = trans.int_den_dif(X, y, centers, 3, 0.8, 0.8)  
    
    if("int_den_comp" in tr):
        X, y = trans.int_den_comp(X, y, centers, 1, 1, 1)
        
    if("int_size_grow" in tr):
        X, y = trans.int_size_grow(X, y, centers, 0, 30)
        
    if("int_size_reduc" in tr):
        X, y = trans.int_size_reduc(X, y, 1, 6)
                
    if("int_local" in tr):   
        X, y = trans.int_local(X, y, 2, -5, 10)
        
    #external transitions
                
    return X, y, colors
    