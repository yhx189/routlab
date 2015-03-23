#include "node.h"
#include "context.h"
#include "error.h"

//Node::Node() 
// { throw GeneralException(); }

Node::Node(const Node &rhs) : 
  number(rhs.number), context(rhs.context), bw(rhs.bw), lat(rhs.lat) {}

Node & Node::operator=(const Node &rhs) 
{
  return *(new(this)Node(rhs));
}

void Node::SetNumber(const unsigned n) 
{ number=n;}

unsigned Node::GetNumber() const 
{ return number;}

void Node::SetLatency(const double l)
{ lat=l;}

double Node::GetLatency() const 
{ return lat;}

void Node::SetBW(const double b)
{ bw=b;}

double Node::GetBW() const 
{ return bw;}

Node::~Node()
{}

// Implement these functions  to post an event to the event queue in the event simulator
// so that the corresponding node can recieve the ROUTING_MESSAGE_ARRIVAL event at the proper time
void Node::SendToNeighbors(const RoutingMessage *m)
{
/*	//cout << "seding to neighbors\n";
	deque<Node*> *neighbors = GetNodeNeighbors(number);
	//for(deque<Node*>::iterator it = neighbors->begin(); it!=neighbors->end();it++)
	//	SendToNeighbor(*it,  m);
//	cout << number;
//	cout << "there are " << neighbors->size() << " neighbors\n";
	for(int i = 0; i < neighbors->size(); i++){
//		cout <<  neighbors->at(i)->number<<endl;
		Node *tmp = new Node(neighbors->at(i)->number, NULL, 0, 0);
		RoutingMessage* mt = new RoutingMessage(*m);
		cout <<*mt;
		SendToNeighbor(tmp,  mt);
	}
	cout << "sending to neighbors finished\n";
*/
	context->SendToNeighbors(this, m);
}



void Node::SendToNeighbor(const Node *n, const RoutingMessage *m)
{
	
/*	cout << "sending to neighbor:" << *n << endl;
	Link tmp(number, n->number, NULL, 0, 0);
	//deque<Link*>* links = GetOutgoingLinks();
	Link *ret= context->FindMatchingLink(&tmp);
	double lat = ret->GetLatency();
	double t = context->GetTime() + lat;
	Event *ee = new Event(t, ROUTING_MESSAGE_ARRIVAL, (void*)n , (void*) m);
	context->PostEvent(ee);
	//cout << *m;
	//cout << *ee;
	//n->ProcessIncomingRoutingMessage(m);
*/
	context->SendToNeighbor(this, n, m);
}


deque<Node*> *Node::GetNodeNeighbors(unsigned num)
{
  	Node tmp = Node(num, context, bw, lat);
//	cout << "getting neighbors at node " << number << " , context =" 
//		<< *context << "...\n";
	return context->GetNeighbors(&tmp);
}

void Node::SetTimeOut(const double timefromnow)
{
  context->TimeOut(this,timefromnow);
}


bool Node::Matches(const Node &rhs) const
{
  return number==rhs.number;
}


#if defined(GENERIC)
Node::Node(const unsigned n, SimulationContext *c, double b, double l) : 
    number(n), context(c), bw(b), lat(l) 
{}

void Node::LinkHasBeenUpdated(const Link *l)
{
  cerr << *this << " got a link update: "<<*l<<endl;
  //Do Something generic:
  SendToNeighbors(new RoutingMessage);
}


void Node::ProcessIncomingRoutingMessage(const RoutingMessage *m)
{
  cerr << *this << " got a routing messagee: "<<*m<<" Ignored "<<endl;
}


void Node::TimeOut()
{
  cerr << *this << " got a timeout: ignored"<<endl;
}

Node *Node::GetNextHop(const Node *destination) const
{
  return 0;
}

Table *Node::GetRoutingTable() const
{
  return new Table;
}


ostream & Node::Print(ostream &os) const
{
  os << "Node(number="<<number<<", lat="<<lat<<", bw="<<bw<<")";
  return os;
}

#endif

#if defined(LINKSTATE)
Node::Node(const unsigned n, SimulationContext *c, double b, double l) : 
    number(n), context(c), bw(b), lat(l) 
{
  
}


