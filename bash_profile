#!/bin/bash
bashrc文件设置 
分类： debian 2009-12-31 17:50
    ~/.bashrc文件决定了交互shell的行为. 好好的了解这个文件, 将会使你更加了解Bash.
    下面是一个人的.bashrc文件，仔细的学习这个文件, 直到你可以自由重用其中的代码片断和函数, 并把它们用到你自己的.bashrc文件中, 甚至可以放到你的脚本中.


    例子 K-1. .bashrc文件样本

1 #===============================================================
2 #
3 # 个人的$HOME/.bashrc文件, 基于bash-2.05a(或更高版本)
4 #
5 # 最后更新日期: 星期2 4月15 20:32:34 CEST 2003
6 #
7 # 这个文件(一般情况下)被只会被交互式shell读取. 
8 # 这里可以定义你的别名, 函数, 
9 # 和其他的一些交互式特征, 比如你的提示符. 
10 #
11 # 这个文件(开始时)是为Solaris设计的, 
12 # 但是基于Redhat的默认.bashrc文件
13 # --> 为Linux修改. 
14 # 你在这里看到的大部分代码都是从网上找来的
15 # (即internet). 
16 # 这个bashrc文件有点挤 - 
17 # 记住, 它仅仅是个例子而已. 按照你自己的需求进行裁减. 
18 #
19 #
20 #===============================================================
21 
22 # --> 注释由HOWTO的作者添加. 
23 # --> 然后又被ER编辑了一下 :-)
24 
25 #--------------------------------------
26 # 如果有源代码的全局定义, 请在此处定义.
27 #--------------------------------------
28 
29 if [ -f /etc/bashrc ]; then
30         . /etc/bashrc   # --> 读取/etc/bashrc, 如果存在的话. 
31 fi
32 
33 #-------------------------------------------------------------
34 # $DISPLAY的自动设置 (如果还没设置的话)
35 # 这用于linux - 可能运行的结果不同.... 
36 # 问题是不同的终端种类对于'who am i'来说, 
37 # 将会给出不同的答案......
38 # 我还没发现一种'通用'方法
39 #-------------------------------------------------------------
40 
41 function get_xserver ()
42 {
43     case $TERM in
44  xterm )
45             XSERVER=$(who am i | awk '{print $NF}' | tr -d ')''(' ) 
46             # Ane-Pieter Wieringa建议使用下面这种方式:
47             # I_AM=$(who am i)
48             # SERVER=${I_AM#*(}
49             # SERVER=${SERVER%*)}
50 
51             XSERVER=${XSERVER%%:*}
52      ;;
53  aterm | rxvt)
54   # 找出一些运行在这里的代码.....
55      ;;
56     esac  
57 }
58 
59 if [ -z ${DISPLAY:=""} ]; then
60     get_xserver
61     if [[ -z ${XSERVER}  || ${XSERVER} == $(hostname) || ${XSERVER} == "unix" ]]; then 
62  DISPLAY=":0.0"  # 在本地主机上显示
63     else  
64  DISPLAY=${XSERVER}:0.0 # 在远端主机上显示
65     fi
66 fi
67 
68 export DISPLAY
69 
70 #----------
71 # 一些设置
72 #----------
73 
74 ulimit -S -c 0  # 不需要任何coredump
75 set -o notify
76 set -o noclobber
77 set -o ignoreeof
78 set -o nounset
79 #set -o xtrace          # 对于调试来说非常有用
80 
81 # 使能选项:
82 shopt -s cdspell
83 shopt -s cdable_vars
84 shopt -s checkhash
85 shopt -s checkwinsize
86 shopt -s mailwarn
87 shopt -s sourcepath
88 shopt -s no_empty_cmd_completion  # 仅限于bash>=2.04
89 shopt -s cmdhist
90 shopt -s histappend histreedit histverify
91 shopt -s extglob # 对于complete命令(按情况补全)来说是必要的
92 
93 # 禁用选项:
94 shopt -u mailwarn
95 unset MAILCHECK  # 当有邮件到达时, 我不希望我的shell提示我
96 
97 
98 export TIMEFORMAT=$'\nreal %3R\tuser %3U\tsys %3S\tpcpu %P\n'
99 export HISTIGNORE="&:bg:fg:ll:h"
100 export HOSTFILE=$HOME/.hosts # 将远端主机的列表放入~/.hosts
101 
102 
103 
104 #-----------------------
105 # 问候, 问侯报文等等...
106 #-----------------------
107 
108 # 先定义一些颜色:
109 red='\e[0;31m'
110 RED='\e[1;31m'
111 blue='\e[0;34m'
112 BLUE='\e[1;34m'
113 cyan='\e[0;36m'
114 CYAN='\e[1;36m'
115 NC='\e[0m'              # 没有颜色
116 # --> 很好. 与使用"ansi.sys"的DOS效果相同. 
117 
118 # 在黑色背景下看起来非常好.....
119 echo -e "${CYAN}This is BASH ${RED}${BASH_VERSION%.*}${CYAN} - DISPLAY on ${RED}$DISPLAY${NC}\n"
120 date
121 if [ -x /usr/games/fortune ]; then
122     /usr/games/fortune -s     # 让我们的每天充满乐趣.... :-)
123 fi
124 
125 function _exit() # 在退出shell时运行的函数
126 {
127     echo -e "${RED}Hasta la vista, baby${NC}"
128 }
129 trap _exit EXIT
130 
131 #---------------
132 # Shell提示符
133 #---------------
134 
135 if [[ "${DISPLAY#$HOST}" != ":0.0" &&  "${DISPLAY}" != ":0" ]]; then  
136     HILIT=${red}   # 远端主机: 提示符为红
137 else
138     HILIT=${cyan}  # 本地主机: 提示符为青色
139 fi
140 
141 #  --> 下面提示符函数中\W和\w的替换实例, 
142 #+ --> 用来获得完整路径名的显示. 
143 
144 function fastprompt()
145 {
146     unset PROMPT_COMMAND
147     case $TERM in
148         *term | rxvt )
149             PS1="${HILIT}[\h]$NC \W > \[\033]0;\${TERM} [\u@\h] \w\007\]" ;;
150  linux )
151      PS1="${HILIT}[\h]$NC \W > " ;;
152         *)
153             PS1="[\h] \W > " ;;
154     esac
155 }
156 
157 function powerprompt()
158 {
159     _powerprompt()
160     {
161         LOAD=$(uptime|sed -e "s/.*: \([^,]*\).*/\1/" -e "s/ //g")
162     }
163 
164     PROMPT_COMMAND=_powerprompt
165     case $TERM in
166         *term | rxvt  )
167             PS1="${HILIT}[\A \$LOAD]$NC\n[\h \#] \W > \[\033]0;\${TERM} [\u@\h] \w\007\]" ;;
168         linux )
169             PS1="${HILIT}[\A - \$LOAD]$NC\n[\h \#] \w > " ;;
170         * )
171             PS1="[\A - \$LOAD]\n[\h \#] \w > " ;;
172     esac
173 }
174 
175 powerprompt     # 这是默认提示符 - 可能比较慢
176                 # 如果很慢的话, 可以使用fastprompt来代替....
177 
178 #===============================================================
179 #
180 # 别名和函数
181 #
182 # 事实上, 这里定义的一些函数非常大
183 # (比如'lowercase'), 但是我的机器是512M内存, 所以 .....
184 # 如果你想让这个文件小一点, 
185 # 可以将这些函数放到脚本中. 
186 #
187 # 其中的许多函数来自于bash-2.04
188 # 中的例子. 
189 #
190 #===============================================================
191 
192 #-------------------
193 # 个人的别名
194 #-------------------
195 
196 alias rm='rm -i'
197 alias cp='cp -i'
198 alias mv='mv -i'
199 # -> 防止偶然的文件误操作. 
200 alias mkdir='mkdir -p'
201 
202 alias h='history'
203 alias j='jobs -l'
204 alias r='rlogin'
205 alias which='type -all'
206 alias ..='cd ..'
207 alias path='echo -e ${PATH//:/\\n}'
208 alias print='/usr/bin/lp -o nobanner -d $LPDEST'   # 假设LPDEST被定义
209 alias pjet='enscript -h -G -fCourier9 -d $LPDEST'  # 使用enscript的漂亮的打印
210 alias background='xv -root -quit -max -rmode 5'    # 将一张图片作为背景
211 alias du='du -kh'
212 alias df='df -kTh'
213 
214 # 'ls'家族 (假定使用GNU ls)
215 alias la='ls -Al'               # 显示隐藏文件
216 alias ls='ls -hF --color' # 为识别的文件类型添加颜色
217 alias lx='ls -lXB'              # 按扩展名排序
218 alias lk='ls -lSr'              # 按尺寸排序
219 alias lc='ls -lcr'  # 按修改时间排序
220 alias lu='ls -lur'  # 按访问时间排序
221 alias lr='ls -lR'               # 递归ls
222 alias lt='ls -ltr'              # 按日期排序
223 alias lm='ls -al |more'         # 管道给'more'
224 alias tree='tree -Csu'  # 'ls'的另一种好方法
225 
226 # 裁减'less'
227 alias more='less'
228 export PAGER=less
229 export LESSCHARSET='latin1'
230 export LESSOPEN='|/usr/bin/lesspipe.sh %s 2>&-' # 如果lesspipe.sh存在, 就用这个
231 export LESS='-i -N -w  -z-4 -g -e -M -X -F -R -P%t?f%f \
232 :stdin .?pb%pb\%:?lbLine %lb:?bbByte %bb:-...'
233 
234 # 拼写错误 - 纯粹个人喜好 :-)
235 alias xs='cd'
236 alias vf='cd'
237 alias moer='more'
238 alias moew='more'
239 alias kk='ll'
240 
241 #----------------
242 # 一些有趣东西
243 #----------------
244 
245 function xtitle ()
246 {
247     case "$TERM" in
248         *term | rxvt)
249             echo -n -e "\033]0;$*\007" ;;
250         *)  
251      ;;
252     esac
253 }
254 
255 # 别名...
256 alias top='xtitle Processes on $HOST && top'
257 alias make='xtitle Making $(basename $PWD) ; make'
258 alias ncftp="xtitle ncFTP ; ncftp"
259 
260 # .. 和函数
261 function man ()
262 {
263     for i ; do
264  xtitle The $(basename $1|tr -d .[:digit:]) manual
265  command man -F -a "$i"
266     done
267 }
268 
269 function ll(){ ls -l "$@"| egrep "^d" ; ls -lXB "$@" 2>&-| egrep -v "^d|total "; }
270 function te()  # xemacs/gnuserv的包装器
271 {
272     if [ "$(gnuclient -batch -eval t 2>&-)" == "t" ]; then
273         gnuclient -q "$@";
274     else
275         ( xemacs "$@" &);
276     fi
277 }
278 
279 #---------------------------
280 # 与文件和字符串相关的函数:
281 #---------------------------
282 
283 # 使用名字模式来查找文件:
284 function ff() { find . -type f -iname '*'$*'*' -ls ; }
285 # 使用pattern $1和Execute $2来查找文件: 
286 function fe() { find . -type f -iname '*'$1'*' -exec "${2:-file}" {} \;  ; }
287 # 在一系列文件中找到模式, 并高亮
288 function fstr()
289 {
290     OPTIND=1
291     local case=""
292     local usage="fstr: find string in files.
293 Usage: fstr [-i] \"pattern\" [\"filename pattern\"] "
294     while getopts :it opt
295     do
296         case "$opt" in
297         i) case="-i " ;;
298         *) echo "$usage"; return;;
299         esac
300     done
301     shift $(( $OPTIND - 1 ))
302     if [ "$#" -lt 1 ]; then
303         echo "$usage"
304         return;
305     fi
306     local SMSO=$(tput smso)
307     local RMSO=$(tput rmso)
308     find . -type f -name "${2:-*}" -print0 | xargs -0 grep -sn ${case} "$1" 2>&- | \
309 sed "s/$1/${SMSO}\0${RMSO}/gI" | more
310 }
311 
312 function cuttail() # 在文件中切掉n行, 默认为10行
313 {
314     nlines=${2:-10}
315     sed -n -e :a -e "1,${nlines}!{P;N;D;};N;ba" $1
316 }
317 
318 function lowercase()  # 将文件名转换为小写
319 {
320     for file ; do
321         filename=${file##*/}
322         case "$filename" in
323         */*) dirname==${file%/*} ;;
324         *) dirname=.;;
325         esac
326         nf=$(echo $filename | tr A-Z a-z)
327         newname="${dirname}/${nf}"
328         if [ "$nf" != "$filename" ]; then
329             mv "$file" "$newname"
330             echo "lowercase: $file --> $newname"
331         else
332             echo "lowercase: $file not changed."
333         fi
334     done
335 }
336 
337 function swap()         # 交换两个文件名
338 {
339     local TMPFILE=tmp.$$
340     mv "$1" $TMPFILE
341     mv "$2" "$1"
342     mv $TMPFILE "$2"
343 }
344 
345 
346 #----------------------
347 # 进程/系统相关的函数:
348 #----------------------
349 
350 function my_ps() { ps $@ -u $USER -o pid,%cpu,%mem,bsdtime,command ; }
351 function pp() { my_ps f | awk '!/awk/ && $0~var' var=${1:-".*"} ; }
352 
353 # 这个函数与linux上的'killall'基本一致
354 # 但是与Solaris上的却不相同
355 function killps()   # 按进程名进行kill
356 {
357     local pid pname sig="-TERM"   # 默认signal
358     if [ "$#" -lt 1 ] || [ "$#" -gt 2 ]; then
359         echo "Usage: killps [-SIGNAL] pattern"
360         return;
361     fi
362     if [ $# = 2 ]; then sig=$1 ; fi
363     for pid in $(my_ps| awk '!/awk/ && $0~pat { print $1 }' pat=${!#} ) ; do
364         pname=$(my_ps | awk '$1~var { print $5 }' var=$pid )
365         if ask "Kill process $pid <$pname> with signal $sig?"
366             then kill $sig $pid
367         fi
368     done
369 }
370 
371 function my_ip() # 获得IP地址
372 {
373     MY_IP=$(/sbin/ifconfig ppp0 | awk '/inet/ { print $2 } ' | sed -e s/addr://)
374     MY_ISP=$(/sbin/ifconfig ppp0 | awk '/P-t-P/ { print $3 } ' | sed -e s/P-t-P://)
375 }
376 
377 function ii()   # 获得当前主机相关的信息
378 {
379     echo -e "\nYou are logged on ${RED}$HOST"
380     echo -e "\nAdditionnal information:$NC " ; uname -a
381     echo -e "\n${RED}Users logged on:$NC " ; w -h
382     echo -e "\n${RED}Current date :$NC " ; date
383     echo -e "\n${RED}Machine stats :$NC " ; uptime
384     echo -e "\n${RED}Memory stats :$NC " ; free
385     my_ip 2>&- ;
386     echo -e "\n${RED}Local IP Address :$NC" ; echo ${MY_IP:-"Not connected"}
387     echo -e "\n${RED}ISP Address :$NC" ; echo ${MY_ISP:-"Not connected"}
388     echo
389 }
390 
391 # 杂项工具:
392 
393 function repeat()       # 重复n次的命令
394 {
395     local i max
396     max=$1; shift;
397     for ((i=1; i <= max ; i++)); do  # --> C风格的语法
398         eval "$@";
399     done
400 }
401 
402 function ask()
403 {
404     echo -n "$@" '[y/n] ' ; read ans
405     case "$ans" in
406         y*|Y*) return 0 ;;
407         *) return 1 ;;
408     esac
409 }
410 
411 #=========================================================================
412 #
413 # 按情况补全, complete命令 - BASH-2.04及其后续版本
414 # 大部分摘自bash 2.05文档
415 # 和Ian McDonalds的'Bash completion'软件包(http://www.caliban.org/bash/index.shtml#completion)
416 # 某些特征可能需要使用bash-2.05a
417 #
418 #=========================================================================
419 
420 if [ "${BASH_VERSION%.*}" \< "2.05" ]; then
421     echo "You will need to upgrade to version 2.05 for programmable completion"
422     return
423 fi
424 
425 shopt -s extglob        # 必须的
426 set +o nounset          # 否则某些自动补全将会失败
427 
428 complete -A hostname   rsh rcp telnet rlogin r ftp ping disk
429 complete -A export     printenv
430 complete -A variable   export local readonly unset
431 complete -A enabled    builtin
432 complete -A alias      alias unalias
433 complete -A function   function
434 complete -A user       su mail finger
435 
436 complete -A helptopic  help     # 通常与内建命令一样
437 complete -A shopt      shopt
438 complete -A stopped -P '%' bg
439 complete -A job -P '%'     fg jobs disown
440 
441 complete -A directory  mkdir rmdir
442 complete -A directory   -o default cd
443 
444 # 压缩
445 complete -f -o default -X '*.+(zip|ZIP)'  zip
446 complete -f -o default -X '!*.+(zip|ZIP)' unzip
447 complete -f -o default -X '*.+(z|Z)'      compress
448 complete -f -o default -X '!*.+(z|Z)'     uncompress
449 complete -f -o default -X '*.+(gz|GZ)'    gzip
450 complete -f -o default -X '!*.+(gz|GZ)'   gunzip
451 complete -f -o default -X '*.+(bz2|BZ2)'  bzip2
452 complete -f -o default -X '!*.+(bz2|BZ2)' bunzip2
453 # Postscript,pdf,dvi.....(译者: 打印格式相关)
454 complete -f -o default -X '!*.ps'  gs ghostview ps2pdf ps2ascii
455 complete -f -o default -X '!*.dvi' dvips dvipdf xdvi dviselect dvitype
456 complete -f -o default -X '!*.pdf' acroread pdf2ps
457 complete -f -o default -X '!*.+(pdf|ps)' gv
458 complete -f -o default -X '!*.texi*' makeinfo texi2dvi texi2html texi2pdf
459 complete -f -o default -X '!*.tex' tex latex slitex
460 complete -f -o default -X '!*.lyx' lyx
461 complete -f -o default -X '!*.+(htm*|HTM*)' lynx html2ps
462 # 多媒体
463 complete -f -o default -X '!*.+(jp*g|gif|xpm|png|bmp)' xv gimp
464 complete -f -o default -X '!*.+(mp3|MP3)' mpg123 mpg321
465 complete -f -o default -X '!*.+(ogg|OGG)' ogg123
466 
467 
468 
469 complete -f -o default -X '!*.pl'  perl perl5
470 
471 # 这是一个'通用的'补全函数 - 当命令具有一个所谓的"长选项"模式it works when commands have
472 # 的时候, 它就会工作, 比如: 'ls --all' 代替 'ls -a'
473 
474 _get_longopts () 
475 { 
476     $1 --help | sed  -e '/--/!d' -e 's/.*--\([^[:space:].,]*\).*/--\1/'| \
477 grep ^"$2" |sort -u ;
478 }
479 
480 _longopts_func ()
481 {
482     case "${2:-*}" in
483  -*) ;;
484  *) return ;;
485     esac
486 
487     case "$1" in
488  \~*) eval cmd="$1" ;;
489  *) cmd="$1" ;;
490     esac
491     COMPREPLY=( $(_get_longopts ${1} ${2} ) )
492 }
493 complete  -o default -F _longopts_func configure bash
494 complete  -o default -F _longopts_func wget id info a2ps ls recode
495 
496 
497 _make_targets ()
498 {
499     local mdef makef gcmd cur prev i
500 
501     COMPREPLY=()
502     cur=${COMP_WORDS[COMP_CWORD]}
503     prev=${COMP_WORDS[COMP_CWORD-1]}
504 
505     # 如果之前的参数为-f, 那就返回可能的补全文件名. 
506     # 我们可以让它更智能一些, 并且返回匹配的
507     # `makefile Makefile *.mk', 不管存在与否
508     case "$prev" in
509         -*f)    COMPREPLY=( $(compgen -f $cur ) ); return 0;;
510     esac
511 
512     # 如果我们需要一个选项, 那就返回可能的posix选项
513     case "$cur" in
514         -)      COMPREPLY=(-e -f -i -k -n -p -q -r -S -s -t); return 0;;
515     esac
516 
517     # 前尝试`makefile'再尝试`Makefile'
518     if [ -f makefile ]; then
519         mdef=makefile
520     elif [ -f Makefile ]; then
521         mdef=Makefile
522     else
523         mdef=*.mk               # 局部约定
524     fi
525 
526     # 在我们扫描目标文件之前, 察看makefile文件名是否
527     # 使用-f指定
528     for (( i=0; i < ${#COMP_WORDS[@]}; i++ )); do
529         if [[ ${COMP_WORDS[i]} == -*f ]]; then
530             eval makef=${COMP_WORDS[i+1]}       # eval for tilde expansion(波浪号扩展)
531             break
532         fi
533     done
534 
535         [ -z "$makef" ] && makef=$mdef
536 
537     # 如果我们有特别偏爱的补全单词, 
538     # 那么可以限制的补全这个单词
539     if [ -n "$2" ]; then gcmd='grep "^$2"' ; else gcmd=cat ; fi
540 
541     # 如果我们不想使用*.mk, 我们可以使用
542     # 或者使用test -f $makef或者使用输入重定向
543     COMPREPLY=( $(cat $makef 2>/dev/null | awk 'BEGIN {FS=":"} /^[^.#   ][^=]*:/ {print $1}' | tr -s ' ' '\012' | sort -u | eval $gcmd ) )
544 }
545 
546 complete -F _make_targets -X '+($*|*.[cho])' make gmake pmake
547 
548 
549 # cvs(1) 补全
550 _cvs ()
551 {
552     local cur prev
553     COMPREPLY=()
554     cur=${COMP_WORDS[COMP_CWORD]}
555     prev=${COMP_WORDS[COMP_CWORD-1]}
556 
557     if [ $COMP_CWORD -eq 1 ] || [ "${prev:0:1}" = "-" ]; then
558         COMPREPLY=( $( compgen -W 'add admin checkout commit diff \
559         export history import log rdiff release remove rtag status \
560         tag update' $cur ))
561     else
562         COMPREPLY=( $( compgen -f $cur ))
563     fi
564     return 0
565 }
566 complete -F _cvs cvs
567 
568 _killall ()
569 {
570     local cur prev
571     COMPREPLY=()
572     cur=${COMP_WORDS[COMP_CWORD]}
573 
574     # 获得进程列表(第一个sed表达式处理
575     # swap out出去的进程, 第二个
576     # 获得进程的basename)
577     COMPREPLY=( $( /usr/bin/ps -u $USER -o comm  | \
578         sed -e '1,1d' -e 's#[]\[]##g' -e 's#^.*/##'| \
579         awk '{if ($0 ~ /^'$cur'/) print $0}' ))
580 
581     return 0
582 }
583 
584 complete -F _killall killall killps
585 
586 
587 # 一个元命令补全函数, 用于sudo(8)这种命令, 
588 # 需要先对这个命令进行补全, 然后需要按照这个命令自己的补全定义进行补全
589 #  - 当前并不是非常可靠(比如 mount和umount命令
590 # 就不能很好的工作), 但还是很有用的 - 作者, Ian McDonald, 我修改了一下. 
591 
592 _my_command()
593 {
594     local cur func cline cspec
595     
596     COMPREPLY=()
597     cur=${COMP_WORDS[COMP_CWORD]}
598 
599     if [ $COMP_CWORD = 1 ]; then
600  COMPREPLY=( $( compgen -c $cur ) )
601     elif complete -p ${COMP_WORDS[1]} &>/dev/null; then
602  cspec=$( complete -p ${COMP_WORDS[1]} )
603  if [ "${cspec%%-F *}" != "${cspec}" ]; then
604      # complete -F <function>
605      #
606      # COMP_CWORD和COMP_WORDS()不是只读的,
607      # 所以我们可以在传递到补全例程之前, 
608      # 设置它们
609  
610      # 设置当前的标志号减1
611      COMP_CWORD=$(( $COMP_CWORD - 1 ))
612      # 获得函数名
613      func=${cspec#*-F }
614      func=${func%% *}
615      # 获得去掉第一个命令后的命令行
616      cline="${COMP_LINE#$1 }"
617      # 分离当前命令, 传递给数组
618   COMP_WORDS=( $cline )
619      $func $cline
620  elif [ "${cspec#*-[abcdefgjkvu]}" != "" ]; then
621      # complete -[abcdefgjkvu]
622      #func=$( echo $cspec | sed -e 's/^.*\(-[abcdefgjkvu]\).*$/\1/' )
623      func=$( echo $cspec | sed -e 's/^complete//' -e 's/[^ ]*$//' )
624      COMPREPLY=( $( eval compgen $func $cur ) )
625  elif [ "${cspec#*-A}" != "$cspec" ]; then
626      # complete -A <type>
627      func=${cspec#*-A }
628  func=${func%% *}
629  COMPREPLY=( $( compgen -A $func $cur ) )
630  fi
631     else
632  COMPREPLY=( $( compgen -f $cur ) )
633     fi
634 }
635 
636 
637 complete -o default -F _my_command nohup exec eval trace truss strace sotruss gdb
638 complete -o default -F _my_command command type which man nice
639 
640 # 本地变量:
641 # mode:shell-script
642 # sh-shell:bash
643 # End:
