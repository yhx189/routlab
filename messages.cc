#include "messages.h"


#if defined(GENERIC)
ostream &RoutingMessage::Print(ostream &os) const
{
  os << "RoutingMessage()";
  return os;
}
#endif


#if defined(LINKSTATE)

ostream &RoutingMessage::Print(ostream &os) const
{
  os << endl;
  for(int i = 0; i < table.costtable.size(); i++)
	  os << "dst:"<<i <<" nextHop:"<<table.nextHop.at(i) << " distance:"<< table.costtable.at(i) <<endl;
  os<<endl;
  return os;
}

RoutingMessage::RoutingMessage()
{}


RoutingMessage::RoutingMessage(const RoutingMessage &rhs)
{
  this->table = rhs.table;
  this->number = rhs.number;
}
RoutingMessage::RoutingMessage(Table table, unsigned number){
  this->table = table;
  this->number = number;
}
#endif


#if defined(DISTANCEVECTOR)

ostream &RoutingMessage::Print(ostream &os) const
{
  os << "routing message\n";
  for(int i = 0; i < DV.size(); i++)
	  os<<DV[i] << " ";
  os << endl;
  return os;
}

RoutingMessage::RoutingMessage()
{}

RoutingMessage::RoutingMessage(vector<double> DV, unsigned number)
{
	this->DV = DV;
	this->number = number;
}


RoutingMessage::RoutingMessage(const RoutingMessage &rhs)
{
	this->DV = rhs.DV;
	this->number = rhs.number;
}

#endif

