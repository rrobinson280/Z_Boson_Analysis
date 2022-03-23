#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <ZAnalysis/ZAnalysis.h>
#include <iostream>

// this is needed to distribute the algorithm to the workers
ClassImp(ZAnalysis)



ZAnalysis :: ZAnalysis ()
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().
}



EL::StatusCode ZAnalysis :: setupJob (EL::Job& job)
{
  // Here you put code that sets up the job on the submission object
  // so that it is ready to work with your algorithm, e.g. you can
  // request the D3PDReader service or add output files.  Any code you
  // put here could instead also go into the submission script.  The
  // sole advantage of putting it here is that it gets automatically
  // activated/deactivated when you add/remove the algorithm from your
  // job, which may or may not be of value to you.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ZAnalysis :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.

  h_event_selection = new TH1F("h_event_selection","",11, 0, 11);
  wk()->addOutput(h_event_selection);
  h_event_selection->GetXaxis()->SetBinLabel(1,"All events");
  h_event_selection->GetXaxis()->SetBinLabel(2,"Pass trigger");
  h_event_selection->GetXaxis()->SetBinLabel(3,"Good GRL");
  h_event_selection->GetXaxis()->SetBinLabel(4,"Good vertex");
  h_event_selection->GetXaxis()->SetBinLabel(5,"Two good lepton");
  h_event_selection->GetXaxis()->SetBinLabel(6,"Leptons have opposite charge");
  h_event_selection->GetXaxis()->SetBinLabel(7,"Leptons have same flavour");
  h_event_selection->GetXaxis()->SetBinLabel(8,"|m_{ll}-m_{PDG Z}|<20 GeV");


  h_trigger_pass = new TH1F("h_trigger_pass","",7, 0, 7);
  wk()->addOutput(h_trigger_pass);
  //The lines below setup the bin labels for the "trigger" histogram
  h_trigger_pass->GetXaxis()->SetBinLabel(1,"fail egamma");
  h_trigger_pass->GetXaxis()->SetBinLabel(2,"pass egamma");
  h_trigger_pass->GetXaxis()->SetBinLabel(3,"fail muon");
  h_trigger_pass->GetXaxis()->SetBinLabel(4,"pass muon");
  h_trigger_pass->GetXaxis()->SetBinLabel(5,"fail both");
  h_trigger_pass->GetXaxis()->SetBinLabel(6,"pass both");
  h_trigger_pass->GetXaxis()->SetBinLabel(7,"events processed");

  //Below you can find histograms that will be filled with the kinematics of the (objects in the) event
  
  h_lep_pt_f = new TH1F("h_lep_pt_f",";p_{T}^{lepton} [GeV];", 200, 0, 500);
  wk()->addOutput(h_lep_pt_f);
  h_lep_eta_f = new TH1F("h_lep_eta_f",";#eta^{lepton};", 100, -5, 5);
  wk()->addOutput(h_lep_eta_f);
  h_lep_phi_f = new TH1F("h_lep_phi_f",";#phi^{lepton};", 64, -3.2, 3.2);
  wk()->addOutput(h_lep_phi_f);

  h_recon_llmass = new TH1F("h_recon_llmass",";Recon mass_{ll};", 250, 0, 200);
  wk()->addOutput(h_recon_llmass);
  h_recon_Zmass = new TH1F("h_recon_Zmass",";M_{Z}(Z#rightarrowl#bar{l});", 200, 65 , 115);
  wk()->addOutput(h_recon_Zmass);
  h_lep_charge = new TH1F("h_lep_charge",";Lep charge;", 8, -2, 2);
  wk()->addOutput(h_lep_charge);
  h_lep_type = new TH1F("h_lep_type","lep type", 8, 10, 15);
  wk()->addOutput(h_lep_type);

  h_lep_n = new TH1F("h_lep_n",";N^{leptons};", 101, -0.5, 100.5);
  wk()->addOutput(h_lep_n);
  h_el_n = new TH1F("h_el_n",";N^{electrons};", 101, -0.5, 100.5);
  wk()->addOutput(h_el_n);
  h_mu_n = new TH1F("h_mu_n",";N^{muons};", 101, -0.5, 100.5);
  wk()->addOutput(h_mu_n);
  h_lep_pt = new TH1F("h_lep_pt",";p_{T}^{lepton} [GeV];", 200, 0, 2000);
  wk()->addOutput(h_lep_pt);
  h_lep_eta = new TH1F("h_lep_eta",";#eta^{lepton};", 100, -5, 5);
  wk()->addOutput(h_lep_eta);
  h_lep_phi = new TH1F("h_lep_phi",";#phi^{lepton};", 64, -3.2, 3.2);
  wk()->addOutput(h_lep_phi);

  //These are TEfficiency. They can be used to measure trigger efficiencies as a functio of a known varianles
  //In this case the lepton (electron or muon) pT. 
  h_egamma_trig_eff = new TEfficiency("h_egamma_trig_eff",";p_{T}^{electron} [GeV];Trigger Acceptance;", 200, 0, 2000);
  wk()->addOutput(h_egamma_trig_eff);
  h_muon_trig_eff = new TEfficiency("h_muon_trig_eff",";p_{T}^{muon} [GeV];Trigger Acceptance;", 200, 0, 2000);
  wk()->addOutput(h_muon_trig_eff);

  h_el_pt = new TH1F("h_el_pt",";p_{T}^{electron} [GeV];", 400, 0, 2000);
  wk()->addOutput(h_el_pt);
  h_mu_pt = new TH1F("h_mu_pt",";p_{T}^{muon} [GeV];", 400, 0, 2000);
  wk()->addOutput(h_mu_pt);
  h_el_isoptcone30rel = new TH1F("h_el_isoptcone30rel",";el_isoptcone30rel;", 120, 0, 1.2);
  wk()->addOutput(h_el_isoptcone30rel);
  h_el_isoetcone20rel = new TH1F("h_el_isoetcone20rel",";el_isoetcone20rel;", 120, 0, 1.2);
  wk()->addOutput(h_el_isoetcone20rel);
  h_mu_isoptcone30rel = new TH1F("h_mu_isoptcone30rel",";mu_isoptcone30rel;", 120, 0, 1.2);
  wk()->addOutput(h_mu_isoptcone30rel);
  h_mu_isoetcone20rel = new TH1F("h_mu_isoetcone20rel",";mu_iso2tcone20rel;", 120, 0, 1.2);
  wk()->addOutput(h_mu_isoetcone20rel);

  /* h_jet_n = new TH1F("h_jet_n",";N^{jets};", 101, -0.5, 100.5);
  wk()->addOutput(h_jet_n);
  h_bjet_n = new TH1F("h_bjet_n",";N^{b-jets};", 101, -0.5, 100.5);
  wk()->addOutput(h_bjet_n);
  h_jet_pt = new TH1F("h_jet_pt",";p_{T}^{jet};", 400, 0, 2000);
  wk()->addOutput(h_jet_pt);
  h_jet_eta = new TH1F("h_jet_eta",";#eta^{jet};", 100, -5, 5);
  wk()->addOutput(h_jet_eta);
  h_jet_jvf = new TH1F("h_jet_jvf",";Jet Vertex Fraction;", 200, -1, 1);
  wk()->addOutput(h_jet_jvf);
  h_jet_mv1 = new TH1F("h_jet_mv1",";MV1 response;", 100, 0, 1);
  wk()->addOutput(h_jet_mv1);
  h_bjet_pt = new TH1F("h_bjet_pt",";p_{T}^{b-jet};", 400, 0, 2000);
  wk()->addOutput(h_bjet_pt);
  h_bjet_eta = new TH1F("h_bjet_eta",";#eta^{b-jet};", 100, -5, 5);
  wk()->addOutput(h_bjet_eta);*/
  /*
  h_met = new TH1F("h_met",";E_{T}^{miss} [GeV];", 400, 0, 2000);
  wk()->addOutput(h_met);
  h_el_met = new TH1F("h_el_met",";E_{T}^{miss} [GeV];", 400, 0, 2000);
  wk()->addOutput(h_el_met);
  h_mu_met = new TH1F("h_mu_met",";E_{T}^{miss} [GeV];", 400, 0, 2000);
  wk()->addOutput(h_mu_met);
  */
  /*
  h_ZpInvM = new TH1F("h_ZpInvM", ";m_{inv}(l,E_{T}^{miss}, 4j);", 800, 0, 4000);
  wk()->addOutput(h_ZpInvM);
  */

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ZAnalysis :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ZAnalysis :: changeInput (bool firstFile)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.

  tree = wk()->tree();
  tree->SetBranchAddress("runNumber", &runNumber);  
  tree->SetBranchAddress("eventNumber", &eventNumber);
  tree->SetBranchAddress("channelNumber", &channelNumber);
  tree->SetBranchAddress("mcWeight", &mcWeight);  
  tree->SetBranchAddress("pvxp_n", &pvxp_n);  
  tree->SetBranchAddress("vxp_z", &vxp_z); 
  tree->SetBranchAddress("scaleFactor_PILEUP", &scaleFactor_PILEUP);
  tree->SetBranchAddress("scaleFactor_ELE", &scaleFactor_ELE);  
  tree->SetBranchAddress("scaleFactor_MUON", &scaleFactor_MUON);  
  tree->SetBranchAddress("scaleFactor_BTAG", &scaleFactor_BTAG);  
  tree->SetBranchAddress("scaleFactor_TRIGGER", &scaleFactor_TRIGGER);
  tree->SetBranchAddress("scaleFactor_JVFSF", &scaleFactor_JVFSF);  
  tree->SetBranchAddress("scaleFactor_ZVERTEX", &scaleFactor_ZVERTEX);
  tree->SetBranchAddress("trigE", &trigE);
  tree->SetBranchAddress("trigM", &trigM);
  tree->SetBranchAddress("passGRL", &passGRL);
  tree->SetBranchAddress("hasGoodVertex", &hasGoodVertex);
  tree->SetBranchAddress("lep_n", &lep_n);
  tree->SetBranchAddress("lep_truthMatched", lep_truthMatched);
  tree->SetBranchAddress("lep_trigMatched", lep_trigMatched);
  tree->SetBranchAddress("lep_pt", lep_pt);
  tree->SetBranchAddress("lep_eta", lep_eta);
  tree->SetBranchAddress("lep_phi", lep_phi);
  tree->SetBranchAddress("lep_E", lep_E);
  tree->SetBranchAddress("lep_z0", lep_z0);
  tree->SetBranchAddress("lep_charge", lep_charge);
  tree->SetBranchAddress("lep_type", lep_type);
  tree->SetBranchAddress("lep_flag", lep_flag);
  tree->SetBranchAddress("lep_ptcone30", lep_ptcone30);
  tree->SetBranchAddress("lep_etcone20", lep_etcone20);
  tree->SetBranchAddress("lep_trackd0pvunbiased", lep_trackd0pvunbiased);
  tree->SetBranchAddress("lep_tracksigd0pvunbiased", lep_tracksigd0pvunbiased);
  tree->SetBranchAddress("met_et", &met_et);
  tree->SetBranchAddress("met_phi", &met_phi);
  tree->SetBranchAddress("jet_n", &jet_n); 
  tree->SetBranchAddress("alljet_n", &alljet_n);
  tree->SetBranchAddress("jet_pt", jet_pt);
  tree->SetBranchAddress("jet_eta", jet_eta);
  tree->SetBranchAddress("jet_phi", jet_phi);
  tree->SetBranchAddress("jet_E", jet_E);
  tree->SetBranchAddress("jet_m", jet_m);
  tree->SetBranchAddress("jet_jvf", jet_jvf);
  tree->SetBranchAddress("jet_trueflav", jet_trueflav);
  tree->SetBranchAddress("jet_truthMatched", jet_truthMatched);  
  tree->SetBranchAddress("jet_SV0", jet_SV0); 
  tree->SetBranchAddress("jet_MV1", jet_MV1); 

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ZAnalysis :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ZAnalysis :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  wk()->tree()->GetEntry (wk()->treeEntry());

  getLeptons();
  getJets();
  getMET();
  getEventInfo();


  // This line sets the event weight: 1 for data, and variable for MC
  // This weight depends on a number of things: MC generator weight, 
  // trigger Scale Factors, lepton identification Scale factors, etc.
  float weight = isData ? 1.0 : eventInfo.evtWeight*eventInfo.evtSF;


  // Trigger Acceptance code
  // Here we check whether the event passed the trigger
  bool pass_trigger = eventInfo.passEgamma || eventInfo.passMuon;
  if(!eventInfo.passEgamma) h_trigger_pass->Fill(0., weight);
  else h_trigger_pass->Fill(1, weight);
  if(!eventInfo.passMuon) h_trigger_pass->Fill(2, weight);
  else h_trigger_pass->Fill(3, weight);
  if(!eventInfo.passEgamma && !eventInfo.passMuon) h_trigger_pass->Fill(4, weight);
  if(eventInfo.passEgamma && eventInfo.passMuon) h_trigger_pass->Fill(5, weight);
  h_trigger_pass->Fill(6, weight);


  ////// Start Event selection
  // 0. All events
  // 1. Trigger
  // 2. Pass Good Run List
  // 3. Has good vertex
  // 4. Exactly two good leptons
  // 5. Leptons have opposite charge
  // 6. Leptons have the same flavour
  // 7. |recon ll mass - PDG Z mass| < 20 GeV
  //

  // 0. All events
  h_event_selection->Fill(0);

  // 1. Pass trigger
  if( !pass_trigger) return EL::StatusCode::SUCCESS;
  h_event_selection->Fill(1);

  // 2. Pass Good Run List selection 
  if(!eventInfo.passGRL) return EL::StatusCode::SUCCESS;
  h_event_selection->Fill(2);

  // 3. Good vertex 
  if( !eventInfo.hasGoodVertex ) return EL::StatusCode::SUCCESS;
  h_event_selection->Fill(3);

  // Selection of "good leptons"
  int n_el = 0;
  int n_mu = 0;
  std::vector<lepton> goodLeptons;
  for (unsigned int i = 0; i < leptons.size(); i++){
    lepton lep = leptons.at(i);
    
    //Kinematics of "all leptons"
    h_lep_pt->Fill(lep.tlv.Pt()*GeV, weight);
    h_lep_eta->Fill(lep.tlv.Eta(), weight);
    h_lep_phi->Fill(lep.tlv.Phi(), weight);
    if(lep.type == 11){
      n_el++;
      h_el_isoptcone30rel->Fill( lep.relptcone30,weight);
      h_el_isoetcone20rel->Fill( lep.reletcone20,weight);
    }else if(lep.type == 13){
      n_mu++; 
      h_mu_isoptcone30rel->Fill( lep.relptcone30,weight);
      h_mu_isoetcone20rel->Fill( lep.reletcone20,weight);
    }
    if(!lep.isTight) continue;
    if(lep.tlv.Pt()*GeV < 25 ) continue;
    if(lep.relptcone30 > 0.15) continue;
    if(lep.reletcone20 > 0.15) continue;

    goodLeptons.push_back(lep);
  }
  // Number of leptons in the event before lepton selection
  h_lep_n->Fill(n_el + n_mu, weight);
  h_el_n->Fill(n_el, weight);
  h_mu_n->Fill(n_mu, weight);

  // 4. Exactly two good leptons
  if(goodLeptons.size() != 2) return EL::StatusCode::SUCCESS;
  h_event_selection->Fill(4);

  // Kinematics of the good leptons
  //Lepton pT for electron and muon channels leptontype == 11 -> electrons, leptontype == 13 -> muon
  for(unsigned int i = 0; i < leptons.size(); i++){
    if(goodLeptons.at(0).type == 11 ) h_el_pt->Fill(goodLeptons.at(0).tlv.Pt()*GeV, weight);
    else if (goodLeptons.at(0).type == 13 )h_mu_pt->Fill(goodLeptons.at(0).tlv.Pt()*GeV, weight);
    if(goodLeptons.at(1).type == 11 ) h_el_pt->Fill(goodLeptons.at(1).tlv.Pt()*GeV, weight);
    else if (goodLeptons.at(1).type == 13 )h_mu_pt->Fill(goodLeptons.at(1).tlv.Pt()*GeV, weight);
  }
  /*
  // Seletion of "good jets"
  std::vector<jet> goodJets;
  for (unsigned int i = 0;i < jets.size(); i++){
    jet jet = jets.at(i);
    // kinematics of jets before "good jet selection"
    h_jet_pt->Fill(jet.tlv.Pt()*GeV, weight);
    h_jet_eta->Fill(jet.tlv.Eta(), weight);

    if(jet.tlv.Pt()*GeV < 25 ) continue;
    if( fabs(jet.tlv.Eta()) > 2.5 ) continue;
    if(jet.tlv.Pt()*GeV < 50 ){
      if(fabs(jet.tlv.Eta()) < 2.4){
	h_jet_jvf->Fill(jet.jvf, weight); // Jet vertex fraction histogram
	if(jet.jvf >= 0.5) goodJets.push_back(jet);
      }
    }
    else goodJets.push_back(jet);
  }
  // number of good jets in the event
  h_jet_n->Fill(goodJets.size(), weight);*/

for(unsigned int i = 0;i < goodLeptons.size(); i++){
  h_lep_charge->Fill(goodLeptons.at(i).charge);
 }

  // 5. Leptons have opposite charge
if(goodLeptons.at(0).charge * goodLeptons.at(1).charge > 0) return EL::StatusCode::SUCCESS;
  h_event_selection->Fill(5);

  /*
  // Selection of b-jets
  std::vector<jet> bJets;
  bool isBjetTopFour = false;
  for (unsigned int i = 0;i < goodJets.size(); i++){
    jet bjet= goodJets.at(i);
    h_jet_mv1->Fill(bjet.mv1, weight);
    if(bjet.mv1 >= 0.7892) bJets.push_back(bjet);
    if(i < 4) isBjetTopFour = true;
    }
  // number of b-jets in the event
  h_jet_n->Fill(goodJets.size(), weight);
  */

for(unsigned int i = 0; i < goodLeptons.size(); i++){
  h_lep_type->Fill(goodLeptons.at(i).type);
 }
  // 6. Leptons have the same flavour
 if(abs(goodLeptons.at(0).type) != abs(goodLeptons.at(1).type)) return EL::StatusCode::SUCCESS;
  h_event_selection->Fill(6);
  
  // Missing transverse energy in the event
  //h_met->Fill(MET.tlv.Et()*GeV, weight);

h_recon_llmass->Fill((goodLeptons.at(0).tlv+goodLeptons.at(1).tlv).M()*GeV,weight);
  // 7. Reconstructed mass of leptons pair - PDG mass Z < 20 GeV
 if(abs((goodLeptons.at(0).tlv+goodLeptons.at(1).tlv).M()*GeV - 91.1876) > 20) return EL::StatusCode::SUCCESS;
  h_event_selection->Fill(7);

/*
  double ZpInvM = -999;
  if( isBjetTopFour ) {
    ZpInvM = (goodLeptons.at(0).tlv + 
	      goodJets.at(0).tlv + goodJets.at(1).tlv + goodJets.at(2).tlv + goodJets.at(3).tlv +
	      MET.tlv).M();
    h_ZpInvM->Fill(ZpInvM*GeV, weight);
  } 
*/

for(unsigned int i = 0; i < goodLeptons.size(); i++){
  h_lep_pt_f->Fill(goodLeptons.at(i).tlv.Pt()*GeV, weight);
  h_lep_phi_f->Fill(goodLeptons.at(i).tlv.Phi(), weight);
  h_lep_eta_f->Fill(goodLeptons.at(i).tlv.Eta(), weight);
 }

h_recon_Zmass->Fill((goodLeptons.at(0).tlv+goodLeptons.at(1).tlv).M()*GeV,weight);


  return EL::StatusCode::SUCCESS;
}




