import random as rand

def writeStream(i):

	doc = open("/home/afonso/Documentos/Dissertação/Masters Degree/tests/real_test/podclus/in/" + str(i), "w")

	for j in range(10):
		sta = round(rand.uniform(10.0,100.0), 1)
		doc.write(str(sta))
		doc.write("\n")

	doc.close()

for i in range(1,501):

	writeStream(i)
