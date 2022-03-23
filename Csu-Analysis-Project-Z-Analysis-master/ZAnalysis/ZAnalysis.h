#ifndef ZAnalysis_ZAnalysis_H
#define ZAnalysis_ZAnalysis_H

#include <EventLoop/Algorithm.h>
#include <TTree.h>
#include <TH1.h>
#include <TEfficiency.h>
#include <TLorentzVector.h>

#define GeV 0.001

class ZAnalysis : public EL::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:
  // float cutValue;

  bool isData;

  struct lepton{
    TLorentzVector tlv;
    int type;
    bool isTight;
    float relptcone30;
    float reletcone20;
    int charge;
  };

  struct jet{
    TLorentzVector tlv;
    float jvf;
    float mv1;
    int flavor;
  };


  struct met{
    TLorentzVector tlv;
  };

  struct event{
    int nvertices;
    bool passEgamma;
    bool passMuon;
    bool passGRL;
    bool hasGoodVertex;
    float evtWeight;
    float evtSF;
  };

  std::vector<lepton> leptons;
  std::vector<lepton> electrons;
  std::vector<lepton> muons;
  std::vector<jet> jets;
  met MET;
  event eventInfo;

  int allEvents = 0;
  int passTrigger = 1;
  int passGrl = 2;
  int passGoodVertex = 3;
  int passOneGoodLepton = 4;
  int passFourGoodJets = 5;
  int passOneBjet = 6;
  int passMet30 = 7;
  int passWmt30 = 8;

  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)
public:
  // Tree *myTree; //!
  // TH1 *myHist; //!

  TTree *tree; //!

  // Branches
  Int_t           runNumber;
  Int_t           eventNumber;
  Int_t           channelNumber;
  Float_t         mcWeight;
  Int_t           pvxp_n;
  Float_t         vxp_z;
  Float_t         scaleFactor_PILEUP;
  Float_t         scaleFactor_ELE;
  Float_t         scaleFactor_MUON;
  Float_t         scaleFactor_BTAG;
  Float_t         scaleFactor_TRIGGER;
  Float_t         scaleFactor_JVFSF;
  Float_t         scaleFactor_ZVERTEX;
  Bool_t          trigE;
  Bool_t          trigM;
  Bool_t          passGRL;
  Bool_t          hasGoodVertex;
  UInt_t          lep_n;
  Bool_t          lep_truthMatched[10];   //!
  UShort_t        lep_trigMatched[10];   //!
  Float_t         lep_pt[10];   //!
  Float_t         lep_eta[10];   //!
  Float_t         lep_phi[10];   //!
  Float_t         lep_E[10];   //!
  Float_t         lep_z0[10];   //!
  Float_t         lep_charge[10];   //!
  UInt_t          lep_type[10];   //!
  UInt_t          lep_flag[10];   //!
  Float_t         lep_ptcone30[10];   //!
  Float_t         lep_etcone20[10];   //!
  Float_t         lep_trackd0pvunbiased[10];   //!
  Float_t         lep_tracksigd0pvunbiased[10];   //!
  Float_t         met_et;
  Float_t         met_phi;
  UInt_t          jet_n;
  UInt_t          alljet_n;
  Float_t         jet_pt[30];   //!
  Float_t         jet_eta[30];   //!
  Float_t         jet_phi[30];   //!
  Float_t         jet_E[30];   //!
  Float_t         jet_m[30];   //!
  Float_t         jet_jvf[30];   //!
  Int_t           jet_trueflav[30];   //!
  Int_t           jet_truthMatched[30];   //!
  Float_t         jet_SV0[30];   //!
  Float_t         jet_MV1[30];   //!
  

  // histograms
  TH1F *h_trigger_pass; //!
  TH1F *h_event_selection; //!

  TH1F *h_lep_charge; //!
  TH1F *h_lep_type; //!
  TH1F *h_recon_llmass; //!
  TH1F *h_lep_pt_f; //!
  TH1F *h_lep_phi_f; //!
  TH1F *h_lep_eta_f; //!
  TH1F *h_lep_n_f; //!

  TH1F *h_recon_Zmass; //!

  TH1F *h_lep_n; //!
  TH1F *h_el_n; //!
  TH1F *h_mu_n; //!
  TH1F *h_lep_pt; //!
  TH1F *h_lep_eta; //!
  TH1F *h_lep_phi; //!

  TH1F *h_el_pt; //!
  TH1F *h_mu_pt; //!
  TH1F *h_el_isoptcone30rel; //!
  TH1F *h_el_isoetcone20rel; //!
  TH1F *h_mu_isoptcone30rel; //!
  TH1F *h_mu_isoetcone20rel; //!

  TH1F *h_bjet_pt; //!
  TH1F *h_bjet_eta; //!
  TH1F *h_jet_n; //!
  TH1F *h_bjet_n; //!
  TH1F *h_jet_pt; //!
  TH1F *h_jet_eta; //!
  TH1F *h_jet_jvf; //!
  TH1F *h_jet_mv1; //!

  TH1F *h_met; //!
  TH1F *h_el_met; //!
  TH1F *h_mu_met; //!
  TH1F *h_ZpInvM; //!


  TEfficiency *h_egamma_trig_eff; //!
  TEfficiency *h_muon_trig_eff; //!


  // this is a standard constructor
  ZAnalysis ();

  // these are the functions inherited from Algorithm
  virtual EL::StatusCode setupJob (EL::Job& job);
  virtual EL::StatusCode fileExecute ();
  virtual EL::StatusCode histInitialize ();
  virtual EL::StatusCode changeInput (bool firstFile);
  virtual EL::StatusCode initialize ();
  virtual EL::StatusCode execute ();
  virtual EL::StatusCode postExecute ();
  virtual EL::StatusCode finalize ();
  virtual EL::StatusCode histFinalize ();

  void getLeptons();
  void getJets();
  void getMET();
  void getEventInfo();

  void setData(bool);
  // this is needed to distribute the algorithm to the workers
  ClassDef(ZAnalysis, 1);
};

#endif
