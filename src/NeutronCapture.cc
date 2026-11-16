#include <ANNRIGd_156GdContinuumModelV2.hh>
#include <ANNRIGd_156GdDiscreteModel.hh>
#include <ANNRIGd_158GdContinuumModelV2.hh>
#include <ANNRIGd_158GdDiscreteModel.hh>
#include <ANNRIGd_GdNCaptureGammaGenerator.hh>
#include <G4Electron.hh>
#include <G4Gamma.hh>
#include <G4HadronicInteractionRegistry.hh>
#include <G4HadronicProcess.hh>
#include <G4IonTable.hh>
#include <G4Neutron.hh>
#include <G4NeutronHPCapture.hh>
#include <G4NucleiProperties.hh>
#include <G4ParticleHPChannel.hh>
#include <G4ParticleHPManager.hh>
#include <G4PhysListUtil.hh>
#include <G4PhysicsModelCatalog.hh>
#include <G4RunManager.hh>
#include <G4SystemOfUnits.hh>
#include <NeutronCapture.hh>
#include <RAT/GLG4SteppingAction.hh>
#include <RAT/Log.hh>
#include <RAT/Rat.hh>
#include <Randomize.hh>
#include <TDirectory.h>

#include <filesystem>

// namespace for our detector implementation
namespace ANNIE {

// unnamed namespace for internal linkage of SteppingAction, CaptureModel, etc.
namespace {
// The ANNRI-Gd authors put all their classes in the namespace ANNRIGdGammaSpecModel
namespace AGd = ANNRIGdGammaSpecModel;

enum Cascade { kFull, kDiscrete, kContinuum };

// Set by an ANNRI-Gd capture, cleared by SteppingAction at the end of that step
G4bool gANNRIGdStep = false;

// CaptureModel is a wrapper around Geant4's capture model nRadCaptureHP.
// Every neutron capture below 20 MeV goes through it.
// Takes over the energy range of nRadCaptureHP in the neutron capture process:
//  1. isotope fix: re-picks the target isotope inside the element the process has chosen, weighted
//     by abundance x isotope capture cross section from G4NDL (the per-element HP channels that
//     G4NeutronHPCapture builds);
//  2. captures on 155Gd and 157Gd: gammas from ANNRI-Gd;
//  3. all other captures: the unchanged nRadCaptureHP.
//
class CaptureModel : public G4HadronicInteraction {
  public:
    CaptureModel(G4HadronicInteraction *radCaptureHP, G4bool isotopeFix, AGd::ANNRIGd_GdNCaptureGammaGenerator *annriGd, Cascade cascade) :
      G4HadronicInteraction("nRadCaptureHP_ANNIE"),
      fModel(radCaptureHP),
      fNDL(isotopeFix ? new G4NeutronHPCapture : nullptr),
      fANNRIGd(annriGd),
      fCascade(cascade) {
      SetMinEnergy(radCaptureHP->GetMinEnergy());
      SetMaxEnergy(radCaptureHP->GetMaxEnergy());
    }

  void BuildPhysicsTable(const G4ParticleDefinition &p) override {
    if (fNDL) fNDL->BuildPhysicsTable(p);  // field of the G4 Neutron Data Library: per-isotope G4NDL data, registered in G4ParticleHPManager
    fSecID = G4PhysicsModelCatalog::GetModelID("model_nRadCaptureHP");
  }

  // PickIsotope keeps the element Geant4 chose and draws the isotope again, weighted by abundance × isotope cross section. 
  // The nucleus is passed by reference (G4Nucleus &), and it's the process's own targetNucleus.
  G4HadFinalState *ApplyYourself(const G4HadProjectile &track, G4Nucleus &nucleus) override {
    if (fNDL) PickIsotope(track, nucleus);
    const G4int A = nucleus.GetA_asInt();
    // For Gd-155 and Gd-157, the gammas come from ANNRI-Gd, and nRadCaptureHP isn't called at all.
    if (fANNRIGd && nucleus.GetZ_asInt() == 64 && (A == 155 || A == 157)) return ANNRIGd(track, A);
    // Everything else (H, other Gd isotopes, Cl, Fe, …) goes to the unchanged nRadCaptureHP, but now with the correct isotope
    return fModel->ApplyYourself(track, nucleus);
  }

 private:
  // since Geant4 version 11.4 the isotope is drawn by abundance only, this is a local fix for this bug
  void PickIsotope(const G4HadProjectile &track, G4Nucleus &nucleus) const {
    const G4double ekin = track.GetKineticEnergy();
    for (const G4Element *elm : *track.GetMaterial()->GetElementVector()) {
      if (elm->GetZasInt() != nucleus.GetZ_asInt()) continue;
      // channel isotope i == element isotope i (G4ParticleHPChannel::Register)
      const G4ParticleHPChannel *ch = (*G4ParticleHPManager::GetInstance()->GetCaptureFinalStates())[elm->GetIndex()];
      G4double sum = 0.;
      for (G4int i = 0; i < ch->GetNiso(); ++i) sum += ch->HasAnyData(i) ? ch->GetWeightedXsec(ekin, i) : 0.;
      G4double r = sum * G4UniformRand();
      for (G4int i = 0; i < ch->GetNiso() && sum > 0.; ++i) {
        r -= ch->HasAnyData(i) ? ch->GetWeightedXsec(ekin, i) : 0.;
        if (r < 0.) {
          nucleus.SetIsotope(elm->GetIsotope(i));
          return;
        }
      }
      return;
    }
  }

