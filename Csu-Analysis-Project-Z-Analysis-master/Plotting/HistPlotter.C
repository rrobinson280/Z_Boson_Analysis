#include "include/AtlasStyle.C"
#include "include/AtlasLabels.C"
#include "include/AtlasUtils.C"


void groupSamples();
void makeGroupHist();

std::map<TString, float> db;
std::map<TString, float> dy;
std::map<TString, float> w;
std::map<TString, float> z;
std::map<TString, float> stop;
std::map<TString, float> tt;
std::map<TString, float> signalmap;

TH1F *h_db;
TH1F *h_drellyan;
TH1F *h_wjets;
TH1F *h_singletop;
TH1F *h_ttbar;

std::string histname = "h_recon_Zmass";
float lumi = 1000.;
int rebin = 2; //20 - ZmInv //4 - wmt //1 - event selection
bool logy = true;
std::string signalmass = "400";

void HistPlotter(std::string mass = ""){

  if(mass!= "") signalmass = mass;
    
  SetAtlasStyle();
  TH1::SetDefaultSumw2(true);
  gROOT->Reset();

  groupSamples();
  makeGroupHist();

  THStack *h_stack = new THStack("h_stack","");

  h_db->SetFillColor(kRed);
  h_drellyan->SetFillColor(kGreen);
  h_wjets->SetFillColor(kMagenta);
  h_singletop->SetFillColor(kOrange);
  h_ttbar->SetFillColor(kCyan);

  h_stack->Add(h_db);
  h_stack->Add(h_drellyan);
  h_stack->Add(h_wjets);
  h_stack->Add(h_singletop);
  h_stack->Add(h_ttbar);
    
  TH1F *h_err = (TH1F*) h_db->Clone();
  h_err->Add(h_drellyan);
  h_err->Add(h_wjets);
  h_err->Add(h_singletop);
  h_err->Add(h_ttbar);
  h_err->SetFillStyle(3004);
  h_err->SetFillColor(kBlack);
  h_err->SetLineColor(0);
  h_err->SetMarkerStyle(1);


  TFile *f_sig = TFile::Open("hist-Zmumu.root");
  TH1F* h_signal = (TH1F*) f_sig->Get(histname.c_str());
  h_signal->Rebin(rebin);
  TH1F* h_signal_clone = (TH1F*) h_signal->Clone();
  //Rescale signal
  //h_signal->Scale(lumi*1e10*signalmap["Zmumu"]);
  h_signal->Scale(lumi*signalmap["Zmumu"]);
  h_signal->SetLineStyle(7);
  h_signal->SetLineWidth(3);
  h_signal->SetLineColor(kPink-8);


  TFile *f_egamma = TFile::Open("hist-DataEgamma.root");
  TFile *f_muon = TFile::Open("hist-DataMuons.root");
  TH1F *h_data_egamma = (TH1F*) f_egamma->Get(histname.c_str());
  TH1F *h_data_muon = (TH1F*) f_muon->Get(histname.c_str());
  h_data_egamma->Rebin(rebin);
  h_data_muon->Rebin(rebin);

  TH1F *h_data = (TH1F*) h_data_egamma->Clone();
  h_data->Add(h_data_muon);
  h_data->SetMarkerSize(1);
  h_data->SetMarkerStyle(20);
  h_data->SetMarkerColor(kBlack);

  /// Chi2
  //double chi2_test = h_data->Chi2Test(h_err, "UW,P");
  //cout<<"chi2_test  "<<chi2_test<<endl;

  // I will print some numbers here based on the h_signal, h_data and h_err (sum of all backgrounds) histograms
  float cut = 0;

  if(signalmass == "400") cut = 0.;
  if(signalmass == "500") cut = 0.;
  if(signalmass == "750") cut = 400.;
  if(signalmass == "1000") cut = 500.;
  if(signalmass == "1250") cut = 800.;
  if(signalmass == "1500") cut = 1000.;
  if(signalmass == "1750") cut = 1250.;
  if(signalmass == "2000") cut = 1500.;
  if(signalmass == "2250") cut = 1500.;
  if(signalmass == "2500") cut = 1500.;
  if(signalmass == "3000") cut = 1500.;

  
  cout<<"#####\t Event yields"<<endl;
  cout<<"#####\t Data   \t:\t"<< h_data->Integral(h_data->GetXaxis()->FindBin(cut), h_data->GetNbinsX()+1)<<endl;
  cout<<"#####\t MC     \t:\t"<< h_err->Integral(h_err->GetXaxis()->FindBin(cut), h_err->GetNbinsX()+1)<<endl;
  cout<<"#####\t Signal     \t:\t"<< h_signal_clone->Integral(h_signal_clone->GetXaxis()->FindBin(cut), h_signal_clone->GetNbinsX()+1)<<endl;

  //calculate event selection efficiency
  if(  histname== "h_event_selection"){
    for(int bin = 2; bin < h_data->GetNbinsX(); bin++){
      double eff_rel = h_err->GetBinContent(bin-1) != 0 ? h_err->GetBinContent(bin)/h_err->GetBinContent(bin-1) : 0;
      double eff_abs = h_err->GetBinContent(1) != 0 ? h_err->GetBinContent(bin)/h_err->GetBinContent(1) : 0;

      double eff_rel_err =  h_err->GetBinContent(bin-1) != 0 ? sqrt( eff_rel*(1- eff_rel)/ h_err->GetBinContent(bin)/h_err->GetBinContent(bin-1)) : 0;
      double eff_abs_err = h_err->GetBinContent(1) != 0 ?  sqrt( eff_abs*(1- eff_abs)/ h_err->GetBinContent(bin)/h_err->GetBinContent(bin-1)) : 0;      
      cout<<h_err->GetXaxis()->GetBinLabel(bin)<<endl;
        std::cout<<"eff rel err"<<std::endl;
      std::cout<< eff_rel<<" "<<eff_rel_err<<std::endl;
        std::cout<<"eff abs err"<<std::endl;
      std::cout<< eff_abs<<" "<<eff_abs_err<<std::endl;
    }
  }
  
  
  ///// Plotting
  TCanvas *can = new TCanvas("can","", 600, 600);
  //can->SetRightMargin(0.15);
  //can->SetBottomMargin(0.185);
  can->cd();
  if(logy) can->SetLogy();


  h_stack->Draw("hist");
  h_err->Draw("e2same");
  h_data->Draw("epsame");
  h_signal->Draw("histsame");
  h_stack->SetMaximum(1.15*h_stack->GetMaximum());
  if (logy && histname.find("event_selection") != std::string::npos) h_stack->SetMaximum(1e8*h_stack->GetMaximum());
  if (logy && histname.find("event_selection") != std::string::npos) h_stack->SetMinimum(10);

  //else if(logy) h_stack->SetMinimum(0.5);

  //h_stack->GetXaxis()->SetTitle(h_data_egamma->GetXaxis()->GetTitle());
  if(histname=="h_recon_llmass"){
      h_stack->GetXaxis()->SetTitle("Mass_{l#bar{l}} (Z#rightarrow#it{l}#bar{#it{l}})");
      h_stack->SetMaximum(35000.);
      h_stack->SetMinimum(1.);
  }
  if(histname=="h_recon_Zmass"){
      h_stack->GetXaxis()->SetTitle("Mass_{Z} (Z#rightarrow#it{l}#bar{#it{l}})");
      h_stack->SetMaximum(15000.*rebin);
      h_stack->SetMinimum(1.);
      h_stack->GetXaxis()->SetRangeUser(68, 114);
  }
  if(histname=="h_lep_pt_f"){
      h_stack->GetXaxis()->SetTitle("lep_{pt}");
      h_stack->SetMaximum(50000.*rebin);
      h_stack->SetMinimum(1.);
      h_stack->GetXaxis()->SetRangeUser(0, 1000);
  }
  if(histname=="h_lep_eta_f"){
      h_stack->GetXaxis()->SetTitle("lep_{#eta}");
      h_stack->SetMaximum(40000.*rebin);
      h_stack->SetMinimum(1.);
      h_stack->GetXaxis()->SetRangeUser(-3, 3);
  }
  if(histname=="h_lep_phi_f"){
      h_stack->GetXaxis()->SetTitle("lep_{#phi}");
      h_stack->SetMaximum(40000.*rebin);
      h_stack->SetMinimum(1.);
      h_stack->GetXaxis()->SetRangeUser(-5, 5);
  }
  h_stack->GetXaxis()->SetTitleOffset(1.60);
  h_stack->GetYaxis()->SetTitle("Events");
  h_stack->GetYaxis()->SetTitleOffset(1.60);
  if(histname.find("wmt") != std::string::npos)h_stack->GetXaxis()->SetRangeUser(0, 775);
  else if(histname.find("ZmInv") != std::string::npos)h_stack->GetXaxis()->SetRangeUser(0, 3775);
  else if(histname.find("ZmT") != std::string::npos)h_stack->GetXaxis()->SetRangeUser(0, 3575);
  else if(histname.find("lep_pt") != std::string::npos)h_stack->GetXaxis()->SetRangeUser(0, 1575);
  else if(histname.find("h_ZpInvM") != std::string::npos) h_stack->GetXaxis()->SetRangeUser(0, 3350);

  //TLegend *leg = new TLegend(0.55, 0.55, 0.75, 0.9);
  //TLegend *leg = new TLegend(0.35, 0.65, 0.25, 0.9);
  //TLegend *leg = new TLegend(0.60,0.62,0.77,0.915);
  TLegend *leg = new TLegend(0.68,0.72,0.83,0.94);
  leg->SetBorderSize(0);
  leg->SetTextFont(42);
  leg->SetTextSize(0.035);
  leg->SetLineColor(1);
  leg->SetLineStyle(1);
  leg->SetLineWidth(1);
  leg->SetFillColor(0);
  leg->SetFillStyle(1001);
  leg->SetHeader("     #sqrt{s} = 8 TeV, 1 fb^{-1}");
  leg->AddEntry(h_data,"Data","p");
  leg->AddEntry(h_ttbar,"t#bar{t}","f");
  leg->AddEntry(h_singletop,"Single top","f");
  leg->AddEntry(h_wjets,"W","f");
  leg->AddEntry(h_drellyan,"Drell-Yan","f");
  leg->AddEntry(h_db,"Diboson","f");
  leg->AddEntry(h_err,"MC stat. uncertainty","f");
  if(histname == "h_event_selection"){
    leg->AddEntry(h_signal,"(Z#rightarrowl#bar{l})#times10^{10}","l");
  }
  else leg->AddEntry(h_signal,"Z#rightarrowl#bar{l}","l");
    //(" + signalmass + " GeV)#rightarrow t#bar{t} x 10^{5}").c_str(),"l");
  leg->Draw();

  std::string outfile = histname.substr(2, std::string::npos);
  if(logy) can->SaveAs((outfile+"_log.pdf").c_str());
  else can->SaveAs((outfile+".pdf").c_str());
}

