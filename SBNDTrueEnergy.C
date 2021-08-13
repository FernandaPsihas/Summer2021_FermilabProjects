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
const Var kSBNDTrueE_simple = SIMPLEVAR(truth.E);

 const Var kSBNDTrueE([](const caf::SRSliceProxy* slc) -> float {
    float E=0.; 
     if (slc->truth.E > 0)
       E = slc->truth.E;
     return E;


     //elseif (slc->truth.E = nan)
     //continue;
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



// ---- CUTS -----
// A Cut returns a boolean per slice
const Cut kNTrkCut([](const caf::SRSliceProxy* slc) {
  return slc->reco.ntrk > 0;
});

// Can also Cut on Vars
const Cut kTrkLenCut = kLongestTrkLen > 50.f;
const Cut kSBNDTrueECut = kSBNDTrueE> 0.f;
const Cut KSBNDTrackCut = kSBNDTrueE> 0.f && kLongestTrkLen > 50.f;


// This is shorthand for the following:

// const Cut kTrkLenCut([](const caf::SRSliceProxy* slc) {
//   return kLongestTrkLen(slc) > 50.f;
// });

void SBNDTrueEnergy(const std::string inputName = "/pnfs/sbnd/persistent/sbndpro/mcp/mc/workshop/SBNWorkshop0421/prodoverlay_corsika_cosmics_proton_genie_nu_spill_gsimple-configf-v1_tpc/v09_19_00_01/caf/flat_caf_*")
{
  // Environment variables and wildcards work as arguments to
  // SpectrumLoader. As do SAM datasets.
  // Source of events
  SpectrumLoader loader(inputName);

  // ---- SPECTRA -----
  // A spectrum is a histogram with associated POT information
  const Binning binsN = Binning::Simple(10, 0, 10);
  const Binning binsLen = Binning::Simple(40, 0, 200);
  const Binning binsE = Binning::Simple(40, 0, 6.5);
  

  // HistAxis(Title, Binning, Var)
  const HistAxis axNTrk("Number of Trks", binsN, kNTrk);
  const HistAxis axTrkLen("Len of Longest Track", binsLen, kLongestTrkLen);
  const HistAxis axSliceE("[GeV] True Energy ", binsE, kSBNDTrueE);
  //const HistAxis axSliceE_simple("True Energy Simple ", binsE, kSBNDTrueE_simple); This was practice code--can disregard

  // Spectrum(Spectrumloader, HistAxis, Cut)
  Spectrum sNTrks(loader, axNTrk, kNoCut);
  Spectrum sSliceEntotal(loader, axSliceE, kSBNDTrueECut);// plot with no cuts; only energy >0
  Spectrum sSliceEn(loader, axSliceE,KSBNDTrackCut);  
  Spectrum sTrkLenAll(loader, axTrkLen, kNoCut);
  Spectrum sTrkLenCut(loader, axTrkLen, kTrkLenCut);


  // This is the call that actually fills in the spectrum
  loader.Go();

  // ---- DRAW -----
  // For plotting purposes we can convert spectra to a TH1
  TCanvas* c1 = new TCanvas("c1", "c1");
  TH1* hNTrks = sNTrks.ToTH1(6.6e20);
  hNTrks->Draw("hist");
  c1->Print("sbndplots/SBNDnTrks.png");


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
  c2->Print("sbndplots/SBNDtrkLen.png");

/////////////////////////////////////////////////////

  //Slice Energy plots
  TCanvas* c3 = new TCanvas("c3", "c3");
  TH1* hEtotal = sSliceEntotal.ToTH1(6.6e20);
  hEtotal->SetLineColor(kOrange + 10);
  //hEtotal->SetFillColor(kOrange + 10);
  hEtotal->Draw("hist");
  TH1* hE = sSliceEn.ToTH1(6.6e20);
  hE->Draw("hist same");
  
  //Legend for slice energy plots
  TLegend* legend = new TLegend(0.6, 0.7, 0.85, 0.8);
  legend->SetTextFont();
  legend->AddEntry(hE, "Energy w/ Track Lengths >50", "l");
  legend->AddEntry(hEtotal, "Energy w/ all Track Lengths", "l");
  legend->Draw();
  c3->Print("sbndplots/SBNDSliceEnergy.pdf");

}
