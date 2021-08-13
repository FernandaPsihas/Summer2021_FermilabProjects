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


const Var kSumTrkLen([](const caf::SRSliceProxy* slc) -> float {
  float sum(0.f);
  for (auto const& shw : slc->reco.shw) {
    sum += shw.energy_plane2; 
  }
  return sum;
});


void ShowerSumEnergy(const std::string inputName = "/pnfs/sbnd/persistent/sbndpro/mcp/mc/workshop/SBNWorkshop0421/prodoverlay_corsika_cosmics_proton_genie_nu_spill_gsimple-configf-v1_tpc/v09_19_00_01/caf/flat_caf_*")
//change path file to all files in icarus && change to look for calo2t rk energy (see Tbrwoser)
{

  SpectrumLoader loader(inputName);

  // ---- SPECTRA -----
  // A spectrum is a histogram with associated POT information
  const Binning binsTrksum = Binning::Simple(40, 0, 200);

  // HistAxis(Title, Binning, Var)
   const HistAxis axTrkEnergy("Sum of Shower Energies [MeV] ", binsTrksum, kSumTrkLen);

 // Spectrum(Spectrumloader, HistAxis, Cut)
  Spectrum sSumTrk(loader, axTrkEnergy, kNoCut);

  loader.Go();

 //Sum Track Length Plot
  TCanvas* c1 = new TCanvas("c1", "c1");
  TH1* hSumTrks = sSumTrk.ToTH1(6.6e20);
  hSumTrks->Draw("hist");
  c1->Print("sbndplots/SBND_SumofShowerEnergies.png");

}


