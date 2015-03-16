#include "table.h"

#if defined(GENERIC)
ostream & Table::Print(ostream &os) const
{
  // WRITE THIS
  os << "Table()";
  return os;
}
#endif

#if defined(LINKSTATE)

Table::Table()
{
	map<unsigned, double> rows;	
	this->costtable = rows;
	map<unsigned, unsigned> hops;
	nextHop = hops;
}


Table::Table(unsigned nodeNumber, unsigned tableSize){
        map<unsigned, double> rows;	
	number = nodeNumber;
	this->costtable = rows;
}

Table::~Table(){

}
Table::Table(const Table &rhs){
	number = rhs.number;
	costtable = rhs.costtable;
	nextHop = rhs.nextHop;
}
Table & Table::operator=(const Table &rhs){
	return *(new(this)Table(rhs));
}

void Table::modify( unsigned dst, double newVal){
	this->costtable[dst] = newVal;
}

double Table::find( unsigned dst){
	return this->costtable[dst];
}

ostream & Table::Print(ostream &os)  const
{
 
  os << "printing Link State: Table()" << endl;
  os << "table size is "<< costtable.size()<<endl;
  for(unsigned i = 0; i < costtable.size();i++){
	  
	  os<< "dst:"<< i <<" nextHop:" <<nextHop.at(i) << " ";
	  os << "distance: "<<costtable.at(i);
	  os << endl;
  }

  return os;
}


#endif

#if defined(DISTANCEVECTOR)
Table::Table()
{
	
	vector<vector <double> > rows(1,vector<double>(1,0));
	this->table = rows;
	//cout << "construcing table without param\n";
}


Table::Table(unsigned nodeNumber, unsigned tableSize){
	//cout << "constructing table\n";
	vector<double> col (tableSize, INF);
	vector<vector <double> > rows(tableSize, col);
	number = nodeNumber;
	this->table = rows;
}

Table::~Table(){

}
Table::Table(const Table &rhs){
	number = rhs.number;
	table = rhs.table;

}
Table & Table::operator=(const Table &rhs){
	return *(new(this)Table(rhs));
}

void Table::modify(unsigned src, unsigned dst, double newVal){
	this->table.at(src).at(dst) = newVal;
}

double Table::find(unsigned src, unsigned dst){
	return this->table.at(src).at(dst);
}

ostream & Table::Print(ostream &os)  const
{
  // WRITE THIS
  os << "printing Distance vector: Table()" << endl;
  os << "table size is "<< table.size()<<endl;
  for(int i = 0; i < table.size();i++){
	  for(int j = 0; j < table[i].size();j++)
		  os << table[i][j] << " ";
	  os << endl;
  }

  return os;
}


#endif