EL::StatusCode ZAnalysis :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ZAnalysis :: finalize ()
{
  // This method is the mirror image of initialize(), meaning it gets
  // called after the last event has been processed on the worker node
  // and allows you to finish up any objects you created in
  // initialize() before they are written to disk.  This is actually
  // fairly rare, since this happens separately for each worker node.
  // Most of the time you want to do your post-processing on the
  // submission node after all your histogram outputs have been
  // merged.  This is different from histFinalize() in that it only
  // gets called on worker nodes that processed input events.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ZAnalysis :: histFinalize ()
{
  // This method is the mirror image of histInitialize(), meaning it
  // gets called after the last event has been processed on the worker
  // node and allows you to finish up any objects you created in
  // histInitialize() before they are written to disk.  This is
  // actually fairly rare, since this happens separately for each
  // worker node.  Most of the time you want to do your
  // post-processing on the submission node after all your histogram
  // outputs have been merged.  This is different from finalize() in
  // that it gets called on all worker nodes regardless of whether
  // they processed input events.
  return EL::StatusCode::SUCCESS;
}

void ZAnalysis :: getLeptons()
{

  leptons.clear();
  for(unsigned int i = 0; i < lep_n; i++){
    lepton lep;
    lep.tlv.SetPtEtaPhiE( lep_pt[i], lep_eta[i], lep_phi[i], lep_E[i]);
    lep.type = lep_type[i];
    lep.isTight =  lep_flag[i] & 512 ? true : false;
    lep.relptcone30 = lep_ptcone30[i]/lep_pt[i];
    lep.reletcone20 = lep_etcone20[i]/lep_pt[i];
    leptons.push_back(lep);
    lep.charge = lep_charge[i];
  }

  return;
}

void ZAnalysis :: getJets()
{

  jets.clear();
  for(unsigned int i = 0; i < alljet_n; i++){
    jet jet;
    jet.tlv.SetPtEtaPhiM( jet_pt[i], jet_eta[i], jet_phi[i], jet_m[i]);
    jet.jvf = jet_jvf[i];
    jet.flavor =  jet_trueflav[i];
    jet.mv1 = jet_MV1[i];
    jets.push_back(jet);
  }

  return;
}

void ZAnalysis :: getMET()
{

  MET.tlv.SetPtEtaPhiE(met_et, 0, met_phi, met_et);

  return;
}

void ZAnalysis :: getEventInfo()
{
  eventInfo.nvertices = pvxp_n;
  eventInfo.passEgamma = trigE;
  eventInfo.passMuon = trigM;
  eventInfo.passGRL = passGRL;
  eventInfo.hasGoodVertex = hasGoodVertex;
  eventInfo.evtWeight = mcWeight*scaleFactor_PILEUP*scaleFactor_ZVERTEX;
  eventInfo.evtSF = scaleFactor_ELE*scaleFactor_MUON*scaleFactor_TRIGGER;

  return;
}

void ZAnalysis :: setData(bool data)
{
  isData = data;

  return;
}
