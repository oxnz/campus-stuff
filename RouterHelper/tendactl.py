#!/usr/bin/env python

import sys, getopt

if __name__ == '__main__':
	from RouterHelper import Tenda
	t = Tenda()
	try:
		opts, args = getopt.getopt(sys.argv[1:], "hs:")
		for op, value in opts:
			if op == "-h":
				t.help()
				exit
			elif op == "-s":
				t.login()
				t.speedlimit(speed=int(value),enable=1)
	except getopt.GetoptError, e:
		print '*** Error: %s' % e
		sys.exit(1)
