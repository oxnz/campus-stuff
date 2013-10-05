/* Watch entry into kevent() */
syscall::kevent:entry
/execname == "dirwatcher" || execname == "DirectoryServic"/
{
	printf("%s called kevent()", execname);
}
