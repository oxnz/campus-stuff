dist_bin_SCRIPTS = rdreport.sh rdextract.sh rdgentest.awk
CLEANFILES = $(dist_bin_SCRIPTS)
EXTRA_DIST = rdreport.in rdextract.in rdgentest.in

do_subst = sed -e 's,[@]VERSION[@],$(VERSION),g' \
		   -e 's,[@]PACKAGE[@],$(PACKAGE),g'

rdreport.sh: rdreport.in Makefile
	$(do_subst) < $(srcdir)/rdreport.in > rdreport.sh
	chmod +x rdreport.sh

rdextract.sh: rdextract.in Makefile
	$(do_subst) < $(srcdir)/rdextract.in > rdextract.sh
	chmod +x rdextract.sh

rdgentest.awk: rdgentest.in
	$(do_subst) < $(srcdir)/rdgentest.in > $@
	chmod +x $@