unsigned Node::findNum( vector<unsigned> np, unsigned ele){
	for(int i = 0; i < np.size();i++){
		if(np[i] == ele)
			return i;
	
	}
	return INF;

}
void Node::LinkHasBeenUpdated(const Link *l)
{
  cout << "link update\n";
  cout << nodeTable;
  if(nodeTable.costtable.empty()){
		//  || (nodeTable.table.size() <= l->GetSrc())
		  //||( nodeTable.table.size() <= l->GetDest())){
  	//cout << "empty node table\n";
	unsigned max = (l->GetSrc() > l->GetDest())?l->GetSrc():l->GetDest();
	for(unsigned i = nodeTable.costtable.size(); i <= max; i++){
		nodeTable.costtable[i] = INF;
		nodeTable.nextHop[i] = i;
		allNodes.push_back(i);
	}
	nodeTable.tableSize = max;
	nodeTable.costtable[number] = 0;
	np.push_back(number);	

  }else if( (nodeTable.costtable.size() <= l->GetSrc())
		  ||( nodeTable.costtable.size() <= l->GetDest())){
  	unsigned max = (l->GetSrc() > l->GetDest())?l->GetSrc():l->GetDest();
	for(unsigned i = nodeTable.costtable.size(); i <= max; i++){
		nodeTable.costtable[i] = INF;
		nodeTable.nextHop[i] = i;
		allNodes.push_back(i);
	}
	nodeTable.tableSize = max;
	nodeTable.costtable[number] = 0;
  }



  double prev = nodeTable.costtable[l->GetDest()];
  nodeTable.costtable[l->GetDest()] = l->GetLatency();
  deque<Node*>*neighbors = context->GetNeighbors(this);

  
  
  nodeTable.nextHop[l->GetDest()] = l->GetDest();

  
  // send message
  RoutingMessage * rout = new RoutingMessage(nodeTable, number);
  for(int i = 0; i < allNodes.size();i++){
	
	// double t = context->GetTime() + nodeTable.table[i];
	//Node* tmp = new Node(allNodes[i], context, 0, 0);
	//cout << "sending messages\n";	
	//Event *ee = new Event(t, ROUTING_MESSAGE_ARRIVAL, (void*)tmp , (void*) rout);
	if(i != number){
		//SendToNeighbors(rout);
		Node* dest = new Node(allNodes[i], context, 0, 0);
		if( nodeTable.costtable[i] != INF)
		context->PostEvent(new Event(context->GetTime()+nodeTable.costtable[i],
				ROUTING_MESSAGE_ARRIVAL,
				context->FindMatchingNode(dest),
				(void*) rout));
	}
	 
  }

  // update next hop
  // update table
    cout <<"after link update:" << nodeTable;
    cerr << *this<<": Link Update: "<<*l<<endl;
}


void Node::ProcessIncomingRoutingMessage(const RoutingMessage *m)
{
 
  cout << "processing incoming message at " <<number <<" from "<<m->number <<"\n" << *m;
  cout << nodeTable;

  if(m->table.costtable.size() > nodeTable.costtable.size()){
     for(int i = nodeTable.costtable.size(); i < m->table.costtable.size(); i++){
        nodeTable.costtable[i] = INF;
	nodeTable.nextHop[i] = m->number;
	allNodes.push_back((unsigned)i);
     }
  }
 Node* src = new Node(m->number, context, 0, 0);
 Node* ori = context->FindMatchingNode(src);
 
 ori->np.push_back(number);


 for(int i = 0; i < m->table.costtable.size(); i++){
 if(nodeTable.costtable[i] > (nodeTable.costtable[m->number] + m->table.costtable.at(i))) {
	nodeTable.nextHop[i] = m->number;
  	nodeTable.costtable[i] = nodeTable.costtable[m->number] + m->table.costtable.at(i);
 }
 if(ori->nodeTable.costtable[i] > ori->nodeTable.costtable[number] + nodeTable.costtable[i]){
	 ori->nodeTable.nextHop[i] = number;
	 ori->nodeTable.costtable[i] = ori->nodeTable.costtable[number] + nodeTable.costtable[i];
 }
 
 }
  //update node table

  // update next hop 
  /*
  for(int i = 0; i < nodeTable.nextHop.size();i++){
  	if(nodeTable.costtable[i] > (m->table.at(i) + nodeTable.table[m->number])){
		nodeTable.costtable[i] = (m->table.at(i) + nodeTable.table[m->number]) ;
		nodeTable.nextHop.at(i) = m->number;
		//cout << "routing table updated!\n ";
	
	}
  }
  */
  cout << "after processing incoming message "<<nodeTable;
  //cerr << *this << " Routing Message: "<<*m;
}

void Node::TimeOut()
{
  cerr << *this << " got a timeout: ignored"<<endl;
}

Node *Node::GetNextHop(const Node *destination) const
{
  cout << "getting next hop src: "<<number <<" dst: " << destination->number;
  Table *tmp = GetRoutingTable();
  if(destination->number >= tmp->costtable.size()){
 	  cout << "this should not happen\n";
	  return NULL; 
  }else{
 	 Node* ret = new Node(tmp->nextHop.at(destination->number), NULL, 0, 0);
	 cout << " nextHop: "<<tmp->nextHop.at(destination->number);
  	 cout << endl;
	 return ret;
  }
}

Table *Node::GetRoutingTable() const
{
  // WRITE
	//cout <<"get routing table:\n";
	Table* ret = new Table(nodeTable);
	//cout << *ret;
	return ret;
}


ostream & Node::Print(ostream &os) const
{
  os << "Node(number="<<number<<", lat="<<lat<<", bw="<<bw<<")";
  return os;
}
#endif


#if defined(DISTANCEVECTOR)
Node::Node(const unsigned n, SimulationContext *c, double b, double l) : 
    number(n), context(c), bw(b), lat(l) {
	//cout << "constructing node\n";

}

