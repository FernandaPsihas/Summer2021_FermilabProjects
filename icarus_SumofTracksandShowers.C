// Make a plot with cuts
#include "sbnana/CAFAna/Core/Binning.h"
#include "sbnana/CAFAna/Core/Spectrum.h"
#include "sbnana/CAFAna/Core/SpectrumLoader.h"

using namespace ana;

#include "sbnana/CAFAna/StandardRecord/Proxy/SRProxy.h"
//#include "sbnanaobj/StandardRecord/SRTrackCalo.h"

#include "TCanvas.h"
#include "TH1.h"
#include "TLegend.h"

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

const Var kTotalSum = kSumTrackE + kSumShowerE;

void icarus_SumofTracksandShowers(const std::string inputName = "/pnfs/sbn/persistent/analysis/CAF/202106PAC/workshop_SBNWorkshop0421_prodoverlay_corsika_cosmics_cosmics_proton_genie_nu_spill_gsimple-config_caf_icarus/reduced_flatcafs/*.root")
//change path file to all files in icarus && change to look for calo2t rk energy (see Tbrwoser)
{

  SpectrumLoader loader(inputName);

  // ---- SPECTRA -----
  // A spectrum is a histogram with associated POT information
  const Binning binsTrksum = Binning::Simple(70, 0, 1700);

  // HistAxis(Title, Binning, Var)
   const HistAxis axTotEnergy("Sum of Tracks and Showers[MeV]", binsTrksum, kTotalSum);

 // Spectrum(Spectrumloader, HistAxis, Cut)
  Spectrum sSumE(loader, axTotEnergy, kNoCut);

  loader.Go();

 //Sum Track Length Plot
  TCanvas* c1 = new TCanvas("c1", "c1");
  TH1* hSumEnergy = sSumE.ToTH1(6.6e20);
  hSumEnergy->Draw("hist");
  c1->Print("icarusplots/SumofTracksandShowers_Icarus.png");

}
