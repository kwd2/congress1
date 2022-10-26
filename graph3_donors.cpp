#include <iomanip>
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cmath>
#include <ctype.h>

#include <list>
#include <vector>
#include <map>
using namespace std;



class Node{
public:
  string              symbol;                  

  long long int            dollars;                              
  list<long long int>      dollars_list;                              

  int                 number_donations;
  
  list<Node>          branches;
  map<string,Node*>   branches_map;


  void   input        (list<string>::iterator sym, long int amount);
  void   input        (list<string>::iterator sym, long int amount, long long int number_donations);

  void   output       ( int  depth,  int tabs );
  void   output1      ( int  depth,  int tabs, list<string> head_nodes );


  void   roll_dollars     (int depth);
  void   roll_dollars_sort(int depth);

  
  Node(){dollars=0;number_donations=0;};
  Node(string name, long long int value ){symbol=name;dollars= value;number_donations=0;/*cout<<name<<" "<<value<<endl;*/}

}; 




//----------------------------------------
//
void
Node::roll_dollars( int depth){


  dollars_list.push_front(dollars);

  dollars=0;

  if (depth > 0 ){

    list<Node>::iterator b=branches.begin();

    for (;b!=branches.end();b++){

      //cout<<"r- "<<b->symbol<<endl;

      
      b->roll_dollars(depth-1);
    }
  }

}

//----------------------------------------
//
void
Node::roll_dollars_sort( int depth){


  dollars_list.push_front(dollars);

  dollars=0;

  if (depth > 0 ){

    
    multimap<long long int,Node*>                    br_sort;


    list<Node>::iterator s = branches.begin();

    for (;s!=branches.end();s++){

      long long int d1=0;

      d1 = atol(s->symbol.c_str());

      if (d1>-1)
      br_sort.insert(pair<long long int,Node*>(d1, &(*s)));
    }

    
    multimap<long long int,Node*>::iterator  b = br_sort.begin();

    long long int total = 0;
    
    for ( ; b!= br_sort.end(); b++){

      if (b->second->dollars >0) {

	total += b->second->dollars;

	b->second->dollars = total;

      }
    }

    
      list<Node>::iterator b1=branches.begin();

      for (;b1!=branches.end();b1++){

	//cout<<"r- "<<b1->symbol<<endl;

      
	b1->roll_dollars_sort(depth-1);
      }

  }
}



//----------------------------------------
//
void
Node::input( list<string>::iterator symbol, long int amount, long long int number ){

  //cout<< *symbol<<endl;

    dollars += amount;

    number_donations += number;
    
  if (*symbol=="-stop") return;

  
  map< string, Node*>::iterator sn = branches_map.find(*symbol);

   if (sn == branches_map.end()){
    
    Node temp(*symbol, 0 );

    branches.push_back(temp);

    branches_map.insert( pair<string,Node*>(*symbol,&(branches.back() )));
    
    sn = branches_map.find(*symbol);
  }


   //sn->second->dollars += amount;

   sn->second->input( ++symbol, amount, number );
    
}

//----------------------------------------
//
void
Node::input( list<string>::iterator symbol, long int amount){

  //cout<< *symbol<<endl;

    dollars += amount;

    number_donations++;
    
  if (*symbol=="-stop") return;

  
  map< string, Node*>::iterator sn = branches_map.find(*symbol);

   if (sn == branches_map.end()){
    
    Node temp(*symbol, 0 );

    branches.push_back(temp);

    branches_map.insert( pair<string,Node*>(*symbol,&(branches.back() )));
    
    sn = branches_map.find(*symbol);
  }


   //sn->second->dollars += amount;

  sn->second->input( ++symbol, amount );
    
}

