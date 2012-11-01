#!/bin/sh
if [ `id -u` -ne 0 ]
then
	echo "Root privilige needed"
	exit 1
fi

echo “安装必要软件”
p7zip
unrar 
flash-plugin-installer
vim-data

echo "字体配置"
mkfontdir
fc-cache -fv
