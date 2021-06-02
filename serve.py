import os
import sys
import re

def run(filename,outfilename):
    with open(filename,'r') as file:
        r = file.read()
    crit = re.compile(r'#include"(.*?)"')
    while len(crit.findall(r)) > 0:
        for match in crit.finditer(r):
            header_name = match.group(1)
            matched_string = match.group(0)
            print(matched_string)
            try:
                with open(header_name,'r') as file:
                    header_content = file.read()
                r = r.replace(matched_string,header_content)
            except FileNotFoundError:
                print(header_name+" is not found within folder")
    with open(outfilename,'w') as file:
        file.write(r)

if __name__=="__main__":
    if len(sys.argv)<3:
        print("usage: python "+sys.argv[0]+" (input) (output)")
    else:
        run(sys.argv[1],sys.argv[2])