void groupSamples(){

  std::ifstream samples;
  samples.open("samples.txt");
  
  while(!samples.eof()){
    TString sample;
    float xsec;
    float red_eff;
    float sumw;
    float nevt;

    samples >> sample >> xsec >> red_eff >> sumw >> nevt;

    nevt=1;
    //sumw=1;
    if(sample.Contains("WW")  || sample.Contains("WZ")  || sample.Contains("ZZ")  ) db[sample] = xsec*nevt/(red_eff*sumw);

    if(sample.Contains("DY")  ) dy[sample] = xsec*nevt/(red_eff*sumw);

    if(sample.Contains("Wenu")  || sample.Contains("Wmunu")  || sample.Contains("Wtaunu")  ) w[sample] = xsec*nevt/(red_eff*sumw);

    if(sample.Contains("Zee")  || sample.Contains("Zmumu")  || sample.Contains("Ztautau")  ) signalmap[sample] = xsec*nevt/(red_eff*sumw);
    
      
      
    if(sample.Contains("stop")  ) stop[sample] = xsec*nevt/(red_eff*sumw);

    if(sample.Contains("ttbar")  ) tt[sample] = xsec*nevt/(red_eff*sumw);

    //if(sample.Contains("ZPrime")  ); //signalmap[sample] = xsec*nevt/(red_eff*sumw);


  }


  return;
}

