HistStack contains the histogram stacks of the kinematic variables as well as the cutflow stack. I included the invariant Z mass plotted with log y axis and without since I thought both were interesting. The event selection reduces the Drell-Yan background greatly.

Analysis Project Q's:
1. Identify the final state of your signal.
    - How many Leptons do you expect?

    My signal is the decay of the Z boson into lepton anti-lepton pairs. Thus we expect the final state of our signal to be two leptons of opposite charge.
    - How many jets?

    There are no jets in our signal. 
    - Do you expect neutrinos or other sources of missing transverse energy?

    The Z boson could decay into two neutrinos, however, if this was the case we would not observe it since the neutrinos would go right through the detector, only being noticeable as missing transverse energy. Given our signal of a Z boson decaying to two leptons, we do not expect any missing transverse energy in our signal.  For one thing, our signal is a Z boson decaying into a lepton anti-lepton pair of the same flavour. Lepton number is conserved therein since an electron and positron or muon and anti-muon will conserve the respective lepton numbers in the reaction (0 before, 0 after).
2. What other Standard Model Processes share the same final state?
    - List the background processes of your analysis.
        
    The background processes of our analysis are the Drell-Yan production, Diboson, W+jets, Single top and ttbar processes.

3. Which samples from the list of datasets would you use. List them all and explain why.

    In our analysis we would use Zee or Zmumu since these data sets invovle Z bosons decaying into charged leptons. We would not use Ztautau, since DataEgamma.root only contains data from photon and electron triggers and Datamuons.root only contains data with muon triggers.
    

Fist time setup
```
mkdir YOURTOPDIR
cd YOURTOPDIR
setupATLAS
lsetup git
rcsetup Base,2.4.28
mkdir run
git clone ssh://git@gitlab.cern.ch:7999/rrobinso/Csu-Analysis-Project-Z-Analysis.git ZAnalysis
rc build
```

On every login
```
cd YOURTOPDIR	
setupATLAS
lsetup git
rcsetup Base,2.4.28
```

After making modifications to the code
```
cd YOURTOPDIR
rc build
```
or
```
rc compile_pkg ZAnalysis
```

To run the analysis code
```
cd YOURTOPDIR/run
RunAnalysis OUTPUTDIR
```
OUTPUTDIR has to be unique so that you don't write over existing data.

If the RunAnalysis command doesn't appear, type
```
rc build
```
from YOURTOPDIR


To run the plotting macro HistPlotter.C you need to copy all the output data over to the Plotting directory
```
cd YOURTOPDIR/run
RunAnalysis OUTPUTDIR
cd ../ZAnalysis/Plotting
cp ../../run/OUTPUTDIR_*/hist-*.root .
```
Modify the histogram name string and all the other configurables, then run
```
root -l -q -b HistPlotter.C
```
a pdf file named HISTOGRAMNAME.pdf or HISTOGRAMNAME_log.pdf should appear now in YOURTOPDIR/ZAnalysis/Plotting.