  // ANNRI-Gd describes thermal capture: gammas in the rest frame of the compound nucleus, summing to
  // the neutron separation energy. They are boosted to the lab; the 156Gd/158Gd recoil, in its ground
  // state, takes the remaining momentum (as in ANNRI-Gd's own GdNeutronHPCaptureFS).
  G4HadFinalState *ANNRIGd(const G4HadProjectile &track, G4int A) {
    const AGd::ReactionProductVector gammas = Generate(A);
    theParticleChange.Clear();
    theParticleChange.SetStatusChange(stopAndKill);
    // Target at rest: its thermal motion would shift the gamma energies by < 10 eV
    G4LorentzVector lab = track.Get4Momentum() + G4LorentzVector(0., 0., 0., G4NucleiProperties::GetNuclearMass(A, 64));
    const G4ThreeVector boost = lab.boostVector();
    for (const AGd::ReactionProduct &g : gammas) {
      G4LorentzVector p(g.px_ * MeV, g.py_ * MeV, g.pz_ * MeV, g.eTot_ * MeV);
      p.boost(boost);
      lab -= p;
      const G4ParticleDefinition *def = G4Gamma::Gamma();
      if (g.pdgID_ != 22) def = G4Electron::Electron();
      theParticleChange.AddSecondary(new G4DynamicParticle(def, p.vect().unit(), p.e() - def->GetPDGMass()), fSecID);
    }
    const G4ParticleDefinition *recoil = G4IonTable::GetIonTable()->GetIon(64, A + 1, 0.);
    const G4double m = recoil->GetPDGMass();
    theParticleChange.AddSecondary(new G4DynamicParticle(recoil, lab.vect().unit(), std::sqrt(lab.vect().mag2() + m * m) - m), fSecID);
    gANNRIGdStep = true;
    return &theParticleChange;
  }

  AGd::ReactionProductVector Generate(G4int A) {
    switch (fCascade) {
      case kDiscrete:
        return A == 157 ? fANNRIGd->Generate_158Gd_Discrete() : fANNRIGd->Generate_156Gd_Discrete();
      case kContinuum:
        return A == 157 ? fANNRIGd->Generate_158Gd_Continuum() : fANNRIGd->Generate_156Gd_Continuum();
      default:
        return A == 157 ? fANNRIGd->Generate_158Gd() : fANNRIGd->Generate_156Gd();
    }
  }

