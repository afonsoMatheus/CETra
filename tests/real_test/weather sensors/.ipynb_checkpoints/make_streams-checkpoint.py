import csv
import pandas as pd
import glob

files = glob.glob('**/**/*.csv', recursive=True)

i = 1
info = open("/home/afonso/Documentos/Dissertação/Masters Degree/tests/real_test/info", "w")
for fp in files:
    
    df = pd.read_csv(fp)
    
    info.write(str(i) + " " + fp + "\n")
    
    df.sort_values(by=['Data'])
    
    doc = open("/home/afonso/Documentos/Dissertação/Masters Degree/tests/real_test/podclus/wInput/" + str(i), "w")
    
    aux = list(df['Temperatura_Maxima'])
    
    doc.write("TemperaturaMaxima\n")
    [doc.write(str(t)+"\n") for t in aux]
    
    i = i + 1
    
    doc.close()
    
info.close()