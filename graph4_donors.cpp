
#include <TError.h>
#include "TCanvas.h"
#include "TStyle.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TMath.h"
#include "TGraph.h"

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

  long int            dollars;                              
  list<long int>      dollars_list;                              

  int                 number_donations;
  
  list<Node>          branches;
  map<string,Node*>   branches_map;


  void   input        (list<string>::iterator sym, long int amount);

  void   output       ( int  depth,  int tabs );
  void   output1      ( int  depth,  int tabs, list<string> head_nodes );


  void   roll_dollars(int depth);

  
  Node(){dollars=0;number_donations=0;};
  Node(string name, long int value ){symbol=name;dollars= value;number_donations=0;/*cout<<name<<" "<<value<<endl;*/}

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

      b->roll_dollars(depth-1);
    }
  }

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


  cout<<symbol;

  if (depth == 1 ) {
  
  int size = symbol.size();  if (size>40) size=40;

  for (int i=0;i!=40-size;i++) cout<<" ";
  cout<<"\t";

  //cout<<number_donations<<"\t";
  
  for  (int i=0;i!=12;i++) if (dollars<pow(10,i)) cout<<" ";
  //cout << dollars ;
  if (depth==1) cout << dollars ;

  //cout<<"\t"<<dollars/number_donations;  //<<"\t"<<branches.size();
  
  list<long int>::iterator d = dollars_list.begin();

  for ( ; d!=dollars_list.end(); d++){

    //cout<<"\t"<<*d;
    if (depth==1) cout<<"\t"<<*d;
  }

  }  // depth==0
  
  cout << endl;
  
  if (depth-1>0){

      head_nodes.push_back(symbol);
    
    multimap<long int,Node*>                    br_sort;
    multimap<long int,Node*>::reverse_iterator  br;

    list<Node>::iterator s = branches.begin();

    int s1=0;
    for (;s!=branches.end();s++,s1++){

      int d1=0;
      //int y1=0;

      /*
      if (s->dollars_list.size()>0){


	list<long int>::iterator dl = s->dollars_list.begin();
	for (; dl != s->dollars_list.end();dl++) if (*dl>0) y1++;

	d1=   s->dollars_list.size()*100000  + (s->dollars_list.back())  +y1;



      }else{
      */
      //d1 = s->dollars;


      d1 = s->dollars;
      //y1=1;

      //d1 += pow(1000,10);	
      
      //list<long int>::iterator dl = s->dollars_list.begin();

      //for (; dl != s->dollars_list.end();dl++,y1++) if (d1==0) d1 = -*dl;


      //br_sort.insert(pair<long int,Node*>(s1, &(*s)));
      br_sort.insert(pair<long int,Node*>(d1, &(*s)));
      //br_sort.insert(pair<long int,Node*>(d1+y1, &(*s)));
    }
    
    //for (;s!=branches.end();s++) br_sort.insert(pair<long int,Node*>(s->dollars, &(*s)));
    
    for (br=br_sort.rbegin();br!=br_sort.rend();++br){

      cout<<tabs<<"\t";
      list<string>::iterator h = head_nodes.begin();
      
      for (;h!=head_nodes.end();h++) cout<<*h<<"\t";
      
      for (int i=0; i!= tabs; i++) cout<<"\t";
      
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

  list<long int>::iterator d = dollars_list.begin();

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
int main()
{

  //--------------------------------- ROOT histogram stuff
  //   TH1F* h = new TH1F("","", , , );
  gStyle->SetOptStat(110010);    //ksiourmen  // supress leftmost 0s
  //----------------------------------- ROOT histogram stuff

 
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
  
    while ( s2 != (int) string::npos ){

      s2=line.find("|", s1);field=line.substr(s1,s2-s1); s1+=s2-s1+1;

      fields.push_back(field);
      //cout<<field<<endl;
    }

    //cout<<fields.size()<<endl;
    if (fields.size()>2){
      cm_map.insert(pair<string,string>(fields[0],fields[1]));
    }
    
  }

  cout<<"number of committess "<<cm_map.size()<<endl;
    
  // ----- parse committee_master file

  

  // ----- parse candidate_master file

  multimap<string,string> cn_map;
  
  ifstream datafile2a;  datafile2a.open("data/cn.txt");  if(!datafile2a.is_open()){cout<<"Error 2a"<<endl;return 0;}
  
  while (getline(datafile2a, line)){
    if(datafile2a.eof()) break;

    vector<string> fields;

    s1 =0;
    s2 =0;
  
    while (s2 != (int) string::npos ){

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
  //int year_count=0;
  
  ifstream datafile1;  datafile1.open("data/header_donor.txt");  if(!datafile1.is_open()){cout<<"Error 1"<<endl;return 0;}


  getline(datafile1, line);  //  1st line

  //cout<<line<<endl;      

  s2 = 0;
  
  while (s2 != (int)string::npos ){

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

 
 
    while ( s2 != (int) string::npos ){
 
	num_commas++;
      
	s2=line.find("|", s1);field=line.substr(s1,s2-s1); s1+=s2-s1+1;    

	fields.push_back(field);	

	//cout<<field<<endl;

    
      if (num_commas==15){                                 // ------------  the $$$

	dollars1   =           atoi(field.c_str());
	//cout<<field<<"\t"<<dollars1<<endl;
      }


      if (num_commas==14){                                 // ---------------  the day

	/*
	//cout<<field<<endl;

	if (field.size()==8){
	
	year  = field.substr(4,4);
	month = field.substr(0,2);
	day   = field.substr(2,2);
	}
	
	//cout<<field<<"  "<<year<<" "<<month<<" "<<day<<endl;
	*/
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
    //map    <string,Node*> ::iterator   c  = columns.begin();
    vector <string>       ::iterator   c1 = columns1.begin();

    //cout<<endl;

    int ccnt=0;
    for (;c1!=columns1.end();c1++,f++,ccnt++){


      //cou<<*f<<"\t"<<*c1<<endl;
      
      symbols.clear();

      symbols.push_back( *c1  );
      symbols.push_back( *f  );

      //if (ccnt==0)  symbols.push_back(fields[14]);  //  date 
      //if (ccnt==0)  symbols.push_back(fields[11]);  //    employer2
      //if (ccnt==0)  symbols.push_back(fields[7]);  //    employer2

      /*
      if (ccnt==0) {                           //committee state city donor zip - nancy pelosi study
	symbols.push_back(fields[15]);  //    
	symbols.push_back(fields[9]);  //    
	symbols.push_back(fields[8]);  //    
	symbols.push_back(fields[7]);  //    
	//symbols.push_back(fields[10]);  //    

      }
      */
      
      /*
      if (ccnt==0)  symbols.push_back(fields[5]);  // 
      if (ccnt==0)  symbols.push_back(fields[15]);  // 
      if (ccnt==0)  symbols.push_back(fields[7]);  //
      if (ccnt==0)  symbols.push_back(fields[1]);  //
      */

      
      //if (ccnt==7)  symbols.push_back(fields[13]);  //  name - date

      //if (ccnt==15) symbols.push_back(fields[0]);  // other - committee id

      
      symbols.push_back("-stop");
      s = symbols.begin();

      if (ccnt!=4  && ccnt!=10 && ccnt<16){
      //if (ccnt==0){
      
	//column_graph.input( s, 1 );
      
	column_graph.input( s, dollars1 );
       }

    }
    // -----------------------------------  make data graph   

	
  }
  // ------------------------------------ parse main data file

  cout<<num_lines<<endl;



 
  
 
  // -----------------------------------  print out data graph   
  
  list<string> s_e; 


  list<Node>::iterator b =column_graph.branches.begin();
  for (               ;b!=column_graph.branches.end();b++){


      cout<<endl<<"--- ";   
      b->output1(2,0, s_e);

       /*    
      if (b->symbol=="CMTE_ID"){                
	cout<<endl<<"--- ";   
	b->output1(3,0, s_e);
      }

      
      if (b->symbol=="NAME"){                
	cout<<endl<<"--- ";   
	b->output1(3,0, s_e);
      }

     */ 
      
      /*
 
      if (b->symbol=="CMTE_ID"){                
	cout<<endl<<"--- ";   
	b->output1(4,0, s_e);
      }
      if (b->symbol=="CMTE_ID"){                
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
      
     /*      
      if (b->symbol=="OTHER_ID"){                // committee id - name
	cout<<endl<<"--- ";   
	b->output1(3,0, s_e);
      }
      */
  }

  // -----------------------------------  print out data graph    



  // -----------------------------------  root histograms


 TCanvas* c11 = new TCanvas("c11","data",200,10,2000,1400);


 TH1I* hta = new TH1I("hta","transaction ammount ",300, 0,7500.);
 TH1I* hta1 = new TH1I("hta1","transaction ammount ",100,0,3000000);

 
 map<string,Node*>::iterator     cgbm =column_graph.branches_map.find("NAME");
 //map<string,Node*>::iterator     cgbm =column_graph.branches_map.find("TRANSACTION_AMT");

 if (cgbm!= column_graph.branches_map.end()){

   list<Node>::iterator b = cgbm->second->branches.begin();
   for(;b!=cgbm->second->branches.end();b++){


  
     hta->Fill(  b->dollars/b->number_donations, b->dollars);
     hta1->Fill(  b->dollars/b->number_donations, b->dollars);
   }

   //gPad->SetLogy();
   hta->Draw("hist");c11->Print("graph4_donors.pdf(","pdf");  c11->Clear();

   gPad->SetLogy();
   //hta1->GetYaxis()->SetRangeUser(.1,10000);
   hta1->Draw("hist");c11->Print("graph4_donors.pdf(","pdf");  c11->Clear();
   gPad->SetLogy(0);


 

   multimap<int,int> amount_map;
   multimap<int,int> amount_map1;
   
   b = cgbm->second->branches.begin();
   for(;b!=cgbm->second->branches.end();b++){

     //int d = atoi(b->symbol.c_str());
     int d = b->dollars/b->number_donations;

     if (d>0) amount_map .insert(pair<int,int>(d,b->dollars));       

     if (d>0) amount_map1.insert(pair<int,int>(-1* b->dollars, b->dollars));       
   }


   //cout<<amount_map.size()<<"  "<<amount_map1.size()<<endl;

   
   Double_t* x = new Double_t[amount_map.size()+1];
   Double_t* y = new Double_t[amount_map.size()+1];
   Double_t* x1 = new Double_t[amount_map.size()+1];
   Double_t* y1 = new Double_t[amount_map.size()+1];

 
   multimap<int,int>::iterator am  = amount_map.begin();
   multimap<int,int>::iterator am1 = amount_map1.begin();

   long int total = 0;
   long int total1= 0;
   int i     = 0;
   int i1     = 0;

   for (;am != amount_map.end(); am++, i++){

     total  += am ->second;
         
     x [i] = am->first;
     y [i] = total;
     //cout<< i <<"  "<< total <<"  "<<am->first<<"  "<< am->second<<endl;

   }


   for (;am1 != amount_map1.end(); am1++, i1++){

     total1 += am1->second;
         
     x1[i1] = i1;
     y1[i1] = total1;


   }

   

  TGraph* gr1 = new TGraph (i, x, y );



  gr1->SetTitle("average donation of each donor vs total donated");
  gr1->GetXaxis()->SetTitle("average donation, sorted least to most");
  gr1->GetYaxis()->SetTitle("total dollars donated");

  gPad->SetLogx();
  gr1->Draw("AP");  c11->Print("graph4_donors.pdf(","pdf");  c11->Clear();
  gPad->SetLogx(0);
  //gr1->Draw("AP");  c11->Print("graph4_donors.pdf(","pdf");  c11->Clear();
  
  for (unsigned int ii=0;ii!=amount_map.size()+1;ii++) y[ii] = total - y[ii];
  
  TGraph* gr11 = new TGraph (i1, x1, y1 );

  gPad->SetLogx(0);

  gr11->SetTitle("total donated by each donor vs total donated");
  gr11->GetXaxis()->SetTitle("donor number, sorted by total donated most to least");
  gr11->GetYaxis()->SetTitle("total dollars donated");

  gr11->Draw("AP");  c11->Print("graph4_donors.pdf(","pdf");  c11->Clear();




  
 }                  // TRANSACTION_AMT




 // --- date
 

 TH1I* hdate = new TH1I("hdate","date ",1600, 0.5, 1600.5 );

 TH1I* hdate1= new TH1I("hdate1","date ",6000/7, 0.5, 6000.5/7 );

 TH1I* hdate2= new TH1I("hdate2","date ",12000/30, 0.5, 12000.5/30 );

 TH1I* hdate3= new TH1I("hdate3","total donations (per month) ",16000/30, 0.5, 16000.5/30 );




	map<string,Node*>::iterator     dt =column_graph.branches_map.find("TRANSACTION_DT");

	if (dt!= column_graph.branches_map.end()){

	  list<Node>::iterator b = dt->second->branches.begin();
	  for(;b!=dt->second->branches.end();b++){

	    //cout<<b->symbol<<"  "<<b->symbol.size()<<endl;
	    
	    if (b->symbol.size()==8){
	
	      int year  = atoi(b->symbol.substr(4,4).c_str());
	      int month = atoi(b->symbol.substr(0,2).c_str());
	      int day   = atoi(b->symbol.substr(2,2).c_str());


	      cout<<year<<"  "<<month<<"  "<<day<<endl;
	      
	      int d = (year-1978)*365 + month*31 + day;
	      int d1 = ((year-1978)*365 + month*31 + day)/7;
	      int d2 = ((year-1978)*365 + month*31 + day)/30;

	      hdate->Fill(d, b->dollars);
	      hdate1->Fill(d1, b->dollars);
	      hdate2->Fill(d2, b->dollars);
	      hdate3->Fill(d2, b->dollars);
	      
	    }


	    
	  }

	}
	
   gPad->SetLogy();
   //hdate->Draw("hist");c11->Print("graph4_donors.pdf(","pdf");  c11->Clear();
   //hdate1->Draw("hist");c11->Print("graph4_donors.pdf(","pdf");  c11->Clear();

  gPad->SetLogy(0);
  //hdate->Draw("hist");c11->Print("graph4_donors.pdf(","pdf");  c11->Clear();
  hdate1->Draw("hist");c11->Print("graph4_donors.pdf(","pdf");  c11->Clear();
  hdate2->Draw("hist");c11->Print("graph4_donors.pdf(","pdf");  c11->Clear();


  hdate3->GetYaxis()->SetRangeUser(0.,1000000000.);
  hdate3->Draw("hist");c11->Print("graph4_donors.pdf(","pdf");  c11->Clear();







 

 
  c11->Print("graph4_donors.pdf)","pdf");  c11->Clear();  // final print

  // -----------------------------------  root histograms



}
