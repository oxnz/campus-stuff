dist_bin_SCRIPTS = report.sh extract.sh
CLEANFILES = $(dist_bin_SCRIPTS)
EXTRA_DIST = report.in extract.in

do_subst = sed -e 's,[@]VERSION[@],$(VERSION),g' \
		   -e 's,[@]PACKAGE[@],$(PACKAGE),g'

report.sh: report.in Makefile
	$(do_subst) < $(srcdir)/report.in > report.sh
	chmod +x report.sh

extract.sh: extract.in Makefile
	$(do_subst) < $(srcdir)/extract.in > extract.sh
	chmod +x extract.sh
