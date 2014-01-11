#!/usr/bin/env python
#-*- coding: utf-8 -*-

import sys

class LRParser:
    def __init__(self):
        self.__vnt = {}
    class Production:
        pass
    def addprod(self, p):
        try:
            left, right = p.split('::=', 1)
            print "prod: {0} -> {1}".format(left.strip(), right.strip())
        except ValueError as e:
            print "Invalid production: ({0})".format(e.message)

def main(args):
    parser = LRParser();
    ifs = sys.stdin
    for line in ifs:
        parser.addprod(line.strip())

if __name__ == "__main__":
    main(sys.argv)
