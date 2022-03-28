import numpy as np
import random as rand

def writeClustering(s, c, w, S, j):

	doc = open("/home/afonso/Documentos/Dissertação/Masters Degree/tests/stress_test/" + "c" + str(j), "w")

	for i in s:
		doc.write(str(i) + " ")
	doc.write("\n")

	for i in c:
		doc.write(str(i) + " ")
	doc.write("\n")

	for i in w:
		doc.write(str(i) + " ")
	doc.write("\n")

	for i in S:
		doc.write(str(i) + " ")
	doc.write("\n")

	doc.close()

def makeClustering(nsensors):
	
	s = list(range(0, nsensors))
	c = np.concatenate((np.full(int(nsensors/2), 1, dtype=int), np.full(int(nsensors/2), 2, dtype=int)), axis=0)  
	w = np.full(nsensors, 1, dtype=int)
	S = [100,100]

	return s, c, w, S

nsensors = 100

for j in range(20):

	s, c, w, S = makeClustering(nsensors)

	nsensors = nsensors + 100

	writeClustering(s, c, w, S, j)