//----------------------------------------
//
void
Node::output1( int depth, int tabs, list<string> head_nodes ){


  //cout<<symbol;
  cout<<"\t"<<symbol<<"\t";

  if (depth == 1 ) {
  
    int size = symbol.size();  if (size>60) size=60;

    for (int i=0;i!=60-size;i++) cout<<" ";
    cout<<"\t";

    //cout<<number_donations<<"\t";
  
    for  (int i=0;i!=12;i++) if (dollars<pow(10,i)) cout<<" ";

    cout << dollars ;
    //if (depth==1) cout << dollars ;

    if (number_donations==0) number_donations=-1;
    
    //cout<<"\t"<<dollars/number_donations<<"\t"<<number_donations;   //<<"\t"<<branches.size();
  
    list<long long int>::iterator d = dollars_list.begin();

    for ( ; d!=dollars_list.end(); d++){

      //cout<<"\t"<<*d;
      if (depth==1) cout<<"\t"<<*d;
    }

  }  // depth==1
  
  cout << endl;
  
  if (depth-1>0){

      head_nodes.push_back(symbol);
    
    multimap<long long int,Node*>                    br_sort;
    multimap<long long int,Node*>::reverse_iterator  br;

    list<Node>::iterator s = branches.begin();

    int s1=0;
    for (;s!=branches.end();s++,s1++){

      long long int d1=0;
      long long int y1=0;

      /*
      if (s->dollars_list.size()>0){


	list<long int>::iterator dl = s->dollars_list.begin();
	for (; dl != s->dollars_list.end();dl++) if (*dl>0) y1++;

	d1=   s->dollars_list.size()*100000  + (s->dollars_list.back())  +y1;



      }else{
      */
 

      //d1 = s->dollars;
      //d1 = s->dollars/ s->number_donations;

      d1 = atol(s->symbol.c_str());

      
      //y1=1;
      //d1 += pow(1000,10);	
      //list<long int>::iterator dl = s->dollars_list.begin();
      //for (; dl != s->dollars_list.end();dl++,y1++) if (d1==0) d1 = -*dl;
      //br_sort.insert(pair<long int,Node*>(s1, &(*s)));
      //br_sort.insert(pair<long int,Node*>(d1+y1, &(*s)));

      if (d1>-1)
      br_sort.insert(pair<long long int,Node*>(d1, &(*s)));


    }
    
    //for (;s!=branches.end();s++) br_sort.insert(pair<long int,Node*>(s->dollars, &(*s)));


    multimap<long long int,Node*>::iterator  b = br_sort.begin();

    long long int total = 0;
    
    for ( ; b!= br_sort.end(); b++){

      if (b->second->dollars >0) b->second->dollars = total + b->second->dollars;

    }

    
    for (br=br_sort.rbegin();br!=br_sort.rend();++br){

      cout<<tabs<<"\t";
      list<string>::iterator h = head_nodes.begin();
      
      for (;h!=head_nodes.end();h++) cout<<*h<<"\t";
      
      //for (int i=0; i!= tabs; i++) cout<<"\t";
      
      br->second->output1(depth-1, tabs+1, head_nodes);
      
    }
    
  }
  
}




//----------------------------------------
//
void
Node::output( int depth, int tabs ){

  cout<<symbol;

  int size = symbol.size();  if (size>60) size=60;

  for (int i=0;i!=60-size;i++) cout<<" ";
  

  for  (int i=0;i!=12;i++) if (dollars<pow(10,i)) cout<<" ";
  cout << dollars ;

  list<long long int>::iterator d = dollars_list.begin();

  for ( ; d!=dollars_list.end(); d++){

    cout<<"\t"<<*d;
  }

  cout << endl;
  
  if (depth-1>0){

    multimap<long int,Node*>                    br_sort;
    multimap<long int,Node*>::reverse_iterator  br;

    list<Node>::iterator s = branches.begin();

    for (;s!=branches.end();s++) br_sort.insert(pair<long int,Node*>(s->dollars, &(*s)));
    
    for (br=br_sort.rbegin();br!=br_sort.rend();++br){

      cout<<symbol;
      
      for (int i=0; i!= tabs+1; i++) cout<<"\t";
  
      br->second->output(depth-1, tabs+1);

    }

  }

}