void Node::modifyTable(unsigned newVal){
  
  newVal+= 1;
  for(int i = 0; i < nodeTable.table.size();i++){
   	nodeTable.table[i].resize(newVal, INF);
   
   }
   nodeTable.table.resize(newVal, vector<double>(newVal, INF));
   for(int i = 0; i< (int)newVal ; i++)
		nodeTable.table[i][i] = 0;
   nodeTable.tableSize = newVal;
}
void Node::createTable(unsigned s){
	//cout << "creating new table\n";
	unsigned newSize = s + 1;
	for(int i = 0; i < nodeTable.table.size();i++){
   		nodeTable.table[i].resize(newSize, INF);
  	 }
	nodeTable.table.resize(newSize);
	for(int i=0; i< (int)newSize ; i++)
		nodeTable.table[i] = vector<double>(newSize, INF);
	for(int i=0; i< (int)newSize ; i++)
		nodeTable.table[i][i] = 0;
	nodeTable.tableSize = newSize;
	//cout << nodeTable;


}
void Node::LinkHasBeenUpdated(const Link *l)
{
  
  cout << "updating link...\n";
  cout << *l << endl; 

  if(nodeTable.tableSize == 0)
	  createTable(l->GetDest());
  if(l->GetSrc() >= nodeTable.tableSize){
  	modifyTable(l->GetSrc());
  }

  if(l->GetDest() >= nodeTable.tableSize){
  	
	modifyTable( l->GetDest());
  }
   
  nodeTable.table[l->GetSrc()][ l->GetDest()] = l->GetLatency();


   
   // send message to neighbors
   vector<double> DV = this->nodeTable.table[number];
   RoutingMessage *message = new RoutingMessage(DV, this->number);

   SendToNeighbors(message);
   cout << "after updating link,\n" << nodeTable;

}


void Node::ProcessIncomingRoutingMessage(const RoutingMessage *m)
{
	
	cout << "received incoming message at node " <<number<<" from " << m->number <<": "<< *m<<endl;
        //cout << "time is " << context->GetTime()<<endl;
	nodeTable = *(GetRoutingTable());
	//if(nodeTable.tableSize == 0)
	// 	 createTable((m->number>number)?m->number:number);
	
	vector <double> DV = m->DV;
	if(m->DV.size() < nodeTable.table.size())
		DV.resize(nodeTable.table.size(), INF);
	if(m->DV.size() > nodeTable.table.size())
		modifyTable(m->DV.size() - 1);
	unsigned curNum = m->number;
	
	nodeTable.table[curNum] = DV;
	bool update = false;
	
        deque<Node*>* neighbors = context->GetNeighbors(this);
	for(deque<Node*>::iterator it = neighbors->begin();it!=neighbors->end();it++){
		int j = (*it)->number;
	for(int i = 0; i < nodeTable.tableSize;i++){
		if((i!=j) &&( nodeTable.table[j][i] > (DV[i] + nodeTable.table[j][curNum]))){
			
			nodeTable.table[j][i] = (DV[i] + nodeTable.table[j][curNum]);
			update = true;
			vector<double> newDV = this->nodeTable.table[j];
   			RoutingMessage *message = new RoutingMessage(newDV, this->number);

  		        SendToNeighbors(message);
		}
	
	}
	}
	/*if(update){
	vector<double> newDV = this->nodeTable.table[number];
   	RoutingMessage *message = new RoutingMessage(newDV, this->number);

  	 SendToNeighbors(message);
	}
	*/
	//cout << "after processing message,\n" << nodeTable;
}

void Node::TimeOut()
{
  cerr << *this << " got a timeout: ignored"<<endl;
}


Node *Node::GetNextHop(const Node *destination) const
{
	
	unsigned srcNum = number;
	unsigned dstNum = destination->number;
	double min = INF;
	unsigned nextNum = 0;
	deque<Node*>* neighbors = context->GetNeighbors(this);

	nextNum = (*neighbors->begin())->number;
	for(deque<Node*>::iterator it = neighbors->begin(); it!=neighbors->end();it++){
		int i = (*it)->number;
		//cout << "i = "<<i<<endl;
		
		if( min > (nodeTable.table[srcNum][i] 
					+ nodeTable.table[i][dstNum])){
			min = nodeTable.table[i][dstNum] + nodeTable.table[srcNum][i];
			nextNum = i;
		}
	}
	Node*ret = new Node(nextNum,0,0,0);
	 
	cout << nodeTable;
	cout << "src: "<<number <<" dest: "<<destination->number<< " nextHop: " <<nextNum<<endl;
	 return ret;
}



Table *Node::GetRoutingTable() const
{
	/*cout <<"getting routing table\n";
        cout << context;	
	Node* tmp = new Node(number, context, 0, 0);
	Node* tmp2 = new Node(*tmp);
	cout << tmp <<" there\n";
	 context->FindMatchingNode(this);
	cout << "there\n";
	Table * ret = new Table(tmp2->nodeTable);
	
	cout << *ret;
	*/
	return new Table(nodeTable);


}


ostream & Node::Print(ostream &os) const
{
  os << "Node(number="<<number<<", lat="<<lat<<", bw="<<bw << ")" << endl;
  return os;
}
#endif
