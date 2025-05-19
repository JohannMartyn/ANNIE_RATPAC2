#include <TFile.h>
#include <TTimeStamp.h>
#include <TTree.h>
#include <TVector3.h>

#include <RAT/DB.hh>
#include <RAT/DS/DigitPMT.hh>
#include <RAT/DS/EV.hh>
#include <RAT/DS/MC.hh>
#include <RAT/DS/MCPMT.hh>
#include <RAT/DS/MCParticle.hh>
#include <RAT/DS/MCSummary.hh>
#include <RAT/DS/PMT.hh>
#include <RAT/DS/PMTInfo.hh>
#include <RAT/DS/Root.hh>
#include <RAT/DS/Run.hh>
#include <RAT/DS/RunStore.hh>
#include <RAT/Log.hh>
#include <OutANNIEClusterProc.hh>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

namespace RAT {

OutANNIEClusterProc::OutANNIEClusterProc() : Processor("outanniecluster") {
  outputFile = nullptr;
  outputTree = nullptr;
  metaTree = nullptr;
  runBranch = new DS::Run();

  // Load options from the database
  DB *db = DB::Get();
  DBLinkPtr table = db->GetLink("IO", "OutANNIEClusterProc");
  try {
    defaultFilename = table->GetS("default_output_filename");
    if (defaultFilename.find(".") == std::string::npos) {
      defaultFilename += ".ntuple.root";
    }
  } catch (DBNotFoundError &e) {
    defaultFilename = "output.ntuple.root";
  }
  try {
    options.tracking = table->GetZ("include_tracking");
    options.mcparticles = table->GetZ("include_mcparticles");
    options.pmthits = table->GetZ("include_pmthits");
    options.untriggered = table->GetZ("include_untriggered_events");
    options.mchits = table->GetZ("include_mchits");
    options.clusteredhits = table->GetZ("include_clusteredhits");
  } catch (DBNotFoundError &e) {
    options.tracking = false;
    options.mcparticles = false;
    options.pmthits = true;
    options.untriggered = false;
    options.mchits = true;
    options.clusteredhits = true;
  }
  try{
    clusterSettings.clusterFindingWindow = table->GetI("clusterSettings.clusterFindingWindow");
    clusterSettings.acqTimeWindow = table->GetI("acqTimeWindow");
    clusterSettings.clusterIntegrationWindow = table->GetI("clusterIntegrationWindow");
    clusterSettings.minHitsPerCluster = table->GetI("minHitsPerCluster");
    clusterSettings.end_of_window_time_cut = table->GetD("end_of_window_time_cut");
    clusterSettings.datalikeIntegrationWindow = table->GetI("datalikeIntegrationWindow");
  } catch (DBNotFoundError &e) {
    clusterSettings.clusterFindingWindow = 40;
    clusterSettings.acqTimeWindow = 70000;
    clusterSettings.clusterIntegrationWindow = 40;
    clusterSettings.minHitsPerCluster = 5;
    clusterSettings.end_of_window_time_cut = 0.95;
    clusterSettings.datalikeIntegrationWindow = 10;
  }
}

bool OutANNIEClusterProc::OpenFile(std::string filename) {
  int i = 0;
  outputFile = TFile::Open(filename.c_str(), "RECREATE");
  // Meta Tree
  metaTree = new TTree("meta", "meta");
  metaTree->Branch("runId", &runId);
  metaTree->Branch("runType", &runType);
  metaTree->Branch("runTime", &runTime);
  metaTree->Branch("dsentries", &dsentries);
  metaTree->Branch("macro", &macro);
  metaTree->Branch("pmtType", &pmtType);
  metaTree->Branch("pmtId", &pmtId);
  metaTree->Branch("pmtEff", &pmtEff);
  metaTree->Branch("pmtX", &pmtX);
  metaTree->Branch("pmtY", &pmtY);
  metaTree->Branch("pmtZ", &pmtZ);
  metaTree->Branch("pmtU", &pmtU);
  metaTree->Branch("pmtV", &pmtV);
  metaTree->Branch("pmtW", &pmtW);
  this->AssignAdditionalMetaAddresses();
  dsentries = 0;
  // Data Tree
  outputTree = new TTree("output", "output");
  // These are the *first* particles MC positions, directions, and time
  outputTree->Branch("mcpdg", &mcpdg);
  outputTree->Branch("mcx", &mcx);
  outputTree->Branch("mcy", &mcy);
  outputTree->Branch("mcz", &mcz);
  outputTree->Branch("mcu", &mcu);
  outputTree->Branch("mcv", &mcv);
  outputTree->Branch("mcw", &mcw);
  outputTree->Branch("mcke", &mcke);
  outputTree->Branch("mct", &mct);

  // Getting the stopping point / end of track of the first particle
  outputTree->Branch("mcx_firstStep", &mcx_firstStep);
  outputTree->Branch("mcy_firstStep", &mcy_firstStep);
  outputTree->Branch("mcz_firstStep", &mcz_firstStep);
  outputTree->Branch("mcx_lastStep", &mcx_lastStep);
  outputTree->Branch("mcy_lastStep", &mcy_lastStep);
  outputTree->Branch("mcz_lastStep", &mcz_lastStep);


  // Event IDs and trigger time and nhits
  outputTree->Branch("evid", &evid);
  outputTree->Branch("subev", &subev);
  outputTree->Branch("nhits", &nhits);
  outputTree->Branch("triggerTime", &triggerTime);
  // MC Information
  outputTree->Branch("mcparticlecount", &mcpcount);
  outputTree->Branch("mcpecount", &mcpecount);
  outputTree->Branch("mcnhits", &mcnhits);
  outputTree->Branch("scintEdep", &scintEdep);
  outputTree->Branch("scintEdepQuenched", &scintEdepQuenched);
  // Total number of produced photons of each type
  outputTree->Branch("scintPhotons", &scintPhotons);
  outputTree->Branch("remPhotons", &remPhotons);
  outputTree->Branch("cherPhotons", &cherPhotons);
  if (options.mcparticles) {
    // Save information about *all* particles that are simulated
    // Variable naming is the same as the first particle, just plural.
    outputTree->Branch("mcpdgs", &pdgcodes);
    outputTree->Branch("mcxs", &mcPosx);
    outputTree->Branch("mcys", &mcPosy);
    outputTree->Branch("mczs", &mcPosz);
    outputTree->Branch("mcus", &mcDirx);
    outputTree->Branch("mcvs", &mcDiry);
    outputTree->Branch("mcws", &mcDirz);
    outputTree->Branch("mckes", &mcKEnergies);
    outputTree->Branch("mcts", &mcTime);
  }
  if (options.pmthits) {
    // Save full PMT hit informations
    outputTree->Branch("hitPMTID", &hitPMTID);
    // Information about *first* detected PE
    outputTree->Branch("hitPMTTime", &hitPMTTime);
    outputTree->Branch("hitPMTCharge", &hitPMTCharge);
    // Output of the waveform analysis
    outputTree->Branch("hitPMTDigitizedTime", &hitPMTDigitizedTime);
    outputTree->Branch("hitPMTDigitizedCharge", &hitPMTDigitizedCharge);
    outputTree->Branch("hitPMTNCrossings", &hitPMTNCrossings);
  }
  if (options.mchits) {
    // Save full MC PMT hit information
    outputTree->Branch("mcPMTID", &mcpmtid);
    outputTree->Branch("mcPMTNPE", &mcpmtnpe);
    outputTree->Branch("mcPMTCharge", &mcpmtcharge);

    outputTree->Branch("mcPEHitTime", &mcpehittime);
    outputTree->Branch("mcPEFrontEndTime", &mcpefrontendtime);
    // Production process
    // 1=Cherenkov, 0=Dark noise, 2=Scint., 3=Reem., 4=Unknown
    outputTree->Branch("mcPEProcess", &mcpeprocess);
    outputTree->Branch("mcPEWavelength", &mcpewavelength);
    outputTree->Branch("mcPEx", &mcpex);
    outputTree->Branch("mcPEy", &mcpey);
    outputTree->Branch("mcPEz", &mcpez);
    outputTree->Branch("mcPECharge", &mcpecharge);
  }
  if (options.tracking) {
    // Save particle tracking information
    outputTree->Branch("trackPDG", &trackPDG);
    outputTree->Branch("trackPosX", &trackPosX);
    outputTree->Branch("trackPosY", &trackPosY);
    outputTree->Branch("trackPosZ", &trackPosZ);
    outputTree->Branch("trackMomX", &trackMomX);
    outputTree->Branch("trackMomY", &trackMomY);
    outputTree->Branch("trackMomZ", &trackMomZ);
    outputTree->Branch("trackKE", &trackKE);
    outputTree->Branch("trackTime", &trackTime);
    outputTree->Branch("trackProcess", &trackProcess);
    metaTree->Branch("processCodeMap", &processCodeMap);
  }
  if (options.clusteredhits) {
    //gInterpreter->GenerateDictionary("std::vector<std::vector<double> >", "vector");
    //gInterpreter->GenerateDictionary("std::vector<std::vector<int> >", "vector");
    outputTree->Branch("numClusters", &numClusters);
    outputTree->Branch("clusterCharge", &clusterCharge);
    outputTree->Branch("clusterChargeBalance", &clusterChargeBalance);
    outputTree->Branch("clusterNPE", &clusterNPE);
    outputTree->Branch("clusterTime", &clusterTime);
    outputTree->Branch("numClusteredPMTHits", &numClusteredPMTHits);
    outputTree->Branch("clusterHitsPMTID", &clusterHitsPMTID);
    outputTree->Branch("clusterHitsPMTTime", &clusterHitsPMTTime);
    outputTree->Branch("clusterHitsNPE", &clusterHitsNPE);
    outputTree->Branch("clusterHitsPMTCharge", &clusterHitsPMTCharge);
  }
  this->AssignAdditionalAddresses();

  return true;
}

Processor::Result OutANNIEClusterProc::DSEvent(DS::Root *ds) {
  if (!this->outputFile) {
    if (!OpenFile(this->defaultFilename.c_str())) {
      Log::Die("No output file specified");
    }
  }
  runBranch = DS::RunStore::GetRun(ds);
  DS::PMTInfo *pmtinfo = runBranch->GetPMTInfo();
  ULong64_t stonano = 1000000000;
  dsentries++;
  // Clear the previous vectors
  pdgcodes.clear();
  mcKEnergies.clear();
  mcPosx.clear();
  mcPosy.clear();
  mcPosz.clear();
  mcDirx.clear();
  mcDiry.clear();
  mcDirz.clear();
  mcTime.clear();

  DS::MC *mc = ds->GetMC();
  mcpcount = mc->GetMCParticleCount();
  for (int pid = 0; pid < mcpcount; pid++) {
    DS::MCParticle *particle = mc->GetMCParticle(pid);
    pdgcodes.push_back(particle->GetPDGCode());
    mcKEnergies.push_back(particle->GetKE());
    TVector3 mcpos = particle->GetPosition();
    TVector3 mcdir = particle->GetMomentum();
    mcPosx.push_back(mcpos.X());
    mcPosy.push_back(mcpos.Y());
    mcPosz.push_back(mcpos.Z());
    mcDirx.push_back(mcdir.X() / mcdir.Mag());
    mcDiry.push_back(mcdir.Y() / mcdir.Mag());
    mcDirz.push_back(mcdir.Z() / mcdir.Mag());
    mcTime.push_back(particle->GetTime());
  }
  // First particle's position, direction, and time
  mcpdg = pdgcodes[0];
  mcx = mcPosx[0];
  mcy = mcPosy[0];
  mcz = mcPosz[0];
  mcu = mcDirx[0];
  mcv = mcDiry[0];
  mcw = mcDirz[0];
  mct = mcTime[0];
  mcke = accumulate(mcKEnergies.begin(), mcKEnergies.end(), 0.0);

  mcx_firstStep = -666.0;
  mcy_firstStep = -666.0;
  mcz_firstStep = -666.0;
  mcx_lastStep = -666.0;
  mcy_lastStep = -666.0;
  mcz_lastStep = -666.0;

  //Getting the stopping point / end of track of the first particle
  for (int trk = 0; trk < mc->GetMCTrackCount(); trk++) {
    DS::MCTrack *track = mc->GetMCTrack(trk);

    //Look only at the primary simulated particle
    if( track->GetPDGCode() == mcpdg ){
      //info << dformat("OutANNIEClusterProc: track number %d, track pdg code %d, MC pdg code %d \n", trk, track->GetPDGCode(), mcpdg);
      DS::MCTrackStep *step = track->GetMCTrackStep(0);
      TVector3 tv = step->GetEndpoint();
      mcx_firstStep = tv.X();
      mcy_firstStep = tv.Y();
      mcz_firstStep = tv.Z();
      step = track->GetMCTrackStep(track->GetMCTrackStepCount()-1);
      tv = step->GetEndpoint();
      mcx_lastStep = tv.X();
      mcy_lastStep = tv.Y();
      mcz_lastStep = tv.Z();
      break;
    }
    /*
    //Primary particle is neutron, save scattered protons...
    } else if (mcpdg == 2112 && track->GetPDGCode() == 2212){ 
      info << dformat("OutANNIEClusterProc: track number %d, track pdg code %d, MC pdg code %d \n", trk, track->GetPDGCode(), mcpdg);
      DS::MCTrackStep *step = track->GetMCTrackStep(0);
      TVector3 tv = step->GetEndpoint();
      pdgcodes.push_back(track->GetPDGCode());
      mcKEnergies.push_back( track->GetDepositedEnergy() );      
      mcPosx.push_back(tv.X());
      mcPosy.push_back(tv.Y());
      mcPosz.push_back(tv.Z());
      mcDirx.push_back(step->GetGlobalTime());
      mcDiry.push_back(step->GetLocalTime());
      mcDirz.push_back(step->GetProperTime());
      mcTime.push_back(step->GetGlobalTime());
    }
    */
  }

  // Tracking
  if (options.tracking) {
    int nTracks = mc->GetMCTrackCount();
    // Clear previous event
    trackPDG.clear();
    trackPosX.clear();
    trackPosY.clear();
    trackPosZ.clear();
    trackMomX.clear();
    trackMomY.clear();
    trackMomZ.clear();
    trackKE.clear();
    trackTime.clear();
    trackProcess.clear();

    std::vector<double> xtrack, ytrack, ztrack;
    std::vector<double> pxtrack, pytrack, pztrack;
    std::vector<double> kinetic, globaltime;
    std::vector<int> processMapID;
    for (int trk = 0; trk < nTracks; trk++) {
      DS::MCTrack *track = mc->GetMCTrack(trk);
      trackPDG.push_back(track->GetPDGCode());
      xtrack.clear();
      ytrack.clear();
      ztrack.clear();
      pxtrack.clear();
      pytrack.clear();
      pztrack.clear();
      kinetic.clear();
      globaltime.clear();
      processMapID.clear();
      int nSteps = track->GetMCTrackStepCount();
      for (int stp = 0; stp < nSteps; stp++) {
        DS::MCTrackStep *step = track->GetMCTrackStep(stp);
        // Process
        std::string proc = step->GetProcess();
        if (processCodeMap.find(proc) == processCodeMap.end()) {
          processCodeMap[proc] = processCodeMap.size();
          processCodeIndex.push_back(processCodeMap.size() - 1);
          processName.push_back(proc);
        }
        processMapID.push_back(processCodeMap[proc]);
        TVector3 tv = step->GetEndpoint();
        TVector3 momentum = step->GetMomentum();
        kinetic.push_back(step->GetKE());
        globaltime.push_back(step->GetGlobalTime());
        xtrack.push_back(tv.X());
        ytrack.push_back(tv.Y());
        ztrack.push_back(tv.Z());
        pxtrack.push_back(momentum.X());
        pytrack.push_back(momentum.Y());
        pztrack.push_back(momentum.Z());
      }
      trackKE.push_back(kinetic);
      trackTime.push_back(globaltime);
      trackPosX.push_back(xtrack);
      trackPosY.push_back(ytrack);
      trackPosZ.push_back(ztrack);
      trackMomX.push_back(pxtrack);
      trackMomY.push_back(pytrack);
      trackMomZ.push_back(pztrack);
      trackProcess.push_back(processMapID);
    }
  }

  // MCSummary info
  RAT::DS::MCSummary *mcs = mc->GetMCSummary();
  scintEdep = mcs->GetTotalScintEdep();
  scintEdepQuenched = mcs->GetTotalScintEdepQuenched();
  scintPhotons = mcs->GetNumScintPhoton();
  remPhotons = mcs->GetNumReemitPhoton();
  cherPhotons = mcs->GetNumCerenkovPhoton();

  // MCPMT information
  mcpmtid.clear();
  mcpmtnpe.clear();
  mcpmtcharge.clear();

  // MCPE information
  mcpehittime.clear();
  mcpefrontendtime.clear();
  mcpeprocess.clear();
  mcpewavelength.clear();
  mcpex.clear();
  mcpey.clear();
  mcpez.clear();
  mcpecharge.clear();

  mcnhits = mc->GetMCPMTCount();
  mcpecount = mc->GetNumPE();
  if (options.mchits) {
    for (int ipmt = 0; ipmt < mc->GetMCPMTCount(); ipmt++) {
      DS::MCPMT *mcpmt = mc->GetMCPMT(ipmt);
      mcpmtid.push_back(pmtinfo->GetChannelNumber(mcpmt->GetID()));
      mcpmtnpe.push_back(mcpmt->GetMCPhotonCount());
      mcpmtcharge.push_back(mcpmt->GetCharge());
      TVector3 position = pmtinfo->GetPosition(mcpmt->GetID());
      for (int ipe = 0; ipe < mcpmt->GetMCPhotonCount(); ipe++) {
        RAT::DS::MCPhoton *mcph = mcpmt->GetMCPhoton(ipe);
        mcpehittime.push_back(mcph->GetHitTime());
        mcpefrontendtime.push_back(mcph->GetFrontEndTime());
        mcpewavelength.push_back(mcph->GetLambda());
        mcpex.push_back(position.X());
        mcpey.push_back(position.Y());
        mcpez.push_back(position.Z());
        mcpecharge.push_back(mcph->GetCharge());
        if (mcph->IsDarkHit()) {
          mcpeprocess.push_back(noise);
          continue;
        }
        std::string process = mcph->GetCreatorProcess();
        if (process.find("Cerenkov") != std::string::npos) {
          mcpeprocess.push_back(cherenkov);
        } else if (process.find("Scintillation") != std::string::npos) {
          mcpeprocess.push_back(scintillation);
        } else if (process.find("Reemission") != std::string::npos) {
          mcpeprocess.push_back(reemission);
        } else {
          mcpeprocess.push_back(unknown);
        }
      }
    }
  }

  if (options.clusteredhits) {
    //gInterpreter->GenerateDictionary("std::vector<std::vector<double> >", "vector");
    //gInterpreter->GenerateDictionary("std::vector<std::vector<int> >", "vector");
    numClusters = 0;
    clusterCharge = 0;
    clusterChargeBalance = 0;
    clusterNPE = 0;
    clusterTime = 0;
    numClusteredPMTHits = 0;
    clusterHitsPMTID.clear();
    clusterHitsPMTTime.clear();
    clusterHitsNPE.clear();
    clusterHitsPMTCharge.clear();
    this->ClusterFinder(mc);
    for(int iC=0; iC<clusterHitsPMTID.size(); iC++){
      clusterHitsPMTID[iC] = pmtinfo->GetChannelNumber( clusterHitsPMTID[iC] );
    }
    
  }

  // EV Branches
  for (subev = 0; subev < ds->GetEVCount(); subev++) {
    DS::EV *ev = ds->GetEV(subev);
    evid = ev->GetID();
    triggerTime = ev->GetCalibratedTriggerTime();
    auto fitVector = ev->GetFitResults();
    std::map<std::string, double *> fitvalues;
    std::map<std::string, bool *> fitvalids;
    std::map<std::string, int *> intFOMs;
    std::map<std::string, bool *> boolFOMs;
    std::map<std::string, double *> doubleFOMs;
    for (auto fit : fitVector) {
      std::string name = fit->GetFitterName();
      // Check the validity and write it out
      if (fit->GetEnablePosition()) {
        TVector3 pos = fit->GetPosition();
        fitvalues["x_" + name] = new double(pos.X());
        fitvalues["y_" + name] = new double(pos.Y());
        fitvalues["z_" + name] = new double(pos.Z());
        fitvalids["validposition_" + name] = new bool(fit->GetValidPosition());
      }
      if (fit->GetEnableDirection()) {
        TVector3 dir = fit->GetDirection();
        fitvalues["u_" + name] = new double(dir.X());
        fitvalues["v_" + name] = new double(dir.Y());
        fitvalues["w_" + name] = new double(dir.Z());
        fitvalids["validdirection_" + name] = new bool(fit->GetValidDirection());
      }
      if (fit->GetEnableEnergy()) {
        fitvalues["energy_" + name] = new double(fit->GetEnergy());
        fitvalids["validenergy" + name] = new bool(fit->GetValidEnergy());
      }
      if (fit->GetEnableTime()) {
        fitvalues["time_" + name] = new double(fit->GetTime());
        fitvalids["validtime" + name] = new bool(fit->GetValidTime());
      }
      // Figures of merit > 3 types
      for (auto const &[label, value] : fit->boolFiguresOfMerit) {
        boolFOMs[label + "_" + name] = new bool(value);
      }
      for (auto const &[label, value] : fit->intFiguresOfMerit) {
        intFOMs[label + "_" + name] = new int(value);
      }
      for (auto const &[label, value] : fit->doubleFiguresOfMerit) {
        doubleFOMs[label + "_" + name] = new double(value);
      }
    }
    // Write fitter values into TTree
    for (auto const &[label, value] : fitvalues) {
      this->SetBranchValue(label, value);
    }
    for (auto const &[label, value] : fitvalids) {
      this->SetBranchValue(label, value);
    }
    for (auto const &[label, value] : intFOMs) {
      this->SetBranchValue(label, value);
    }
    for (auto const &[label, value] : boolFOMs) {
      this->SetBranchValue(label, value);
    }
    for (auto const &[label, value] : doubleFOMs) {
      this->SetBranchValue(label, value);
    }
    nhits = ev->GetPMTCount();
    if (options.pmthits) {
      hitPMTID.clear();
      hitPMTTime.clear();
      hitPMTCharge.clear();
      hitPMTDigitizedTime.clear();
      hitPMTDigitizedCharge.clear();
      hitPMTNCrossings.clear();

      for (int pmtc = 0; pmtc < ev->GetPMTCount(); pmtc++) {
        RAT::DS::PMT *pmt = ev->GetPMT(pmtc);
        hitPMTID.push_back(pmtinfo->GetChannelNumber(pmt->GetID()));
        hitPMTTime.push_back(pmt->GetTime());
        hitPMTCharge.push_back(pmt->GetCharge());
      }
      for (int pmtc = 0; pmtc < ev->GetDigitPMTCount(); pmtc++) {
        RAT::DS::DigitPMT *digitpmt = ev->GetDigitPMT(pmtc);
        hitPMTDigitizedTime.push_back(digitpmt->GetDigitizedTime());
        hitPMTDigitizedCharge.push_back(digitpmt->GetDigitizedCharge());
        hitPMTNCrossings.push_back(digitpmt->GetNCrossings());
      }
    }
    this->FillEvent(ds, ev);
    outputTree->Fill();
  }
  if (options.untriggered && ds->GetEVCount() == 0) {
    evid = -1;
    triggerTime = 0;
    if (options.pmthits) {
      hitPMTID.clear();
      hitPMTTime.clear();
      hitPMTCharge.clear();
      hitPMTDigitizedTime.clear();
      hitPMTDigitizedCharge.clear();
      hitPMTNCrossings.clear();
    }
    this->FillNoTriggerEvent(ds);
    outputTree->Fill();
  }

  // FIX THE ABOVE
  // int errorcode = outputTree->Fill();
  // if( errorcode < 0 )
  //{
  //  Log::Die(std::string("OutANNIEClusterProc: Error fill ttree, check disk
  //  space"));
  //}
  return Processor::OK;
}

OutANNIEClusterProc::~OutANNIEClusterProc() {
  if (outputFile) {
    outputFile->cd();

    DS::PMTInfo *pmtinfo = runBranch->GetPMTInfo();
    for (int id = 0; id < pmtinfo->GetPMTCount(); id++) {
      int type = pmtinfo->GetType(id);
      TVector3 position = pmtinfo->GetPosition(id);
      TVector3 direction = pmtinfo->GetDirection(id);
      pmtType.push_back(type);
      pmtId.push_back(pmtinfo->GetChannelNumber(id));
      pmtEff.push_back(pmtinfo->GetEfficiencyCorr(id));
      pmtX.push_back(position.X());
      pmtY.push_back(position.Y());
      pmtZ.push_back(position.Z());
      pmtU.push_back(direction.X());
      pmtV.push_back(direction.Y());
      pmtW.push_back(direction.Z());
    }
    runId = runBranch->GetID();
    runType = runBranch->GetType();
    // Converting to unix time
    ULong64_t stonano = 1000000000;
    TTimeStamp rootTime = runBranch->GetStartTime();
    runTime = static_cast<ULong64_t>(rootTime.GetSec()) * stonano + static_cast<ULong64_t>(rootTime.GetNanoSec());
    macro = Log::GetMacro();
    metaTree->Fill();
    metaTree->Write();
    outputTree->Write();
    /*
    TMap* dbtrace = Log::GetDBTraceMap();
    dbtrace->Write("db", TObject::kSingleKey);
    */
    // outputFile->Write(0, TObject::kOverwrite);
    outputFile->Close();
    delete outputFile;
  }
}

void OutANNIEClusterProc::SetBranchValue(std::string name, double *value) {
  if (branchNames.find(name) != branchNames.end()) {
    outputTree->SetBranchAddress(name.c_str(), value);
  } else {
    branchNames.insert(name);
    outputTree->Branch(name.c_str(), value);
  }
}

void OutANNIEClusterProc::SetBranchValue(std::string name, bool *value) {
  if (branchNames.find(name) != branchNames.end()) {
    outputTree->SetBranchAddress(name.c_str(), value);
  } else {
    branchNames.insert(name);
    outputTree->Branch(name.c_str(), value);
  }
}

void OutANNIEClusterProc::SetBranchValue(std::string name, int *value) {
  if (branchNames.find(name) != branchNames.end()) {
    outputTree->SetBranchAddress(name.c_str(), value);
  } else {
    branchNames.insert(name);
    outputTree->Branch(name.c_str(), value);
  }
}

void OutANNIEClusterProc::SetS(std::string param, std::string value) {
  if (param == "file") {
    this->defaultFilename = value;
  }
}

void OutANNIEClusterProc::SetI(std::string param, int value) {
  if (param == "include_tracking") {
    options.tracking = value ? true : false;
  }
  if (param == "include_mcparticles") {
    options.mcparticles = value ? true : false;
  }
  if (param == "include_pmthits") {
    options.pmthits = value ? true : false;
  }
  if (param == "include_untriggered_events") {
    options.untriggered = value ? true : false;
  }
  if (param == "include_mchits") {
    options.mchits = value ? true : false;
  }
  if (param == "include_clusteredhits") {
    options.clusteredhits = value ? true : false;
  }
}

//TODO: Make it so that ClusterFinder is only invoked for the tank PMTs
bool OutANNIEClusterProc::ClusterFinder(DS::MC *mc){

  v_datalike_time.clear();
  v_datalike_charge.clear();
  v_datalike_npe.clear();
  v_datalike_pmtid.clear();

  v_hittimes.clear();
  v_hittimes_sorted.clear();
  v_mini_hits.clear();
  m_time_Nhits.clear();
  v_clusters.clear();
  v_local_cluster_times.clear();

  for (int ipmt = 0; ipmt < mc->GetMCPMTCount(); ipmt++) {
    DS::MCPMT *mcpmt = mc->GetMCPMT(ipmt);
    //----------------------------------------------------------------------------
    //-First we make the MC hit times look more like data, according to the WCSim-
    //----------------------------------------------------------------------------
    std::vector<double> v_temp_time;
    std::vector<double> v_temp_charge;
    for (int ipe = 0; ipe < mcpmt->GetMCPhotonCount(); ipe++) {
      RAT::DS::MCPhoton *mcph = mcpmt->GetMCPhoton(ipe);
      //TODO: There is no simulation of the extended window, so no need to cut on "end_of_window_time_cut*AcqTimeWindow"
      //For now leave this in, as it is also used as such in WCSim...
      if (mcph->GetFrontEndTime() < clusterSettings.end_of_window_time_cut*clusterSettings.acqTimeWindow) {
        v_temp_time.push_back(mcph->GetFrontEndTime());
        v_temp_charge.push_back(mcph->GetCharge());
      }
    }
    //The following code is copied from WCSim.
    //Combine multiple MC hits to one pulse.
    //No need to sort v_datalike_charge, as the hit charges are random anyway.
    std::sort(v_temp_time.begin(),v_temp_time.end());
    std::vector<double> temp_times;
    int temp_npe = 0;
    double temp_charges = 0.0;
    double mid_time;    
    if (v_temp_time.size() > 0){
      for (int i_hit=0; i_hit < v_temp_time.size(); i_hit++){
        double hit1 = v_temp_time.at(i_hit);
        if (temp_times.size()==0) {
          temp_times.push_back(hit1);
          temp_charges += v_temp_charge.at(i_hit);
          temp_npe++;
        }        
        else {
          bool new_pulse = false;
          if (fabs(temp_times[0]-hit1) < clusterSettings.datalikeIntegrationWindow) {
            new_pulse = false;
            temp_charges += v_temp_charge.at(i_hit);
            temp_npe++;
            temp_times.push_back(hit1);
          }
          else new_pulse=true;
          if (new_pulse) {
            // following the DigitBuilder tool --> take median photon hit time as the hit time of the "pulse"
            if (temp_times.size() % 2 == 0){
              mid_time = (temp_times.at(temp_times.size()/2 - 1) + temp_times.at(temp_times.size()/2))/2.0;
            }
            else{
              mid_time = temp_times.at(temp_times.size()/2);
            }

            v_datalike_time.push_back(mid_time);
            v_datalike_charge.push_back(temp_charges);
            v_datalike_npe.push_back(temp_npe);
            v_datalike_pmtid.push_back( mcpmt->GetID());           
            v_hittimes.push_back(mid_time);
            
            temp_times.clear();
            temp_charges = 0.0;
            temp_npe = 0;
            temp_times.push_back(hit1);
            temp_charges += v_temp_charge.at(i_hit);
          }
        }
      }

      if (temp_times.size() % 2 == 0){
        mid_time = (temp_times.at(temp_times.size()/2 - 1) + temp_times.at(temp_times.size()/2))/2;
      }
      else{
        mid_time = temp_times.at(temp_times.size()/2);
      }
      v_datalike_time.push_back(mid_time);
      v_datalike_charge.push_back(temp_charges);
      v_datalike_npe.push_back(temp_npe);
      v_datalike_pmtid.push_back(mcpmt->GetID());           
      v_hittimes.push_back(mid_time);
      
    }
  }

  //TODO: Maybe we can have the situation, that no hits and therefore no cluster is found...
  if (v_hittimes.size() == 0) {
    numClusters = 0;
    clusterCharge = 0;
    clusterChargeBalance = 0;
    clusterNPE = 0;
    clusterTime = 0;
    numClusteredPMTHits = 0;
    return false;
  }

  //Why does WCSim use the below method instead of a simple std::sort(v_hittimes.begin(),v_hittimes.end()); ?
  // Now sort the hit time array, fill the highest time in a new array until the old array is empty
  do {
    double max_time = -9999;
    int i_max_time = 0;
    for (std::vector<double>::iterator it = v_hittimes.begin(); it != v_hittimes.end(); ++it) {
      if (*it > max_time) {
        max_time = *it;
        i_max_time = std::distance(v_hittimes.begin(),it);
      } 
    }
    v_hittimes_sorted.insert(v_hittimes_sorted.begin(),max_time);
    v_hittimes.erase(v_hittimes.begin() + i_max_time);
  } while (v_hittimes.size() != 0);
    

  // Move a time window within the array and look for the window with the highest number of hits
  for (std::vector<double>::iterator it = v_hittimes_sorted.begin(); it != v_hittimes_sorted.end(); ++it) {
    if (*it + clusterSettings.clusterFindingWindow > clusterSettings.acqTimeWindow || *it > clusterSettings.end_of_window_time_cut*clusterSettings.acqTimeWindow) {
      break;
    }
    v_mini_hits.clear();
    for (double j_time = *it; j_time < *it + clusterSettings.clusterFindingWindow; j_time+=1){  // loops through times in the window and check if there's a hit at this time
      for(std::vector<double>::iterator it2 = v_hittimes_sorted.begin(); it2 != v_hittimes_sorted.end(); ++it2) {
        if (static_cast<int>(j_time) == static_cast<int>(*it2)) {     // accept all hit times (some may be smeared to negative values)
          v_mini_hits.push_back(*it2);
        }
      }
    }
    if (!v_mini_hits.empty()) {
      m_time_Nhits.insert(std::pair<double,std::vector<double>>(*it,v_mini_hits)); // fill a map with a pair (window start time; vector of hit times in window)  
    }
  }
  v_mini_hits.clear();

  // Now loop on the time/Nhits map to find maxima (clusters)
  double dummy_hittime_value = -6666.0;
  int max_Nhits = 0;
  double local_cluster = 0;
  v_clusters.clear();
  do {
    //cout << "Start do loop" << endl;
    max_Nhits = 0;
    for (std::map<double,std::vector<double> >::iterator it = m_time_Nhits.begin(); it != m_time_Nhits.end(); ++it) {
      if (int(it->second.size()) > max_Nhits) {
        max_Nhits = it->second.size();
        local_cluster = it->first;
      } 
    }
    if (max_Nhits < clusterSettings.minHitsPerCluster) {
      break;
    }
    else {
      //if (verbose > 0) cout << "Cluster found at " << local_cluster << " ns with " << max_Nhits << " hits" << endl;
      v_clusters.push_back(local_cluster);
      // Remove the cluster and its surroundings for the next loop over the cluster map
      for (std::map<double,std::vector<double>>::iterator it = m_time_Nhits.begin(); it != m_time_Nhits.end(); ++it) {
        //cout << "On the map hits: time " << it->first << " and hits tot " << it->second.size() << endl;
        //cout << "Look at the back of the vector (between): " << it->second.back() << endl;
        for (std::vector<double>::iterator itt = it->second.begin(); itt != it->second.end(); ++itt) {
          //cout << "hits are " << *itt << endl;
          if (*itt >= local_cluster && *itt <= local_cluster + clusterSettings.clusterFindingWindow) {
            //if hit time is in the window, replace it with dummy value to be removed later
            it->second.at(std::distance(it->second.begin(), itt)) = dummy_hittime_value;
          }
        }
        //cout << "Loop of setting dummy hit time values is done..." << endl;
        //cout << "Before erasing values, vector of hits is " << it->second.size() << " hits long" << endl;
        //cout << "Look at the back of the vector (after): " << it->second.back() << endl;

        // This loops erases the dummy hit times values that were flagged before so they are not used anymore by other clusters
        for(std::vector<double>::iterator itt = it->second.end()-1; itt != it->second.begin()-1; --itt) {
          if (*itt == dummy_hittime_value) {
            it->second.erase(it->second.begin() + std::distance(it->second.begin(), itt)); 
          }
        }
      }
    }
  } while (true); 
  m_time_Nhits.clear();

  numClusters = v_clusters.size();
  // Now loop on the hit map again to get info about those local maxima, cluster per cluster
  // We only take the cluster with the largest total charge. This is to avoid vector<vector<double> > structures, 
  // which is hard to read int. 
  // Alternatively it is also possible to write out another TTree with just the clsuter information, where each cluster is an event
  double max_cluster_charge = 0.0;
  for (std::vector<double>::iterator it = v_clusters.begin(); it != v_clusters.end(); ++it) {
    double local_cluster_charge = 0;
    double local_cluster_time = 0;
    double local_cluster_cb = 0.0;
    int local_cluster_npe = 0;
    v_local_cluster_times.clear();
    std::vector<int> local_clusterHitsPMTID;
    std::vector<double> local_clusterHitsPMTTime;
    std::vector<int> local_clusterHitsNPE;
    std::vector<double> local_clusterHitsPMTCharge;

    //TODO: ADD somtehing like this from WCSim: "if (thistube->GetDetectorElement()=="Tank")"
    for(int ihit = 0; ihit < v_datalike_time.size(); ihit++){
      if (v_datalike_time[ihit] >= *it && v_datalike_time[ihit] <= *it + clusterSettings.clusterFindingWindow) {
        local_cluster_charge += v_datalike_charge[ihit];
        local_cluster_npe += v_datalike_npe[ihit];
        local_cluster_cb += v_datalike_charge[ihit]*v_datalike_charge[ihit];
        v_local_cluster_times.push_back(v_datalike_time[ihit]);

        local_clusterHitsPMTID.push_back(v_datalike_pmtid[ihit]);
        local_clusterHitsPMTTime.push_back(v_datalike_time[ihit]);
        local_clusterHitsNPE.push_back(v_datalike_npe[ihit]);
        local_clusterHitsPMTCharge.push_back(v_datalike_charge[ihit]);
      }
    }
    for (std::vector<double>::iterator itt = v_local_cluster_times.begin(); itt != v_local_cluster_times.end(); ++itt) {
     local_cluster_time += *itt;
    }
    local_cluster_time /= v_local_cluster_times.size();
    local_cluster_cb = sqrt(local_cluster_cb/(local_cluster_charge*local_cluster_charge) - 1.0/121.0 );

    if(max_cluster_charge < local_cluster_charge){
      max_cluster_charge = local_cluster_charge;
      clusterCharge = local_cluster_charge;
      clusterChargeBalance = local_cluster_cb;
      clusterNPE = local_cluster_npe;
      clusterTime = local_cluster_time;

      numClusteredPMTHits = local_clusterHitsPMTID.size();
      clusterHitsPMTID.resize(numClusteredPMTHits);
      clusterHitsPMTTime.resize(numClusteredPMTHits);
      clusterHitsNPE.resize(numClusteredPMTHits);
      clusterHitsPMTCharge.resize(numClusteredPMTHits); 
      for(int iCHit = 0; iCHit < numClusteredPMTHits; iCHit++){
        clusterHitsPMTID[iCHit] = local_clusterHitsPMTID[iCHit];
        clusterHitsPMTTime[iCHit] = local_clusterHitsPMTTime[iCHit];
        clusterHitsNPE[iCHit] = local_clusterHitsNPE[iCHit];
        clusterHitsPMTCharge[iCHit] = local_clusterHitsPMTCharge[iCHit];    
      }
     
    }

    //if (verbose > 0) cout << "Local cluster at " << local_cluster_time << " ns with a total charge of " << local_cluster_charge << " (" << v_local_cluster_times.size() << " hits)" << endl;
  }

  return true;
}

}  // namespace RAT