//----------------------------------------
//
int main1()
{

  string this_year  = "2007";
  string this_month =   "05";
  string this_day   =   "01";

  string year;
  string month;
  string day;

  
  //Node state ("state",0);

  
 

  string         line;
  string         field;

  int            s1=0;
  int            s2=0;


  // ----- parse committee_master file

  multimap<string,string> cm_map;
  
  ifstream datafile2;  datafile2.open("data/cm.txt");  if(!datafile2.is_open()){cout<<"Error 1"<<endl;return 0;}


  
  while (getline(datafile2, line)){
    if(datafile2.eof()) break;

    vector<string> fields;

    s1 =0;
    s2 =0;
  
    while (s2 != string::npos ){

      s2=line.find("|", s1);field=line.substr(s1,s2-s1); s1+=s2-s1+1;

      fields.push_back(field);
      //cout<<field<<endl;
    }

    //cout<<fields.size()<<endl;
    if (fields.size()>2){
      cm_map.insert(pair<string,string>(fields[0],fields[1]));
    }
    
  }

  cout<<"number of committees "<<cm_map.size()<<endl;
    
  // ----- parse committee_master file

  

  // ----- parse candidate_master file

  multimap<string,string> cn_map;
  
  ifstream datafile2a;  datafile2a.open("data/cn.txt");  if(!datafile2a.is_open()){cout<<"Error 2a"<<endl;return 0;}
  
  while (getline(datafile2a, line)){
    if(datafile2a.eof()) break;

    vector<string> fields;

    s1 =0;
    s2 =0;
  
    while (s2 != string::npos ){

      s2=line.find("|", s1);field=line.substr(s1,s2-s1); s1+=s2-s1+1;

      fields.push_back(field);
      //cout<<field<<endl;
    }

    //cout<<fields.size()<<endl;
    if (fields.size()>2){
      cn_map.insert(pair<string,string>(fields[0],fields[1]));
    }
    
  }

  cout<<"number of candidates "<<cn_map.size()<<endl;
    
  // ----- parse candidate_master file


  
  // ----- parse header file

  map<string,Node*>  columns;
  vector<string>     columns1;

  int      count=0;
  int year_count=0;
  
  ifstream datafile1;  datafile1.open("data/header_donor.txt");  if(!datafile1.is_open()){cout<<"Error 1"<<endl;return 0;}


  getline(datafile1, line);  //  1st line

  //cout<<line<<endl;      

  s2 = 0;
  
  while (s2 != string::npos ){

    s2=line.find(",", s1);field=line.substr(s1,s2-s1); s1+=s2-s1+1;

    cout<<count++<<"\t"<<field<<endl;
    
    Node* node = new Node(field,0);

    columns.insert(pair<string,Node*>(field, node));

    columns1.push_back(field);
  }

  // ----- parse header file
    
 

  cout<<"num columns= "<<columns1.size()<<endl;    

  Node column_graph("column",0);
  
  int num_lines=0;

  int dollars1 = 0;
  
  long int total_dollars=0;

  // ------------------------------------ parse main data file


  
  while (getline(cin, line)){


    //cout<<line<<endl;

    if (line =="-roll" ){

      column_graph.roll_dollars(6);
      cout<<"rolling"<<endl;
      continue;
    }

    
    num_lines++;    

    dollars1 = -100000000;    
    
    vector<string> fields;
    
    s1=0;s2=0;    

    int num_commas = 0;

 
 
    while (s2 != string::npos ){
 
	num_commas++;
      
	s2=line.find("|", s1);field=line.substr(s1,s2-s1); s1+=s2-s1+1;    

	fields.push_back(field);	

	//cout<<field<<endl;

    
      if (num_commas==15){                                 // ------------  the $$$

	dollars1   =           atoi(field.c_str());

	total_dollars += dollars1;

	//cout<<field<<"\t"<<dollars1<<endl;
      }


      if (num_commas==14){                                 // ---------------  the day
	//cout<<field<<endl;

	if (field.size()==8){
	  year  = field.substr(4,4);
	  month = field.substr(0,2);
	  day   = field.substr(2,2);
	
	  //cout<<year<<" "<<month<<" "<<day<<endl;

	  //field = month+"/"+day+"/"+year;
	  field = month+"/"+year;

	  //cout<<"*"<<field<<"*"<<endl;
	  
	  fields.back() = field;
	}
	
      }

      

    }  // while

    //cout<<"num fields"<<fields.size()<<endl;
   
    if (num_commas != 21 ) {       //error check
	  
      cout<<num_commas<<endl;
      cout<<line<<endl;
      vector<string>::iterator i;
      for ( i=fields.begin();i!=fields.end();i++){
	cout<<*i<<endl;
      }    




      
    }


    // ---------------------   committee / candidate id

    multimap<string,string>::iterator f1= cm_map.find(fields[0]);
    
    if (f1 != cm_map.end() ) fields[0]=f1->second;


        f1  = cm_map.find(   fields[15]);    
    if (f1 != cm_map.end() ) fields[15]=f1->second;

        f1  = cn_map.find(   fields[15]);
    if (f1 != cn_map.end() ) fields[15]=f1->second;

    
    // ---------------------   committee id



    // -----------------------------------  make data graph   
 
    list<string>             symbols;
    list<string>::iterator   s;

    vector <string>       ::iterator   f  = fields.begin();
    map    <string,Node*> ::iterator   c  = columns.begin();
    vector <string>       ::iterator   c1 = columns1.begin();

    //cout<<endl;

    int ccnt=0;
    for (;c1!=columns1.end();c1++,f++,ccnt++){


      //cou<<*f<<"\t"<<*c1<<endl;
      
      symbols.clear();

      symbols.push_back( *c1  );
      symbols.push_back( *f  );

      //if (ccnt==0)  symbols.push_back("NAME"); 
      //if (ccnt==0)  symbols.push_back(fields[7]);  

      if (ccnt==5)  symbols.push_back("TRANSACTION_DT"); 
      if (ccnt==5)  symbols.push_back(fields[13]);  

      //if (ccnt==5) cout<<"**"<<fields[13]<<"*"<<endl;
      
      //if (ccnt==0)  symbols.push_back("TRANSACTION_AMT"); 
      //if (ccnt==0)  symbols.push_back(fields[14]);  



      //if (ccnt==13)  symbols.push_back("CMTE_ID");  
      //if (ccnt==13)  symbols.push_back(fields[0]);  

      
      //if (ccnt==15) symbols.push_back(fields[0]);  // other - committee id

      
      symbols.push_back("-stop");
      s = symbols.begin();

      if (ccnt!=4  && ccnt!=10 && ccnt<16){
      //if (ccnt==0){
      
	//column_graph.input( s, 1 );

	//if (ccnt==0 && fields[0]=="ACTBLUE")
	  //if (ccnt==0 && fields[0]=="WINRED")
	//if (ccnt==0 || ccnt==15)
	if (ccnt==5)
	  column_graph.input( s, dollars1 );
       }

    }
    // -----------------------------------  make data graph   

	
  }
  // ------------------------------------ parse main data file


  
  cout<<endl<<"total donated1\t\t"<< total_dollars<<endl;

  cout<<"average donation\t"<<total_dollars/num_lines<<endl;

  cout<<"number donations\t"<<num_lines<<endl<<endl;



    


  


  // -----------------------------------  print out data graph   
  
  list<string> s_e; 

  int ii=0;

  list<Node>::iterator b =column_graph.branches.begin();
  for (               ;b!=column_graph.branches.end();b++){

    cout<<"--- "<<b->symbol;
    if (b->symbol.size()<12) cout<<"\t";

    cout<<"\t #unique=\t"<<b->branches.size()<<endl; 
    
  }

  

    
  b =column_graph.branches.begin();
  for (               ;b!=column_graph.branches.end();b++){
    //if ( ii==0 ){    // donation target
    //if (  ii == 11){    // donator

    /*
    cout<<endl<<"--- ";    
      b->output1(2,0, s_e);
    */
 

      if (b->symbol=="TRANSACTION_TP"){              

	//cout<<endl<<"--- ";  
	//b->output1(2,0, s_e);

	//s_e.clear();
	
	cout<<endl<<"--- ";   
	b->output1(4,0, s_e);
      }

     /*
      
      if (b->symbol=="CMTE_ID"){                
	//cout<<endl<<"--- ";   
	//b->output1(3,0, s_e);
      }

      if (b->symbol=="CMTE_ID"){                
	cout<<endl<<"--- ";   
	b->output1(4,0, s_e);
      }

      if (b->symbol=="NAME"){                
	//cout<<endl<<"--- ";   
	//b->output1(4,0, s_e);
      }

 

      

 
      if (b->symbol=="CMTE_ID"){                // committee id - name
	cout<<endl<<"--- ";   
	b->output1(4,0, s_e);
      }
      if (b->symbol=="CMTE_ID"){                // committee id - name
	cout<<endl<<"--- ";   
	b->output1(5,0, s_e);
      }

      */

      /*
      if (b->symbol=="CMTE_ID"){                // committee id - name
	cout<<endl<<"--- ";   
	b->output1(5,0, s_e);
      }

      if (b->symbol=="CMTE_ID"){                // committee id - name
	cout<<endl<<"--- ";   
	b->output1(6,0, s_e);
      }

      */     

      if (b->symbol=="OTHER_ID"){                // committee id - name
	//cout<<endl<<"--- ";   
	//b->output1(3,0, s_e);
      }
 
      
     /*      

      if (b->symbol=="EMPLOYER"){                // employer
	cout<<endl<<"--- ";   
	b->output1(3,0, s_e);
      }

      */



  }

      
      //}      
    ii++;

    return 0;
}

