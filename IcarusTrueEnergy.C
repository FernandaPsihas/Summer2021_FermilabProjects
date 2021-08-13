// Make a plot with cuts
#include "sbnana/CAFAna/Core/Binning.h"
#include "sbnana/CAFAna/Core/Spectrum.h"
#include "sbnana/CAFAna/Core/SpectrumLoader.h"

using namespace ana;

#include "sbnana/CAFAna/StandardRecord/Proxy/SRProxy.h"

#include "TCanvas.h"
#include "TH1.h"
#include "TLegend.h"

// ---- VARS -----
// A Var returns a number per slice, a.k.a. variables to plot
const Var kNTrk = SIMPLEVAR(reco.ntrk);
//const Var kITrueE_simple = SIMPLEVAR(truth.E);

 const Var kITrueE([](const caf::SRSliceProxy* slc) -> float {
     float E=0; 
     if (slc->truth.E > 0) 
       E = slc->truth.E;
     return E;  
 });

// This is shorthand for the following:

// const Var kNTrk([](const caf::SRSliceProxy* slc) -> int {
//   return slc->reco.ntrk;
// });

const Var kLongestTrkLen([](const caf::SRSliceProxy* slc) -> float {
  float len(-5.f);
  for (auto const& trk : slc->reco.trk) {
    if (trk.len > len)
      len = trk.len;
  }
  return len;
});

const Var kSumTrkLen([](const caf::SRSliceProxy* slc) -> float {
  float sum(0.f);
  for (auto const& trk : slc->reco.trk) {
    sum += trk.len; 
  }
  return sum;
});

// ---- CUTS -----
// A Cut returns a boolean per slice
const Cut kNTrkCut([](const caf::SRSliceProxy* slc) {
  return slc->reco.ntrk > 0;
});

// Can also Cut on Vars
const Cut kTrkLenCut = kLongestTrkLen > 50.f;
const Cut kITrueECut = kITrueE > 0.f;
const Cut KITrackCut = kITrueE > 0.f && kLongestTrkLen > 50.f;


// This is shorthand for the following:

// const Cut kTrkLenCut([](const caf::SRSliceProxy* slc) {
//   return kLongestTrkLen(slc) > 50.f;
// });

void IcarusTrueEnergy(const std::string inputName = "/pnfs/sbn/persistent/analysis/CAF/202106PAC/workshop_SBNWorkshop0421_prodoverlay_corsika_cosmics_cosmics_proton_genie_nu_spill_gsimple-config_caf_icarus/reduced_flatcafs/*.root")
{
  // Environment variables and wildcards work as arguments to
  // SpectrumLoader. As do SAM datasets.
  // Source of events
  SpectrumLoader loader(inputName);

  // ---- SPECTRA -----
  // A spectrum is a histogram with associated POT information
  const Binning binsN = Binning::Simple(10, 0, 10);
  const Binning binsLen = Binning::Simple(40, 0, 200);
  const Binning binsE = Binning::Simple(40, 0, 7);
  const Binning binsTrksum = Binning::Simple(40, 0, 200);


  // HistAxis(Title, Binning, Var)
  const HistAxis axNTrk("Number of Trks", binsN, kNTrk);
  const HistAxis axTrkLen("Len of Longest Track", binsLen, kLongestTrkLen);
  const HistAxis axSliceE("[GeV] True Energy", binsE, kITrueE);
  const HistAxis axTrkEnergy("Sum of Tracks ", binsTrksum, kSumTrkLen);
  //const HistAxis axSliceE_simple("True Energy Simple ", binsE, kTrueE_simple);

  // Spectrum(Spectrumloader, HistAxis, Cut)
  Spectrum sNTrks(loader, axNTrk, kNoCut);
  Spectrum sSliceEn(loader, axSliceE, kITrueECut);
  Spectrum sSliceEn_simple(loader, axSliceE, KITrackCut);
  Spectrum sTrkLenAll(loader, axTrkLen, kNoCut);
  Spectrum sTrkLenCut(loader, axTrkLen, kTrkLenCut);
  Spectrum sSumTrk(loader, axTrkEnergy, kNoCut);
  // This is the call that actually fills in the spectrum
  loader.Go();

  // ---- DRAW -----
  // For plotting purposes we can convert spectra to a TH1
  TCanvas* c1 = new TCanvas("c1", "c1");
  TH1* hNTrks = sNTrks.ToTH1(6.6e20);
  hNTrks->Draw("hist");
  c1->Print("icarusplots/Icarus_nTrks.png");


  // Let's now plot both with/without the cuts
  TCanvas* c2 = new TCanvas("c2", "c2");
  TH1* hTrkLenAll = sTrkLenAll.ToTH1(6.6e20);
  TH1* hTrkLenCut = sTrkLenCut.ToTH1(6.6e20);
  hTrkLenAll->Draw("hist");
  hTrkLenCut->SetLineColor(kBlue + 2);
  hTrkLenCut->Draw("hist same");

  //Drawing a legend because we are not hobos
  TLegend* leg = new TLegend(0.6, 0.7, 0.85, 0.8);
  leg->AddEntry(hTrkLenAll, "All", "l");
  leg->AddEntry(hTrkLenCut, "Len > 50 ", "l");
  leg->Draw();
  c2->Print("icarusplots/Icarus_trkLen.png");

  //Slice Energy plot
  TCanvas* c3 = new TCanvas("c3", "c3");
  TH1* hE = sSliceEn.ToTH1(6.6e20);
  hE->SetLineColor(kOrange + 10);
  hE->Draw("hist");
  TH1* hEsimple = sSliceEn_simple.ToTH1(6.6e20);
  hEsimple->Draw("hist same");

  //Legends for Slice Energy Plots
  TLegend* legend = new TLegend(0.6, 0.7, 0.85, 0.8);
  legend->AddEntry(hEsimple, "Energy w/ Track Lengths >50", "l");
  legend->AddEntry(hE, "Energy w/ all Track Lengths", "l");
  legend->Draw();
  c3->Print("icarusplots/Icarus_sliceE.png");

  //Sum Track Length Plot
  TCanvas* c4 = new TCanvas("c4", "c4");
  TH1* hSumTrks = sSumTrk.ToTH1(6.6e20);
  hSumTrks->Draw("hist");
  c4->Print("icarusplots/SumofTracks.png");


}
