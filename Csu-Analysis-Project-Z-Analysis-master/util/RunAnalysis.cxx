#include "xAODRootAccess/Init.h"
#include "SampleHandler/SampleHandler.h"
#include "SampleHandler/ScanDir.h"
#include "SampleHandler/ToolsDiscovery.h"
#include "EventLoop/Job.h"
#include "EventLoop/DirectDriver.h"
#include "SampleHandler/DiskListLocal.h"
#include <TSystem.h>


#include "ZAnalysis/ZAnalysis.h"

int main( int argc, char* argv[] ) {

  // Take the submit directory from the input if provided:
  std::string submitDir = "submitDir";
  std::string baseDir = "";
  //std::string sample = "submitDir";
  //if( argc > 1 ) submitDir = argv[ 1 ];
  if( argc > 1 ) baseDir = argv[ 1 ];

  // Set up the job for xAOD access:
  xAOD::Init().ignore();

  //Loop over all the samples
  std::vector<std::string> samples;
  //Zprime
  samples.push_back("ZPrime400");  samples.push_back("ZPrime500");  samples.push_back("ZPrime750");  samples.push_back("ZPrime1000");
  samples.push_back("ZPrime1250");  samples.push_back("ZPrime1500");  samples.push_back("ZPrime1750");  samples.push_back("ZPrime2000");
  samples.push_back("ZPrime2250");  samples.push_back("ZPrime2500");  samples.push_back("ZPrime3000");
  //Diboson
  samples.push_back("WW"); samples.push_back("WZ");samples.push_back("ZZ");
  //Drell-Yan
  samples.push_back("DYeeM08to15"); samples.push_back("DYeeM15to40"); samples.push_back("DYmumuM08to15"); 
  samples.push_back("DYmumuM15to40"); samples.push_back("DYtautauM08to15"); samples.push_back("DYtautauM15to40");  
  //W
  samples.push_back("WenuJetsBVeto"); samples.push_back("WenuWithB"); samples.push_back("WenuNoJetsBVeto"); samples.push_back("WmunuJetsBVeto"); samples.push_back("WmunuWithB"); 
  samples.push_back("WmunuNoJetsBVeto"); samples.push_back("WtaunuJetsBVeto");samples.push_back("WtaunuWithB"); samples.push_back("WtaunuNoJetsBVeto");   
  //Z
  samples.push_back("Zee"); samples.push_back("Zmumu");samples.push_back("Ztautau");
  //single top
  samples.push_back("stop_tchan_top"); samples.push_back("stop_tchan_antitop");samples.push_back("stop_schan"); samples.push_back("stop_wtchan");
  // ttbar
  samples.push_back("ttbar_lep"); samples.push_back("ttbar_had");
  // data
  samples.push_back("DataMuons"); samples.push_back("DataEgamma");

  for (unsigned int i = 0; i < samples.size(); i++){
    // Construct the samples to run on:
    SH::SampleHandler sh;
    
    submitDir = (baseDir+"_"+samples.at(i)).c_str();    

    // use SampleHandler to scan all of the subdirectories of a directory for particular MC single file:
    //const char* inputFilePath = gSystem->ExpandPathName ("/eos/atlas/user/p/palacino/ATLAS_OpenData/MC/ZPrime2500");
    //const char* inputFilePath = gSystem->ExpandPathName ("/eos/atlas/user/p/palacino/ATLAS_OpenData/MC/ttbar_lep");
    //const char* inputFilePath = gSystem->ExpandPathName ("/eos/atlas/user/p/palacino/ATLAS_OpenData/MC/WW");
    //const char* inputFilePath = gSystem->ExpandPathName ("/eos/atlas/user/p/palacino/ATLAS_OpenData/MC/DYeeM15to40");
    //const char* inputFilePath = gSystem->ExpandPathName (("/eos/atlas/user/p/palacino/ATLAS_OpenData/MC/"+samples.at(i)).c_str());
    //const char* inputFilePath = gSystem->ExpandPathName (("/eos/atlas/user/p/palacino/ATLAS_OpenData/Data/"+samples.at(i)).c_str());
    const char* inputFilePath;
    if(samples.at(i).find("Data") == std::string::npos) inputFilePath = gSystem->ExpandPathName (("/eos/atlas/user/p/palacino/ATLAS_OpenData/MC/"+samples.at(i)).c_str());
    else inputFilePath = gSystem->ExpandPathName (("/eos/atlas/user/p/palacino/ATLAS_OpenData/Data/"+samples.at(i)).c_str());
    SH::ScanDir().filePattern("*root*").scan(sh,inputFilePath);
    
    
    // Set the name of the input TTree.
    sh.setMetaString( "nc_tree", "mini" );
    
    // Print what we found:
    sh.print();
    
    // Create an EventLoop job:
    EL::Job job;
    job.sampleHandler( sh );
    job.options()->setDouble (EL::Job::optMaxEvents, -1);
    
    // Add our analysis to the job:
    ZAnalysis* alg = new ZAnalysis();
    if(samples.at(i).find("Data") == std::string::npos)  alg->setData(false);
    else alg->setData(true);
     
    job.algsAdd( alg );

    
    // Run the job using the local/direct driver:
    EL::DirectDriver driver;
    driver.submit( job, submitDir );
    

  }
  return 0;
}