// -----------------------------------  print out data graph    




//----------------------------------------
//
int main2(Node* graph0)
{


  string         line;
  string         field;

  int            s1=0;
  int            s2=0;

  int            first =0;
  int            count=0;

  //Node graph0("fields",0);

  
  

  //----------------------------------------- read the data

    
  while (getline(cin, line)){                //   $ cat datafile.txt | ./a.out > scr.txt

    if (first==1 || line =="-first") {

      first=0;
    
      for (int i = 0;i!=24;i++){     // skip first 24 output lines in fec*.txt files
	getline(cin,line);
      
      }      
    }

    //cout<<line<<endl;

    if (line =="-roll" ){

      //graph0->roll_dollars(6);
      graph0-> roll_dollars_sort(6);
     cout<<"rolling- "<<graph0->symbol<<endl;

      first=1;

      continue;
    }
    
    count++;

    if ( isdigit( line.c_str()[0] )) {

      //cout<<line<<endl;

      int count1 = atoi(line.substr(0,1).c_str());

      count1 = 2+ (count1)*2;

      int    s1      =0;
      int    s2      =0;
      string field;
      list<string>             symbols;
      list<string>::iterator   s;

      
      s2=line.find("\t", s1); s1+=s2-s1+1;     // skip first field
     
      for (int i=0; i!= count1; i++){

	s2=line.find("\t", s1);field=line.substr(s1,s2-s1); s1+=s2-s1+1;  	

	//remove padding white space

	int last_not_space = 0;
	
	int ii =0;
	
	for ( ;ii< field.length(); ++ii ){

	  if (field[ii] != ' ') last_not_space = ii;
	  
	}

	string field1 = field.substr(0, last_not_space+1);
	
    
	//cout<<"*"<<field1<<"*"<<endl;
	
	symbols.push_back(field1);
      }
      
      symbols.push_back("-stop");

      s2=line.find("\t", s1);field=line.substr(s1,s2-s1); s1+=s2-s1+1;
      long long int  dollars = atol (field.c_str());
      //cout<<s1<<"-"<<s2<<endl;
      
      s2=line.find("\t", s1);field=line.substr(s1,s2-s1); s1+=s2-s1+1;
      long long int  average = atol (field.c_str());
      //cout<<s1<<"-"<<s2<<endl;

      s2=line.find("\n", s1);field=line.substr(s1,line.size() /*s2-s1*/); s1+=s2-s1+1;
      long long int  number_donations = atol (field.c_str());
      //cout<<s1<<"-"<<s2<<endl;

      //cout<<"**"<<field<<endl;

      //cout<<s2<<" "<<dollars<<" "<<average<<"  "<<number_donations<<endl;

	s=symbols.begin();

	//if (*s!="NAME"){
	if (*s=="TRANSACTION_AMT"){
	  graph0->input( s, dollars, number_donations);
	}
      
    } 
 
  }

  //cout<<count<<endl;

  //cout<<graph0->branches_map.size()<<endl;

  list<Node>::iterator b =graph0->branches.begin();
  for (               ;b!=graph0->branches.end();b++){

    cout<<"--- "<<b->symbol;
    if (b->symbol.size()<12) cout<<"\t";

    cout<<"\t #unique=\t"<<b->branches.size()<<endl; 
    
  }




   list<string> s_e;
   
  b =graph0->branches.begin();
  for (               ;b!=graph0->branches.end();b++){


    cout<<endl<<"--- ";      b->output1(2,0, s_e);


  }




  
 return 0;
}