  G4HadronicInteraction *fModel;
  G4NeutronHPCapture *fNDL;  // owned by G4HadronicInteractionRegistry, like every model
  AGd::ANNRIGd_GdNCaptureGammaGenerator *fANNRIGd;
  Cascade fCascade;
  G4int fSecID = -1;
};

// ratpac-two's GLG4SteppingAction replaces the gammas of every "nCapture" step that has a Gd158
// secondary with DICEBOX gammas. For ANNRI-Gd captures the step's process is hidden from it while it
// runs; everything else GLG4SteppingAction does is unchanged.
class SteppingAction : public GLG4SteppingAction {
 public:
  void UserSteppingAction(const G4Step *step) override {
    if (!gANNRIGdStep) {
      GLG4SteppingAction::UserSteppingAction(step);
      return;
    }
    gANNRIGdStep = false;
    G4StepPoint *post = step->GetPostStepPoint();
    const G4VProcess *process = post->GetProcessDefinedStep();
    post->SetProcessDefinedStep(nullptr);
    GLG4SteppingAction::UserSteppingAction(step);
    post->SetProcessDefinedStep(process);
  }
};

// Finds a file like /rat/db/load does: first match in the ratdb directories ($ANNIEDATA/ratdb first)
std::string RatdbFile(const std::string &name) {
  for (const auto &dir : RAT::Rat::ratdb_directories)
    if (std::filesystem::exists(dir + "/" + name)) return dir + "/" + name;
  G4Exception("ANNIE::NeutronCapture", "ANNRIGd", FatalException,
              ("cannot find " + name + " in the ratdb directories (annie.sh sourced?)").c_str());
  return name;
}

// Never deleted: its ROOT tables must not be destroyed after ROOT's own cleanup at exit.
AGd::ANNRIGd_GdNCaptureGammaGenerator *LoadANNRIGd() {
  const std::string gd156 = RatdbFile("ANNRIGd/156GdContTbl__E1SLO4__HFB.root");
  const std::string gd158 = RatdbFile("ANNRIGd/158GdContTbl__E1SLO4__HFB.root");
  RAT::info << "NeutronCapture: ANNRI-Gd tables " << gd156 << ", " << gd158 << newline;
  TDirectory::TContext keep;  // reading the tables must not change ROOT's current directory
  return new AGd::ANNRIGd_GdNCaptureGammaGenerator(
      new AGd::ANNRIGd_156GdContinuumModelV2(gd156), new AGd::ANNRIGd_158GdContinuumModelV2(gd158),
      new AGd::ANNRIGd_156GdDiscreteModel, new AGd::ANNRIGd_158GdDiscreteModel);
}
}  // namespace

NeutronCapture::NeutronCapture() : fMessenger(this, "/annie/physics/", "ANNIE physics patches") {
  fMessenger
      .DeclareProperty("hpCaptureIsotopeFix", fIsotopeFix,
                       "true (default): before neutron capture below 20 MeV, the target isotope is re-picked by "
                       "abundance x isotope cross section (G4NDL). false: Geant4 >= 11.4 default, isotope picked by "
                       "abundance only. Must be set before /run/initialize.")
      .SetParameterName("enable", true)
      .SetDefaultValue("true")
      .SetStates(G4State_PreInit);  // Geant4 refuses the command after /run/initialize
  fMessenger
      .DeclareProperty("annriGd", fANNRIGd,
                       "true (default): gammas of 155Gd(n,g) and 157Gd(n,g) from the ANNRI-Gd model."
                       "false: Geant4 nRadCaptureHP, and ratpac-two's DICEBOX for 157Gd(n,g). Must be set before /run/initialize.")
      .SetParameterName("enable", true)
      .SetDefaultValue("true")
      .SetStates(G4State_PreInit);
  fMessenger
      .DeclareProperty("annriGdCascade", fCascade,
                       "ANNRI-Gd spectrum part: full (default: discrete peaks + continuum), discrete, continuum."
                       "Must be set before /run/initialize.")
      .SetCandidates("full discrete continuum")
      .SetStates(G4State_PreInit);
}

// Geant4 calls Notify() on every application-state change. The first change to Idle happens inside
// /run/initialize, right after the physics list has built its processes
// (G4RunManagerKernel::InitializePhysics). Physics tables are only built at the first /run/beamOn,
// so a model registered here is initialised like any other. RAT has set its user actions earlier,
// in Rat::Begin (Gsim::Init).
G4bool NeutronCapture::Notify(G4ApplicationState requestedState) {
  if (fDone || requestedState != G4State_Idle) return true;  // false would veto the state change
  fDone = true;

  if (!fIsotopeFix && !fANNRIGd) {
    RAT::info << "NeutronCapture: isotope fix and ANNRI-Gd disabled, Geant4 default neutron capture" << newline;
    return true;
  }

  G4HadronicInteraction *radCaptureHP = G4HadronicInteractionRegistry::Instance()->FindModel("nRadCaptureHP");
  G4HadronicProcess *capture = G4PhysListUtil::FindCaptureProcess(G4Neutron::Neutron());
  if (!radCaptureHP || !capture) {
    RAT::warn << "NeutronCapture: no nRadCaptureHP model found (Geant4 < 11.4?), nothing patched" << newline;
    return true;
  }

  AGd::ANNRIGd_GdNCaptureGammaGenerator *annriGd = nullptr;
  if (fANNRIGd) {
    annriGd = LoadANNRIGd();
    G4RunManager::GetRunManager()->SetUserAction(new SteppingAction);  // replaces RAT's GLG4SteppingAction
  }
  const Cascade cascade = fCascade == "discrete" ? kDiscrete : fCascade == "continuum" ? kContinuum : kFull;

  // The model takes over the energy range of nRadCaptureHP (0-20 MeV from G4NeutronPHPBuilder) ...
  capture->RegisterMe(new CaptureModel(radCaptureHP, fIsotopeFix, annriGd, cascade));
  // ... and nRadCaptureHP gets an empty range (min == max), so G4EnergyRangeManager only picks the new one.
  radCaptureHP->SetMinEnergy(radCaptureHP->GetMaxEnergy());

  RAT::info << "NeutronCapture: below " << radCaptureHP->GetMaxEnergy() / MeV << " MeV, target isotope "
            << (fIsotopeFix ? "re-picked by abundance x G4NDL cross section" : "as picked by Geant4 (abundance only)")
            << newline;
  if (fANNRIGd)
    RAT::info << "NeutronCapture: 155Gd(n,g) and 157Gd(n,g) gammas from ANNRI-Gd (" << fCascade
              << "), DICEBOX skipped for them; all other captures: nRadCaptureHP" << newline
              << "NeutronCapture: ANNRI-Gd condition of use: publications must cite K. Hagiwara et al., PTEP 2019, "
                 "023D01 and T. Tanaka et al., PTEP 2020, 043D02 (src/ANNRIGd/README.md)"
              << newline;
  else
    RAT::info << "NeutronCapture: ANNRI-Gd disabled, nRadCaptureHP (and ratpac-two's DICEBOX for 157Gd)" << newline;
  return true;
}

}  // namespace ANNIE
