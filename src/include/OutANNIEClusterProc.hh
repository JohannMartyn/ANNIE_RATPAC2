#ifndef __RATOutANNIEClusterProc___
#define __RATOutANNIEClusterProc___

#include <RAT/DS/Run.hh>
#include <RAT/Processor.hh>
#include <functional>

class TFile;
class TTree;

namespace RAT {

class OutANNIEClusterProc : public Processor {
 public:
  static int run_num;
  OutANNIEClusterProc();
  virtual ~OutANNIEClusterProc();

  enum mc_pe_type { noise = 0, cherenkov = 1, scintillation = 2, reemission = 3, unknown = 4 };

  // file - string, name of file to open for output, file will be erased
  // updatefile - string, name of file to append to
  // (do not use both file and update file)
  // virtual void SetS(std::string param, std::string value);

  // autosave - integer, update root file every N kilobytes
  // savetree 0 - Do not save the event tree.  Must set *before* file or
  // updatefile.
  // virtual void SetI(std::string param, int value);

  virtual Processor::Result DSEvent(DS::Root *ds);

  virtual bool OpenFile(std::string theFilename);

  virtual void SetI(std::string param, int value);
  virtual void SetS(std::string param, std::string value);

  // Extensible functions
  virtual void AssignAdditionalAddresses(){};
  virtual void AssignAdditionalMetaAddresses(){};
  virtual void FillEvent(DS::Root *, DS::EV *){};
  virtual void FillNoTriggerEvent(DS::Root *){};
  virtual void FillMeta(){};

  // Exposed members for external tools
  DS::Run *runBranch;
  // Fill Functions
  struct NtupleOptions {
    bool tracking;
    bool mcparticles;
    bool pmthits;
    bool untriggered;
    bool mchits;
    bool clusteredhits;
  };
  NtupleOptions options;

  //Clustering
  struct ClusterSettings {
    int clusterFindingWindow;
    int acqTimeWindow;
    int clusterIntegrationWindow;
    int minHitsPerCluster;
    double end_of_window_time_cut;
    int datalikeIntegrationWindow;
  };
  ClusterSettings clusterSettings;

 protected:
  std::string defaultFilename;
  TFile *outputFile;
  TTree *outputTree;
  TTree *metaTree;
  // Meta Branches
  Int_t runId;
  ULong64_t runType;
  ULong64_t runTime;
  int dsentries;
  std::string macro;
  std::vector<int> pmtType;
  std::vector<int> pmtId;
  std::vector<double> pmtX;
  std::vector<double> pmtY;
  std::vector<double> pmtZ;
  std::vector<double> pmtU;
  std::vector<double> pmtV;
  std::vector<double> pmtW;
  // Data Branches
  Int_t mcpdg;
  double mcx, mcy, mcz;
  double mcu, mcv, mcw;
  double mcke;
  double mct;
  int evid;
  int subev;
  int nhits;
  double triggerTime;
  // MC Summary Information
  double scintEdep;
  double scintEdepQuenched;
  double scintPhotons;
  double remPhotons;
  double cherPhotons;
  // MCPMT
  int mcnhits;
  int mcpecount;
  std::vector<int> mcpmtid;
  std::vector<int> mcpmtnpe;
  std::vector<double> mcpmtcharge;
  // MCPE
  std::vector<double> mcpehittime;
  std::vector<double> mcpefrontendtime;
  std::vector<int> mcpeprocess;
  std::vector<double> mcpewavelength;
  std::vector<double> mcpex;
  std::vector<double> mcpey;
  std::vector<double> mcpez;
  std::vector<double> mcpecharge;
  // MCParticles
  int mcpcount;
  std::vector<Int_t> pdgcodes;
  std::vector<double> mcKEnergies;
  std::vector<double> mcPosx;
  std::vector<double> mcPosy;
  std::vector<double> mcPosz;
  std::vector<double> mcDirx;
  std::vector<double> mcDiry;
  std::vector<double> mcDirz;
  std::vector<double> mcTime;
  // Reconstruted variables
  std::vector<int> fitterId;
  std::vector<double> fitx, fity, fitz;
  std::vector<double> fitu, fitv, fitw;
  // Store PMT Hit Positions
  std::vector<int> hitPMTID;
  std::vector<double> hitPMTTime;
  std::vector<double> hitPMTCharge;
  std::vector<double> hitPMTDigitizedTime;
  std::vector<double> hitPMTDigitizedCharge;
  std::vector<int> hitPMTNCrossings;
  // Tracking
  std::map<std::string, int> processCodeMap;
  std::vector<int> processCodeIndex;
  std::vector<std::string> processName;

  std::vector<int> trackPDG;
  std::vector<std::vector<double>> trackPosX;
  std::vector<std::vector<double>> trackPosY;
  std::vector<std::vector<double>> trackPosZ;
  std::vector<std::vector<double>> trackMomX;
  std::vector<std::vector<double>> trackMomY;
  std::vector<std::vector<double>> trackMomZ;
  std::vector<std::vector<double>> trackKE;
  std::vector<std::vector<double>> trackTime;
  std::vector<std::vector<int>> trackProcess;

  std::set<std::string> branchNames;

  void SetBranchValue(std::string name, double *value);
  void SetBranchValue(std::string name, int *value);
  void SetBranchValue(std::string name, bool *value);

  //Adapted from ToolAnalysis ClusterFinder
  int numClusters;
  std::vector<double> clusterCharge;
  std::vector<double> clusterChargeBalance; //double charge_balance  = sqrt((total_QSquared)/(total_Q*total_Q) - (1./121.)); Change 121 to value from ratdb file
  std::vector<int> clusterNPE;
  std::vector<double> clusterTime;
  std::vector<int> numClusteredPMTHits;
  std::vector<std::vector<int> > clusterHitsPMTID;
  std::vector<std::vector<double> > clusterHitsPMTTime;
  std::vector<std::vector<int> > clusterHitsNPE;
  std::vector<std::vector<double> > clusterHitsPMTCharge;
  bool ClusterFinder(DS::MC *mc);
  //std::vector<std::vector<double> > v_datalike_time;
  //std::vector<std::vector<double> > v_datalike_charge;
  //std::vector<std::vector<double> > v_datalike_npe;
  std::vector<double> v_datalike_time;
  std::vector<double> v_datalike_charge;
  std::vector<int> v_datalike_npe;
  std::vector<int> v_datalike_pmtid;

  std::vector<double> v_hittimes;
  std::vector<double> v_hittimes_sorted;
  std::vector<double> v_mini_hits;
  std::map<double,std::vector<double> > m_time_Nhits;
  std::vector<double> v_clusters;
  std::vector<double> v_local_cluster_times;

};

}  // namespace RAT

#endif