//----------------------------------------
//
int roll_array( Node* graph0, string field1, string field2)
{

  /*
---     TRANSACTION_TP
0	TRANSACTION_TP	15
1	TRANSACTION_TP	15	TRANSACTION_DT
2	TRANSACTION_TP	15	TRANSACTION_DT	03/2022     	   903233622	513	1758068
  */


  Node graph1(field1, 0);
  
  
  map<string,Node*>::iterator bm = graph0->branches_map.find(field1);

  if (bm != graph0->branches_map.end()){


    list<string> column_titles;
    
    map<string,Node*>::iterator bm1 = bm->second->branches_map.begin();

    for ( ; bm1 != bm->second->branches_map.end(); bm1++){


      //cout<<bm1->second->symbol<<endl;

      column_titles.push_back(bm1->second->symbol);

      
      map<string,Node*>::iterator bm2 = bm1->second->branches_map.find(field2);

      if (bm2 != bm1->second->branches_map.end() ){

	  list<string>             symbols;
	  list<string>::iterator   s;
    
	  map<string,Node*>::iterator bm3 = bm2->second->branches_map.begin();

	  for ( ; bm3 != bm2->second->branches_map.end(); bm3++){

	    //cout<<bm3->second->symbol<<endl;

	    list<string>             symbols;
	    list<string>::iterator   s;
  
	    symbols.push_back(field2);

	    symbols.push_back(bm3->second->symbol);

	    symbols.push_back("-stop");
	    s = symbols.begin();

	    graph1.input(s, bm3->second->dollars);


	    
	  }
	  

      }
      
      graph1.roll_dollars(6);
	  
    }    // bm1

    
    list<string>::reverse_iterator c = column_titles.rbegin();

    cout<<"\t\t";
    
    for ( ;c!=column_titles.rend();++c){

      cout<<*c<<"\t";
      
    }
    cout<<endl;

   list<string> s_e;
   graph1.output1(3,0, s_e);



    
  }


  return 0;
}


//----------------------------------------
//
int main()
{

  Node graph0;

  graph0.symbol="graph0";
  //main1();

  main2(&graph0);

  //roll_array(&graph0, "TRANSACTION_TP", "TRANSACTION_DT");

  
}
