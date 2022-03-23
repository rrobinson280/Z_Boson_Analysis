#include "include/AtlasStyle.C"
#include "include/AtlasLabels.C"
#include "include/AtlasUtils.C"

#include <iostream>
#include <TRandom3.h>
#include <TFeldmanCousins.h>
#include <TH1F.h>
#include <TString.h>

using namespace std;

bool logy = false;

int compute_limit_single_signal () 
{

  SetAtlasStyle();
  TH1::SetDefaultSumw2(true);
  gROOT->Reset();

  //Sample names
  // For measurements you only have one
  // 
  string samples = "Signal";
  //These are the number of expected and background events
  //number of signal events after WmT >30 GeV cut
  double sig = 4.24623e+12;
  // SM MC events after WmT > 30 GeV
  double bkg = 776.8;
  // Number of observed events
  double obs = 294469;

  //Signal cross sections
  double xsec =  1241.2072;

  // Limit canculator configuration
  int n_trials = 1000;
  TRandom3 r;
  TFeldmanCousins limit_eval(0.95);
  cout << "trials " << n_trials << endl;

  TH1F *hlimit, *hextraction;
  double mean=0.;
  double  err=0;

  // arrays that would go into thee TGraph
  // 
  double xsec_limit;
  double obs_xsec_limit;

  /////////////  /////////////  /////////////  /////////////
  //Calculation of the limit for all samples happends here
  hlimit = new TH1F((samples+"_limit").c_str(),"Upper limit histo",2000,0.,200);
  hextraction = new TH1F((samples+"_extr").c_str(),"Extraction histo",2000,0.,200);
      
  if(bkg<10)
    {
      for (int i=0; i<n_trials; i++) 
	{
	  if(i%100==0) cout << "trial " << i << endl;
	  double extraction = r.Poisson(bkg);
	  hextraction->Fill(extraction);
	  double limit = limit_eval.CalculateUpperLimit(extraction, bkg);
	  hlimit->Fill(limit);   
	}//end loop over n_trials
      mean=hlimit->GetMean();
      err=hlimit->GetStdDev();
    }
  else 
    {
      mean  = 1.64*sqrt(bkg);
      //mean  = 1.64*sqrt(bkg[isam]+0.15*0.15*bkg[isam]*bkg[isam]);
      //mean  = 1.64*sqrt(bkg[isam]+0.50*0.50*bkg[isam]*bkg[isam]);
      err =0;
    }
      
  xsec_limit = mean/sig*xsec;
      
  cout << "Expected Upper cross section limit for sample " << samples << endl;
  //cout << " mean " << mean << " +/- " << err << endl;
  //cout << " mu " << mean/sig[isam] << " +/- " << err/sig[isam] << endl;
  cout << " xsec " << mean/sig*xsec << " +/- " << err/sig*xsec << " pb" << endl;
  cout << " ************************************************ " << endl; 
  /////////////  /////////////  /////////////  /////////////
  /////////////  /////////////  /////////////  /////////////

  /////////////  /////////////  /////////////  /////////////
  //Calculation of the limit for all samples happends here
  hlimit = new TH1F((samples+"_obs_limit").c_str(),"Upper limit histo",2000,0.,200);
  hextraction = new TH1F((samples+"_obs_extr").c_str(),"Extraction histo",2000,0.,200);
      
  if(obs<10)
    {
      for (int i=0; i<n_trials; i++) 
	{
	  if(i%100==0) cout << "trial " << i << endl;
	  double extraction = r.Poisson(obs);
	  hextraction->Fill(extraction);
	  double limit = limit_eval.CalculateUpperLimit(extraction, obs);
	  hlimit->Fill(limit);   
	}//end loop over n_trials
      mean=hlimit->GetMean();
      err=hlimit->GetStdDev();
    }
  else 
    {
      mean  = 1.64*sqrt(obs);
      //mean  = 1.64*sqrt(obs+0.15*0.15*obs*obs);
      //mean  = 1.64*sqrt(obs+0.50*0.50*obs*obs);
      err =0;
    }
      
  obs_xsec_limit = mean/sig*xsec;
      
  cout << "Observed Upper cross section limit for sample " << samples << endl;
  //cout << " mean " << mean << " +/- " << err << endl;
  //cout << " mu " << mean/sig << " +/- " << err/sig << endl;
  cout << " xsec " << mean/sig*xsec << " +/- " << err/sig*xsec << " pb" << endl;
  cout << " ************************************************ " << endl; 
  /////////////  /////////////  /////////////  /////////////
  /////////////  /////////////  /////////////  /////////////
  

  TCanvas *can = new TCanvas("can","",600,600);
  can->cd(0);
  if(logy) can->SetLogy();


  
  TMultiGraph *limit_plot = new TMultiGraph();
  limit_plot->SetTitle(";m_{Z'}   [GeV];Upper limit on #sigma(Z#rightarrow#it{l}#bar{#it{l}) [pb]");
  //expected limit
  TH1F * my_lim = new TH1F("expected", ";;Upper limit on #sigma(Z#rightarrow#it{l}#bar{#it{l}}) [pb];",2, 0, 2);
  my_lim->SetMaximum(0.8*10e-6);
  my_lim->SetMinimum(-10e-6);
  my_lim->SetBinContent(1, xsec_limit);
  my_lim->SetBinContent(2, xsec_limit);
  my_lim->GetXaxis()->SetBinLabel(1, "");
  my_lim->GetXaxis()->SetBinLabel(2, "");
  //observed limit
  TH1F * my_lim_obs = new TH1F("observed", ";;Upper limit on #sigma(Z#rightarrow#it{l}#bar{#it{l}}) [pb];",2, 0, 2);
  my_lim_obs->SetMaximum(0.8*10e-6);
  my_lim_obs->SetMinimum(-10e-6);
  my_lim_obs->SetBinContent(1, obs_xsec_limit);
  my_lim_obs->SetBinContent(2, obs_xsec_limit);

  my_lim->SetLineStyle(7);
  my_lim->SetLineWidth(2);
  my_lim->SetLineColor(kBlue);
  my_lim_obs->SetLineWidth(2);
  my_lim->Draw("l");
  my_lim_obs->Draw("lsame");


  TLegend *leg = new TLegend(0.55, 0.7, 0.75, 0.9);
  leg->SetBorderSize(0);
  leg->SetTextFont(42);
  leg->SetTextSize(0.035);
  leg->SetLineColor(1);
  leg->SetLineStyle(1);
  leg->SetLineWidth(1);
  leg->SetFillColor(0);
  leg->SetFillStyle(1001);
  leg->SetHeader("     #sqrt{s} = 8 TeV, 1 fb^{-1}");
  leg->AddEntry(my_lim_obs,"Observed","l");
  leg->AddEntry(my_lim,"Expected","l");
  //leg->AddEntry(theory_xsec,"Theory #sigma(Z'#rightarrowt#bar{t})","l");
  leg->Draw();

  can->SaveAs("XsecLimit_single_signal.pdf");
  
  return 0;
  
}

