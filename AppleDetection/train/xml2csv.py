import xml.etree.ElementTree as Xet 
import pandas as pd 
import os
  
cols = ["filename", "width", "height", "name", "xmin", "ymin", "xmax", "ymax"] 
rows = [] 
  

directory = os.getcwd()

path = directory

def parse(file):
    # Parsing the XML file 
    xmlparse = Xet.parse(file) 
    root = xmlparse.getroot() 
    for i in root.iter(): 
        if i.tag == "filename":
            filename = i.text
        if i.tag == "width":
            width = i.text
        if i.tag == "height":
            height = i.text
        if i.tag == "name":
            name = i.text
        if i.tag == "xmin":
            xmin = i.text
        if i.tag == "ymin":
            ymin = i.text
        if i.tag == "xmax":
            xmax = i.text
        if i.tag == "ymax":
            ymax = i.text
 

    rows.append({"filename" : filename, 
            "width" : width,
            "height" : height,
            "name": name,
            "xmin" : xmin,
            "ymin" : ymin,
            "xmax" : xmax,
            "ymax" : ymax}) 

for filename in os.listdir(path):
    if filename.endswith('.xml'):
        parse(filename)

  
df = pd.DataFrame(rows) 
  
# Writing dataframe to csv 
df.to_csv('output.csv') 