import numpy as np
import random as rand

def writeClustering(s, c, w, S, j):

	doc = open("/home/afonso/Documentos/Dissertação/Masters Degree/tests/cont_test/" + "c" + str(j), "w")

	for i in s:
		doc.write(str(i) + " ")
	doc.write("\n")

	for i in c:
		doc.write(str(i) + " ")
	doc.write("\n")

	for i in w:
		doc.write(str(i) + " ")
	doc.write("\n")

	for k in S.keys():
		for i in S[k]:
			doc.write(str(i) + " ")
		doc.write("\n")

	doc.close()


s = list(range(0, 750))
c = np.concatenate((np.full(250, 1, dtype=int), np.full(250, 2, dtype=int), np.full(250, 3, dtype=int)), axis=0)  
w = np.full(750, 1, dtype=int)

S = dict()
S[0] = [10, 15, 20]

clusters = {}
clusters[0] = []
clusters[1] = []
clusters[2] = []
for i in range(len(S[0])):
	clusters[i].append(S[0][i])

writeClustering(s, c, w, S, 0)

for j in range(1, 20):

	for i in S.keys():

		for x in range(len(S[i])):

			inc = round(rand.uniform(0.7,1.3), 1)

			S[i][x] = S[i][x]*inc

			clusters[x].append(S[i][x])

	writeClustering(s, c, w, S, j)

d = open("/home/afonso/Documentos/Dissertação/Masters Degree/tests/cont_test/graph", "w")

for i in clusters.keys():
	for j in clusters[i]:
		d.write(str(j) + " ")
	d.write("\n")

for j in range(1,21):
	d.write(str(j) + " ")
d.write("\n")

d.close()





