// Make a plot with cuts
#include "sbnana/CAFAna/Core/Binning.h"
#include "sbnana/CAFAna/Core/Spectrum.h"
#include "sbnana/CAFAna/Core/SpectrumLoader.h"
#include "TStyle.h"

using namespace ana;

#include "sbnana/CAFAna/StandardRecord/Proxy/SRProxy.h"

#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TLegend.h"

// ---- VARS -----
 const Var kSBNDTrueE([](const caf::SRSliceProxy* slc) -> float {
    float E=0.; 
     if (slc->truth.E > 0)
       E = slc->truth.E;
     return E;
});

const Var kSumTrackE([](const caf::SRSliceProxy* slc) -> float {
  float sum(0.f);
  for (auto const& trk : slc->reco.trk) {

    sum += trk.calo2.ke; 
  }
  return sum;
});

const Var kSumShowerE([](const caf::SRSliceProxy* slc) -> float {
  float sum(0.f);
  for (auto const& shw : slc->reco.shw) {
    sum += shw.energy_plane2; 
  }
  return sum;
});

const Var kTotalSum = kSumTrackE + kSumShowerE; // Using KTotalSum for this 2D plot



// Define some cuts based on the Vars
const Cut kSBNDTrueECut = kSBNDTrueE> 0.f; // selecting only energy values that are greater than 0



// We can then combine the cuts together
// This can be used to combine many cuts together
//const Cut kNTrkShwCut = kNTrkCut && kNShwCut;

// Alternatively we could use or logic
// const Cut = kNTrkCut || kNShwCut;

void TotalEnergyvsTrckShwrSum_2D(const std::string inputName = "/pnfs/sbnd/persistent/sbndpro/mcp/mc/workshop/SBNWorkshop0421/prodoverlay_corsika_cosmics_proton_genie_nu_spill_gsimple-configf-v1_tpc/v09_19_00_01/caf/flat_caf_*")
{
  // Environment variables and wildcards work as arguments to
  // SpectrumLoader. As do SAM datasets.
  // Source of events
  SpectrumLoader loader(inputName);

  // ---- SPECTRA -----
  // A spectrum is a histogram with associated POT information
  const Binning binsE = Binning::Simple(20, 0, 8);
  const Binning binsN = Binning::Simple(40, 0, 2000);

  //HistAxis( <Title>,Binning,Var)
  const HistAxis axTrksandShws("Sum of Tracks and Showers", binsN, kTotalSum);
  const HistAxis axTotalTrueE("SBND True Energy", binsE, kSBNDTrueE);

  //Spectrum(Spectrumloader,HistAxis,Cut)
  Spectrum sTrksandShws(loader, axTrksandShws, kNoCut);
  Spectrum sTotalTrueE(loader, axTotalTrueE, kSBNDTrueECut);

  //Spectrum(Spectrumloader ,HistAxisX HistAxisY, SpillCut, Cut)
  Spectrum sTrkShwvsTrueE(loader, axTrksandShws, axTotalTrueE, kNoSpillCut, kSBNDTrueECut);

  // This is the call that actually fills in the spectrum
  loader.Go();

  // ---- DRAW -----
  // For plotting purposes we can convert spectra to a TH1
  TCanvas* c1 = new TCanvas("c1", "c1");
  TH1* hTrksandShws = sTrksandShws.ToTH1(6.6e20);
  TH1* hTotalTrueE = sTotalTrueE.ToTH1(6.6e20);
  hTrksandShws->Draw("hist");
  hTotalTrueE->SetLineColor(kBlue + 2);
  hTotalTrueE->Draw("hist same");

  //Drawing a legend because we are not hobos
  TLegend* leg = new TLegend(0.6, 0.7, 0.85, 0.8);
  leg->AddEntry(hTrksandShws, "Sum of SBND Tracks and Showers", "l");
  leg->AddEntry(hTotalTrueE, "SBND True Energy", "l");
  leg->Draw();
  c1->Print("sbnd_icarus_plots/sbndplots/Trk&ShwSumvsTrueE.png");

  // Create a 2D plot
  TCanvas* c2 = new TCanvas("c2", "c2");
  TH2* hTrkShwvsTrueE = sTrkShwvsTrueE.ToTH2(6.6e20);
  gStyle->SetPalette(57);
  hTrkShwvsTrueE->SetXTitle("Sum of Showers & Calorimetric Tracks [MeV]");
  hTrkShwvsTrueE->SetYTitle("SBND True Energy [GeV]");
  hTrkShwvsTrueE->Draw("colz");
  c2->Print("sbnd_icarus_plots/sbndplots/Trk&ShwSumvsTrueE_2D.png");

}