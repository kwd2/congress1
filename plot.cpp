
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





int main()
{


 
  //--------------------------------- ROOT histogram stuff
  //   TH1F* h = new TH1F("","", , , );
  gStyle->SetOptStat(110010);    //ksiourmen  // supress leftmost 0s
  //----------------------------------- ROOT histogram stuff

 


  cout<<endl<<"Shannon 2.0"<<endl<<endl;

  TCanvas* c1 = new TCanvas("c1","data",200,10,600,900);


  // -------------- make and plot a simple histogram
  
  TH1F* p0 = new TH1F("hp0","histogram title ",1000,0.,10.);

  p0->Fill(5);
  p0->Fill(5);
  p0->Fill(5);
  p0->Fill(4);
  p0->Fill(4);
  p0->Fill(3);
  
  //p0->Draw();c1->Print("plot.pdf(","pdf");  c1->Clear();

    // -------------- make and plot a simple histogram

  

  //-------------- read rx_bytes data in testdata.txt

  int         x [500000];
  int         y [500000];
  int         y2[500000];
  int            cnt=0;
  int            cnt1=0;

  int            previous_rx_bytes=0;
  
  TH1F*          h1 = new TH1F("hh1","bytes per 1 sec", 1000, 0, 10000000);
  
  string         line;
  string         field;

  
                                                    // ------------open datafile
  
  ifstream       datafile2;  datafile2.open("testdata.txt");  if(!datafile2.is_open()){cout<<"Error 1"<<endl;return 0;}

  while (getline(datafile2, line)){                 // --- loop over ever line

    if(datafile2.eof()) break;
    
     int rx_bytes = atoi(line.c_str());

    //cout<<line<<" - "<<rx_bytes<<endl;             //  debugging

    x [cnt] = cnt;
    y [cnt] = rx_bytes;
    y2[cnt] = rx_bytes - previous_rx_bytes;
    cnt++;

   
    if ( rx_bytes >=  previous_rx_bytes )
      h1->Fill( rx_bytes - previous_rx_bytes );

    previous_rx_bytes = rx_bytes;

    cnt1++;
    if(cnt1>10000){
      cout<<cnt<<endl;
      cnt1=0;
    }
    
  }                                                 // --- loop over ever line

  //-------------- read rx_bytes data in testdata.txt

  
  //--- make plots
  
  TGraph* gr1 = new TGraph (cnt, x, y );
  TGraph* gr2 = new TGraph (cnt, x, y2);

  gr1->SetTitle("total rx_bytes vs second of day");
  gr1->Draw("ACP");  c1->Print("plot.pdf(","pdf");  c1->Clear();

  gr2->GetYaxis()->SetRangeUser(0.,100000000.);
  gr2->SetTitle("rx_bytes / sec  vs second of day");

  gPad->SetLogy();
  gr2->Draw("AP");  c1->Print("plot.pdf(","pdf");  c1->Clear();

  
  h1->GetXaxis()->SetRangeUser(0,6000000);
  
  gPad->SetLogy();

  h1->Draw();c1->Print("plot.pdf(","pdf");  c1->Clear();


  


  
  
  c1->Print("plot.pdf)","pdf");  c1->Clear();  // final print
}
