#!/usr/bin/env python

'''
Tenda:
	operates on Tenda-series router
'''

import urllib
import urllib2
import httplib
import cookielib

class Tenda:
	def __init__(self, ip='192.168.0.1', debuglevel=0):
		'''
		Init cookie and setup other things
		'''
		self.httpHandler	= urllib2.HTTPHandler(debuglevel)
		self.httpsHandler	= urllib2.HTTPHandler(debuglevel)
		self.cookie		= cookielib.CookieJar()
		self.opener		= urllib2.build_opener(self.httpHandler, self.httpsHandler, urllib2.HTTPCookieProcessor(self.cookie))
		urllib2.install_opener(self.opener)
		self.ip			= ip
		self.header		= None
		self.data		= None
		self.request		= None
		self.response		= None
		self.content		= None
	def login(self, username='admin', password='admin'):
		'''
		Login to Tenda router for operations
		'''
		self.header = {
		'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8',
		'Connection': 'keep-alive',
		'User-Agent': 'Mozilla/5.0',
		'Content-type': 'application/x-www-form-urlencoded',
		'Accept-Encoding': 'gzip,deflate,sdch',
		'Accept-Language': 'en-US,en;q=0.8,zh-CN;q=0.6,zh;q=0.4',
		'Accept-Charset': 'GBK,utf-8;q=0.7,*;q=0.3'
		}
		self.data = urllib.urlencode({})
		self.request = urllib2.Request('http://'+self.ip+'/login.asp')
		self.request.add_header('User-Agent', 'Mozilla')
		try:
			self.response = urllib2.urlopen(self.request)
		except urllib2.HTTPError, e:
			print '*** Error: %s' % e
			return
		except urllib2.URLError, e:
			print '*** Error: %s' % e
			return
		self.data = urllib.urlencode({
			'Referer': 'http://'+self.ip+'/login.asp',
			'checkEn': 0,
			'Username': username,
			'Password': password,
			'invalid': None
			})
		self.request = urllib2.Request('http://'+self.ip+'/LoginCheck', self.data, self.header)
		try:
			self.response = urllib2.urlopen(self.request)
		except urllib2.HTTPError, e:
			print '*** Error: %s' % e
			return
		except urllib2.URLError, e:
			print '*** Error: %s' % e
			return
		if self.response.getcode() == 200:
			print 'Login Success'
		else:
			print 'Login Failed with redirect to: %s' % self.response.geturl()
	def speedlimit(self, speed=200, start=2, stop=254, direction=1, enable=True):
		'''
		limit speed for specified IP.
		'''
		try:
			self.response = urllib2.urlopen(
		'&'.join(['http://'+self.ip+'/goform/trafficForm?GO=net_tc.asp',
		'tc_enable=%d' % enable,
		'up_Band=12800',
		'down_Band=12800',
		'cur_number=1',
		'tc_list_1=80,%d,%d,%d,%d,%d,1,1' % (start, stop, direction, speed, speed)]))
		except urllib2.HTTPError, e:
			print '*** Error: %s' % e
			return
		except urllib2.URLError, e:
			print '*** Error: %s' % e
			return
		if self.response.getcode() == 200:
			print 'Speed Limit Success'
		else:
			print '*** Error: %d' % self.response.getcode()
	def help(self):
		print '''RouterHelper:\tA Tool Used For Router Management
	-l\t--login\tsign in the router
	-s\t--\speedlimit\tlimit the speed
	-h\t--help\tprint this message and exit
Bug Report: Yun Xinyi <yunxinyi@gmail.com>'''