void makeGroupHist(){

  //for(unsigned int i = 0; i < db.size(); i++){
  int i = 0;
  for(std::map<TString, float>::iterator it = db.begin(); it != db.end(); ++it){
    TFile *f = TFile::Open(Form("hist-%s.root",(it->first).Data()));
    TH1F *h = (TH1F*) f->Get(histname.c_str());    
    h->Rebin(rebin);
    h->Scale(it->second*lumi);
    //h->Scale(it->second*lumi/h->Integral(0, h->GetNbinsX()+1));
    if(i==0) h_db = (TH1F*) h->Clone();
    else h_db->Add(h);
    i++;
  }

  i = 0;
  for(std::map<TString, float>::iterator it = dy.begin(); it != dy.end(); ++it){
    TFile *f = TFile::Open(Form("hist-%s.root",(it->first).Data()));
    TH1F *h = (TH1F*) f->Get(histname.c_str());    
    h->Rebin(rebin);
    h->Scale(it->second*lumi);
    //h->Scale(it->second*lumi/h->Integral(0, h->GetNbinsX()+1));
    if(i==0) h_drellyan = (TH1F*) h->Clone();
    else h_drellyan->Add(h);
    i++;
  }

  i = 0;
  for(std::map<TString, float>::iterator it = w.begin(); it != w.end(); ++it){
    TFile *f = TFile::Open(Form("hist-%s.root",(it->first).Data()));
    TH1F *h = (TH1F*) f->Get(histname.c_str());    
    h->Rebin(rebin);
    h->Scale(it->second*lumi);
    //h->Scale(it->second*lumi/h->Integral(0, h->GetNbinsX()+1));
    if(i==0) h_wjets = (TH1F*) h->Clone();
    else h_wjets->Add(h);
    i++;
  }

  i = 0;
  for(std::map<TString, float>::iterator it = stop.begin(); it != stop.end(); ++it){
    TFile *f = TFile::Open(Form("hist-%s.root",(it->first).Data()));
    TH1F *h = (TH1F*) f->Get(histname.c_str());    
    h->Rebin(rebin);
    h->Scale(it->second*lumi);
    //h->Scale(it->second*lumi/h->Integral(0, h->GetNbinsX()+1));
    if(i==0) h_singletop = (TH1F*) h->Clone();
    else h_singletop->Add(h);
    i++;
  }

  i = 0;
  for(std::map<TString, float>::iterator it = tt.begin(); it != tt.end(); ++it){
    TFile *f = TFile::Open(Form("hist-%s.root",(it->first).Data()));
    TH1F *h = (TH1F*) f->Get(histname.c_str());    
    h->Rebin(rebin);
    h->Scale(it->second*lumi);
    //h->Scale(it->second*lumi/h->Integral(0, h->GetNbinsX()+1));
    if(i==0) h_ttbar = (TH1F*) h->Clone();
    else h_ttbar->Add(h);
    i++;
  }

}
