#ifndef _table
#define _table


#include <iostream>
#define INF 100000

using namespace std;

#if defined(GENERIC)
class Table {
  // Students should write this class

 public:
  ostream & Print(ostream &os) const;
};
#endif


#if defined(LINKSTATE)
#include <map>
#include <vector>
class Table {
 public:
 	Table(unsigned nodeNumber, unsigned tableSize);
	Table();
	~Table();
	Table(const Table &rhs);
	Table & operator=(const Table &rhs);
	unsigned number;
        map<unsigned, double> costtable;
	map<unsigned, unsigned> nextHop;
	unsigned tableSize;
	void modify( unsigned dst, double newVal);
	double find( unsigned dst);
	ostream & Print(ostream &os) const;
};
#endif

#if defined(DISTANCEVECTOR)

#include <deque>
#include <vector>
class Table {
 public:
 	Table(unsigned nodeNumber, unsigned tableSize);
	Table();
	~Table();
	Table(const Table &rhs);
	Table & operator=(const Table &rhs);
	unsigned number;
        vector<vector<double> > table;
	unsigned tableSize;
	void modify(unsigned src, unsigned dst, double newVal);
	double find(unsigned src, unsigned dst);
	ostream & Print(ostream &os) const;
};
#endif

inline ostream & operator<<(ostream &os, const Table &t) { return t.Print(os);}

#endif
