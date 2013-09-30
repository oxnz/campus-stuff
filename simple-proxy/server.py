#!/usr/bin/env python
#coding=utf-8

#Author     : 0xnz<yunxinyi@gmail.com
#Last-update: 2013-09-30 18:48:18
#Copying    : Copyright (c) 2013 oxnz, All rights reserved.

import sys
import socket
import BaseHTTPServer
from SimpleHTTPServer import SimpleHTTPRequestHandler

def main():
    port = 8000
    if sys.argv[1:]:
        try:
            port = int(sys.argv[1])
        except ValueError:
            print 'Usage: {0} [port]'.format(__file__)
    server_addr = ('127.0.0.1', port)
    SimpleHTTPRequestHandler.protocol_version = 'HTTP/1.0'
    try:
        httpd = BaseHTTPServer.HTTPServer(server_addr, SimpleHTTPRequestHandler)
    except socket.error, e:
        if e.errno == 13:
            print 'bind port under 1024 needs to be root, try preifx "sudo"'
        else:
            print '*** error: {0}'.format(e.message)
            return
    else:
        httpd.serve_forever()

if __name__ == '__main__':
    main()
