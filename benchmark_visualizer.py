import os
import ast
import numpy as np
import pylab as pl
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D


# -----------------------------------------------------------------------            
# -----------------------------------------------------------------------            

# -----------------------------------------------------------------------            
def process_benchmark_dict(bmk_dict):
    """
    """
    print(bmk_dict)
# -----------------------------------------------------------------------            

# -----------------------------------------------------------------------            
def parse_benchmark_file(filename):
    """
    """
    with open(filename) as f:
        content = f.readlines()
        for line in content : 
            process_benchmark_dict(ast.literal_eval(line))
# -----------------------------------------------------------------------            

# -----------------------------------------------------------------------            
def main() : 
    var = raw_input("Enter the benchmark file: ")
    parse_benchmark_file(var)

if __name__ == '__main__' : 
    main()
# -----------------------------------------------------------------------            

