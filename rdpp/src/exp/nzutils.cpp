
//#define NDEBUG
#ifdef NDEBUG
#define assert(cond) ((void)0)
#else

inline void assertImpl(const char* file_, const char* func, int line) {
	cout << "Assert failed: (File: " << file_ << ", Function: " <<
		func << " , Line: " << line << ")" << endl;
	exit(1);
}

#define assert(cond) ((cond) ? (void)0 : \
		assertImpl(__FILE__, __FUNCTION__, __LINE__))
#endif
