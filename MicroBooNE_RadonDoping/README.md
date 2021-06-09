#How to Set Up Your Workspace

### Setting up the Environment
Run these commands when you plan to use root, or other programs.
- cd /wherever_you_are_working/
- source /cvmfs/uboone.opensciencegrid.org/products/setup_uboone.sh
- setup uboonecode v08_00_00_41 -q e17:prof

### Directories Here
There are several directories here with their own purposes:
- elise_firstplots: This is Elise's directory where she began working on the Beta energy spectrum. There are a few tries to fit in here, 
but they did not work. This is also where Elise will work on other tasks. You will have a similar directory.
- jzennamo_testfits: This is where we actually did the beta spectrum fit. Joseph was able to write a code that base the base for fitting the
beta spectrum. It will be edited in the future as we learn more about the data itself.
- jzennamo_testreco: Code in here is for testing the energy reconstruction.

### Some Fitting Practice
Here is a fitting task using an already made C++ file:
- cd /your_directory/
- cp /uboone/app/users/jaz8600/work/FromWhitAndIvan/WorkingArea/Analysis/CheckBetaAlphaEnergies.C .
- root CheckBetaAlphaEnergies.C (root -l allows you to open root without the welcome screen)
-You should get two plots and you will want to fit the Bi214 plot
-Use the following resources to fit the Bi214 plot with a "pol6" function: [TF1 Class Info](https://root.cern.ch/doc/master/classTF1.html)  or [ROOT Guide Fitting 
Histograms](https://root.cern.ch/root/htmldoc/guides/users-guide/FittingHistograms.html) 








