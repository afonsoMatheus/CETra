import plotly.graph_objects as go
import seaborn as sns
import numpy as np
import sys

inp = sys.argv[1]
out = sys.argv[2]

with open("/home/afonso/Documentos/Dissertação/Masters Degree/tests/basic_test/output/" + inp, 'r') as file:
    source = list(file.readline().split(" "))[:-1]
    target = list(file.readline().split(" "))[:-1]
    value = list(file.readline().split(" "))[:-1]
    label = list(file.readline().split(" "))[:-1]

cm = dict()
uni = list(np.unique(source + target))

d = []
for i in range(len(target)):
    if label[int(target[i])] == "DEATH":
        d.append(source[i])

color = sns.color_palette("Paired", len(uni)).as_hex()

j = 0
for i in range(len(uni)):
    if label[int(uni[i])] == "DEATH" or uni[i] in d: 
        cm[uni[i]] = 'rgba(0, 0, 0, 0.5)'
        label[int(uni[i])] = ""
    elif (label[int(uni[i])] == "BIRTH"):
        cm[uni[i]] = 'rgba(207, 207, 207, 0)'
        label[int(uni[i])] = ""
    else:
        cm[uni[i]] = color[j]
        j = j + 1
            
link_colors = []
for i in source:
    if i in d:
        link_colors.append('rgba(207, 207, 207, 0)')
    else:
        link_colors.append(cm[i])

node_colors = []
for i in range(len(label)):
    node_colors.append(cm[str(i)])
    
node = dict(label = label, pad=50, thickness=20, color = node_colors, line = dict(color = "rgba(207, 207, 207, 0)", width = 0.5))
link = dict(source = source, target = target, value = value, color = link_colors)
data = go.Sankey(link = link, node = node, valueformat='.r')

fig = go.Figure(data)
fig.update_layout(title_text="External Transitions Sankey Diagram", 
                  title_font_color="black", font_color = "black", font_family = "sans-serif", font_size=16)

fig.write_image("/home/afonso/Documentos/Dissertação/Masters Degree/tests/basic_test/output/" + out, scale = 3.5)