#!/usr/bin/env python3
# -*- coding: utf-8 -*-
##################################################
# 
# Title: bin2hex
# Created: Tuesday, 25 June 2019
# Author: Raghav Subbaraman
# Organisation: IIT Madras
# A python script that converts large binary
# values in a CSV file to the corresponding hex
# format.
# Changelog: 
#	25 June 2019: File created
##################################################

from optparse import OptionParser
import sys
import time
from sys import stderr, exit
import csv
import pandas as pd
import pdb


def get_options():
	usage="%prog: [options] <input_csv>"
	parser = OptionParser( usage=usage)
	parser.add_option("-N", "--axiw", type="int", dest="axi_w",
		default=64,
		help="Width of the AXI bus")
	parser.add_option("-m", "--mode", type="int", dest="mode",
		default=0,
		help="Mode of variable conversion (0 - For var -> AXI | 1 - for AXI -> var)")
	(options, filename) = parser.parse_args()
	return (options, filename)

def bin2hex(bigbin_vec):

	length = bigbin_vec.shape[0]
	for ind in range(0,length):
		# bigbin_vec[ind] = hex(int(bigbin_vec[ind], 2));#+',';
		hex1 = hex(int(bigbin_vec[ind][64:128], 2));
		hex2 = hex(int(bigbin_vec[ind][0:64], 2));
		bigbin_vec[ind] = hex1+','+hex2;



def main():
	(options,filename) = get_options()
	options.filename = filename;
	df_in = pd.read_csv(options.filename[0])
	df_out = pd.DataFrame()

	bigbin_vec = df_in['tdata'];
	# pdb.set_trace();
	bin2hex(bigbin_vec)
	df_out = df_in['tdata'];
	df_out.to_csv('out.csv',index=False,header=False)#quoting=csv.QUOTE_NONE,escapechar='\n');
	# pdb.set_trace()
	print(options)

if __name__== "__main__":
	main()