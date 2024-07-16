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
    gInterpreter->GenerateDictionary("std::vector<std::vector<double> >", "vector");
    gInterpreter->GenerateDictionary("std::vector<std::vector<int> >", "vector");
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

/*
  void ClusterFinder(){
    

    if (verbose > 0) std::cout <<"Executing Tool ClusterFinder ..."<<endl;

    // get the ANNIEEvent

    int annieeventexists = m_data->Stores.count("ANNIEEvent");
    if(!annieeventexists){ cerr<<"no ANNIEEvent store!"<<endl;}
    
    std::map<unsigned long, std::vector<std::vector<ADCPulse>>> RecoADCHits;

    // Some initialization
    v_hittimes.clear();
    v_hittimes_sorted.clear();
    v_mini_hits.clear();
    m_time_Nhits.clear();
    v_clusters.clear();
    v_local_cluster_times.clear();
    m_all_clusters->clear();
    m_all_clusters_MC->clear();
    m_all_clusters_detkey->clear();

    //----------------------------------------------------------------------------
    //---------------get the members of the ANNIEEvent----------------------------
    //----------------------------------------------------------------------------
    
    m_data->Stores["ANNIEEvent"]->Get("EventNumber", evnum);
    //m_data->Stores["ANNIEEvent"]->Get("BeamStatus", BeamStatus);
    bool got_recoadc = m_data->Stores["ANNIEEvent"]->Get("RecoADCData",RecoADCHits);

    if (HitStoreName == "MCHits"){
      bool got_mchits = m_data->Stores["ANNIEEvent"]->Get("MCHits", MCHits);
      if (!got_mchits){
        std::cout << "No MCHits store in ANNIEEvent!" <<  std::endl;
        return false;
      }
    } else if (HitStoreName == "Hits"){
      bool got_hits = m_data->Stores["ANNIEEvent"]->Get("Hits", Hits);
      if (!got_hits){
        std::cout << "No Hits store in ANNIEEvent! " << std::endl;
        return false;
      }
    } else {
      std::cout << "Selected Hits store invalid.  Must be Hits or MCHits" << std::endl;
      return false;
    }
    // Also load hits from the Hits Store, if available


    //----------------------------------------------------------------------------
    //---------------Read out charge and hit values of PMTs-----------------------
    //----------------------------------------------------------------------------

    for (int i_pmt = 0; i_pmt < n_tank_pmts; i_pmt++){
      unsigned long detkey = pmt_detkeys[i_pmt];
      PMT_ishit[detkey] = 0;
    }

    if(HitStoreName=="MCHits"){
      int vectsize = MCHits->size();
      for(std::pair<unsigned long, std::vector<MCHit>>&& apair : *MCHits){
        unsigned long chankey = apair.first;
        Detector* thistube = geom->ChannelToDetector(chankey);
        int detectorkey = thistube->GetDetectorID();
        if (thistube->GetDetectorElement()=="Tank"){
          std::vector<MCHit>& ThisPMTHits = apair.second;
          PMT_ishit[detectorkey] = 1;
          std::vector<double> hits_2ns_res;
          std::vector<double> hits_2ns_res_charge;
          std::vector<double> datalike_hits;
          std::vector<double> datalike_hits_charge;
          for (MCHit &ahit : ThisPMTHits){
            if (ahit.GetTime() < clusterSettings.end_of_window_time_cut*clusterSettings.acqTimeWindow) {
              //Make MC more like data --> combine multiple photons if they are within a 10ns range
              hits_2ns_res.push_back(ahit.GetTime());
              hits_2ns_res_charge.push_back(ahit.GetCharge());
            }
          }
          //Combine multiple MC hits to one pulse
          std::sort(hits_2ns_res.begin(),hits_2ns_res.end());
          std::vector<double> temp_times;
          double temp_charges = 0.0;
          double mid_time;
          if (verbose > 0){
            std::cout << " " << std::endl;
            std::cout << hits_2ns_res.size() << " total photon hits(s)" << std::endl;
          }
    
          if (hits_2ns_res.size() > 0){
            for (int i_hit=0; i_hit < (int) hits_2ns_res.size(); i_hit++){
              double hit1 = hits_2ns_res.at(i_hit);
              if (temp_times.size()==0) {
                temp_times.push_back(hit1);
                temp_charges += hits_2ns_res_charge.at(i_hit);
              }        
              else {
                bool new_pulse = false;
                if (fabs(temp_times[0]-hit1)<10.) {
                  new_pulse=false;
                  temp_charges+=hits_2ns_res_charge.at(i_hit);
                  temp_times.push_back(hit1);
                }
                else new_pulse=true;
                if (new_pulse) {
                  // following the DigitBuilder tool --> take median photon hit time as the hit time of the "pulse"
                  if (temp_times.size() % 2 == 0){
                    mid_time = (temp_times.at(temp_times.size()/2 - 1) + temp_times.at(temp_times.size()/2))/2;
                  }
                  else{
                    mid_time = temp_times.at(temp_times.size()/2);
                  }

                  datalike_hits.push_back(mid_time);
                  datalike_hits_charge.push_back(temp_charges);
                  temp_times.clear();
                  temp_charges = 0;
                  temp_times.push_back(hit1);
                  temp_charges += hits_2ns_res_charge.at(i_hit);
                }
              }
            }

            if (temp_times.size() % 2 == 0){
              mid_time = (temp_times.at(temp_times.size()/2 - 1) + temp_times.at(temp_times.size()/2))/2;
            }
            else{
              mid_time = temp_times.at(temp_times.size()/2);
            }

            datalike_hits.push_back(mid_time);
            datalike_hits_charge.push_back(temp_charges);

            if (verbose > 0){
              std::cout << " " << std::endl;
              std::cout << datalike_hits.size() << " MC pulse(s) identified from the raw photon hit(s)" << std::endl;
              std::cout << "Pulse time(s):" << std::endl;
              for (int ih=0; ih < (int) datalike_hits.size(); ih++){
                double junk = datalike_hits.at(ih);
                std::cout << junk << " ";
              }
              std::cout << " " << std::endl;
              std::cout << "Pulse charge(s):" << std::endl;
              for (int ih=0; ih < (int) datalike_hits_charge.size(); ih++){
                double junk2 = datalike_hits_charge.at(ih);
                std::cout << junk2 << " ";
              }
              std::cout << " " << std::endl;
            }
          }

          for (int i_hit = 0; i_hit < (int) datalike_hits.size(); i_hit++){
            //v_hittimes.push_back(ahit.GetTime()); // fill a vector with all hit times (unsorted)
            v_hittimes.push_back(datalike_hits.at(i_hit));
          }
          std::vector<int> parents = *(ThisPMTHits.at(0).GetParents());
          ThisPMTHits.clear();
          std::vector<MCHit> newMCHits;
          for (int i_hit=0; i_hit < (int) datalike_hits.size(); i_hit++){
            newMCHits.push_back(MCHit(chankey,datalike_hits.at(i_hit),datalike_hits_charge.at(i_hit),parents));
          }
          MCHits->at(chankey) = newMCHits;
        }
      }
      m_data->Stores["ANNIEEvent"]->Set("MCHits",MCHits,true);
    }

    
    if (v_hittimes.size() == 0) {
      if (verbose > 1) cout << "No hits, event is skipped..." << endl;
        if (HitStoreName == "Hits") m_data->CStore.Set("ClusterMap",m_all_clusters);
        else if (HitStoreName == "MCHits") m_data->CStore.Set("ClusterMapMC",m_all_clusters_MC);
        m_data->CStore.Set("ClusterMapDetkey",m_all_clusters_detkey);
        return true;
    }

    if (verbose > 2) {
      for (std::vector<double>::iterator it = v_hittimes.begin(); it != v_hittimes.end(); ++it) {
        cout << "Hit time -> " << *it << endl;
      }
    }

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
    
    if (verbose > 2) {
      for (std::vector<double>::iterator it = v_hittimes_sorted.begin(); it != v_hittimes_sorted.end(); ++it) {
        cout << "Hit time (sorted) -> " << *it << endl;
      }
    }

    // Move a time window within the array and look for the window with the highest number of hits
    for (std::vector<double>::iterator it = v_hittimes_sorted.begin(); it != v_hittimes_sorted.end(); ++it) {
      if (*it + clusterSettings.clusterFindingWindow > clusterSettings.acqTimeWindow || *it > clusterSettings.end_of_window_time_cut*clusterSettings.acqTimeWindow) {
        if (verbose > 2) cout << "Cluster Finding loop: Reaching the end of the acquisition time window.." << endl;
        break;
      }
      thiswindow_Nhits = 0;   
      v_mini_hits.clear();
      for (double j_time = *it; j_time < *it + clusterSettings.clusterFindingWindow; j_time+=1){  // loops through times in the window and check if there's a hit at this time
        for(std::vector<double>::iterator it2 = v_hittimes_sorted.begin(); it2 != v_hittimes_sorted.end(); ++it2) {
          if(HitStoreName=="MCHits"){
            if (static_cast<int>(j_time) == static_cast<int>(*it2)) {     // accept all hit times (some may be smeared to negative values)
              thiswindow_Nhits++;
              v_mini_hits.push_back(*it2);
            }
          }
          if(HitStoreName=="Hits"){
            if (*it2 > 0 && static_cast<int>(j_time) == static_cast<int>(*it2)) {  // reject hit times in the data that are 0
              thiswindow_Nhits++;
              v_mini_hits.push_back(*it2);
            }
          }

        }
      }
      if (!v_mini_hits.empty()) {
        m_time_Nhits.insert(std::pair<double,std::vector<double>>(*it,v_mini_hits)); // fill a map with a pair (window start time; vector of hit times in window)  
      }
    }
    if (verbose > 1) cout << "Map of times and Nhits filled..." << endl;
    if (verbose > 2){
        for (std::map<double,std::vector<double>>::iterator it = m_time_Nhits.begin(); it != m_time_Nhits.end(); ++it) {
          if (int(it->second.size()) > clusterSettings.minHitsPerCluster) {
            if(verbose>3) cout << "Map of time and NHits: Time = " << it->first << ", NHits = " << it->second.size() << endl;
            if(verbose>3) cout << "Look at the back of the vector (before): " << it->second.back() << endl;
            for (std::vector<double>::iterator itt = it->second.begin(); itt != it->second.end(); ++itt) {
            cout << "At this time, hits are: " << *itt << endl;  
          }       
        }
      }
    }
    v_mini_hits.clear();

    // Now loop on the time/Nhits map to find maxima (clusters) 
    max_Nhits = 0;
    local_cluster = 0;
    v_clusters.clear();
    do {
      //cout << "Start do loop" << endl;
      max_Nhits = 0;
      for (std::map<double,std::vector<double>>::iterator it = m_time_Nhits.begin(); it != m_time_Nhits.end(); ++it) {
        if (int(it->second.size()) > max_Nhits) {
          max_Nhits = it->second.size();
          local_cluster = it->first;
        } 
      }
      if (max_Nhits < clusterSettings.minHitsPerCluster) {
        if (verbose > 1 ) cout << "No more clusters with > " << clusterSettings.minHitsPerCluster<< " hits" << endl; 
        break;
      } else {
        if (verbose > 0) cout << "Cluster found at " << local_cluster << " ns with " << max_Nhits << " hits" << endl;
        v_clusters.push_back(local_cluster);
        // Remove the cluster and its surroundings for the next loop over the cluster map
        for (std::map<double,std::vector<double>>::iterator it = m_time_Nhits.begin(); it != m_time_Nhits.end(); ++it) {
          //cout << "On the map hits: time " << it->first << " and hits tot " << it->second.size() << endl;
          //cout << "Look at the back of the vector (between): " << it->second.back() << endl;
          for (std::vector<double>::iterator itt = it->second.begin(); itt != it->second.end(); ++itt) {
            //cout << "hits are " << *itt << endl;
            if (*itt >= local_cluster && *itt <= local_cluster + clusterSettings.clusterFindingWindow) { //if hit time is in the window, replace it with dummy value to be removed later
              it->second.at(std::distance(it->second.begin(), itt)) = dummy_hittime_value;
            }
          }
          //cout << "Loop of setting dummy hit time values is done..." << endl;
          //cout << "Before erasing values, vector of hits is " << it->second.size() << " hits long" << endl;
          //cout << "Look at the back of the vector (after): " << it->second.back() << endl;

          // This loops erases the dummy hit times values that were flagged before so they are not used anymore by other clusters
          for(std::vector<double>::iterator itt = it->second.end()-1; itt != it->second.begin()-1; --itt) {
            if (verbose > 2) cout << "Time: " << it->first << ", hit time: " << *itt << endl;
            if (*itt == dummy_hittime_value) {
              it->second.erase(it->second.begin() + std::distance(it->second.begin(), itt)); 
              if (verbose > 2) cout << "Erasing " << it->first << " " << *itt << endl;
            }
          }
          if (verbose > 2) cout << "Erasing loop is done and new size of mini_hits is " << it->second.size() << " hits" << endl;
        }
      }
    } while (true); 
    m_time_Nhits.clear();

    // Now loop on the hit map again to get info about those local maxima, cluster per cluster
    for (std::vector<double>::iterator it = v_clusters.begin(); it != v_clusters.end(); ++it) {
      double local_cluster_charge = 0;
      double local_cluster_time = 0;
      v_local_cluster_times.clear();
      if (HitStoreName == "Hits"){
        for(std::pair<unsigned long, std::vector<Hit>>&& apair : *Hits){
          unsigned long chankey = apair.first;
          Detector* thistube = geom->ChannelToDetector(chankey);
          int detectorkey = thistube->GetDetectorID();
          if (thistube->GetDetectorElement()=="Tank"){
            std::vector<Hit>& ThisPMTHits = apair.second;
            PMT_ishit[detectorkey] = 1;
            for (Hit &ahit : ThisPMTHits){
              if (ahit.GetTime() >= *it && ahit.GetTime() <= *it + clusterSettings.clusterFindingWindow) {
                local_cluster_charge += ahit.GetCharge();
                v_local_cluster_times.push_back(ahit.GetTime());
                if (verbose > 2) cout << "Local cluster at " << *it << " and hit is " << ahit.GetTime() << endl;
              }
            }
          }
        }
      } else if (HitStoreName == "MCHits"){
        for(std::pair<unsigned long, std::vector<MCHit>>&& apair : *MCHits){
          unsigned long chankey = apair.first;
          Detector* thistube = geom->ChannelToDetector(chankey);
          int detectorkey = thistube->GetDetectorID();
          if (thistube->GetDetectorElement()=="Tank"){
            std::vector<MCHit>& ThisPMTHits = apair.second;
            PMT_ishit[detectorkey] = 1;
            for (MCHit &ahit : ThisPMTHits){
              if (ahit.GetTime() >= *it && ahit.GetTime() <= *it + clusterSettings.clusterFindingWindow) {
                local_cluster_charge += ahit.GetCharge();
                v_local_cluster_times.push_back(ahit.GetTime());
                if (verbose > 2) cout << "Local cluster at " << *it << " and hit is " << ahit.GetTime() << endl;
              }
            }
          }
        }
      }
      
      for (std::vector<double>::iterator itt = v_local_cluster_times.begin(); itt != v_local_cluster_times.end(); ++itt) {
       local_cluster_time += *itt;
      }
      local_cluster_time /= v_local_cluster_times.size();
      if (verbose > 0) cout << "Local cluster at " << local_cluster_time << " ns with a total charge of " << local_cluster_charge << " (" << v_local_cluster_times.size() << " hits)" << endl;
      h_Cluster_times->Fill(local_cluster_time);
      h_Cluster_charges->Fill(local_cluster_charge);
      if (draw_2D) h_Cluster_charge_time->Fill(local_cluster_time,local_cluster_charge);
      if (v_clusters.size() > 1) {
        h_Cluster_deltaT->Fill(local_cluster_time - *std::min_element(v_clusters.begin(),v_clusters.end()));
        if (draw_2D) h_Cluster_charge_deltaT->Fill(local_cluster_time - *std::min_element(v_clusters.begin(),v_clusters.end()),local_cluster_charge);
      }
      if (verbose > 2) cout << "Next cluster ..." << endl;

      // Fills the map of clusters (to be passed through CStore)
      if (HitStoreName == "Hits"){
        for(std::pair<unsigned long, std::vector<Hit>>&& apair : *Hits) {   
          unsigned long chankey = apair.first;
          Detector* thistube = geom->ChannelToDetector(chankey);
          unsigned long detectorkey = thistube->GetDetectorID();
          if (thistube->GetDetectorElement()=="Tank"){
            std::vector<Hit>& ThisPMTHits = apair.second;
            PMT_ishit[detectorkey] = 1;
            for (Hit &ahit : ThisPMTHits){
              if (ahit.GetTime() >= *it && ahit.GetTime() <= *it + clusterSettings.clusterFindingWindow) { 
                if(m_all_clusters->count(local_cluster_time)==0) {
                  m_all_clusters->emplace(local_cluster_time, std::vector<Hit>{ahit});
                  m_all_clusters_detkey->emplace(local_cluster_time, std::vector<unsigned long>{detectorkey});
                } else { 
                  m_all_clusters->at(local_cluster_time).push_back(ahit);
                  m_all_clusters_detkey->at(local_cluster_time).push_back(detectorkey);
                }
              }
            }  
          }
        }
      } else if (HitStoreName == "MCHits"){
        for(std::pair<unsigned long, std::vector<MCHit>>&& apair : *MCHits) {   
          unsigned long chankey = apair.first;
          Detector* thistube = geom->ChannelToDetector(chankey);
          unsigned long detectorkey = thistube->GetDetectorID();
          if (thistube->GetDetectorElement()=="Tank"){
            std::vector<MCHit>& ThisPMTHits = apair.second;
            PMT_ishit[detectorkey] = 1;
            for (MCHit &ahit : ThisPMTHits){
              if (ahit.GetTime() >= *it && ahit.GetTime() <= *it + clusterSettings.clusterFindingWindow) { 
                if(m_all_clusters_MC->count(local_cluster_time)==0) {
                  m_all_clusters_MC->emplace(local_cluster_time, std::vector<MCHit>{ahit});
                  m_all_clusters_detkey->emplace(local_cluster_time, std::vector<unsigned long>{detectorkey});
                } else { 
                  m_all_clusters_MC->at(local_cluster_time).push_back(ahit);
                  m_all_clusters_detkey->at(local_cluster_time).push_back(detectorkey);
                }
              }
            }  
          }
        }
      }
    }

    // Load the cluster map in a CStore for use by a subsequent tool
    if (HitStoreName == "Hits") m_data->CStore.Set("ClusterMap",m_all_clusters);
    else if (HitStoreName == "MCHits") m_data->CStore.Set("ClusterMapMC",m_all_clusters_MC);
    m_data->CStore.Set("ClusterMapDetkey",m_all_clusters_detkey);

    //check whether PMT_ishit is filled correctly
    for (int i_pmt = 0; i_pmt < n_tank_pmts ; i_pmt++){
      unsigned long detkey = pmt_detkeys[i_pmt];
      if (verbose > 2) std::cout <<"PMT "<<i_pmt<<" is hit: "<<PMT_ishit[detkey]<<std::endl;
    }    

  }
*/

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
      mcpmtid.push_back(mcpmt->GetID());
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
    clusterCharge.clear();
    clusterChargeBalance.clear();
    clusterNPE.clear();
    clusterTime.clear();
    numClusteredPMTHits.clear();
    clusterHitsPMTID.clear();
    clusterHitsPMTTime.clear();
    clusterHitsNPE.clear();
    clusterHitsPMTCharge.clear();
    this->ClusterFinder(mc);
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
        hitPMTID.push_back(pmt->GetID());
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
      pmtId.push_back(id);
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
  /*
  numClusters = 1;
  clusterCharge.push_back(1.0);
  clusterChargeBalance.push_back(1.0);
  clusterNPE.push_back(1.0);
  clusterTime.push_back(1.0);
  numClusteredPMTHits.push_back(1);

  std::vector<int> temp_clusterHitsPMTID;
  std::vector<double> temp_clusterHitsPMTTime;
  std::vector<int> temp_clusterHitsNPE;
  std::vector<double> temp_clusterHitsPMTCharge;

  temp_clusterHitsPMTID.push_back(1);
  temp_clusterHitsPMTTime.push_back(1.0);
  temp_clusterHitsNPE.push_back(1);
  temp_clusterHitsPMTCharge.push_back(1.0);

  clusterHitsPMTID.push_back(temp_clusterHitsPMTID);
  clusterHitsPMTTime.push_back(temp_clusterHitsPMTTime);
  clusterHitsNPE.push_back(temp_clusterHitsNPE);
  clusterHitsPMTCharge.push_back(temp_clusterHitsPMTCharge);
  */

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
            v_datalike_pmtid.push_back(mcpmt->GetID());           
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
    clusterCharge.push_back(0);
    clusterChargeBalance.push_back(0);
    clusterNPE.push_back(0);
    clusterTime.push_back(0);
    numClusteredPMTHits.push_back(0);
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

    clusterCharge.push_back(local_cluster_charge);
    clusterChargeBalance.push_back(local_cluster_cb);
    clusterNPE.push_back(local_cluster_npe);
    clusterTime.push_back(local_cluster_time);
    numClusteredPMTHits.push_back(local_clusterHitsPMTID.size());
    clusterHitsPMTID.push_back(local_clusterHitsPMTID);
    clusterHitsPMTTime.push_back(local_clusterHitsPMTTime);
    clusterHitsNPE.push_back(local_clusterHitsNPE);
    clusterHitsPMTCharge.push_back(local_clusterHitsPMTCharge);
    //if (verbose > 0) cout << "Local cluster at " << local_cluster_time << " ns with a total charge of " << local_cluster_charge << " (" << v_local_cluster_times.size() << " hits)" << endl;
  }

  return true;
}

}  // namespace RAT