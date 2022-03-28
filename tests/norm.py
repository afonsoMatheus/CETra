from os import listdir
from os.path import isfile, join
import re

mypath = "/home/afonso/Documentos/Dissertação/Masters Degree/tests/basic_test/originals/"

files = [f for f in listdir(mypath) if isfile(join(mypath, f))]

files.sort()

kv = dict()

for x in files:
	
	f = open(mypath + x,'r')

	name = x.partition('-')[0]
	
	if name not in kv:
		kv[name] = 1

	w = open("/home/afonso/Documentos/Dissertação/Masters Degree/tests/basic_test/" + name + str(kv[name]), "w")
	kv[name] = kv[name] + 1

	lines = f.readlines()[1:]

	int_sta = dict()
	inter = []

	sensors = []
	clusters = []
	weights = []

	for line in lines:
		if ",\"" in line:

			if "\"cluster\"" in line:
				line = line.replace("\"cluster\",", "")

			if "\"og\"" in line:
				line = line.replace("\"og\",", "")

			result1 = re.search('\",(.*)', line)
			inter = result1.group(1)
			int_sta[line.partition(',')[0]] = inter

			result = re.search('\"(.*)\"', line)
			aux = result.group(1)

			sensors.append(aux.replace(","," "))
			
			#w.write(aux.replace(","," ") + "\n")
			
			for i in range(aux.count(',') + 1):
				clusters.append(line.partition(',')[0])
				#w.write(line.partition(',')[0] + " ")
			#w.write("\n")

			for i in range(aux.count(',') + 1):
				weights.append(str(1))
				#w.write(str(1) + " ")
			#w.write("\n")
			#w.write("\n")

	for i in sensors:
		w.write(i + " ")
	w.write("\n")

	for i in clusters:
		w.write(i + " ")
	w.write("\n")

	for i in weights:
		w.write(i + " ")
	w.write("\n")

	for i in range(inter.count(',') + 1):
		for j in int_sta.keys():
			w.write(int_sta[j].split(',')[i] + " ")

		w.write("\n")


	f.close()
	w.close()

