#!/usr/bin/python
# -*- coding: utf-8 -*-

"""
----------------------------------------------------------
Benchmark visualizer           
----------------------------------------------------------
Utility to plot the output     
produced by benchmark.h        
----------------------------------------------------------
author         : Nikos Athanasiou
last modified  : 04 / 04 / 2015  
website        : https://ngathanasiou.wordpress.com/ 
----------------------------------------------------------
"""
                        
import os
import ast
import numpy as np
import pylab as pl
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from itertools import cycle
import matplotlib.patches as mpatches
from matplotlib.patches import Polygon
import pandas as pd
from Tkinter import *
import tkFileDialog

# -----------------------------------------------------------------------   
def get_numeric_list(any_list):
    """ Gets a list of numbers out of a list of strings 
    (+ mag specifiers) or a list of numbers (ONLY ints)
    """ 
    ret_list = []
    for elem in any_list:
        if isinstance(elem, basestring):          
            num = int(filter(str.isdigit, elem))
            if 'k' in elem:   num *= 1e03
            elif 'M' in elem: num *= 1e06
            elif 'G' in elem: num *= 1e09
            ret_list.append(num)
        else: ret_list.append(elem)

    return ret_list
# -----------------------------------------------------------------------            

# -----------------------------------------------------------------------            
def plot_simple_bmks(simples):
    """ All benchmarks (dictonaries) printed in the same graph
    """
    if len(simples):
        plt.clf()
        data = []
        labl = []
        
        for bmk in simples:
            labl.append(bmk['experiment_name'])
            data. append(np.array(bmk['timings']))

        plt.title(simples[0]['benchmark_name']) 
        plt.boxplot(data)

        plt.ylabel(bmk['time_type'])
        plt.xticks(range(1, 1+len(simples)), labl)
        
        plt.show()
# -----------------------------------------------------------------------            

# -----------------------------------------------------------------------            
def plot_factored_benchmark(bmk, col, syb):
    """ bmk is a dictionary
    """
    xdata = get_numeric_list(bmk['factors'])
    ydata = bmk['timings']

    plt.xlabel(bmk['factor_name'])
    plt.ylabel(bmk['time_type'])

    plt.semilogx(xdata, ydata, syb, color=col, label=bmk['experiment_name'])

    plt.xticks(xdata, bmk['factors'])
    plt.xlim(min(xdata), max(xdata))
    
def plot_factored_bmks(factored):
    """ Each benchmark will have its own plot curve
    """
    plt.clf()
    cols = ['r', 'g', 'b', 'c', 'y', 'k', 'pink', 'gray']
    sybs = ['-o', '-s', '-*', '-+', '-v', '-x', '--o', '--s', '-D']

    if len(factored): plt.title(factored[0]['benchmark_name']) 
    col = cycle(cols)
    syb = cycle(sybs)
    for bmk in factored:
        plot_factored_benchmark(bmk, col.next(), syb.next())

    legend = plt.legend(loc='upper right', shadow=True)
    #legend.get_frame().set_facecolor('#00FFCC') 
    plt.show()
# -----------------------------------------------------------------------            

# -----------------------------------------------------------------------            
def plot_benchmark_dicts(bmk_dict_list):
    """ Plot the list of benchmark dictionaries
    """
    factored = [x for x in bmk_dict_list if 'factors' in x]
    simples  = [x for x in bmk_dict_list if 'factors' not in x]
    if len(factored): plot_factored_bmks(factored)
    if len(simples): plot_simple_bmks(simples)
# -----------------------------------------------------------------------            

# -----------------------------------------------------------------------            
def plot_benchmark_file(filename):
    """ Plot the benchmark file
    """
    with open(filename) as f:
        content   = f.readlines() # every line is a dictionary
        all_dicts = [ast.literal_eval(line) for line in content]
        plot_benchmark_dicts(all_dicts)
# -----------------------------------------------------------------------            

# -----------------------------------------------------------------------            
def openwindows():
    nam = tkFileDialog.askopenfilename()
    if nam != '':
        plot_benchmark_file(nam)

def main(): 
    root = Tk()
    myfiletypes = [('Python files', '*.py'), ('All files', '*')]
    open = tkFileDialog.Open(root, filetypes = myfiletypes)
    Button(root, text="PLOT Benchmark", command=openwindows).pack()

    statusbar = Label(root, text="", bd=1, relief=SUNKEN, anchor=W)
    statusbar.pack(side=BOTTOM, fill=X)
    root.mainloop()

if __name__ == '__main__': 
    main()
# -----------------------------------------------------------------------            

