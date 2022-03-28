import numpy as np

s = list(range(0, 1500))

c = np.concatenate((np.full(250, 1, dtype=int), np.full(250, 2, dtype=int), np.full(500, 3, dtype=int), np.full(500, 5, dtype=int)), axis=0)  

w = np.full(1500, 1, dtype=int)

doc = open("/home/afonso/Documentos/Dissertação/Masters Degree/tests/basic_test/" + "c1", "w")

for i in s:
	doc.write(str(i) + " ")
doc.write("\n")

for i in c:
	doc.write(str(i) + " ")
doc.write("\n")

for i in w:
	doc.write(str(i) + " ")
doc.write("\n")

doc.close()

############################################################################

s = list(range(0, 1500))

c = np.concatenate((np.full(500, 1, dtype=int), np.full(250, 2, dtype=int), np.full(250, 3, dtype=int), np.full(500, 3, dtype=int)), axis=0)  

w = np.full(1500, 1, dtype=int)

doc = open("/home/afonso/Documentos/Dissertação/Masters Degree/tests/basic_test/" + "c2", "w")

for i in s:
	doc.write(str(i) + " ")
doc.write("\n")

for i in c:
	doc.write(str(i) + " ")
doc.write("\n")

for i in w:
	doc.write(str(i) + " ")
doc.write("\n")

doc.close()
