#!/usr/bin/python
# -*- coding: utf-8 -*-

"""
----------------------------------------------------------
Benchmark visualizer           
----------------------------------------------------------
Utility to plot the output produced by benchmark.h        
----------------------------------------------------------
author         : Nikos Athanasiou
last modified  : 21 / 01 / 2017
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
from tkinter import *
import tkinter.filedialog as tkFileDialog
from collections import Counter
import statistics as stat

# -----------------------------------------------------------------------   
def print_data_stats(datalist, name): 
    """ console print of a datalists's statistical measures
    """
    print("=============================================================")
    print("\t{}".format(name))
    print("--------------------- averages & measures of central location")
    print("mean (average)          : {}".format(stat.mean(datalist)))           
    print("median (middle value)   : {}".format(stat.median(datalist)))
    print("median_low              : {}".format(stat.median_low(datalist)))
    print("median_high             : {}".format(stat.median_high(datalist)))   
    print("median_grouped          : {}".format(stat.median_grouped(datalist)))
    try: 
        print("mode (most common value): {}".format(stat.mode(datalist)))
    except Exception as err:
        print("mode: {}".format(err))
    print("------------------------------------------ measures of spread")
    print("population standard deviation: {}".format(stat.pstdev(datalist)))
    print("population variance          : {}".format(stat.pvariance(datalist)))
    print("sample standard deviation    : {}".format(stat.stdev(datalist)))
    print("sample variance              : {}".format(stat.variance(datalist)))
    print("=============================================================")
# -----------------------------------------------------------------------   

# -----------------------------------------------------------------------   
def get_numeric_list(any_list):
    """ Gets a list of numbers out of a list of strings 
    (+ mag specifiers) or a list of numbers (ONLY ints)
    """ 
    ret_list = []
    for elem in any_list:
        if isinstance(elem, str):          
            num = int(filter(str.isdigit, elem))
            if 'k' in elem:   num *= 1e03
            elif 'M' in elem: num *= 1e06
            elif 'G' in elem: num *= 1e09
            ret_list.append(num)
        else: ret_list.append(elem)

    return ret_list
# -----------------------------------------------------------------------

# -----------------------------------------------------------------------   
def get_description_list(any_list):
    """ Gets a list of strings out of a list of numbers or strings
    """ 
    ret_list = []
    for elem in any_list:
        if isinstance(elem, int):          
            if elem >= 1000000: val = '{}M'.format(elem / 1000000.)
            elif elem >= 1000: val = '{}K'.format(elem / 1000.)
            else: val = '{}'.format(elem)
            ret_list.append(val)
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
    xlegend = get_description_list(bmk['factors'])
    ydata = bmk['timings']

    plt.xlabel(bmk['factor_name'])
    plt.ylabel(bmk['time_type'])

    plt.plot(xdata, ydata, syb, color=col, label=bmk['experiment_name'])
    #plt.semilogx(xdata, ydata, syb, color=col, label=bmk['experiment_name'])

    #plt.xticks(xdata, bmk['factors'])
    plt.xticks(xdata, xlegend)
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
        plot_factored_benchmark(bmk, next(col), next(syb))

    legend = plt.legend(loc='upper left', shadow=True)
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
def plot_bar_chart(distr_name, data_Counter): 
    """ Plot a distribution as a bar chart (for low counts of classes)
    """
    objects = [x for x in data_Counter.keys()]
    y_pos = np.arange(len(objects))
    performance = [x for x in data_Counter.values()]
     
    plt.bar(y_pos, performance, align='center', alpha=0.5)
    plt.xticks(y_pos, objects)
    plt.ylabel('Count')
    plt.title(distr_name)
     
    plt.show()
# -----------------------------------------------------------------------            

# -----------------------------------------------------------------------            
def plot_histogram(distr_name, distr_data): 
    """ Make a histogram of the distribution
    """
    bins = np.arange(min(distr_data), max(distr_data) + 1, 10) # fixed bin size

    plt.xlim([min(distr_data) - 5, max(distr_data) + 5])
    plt.hist(distr_data, alpha=0.5) #bins=bins, 
    plt.title(distr_name)
    plt.xlabel('generated numbers')
    plt.ylabel('count')
    plt.show()
# -----------------------------------------------------------------------            

# -----------------------------------------------------------------------            
def plot_distribution(distr_name, distr_data): 
    """ Plot a distribution
    """ 
    print_data_stats(distr_data, distr_name)
    unique_data = np.unique(distr_data)
    data_dict = Counter(distr_data)
    if 11 > len(unique_data): 
        plot_bar_chart(distr_name, data_dict)
    else: 
        plot_histogram(distr_name, distr_data)
# -----------------------------------------------------------------------            

# -----------------------------------------------------------------------            
def plot_distribution_file(filename):
    """ Make a histogram of the distribution
    """
    with open(filename) as f:
        content = f.readlines() # every line is a dictionary
        all_dicts = [ast.literal_eval(line) for line in content]
        for cur in all_dicts: 
            for key in cur:
                plot_distribution(key, cur[key])
# -----------------------------------------------------------------------            

# -----------------------------------------------------------------------            
def open_distribution_file():
    nam = tkFileDialog.askopenfilename()
    if nam != '':
        plot_distribution_file(nam)
# -----------------------------------------------------------------------            

# -----------------------------------------------------------------------            
def openwindows():
    nam = tkFileDialog.askopenfilename()
    if nam != '':
        plot_benchmark_file(nam)

def plot_benchmarks():
    root = Tk()
    myfiletypes = [('Python files', '*.py'), ('All files', '*')]
    open = tkFileDialog.Open(root, filetypes = myfiletypes)
    Button(root, text="PLOT Benchmark", command=openwindows).pack()
    Button(root, text="PLOT Distribution", command=open_distribution_file).pack()

    statusbar = Label(root, text="", bd=1, relief=SUNKEN, anchor=W)
    statusbar.pack(side=BOTTOM, fill=X)
    root.mainloop()

def main(): 
    plot_benchmarks()

if __name__ == '__main__': 
    main()
# -----------------------------------------------------------------------            

