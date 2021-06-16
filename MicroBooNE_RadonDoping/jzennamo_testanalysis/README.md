To be able to analyze these TTree files I do the following:

open the file with root:

> root ../jzennamo_testreco/outfile_mc_TEST.root

this loads the file 

then one can do this:

root [1] .ls
TFile**		../jzennamo_testreco/outfile_mc_TEST.root	
 TFile*								../jzennamo_testreco/outfile_mc_TEST.root	
  KEY: TTree							betaTree;1					ForEachTruthBeta
root [2] betaTree->MakeClass()

This creates in your directory two new files "betaTree.h" and "betaTree.C"

Your analysis code can then be written in "betaTree.C"

To run this analysis code you'll then do the following:

~> root 
root[0] .L betaTree.C+
root[1] betaTree t;
root[2] t.Loop();

This executes the loop through the TTree you created and will then do your analysis and make your plots!