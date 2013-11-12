#!/bin/sh
# Author: Oxnz <yunxinyi@gmail.com>
# This file is part of the RDPP Project
# Copyright (C) 2013, All rights reserved

ofile="./chart.js"
if [[ -e $ofile ]]; then
	: > $ofile
fi
find . -name "2012*.dat.js" -exec cat >> $ofile {} \;
sed -i -e 's/,);/);/' $ofile
echo "ARRAY" >> $ofile
grep new $ofile | sed 's/var //' | sed 's/ = new Array(/,/' >> $ofile
sed -i -e 's/ARRAY/var data = [/' $ofile
sed -i -e '$ s/,/];/' $ofile
echo "DAY" >> $ofile
grep new $ofile | sed 's/var /"/' | sed 's/ = new Array(/",/' >> $ofile
sed -i -e 's/DAY/var day = [/' $ofile
sed -i -e '$ s/,/];/' $ofile

cat >> $ofile << DRAWCHART

var index = 0;

function onload() {
	if (data.length > 0)
		show(data[index]);
}

/*
 * @cmax: mac car cound in ts minute(s).
 * @ts: time slot length in minute(s).
 * @h: height
 * @w: width
 */
Array.prototype.draw = function(ctx, ts, cmax, h, w, title) {
	ctx.fillStyle = '#FFFFFF';
	ctx.fillRect(0, 0, w, h);
	var dx = w*ts/(24*60);
	var dy = h/cmax;
	var n = 24*60/ts;
	ctx.fillStyle = '#00FF00';
	for (var i = 0; i < n; ++i) {
		ctx.fillRect(i*dx, h - dy*this[i], 2, dy*this[i]);
	}
	ctx.fillStyle = '#FF0000';
	ctx.font = '10px Arial';
	dx = w/24;
	for (var i = 0; i < 25; ++i) {
		ctx.fillRect(i*dx, 0, 1, h);
		ctx.fillText(i, i*dx+dx/3, 10);
	}
	ctx.fillText(title, w/2, h/2);
}

function show(d) {
	d.draw(
			ctx = document.getElementById('chart').getContext('2d'),
			ts = 3,
			cmax = 20000,
			h = 400,
			w = 960,
			title = day[index]
			);
}

function showPrev() {
	if (--index >= 0)
		show(data[index]);
	else
		alert("Already the first day, index = " + index);
}

function showNext() {
	if (++index < data.length)
		show(data[index]);
	else
		alert("Already the last day, index = " + index);
}

DRAWCHART
