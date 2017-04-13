/********************************************************
 * JetCalibrator:
 *
 * This class handles jet calibration and systematics
 *
 * Jeff Dandoy (jeff.dandoy@cern.ch)
 *
 ********************************************************/

#ifndef xAODAnaHelpers_JetCalibrator_H
#define xAODAnaHelpers_JetCalibrator_H

// CP interface includes
#include "PATInterfaces/SystematicRegistry.h"
#include "PATInterfaces/SystematicSet.h"
#include "PATInterfaces/SystematicVariation.h"

// external tools include(s):
#include "JetCalibTools/JetCalibrationTool.h"
#include "JetUncertainties/JetUncertaintiesTool.h"
#include "JetResolution/JERTool.h"
#include "JetResolution/JERSmearingTool.h"
#include "JetSelectorTools/JetCleaningTool.h"
#include "JetMomentTools/JetVertexTaggerTool.h"
#include "AsgTools/AnaToolHandle.h"
#include "JetTileCorrection/JetTileCorrectionTool.h"

// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"

/** @rst
  A wrapper to a few JetETMiss packages. By setting the configuration parameters detailed in the header documentation, one can:

    - calibrate a given jet collection
    - apply systematic variations for JES
    - apply systematic variations for JER
    - decorate the jet with the decision of the Jet Cleaning tool

  When considering systematics, a new ``xAOD::JetCollection`` is created for each systematic variation. The names are then saved in a vector for downstream algorithms to use.

@endrst */
class JetCalibrator : public xAH::Algorithm
{
public:
  /// @brief The name of the input container for this algorithm to read from ``TEvent`` or ``TStore``
  std::string m_inContainerName;
  /**
      @brief The name of the nominal output container written by the algorithm to ``TStore``

      If the algorithm applies systematic variations, for each shallow copy saved to ``TStore``, the systematic name will be appended to this.
  */
  std::string m_outContainerName;

  /// @brief set to ``AntiKt4EMTopo`` for ``AntiKt4EMTopoJets``
  std::string m_jetAlgo;
  /// @brief name of vector holding names of jet systematics given by the JetEtmiss Tools
  std::string m_outputAlgo;
  /// @brief config for JetCalibrationTool for Data
  std::string m_calibConfigData;
  /// @brief config for JetCalibrationTool for Full Sim MC
  std::string m_calibConfigFullSim;
  /// @brief config for JetCalibrationTool for AFII MC
  std::string m_calibConfigAFII;
  /// @brief config files actually passed to JetCalibrationTool chosen from the above depending on what information stored in the input file
  std::string m_calibConfig;
  /// @brief List of calibration steps. "Insitu" added automatically if running on data
  std::string m_calibSequence;
  /// @brief config for JES Uncertainty Tool
  std::string m_JESUncertConfig;
  /// @brief JetUncertaintiesTool parameter
  std::string m_JESUncertMCType;
  /** @rst
    If you do not want to use SampleHandler to mark samples as AFII, this flag can be used to force run the AFII configurations.

    With SampleHandler, one can define sample metadata in job steering macro. You can do this with relevant samples doing something like:

    .. code-block:: c++

      // access a single sample
      Sample *sample = sh.get ("mc14_13TeV.blahblahblah");
      sample->setMetaString("SimulationFlavour", "AFII");

  @endrst */
  bool m_setAFII;
  bool m_forceInsitu;

  /// @brief whether the jet collection is trigger or not (soon: different calibrations)
  bool m_isTrigger;

  // @brief Config for JER Uncert Tool. If not empty the tool will run
  std::string m_JERUncertConfig;
  /// @brief Set systematic mode as Full (true) or Simple (false)
  bool m_JERFullSys;
  /// @brief Apply nominal smearing
  bool m_JERApplyNominal;

  /// @brief enable to apply jet cleaning decoration
  bool m_doCleaning;
  /// @brief Cut Level
  std::string m_jetCleanCutLevel;
  /// @brief Save all cleaning decisions as decorators
  bool m_saveAllCleanDecisions;
  /// @brief Do Ugly cleaning ( i.e. TileGap 3 )
  bool m_jetCleanUgly;
  /// @brief Recalculate JVT using the calibrated jet pT
  bool m_redoJVT;
  /// @brief Sort the processed container elements by transverse momentum
  bool    m_sort;
  /// @brief Apply jet cleaning to parent jet
  bool    m_cleanParent;
  bool    m_applyFatJetPreSel;

// systematics
  /// @brief set to true if systematics asked for and exist
  bool m_runSysts;

  /// @brief jet tile correction
  bool m_doJetTileCorr;

private:
  int m_numEvent;         //!
  int m_numObject;        //!

  bool m_isMC;            //!
  bool m_isFullSim;       //!

  std::vector<CP::SystematicSet> m_systList; //!
  std::vector<int> m_systType; //!

  // tools
  asg::AnaToolHandle<IJetCalibrationTool> m_JetCalibrationTool_handle; //!
  asg::AnaToolHandle<ICPJetUncertaintiesTool> m_JetUncertaintiesTool_handle; //!

  asg::AnaToolHandle<IJERTool> m_JERTool_handle;    //!
  asg::AnaToolHandle<IJERSmearingTool> m_JERSmearingTool_handle;    //!

  asg::AnaToolHandle<IJetUpdateJvt> m_JVTUpdateTool_handle; //!

  asg::AnaToolHandle<IJetSelector> m_JetCleaningTool_handle; //!
  std::vector<std::string>  m_decisionNames;    //!
  std::vector< asg::AnaToolHandle<IJetSelector> > m_AllJetCleaningTool_handles;   //!

  asg::AnaToolHandle<CP::IJetTileCorrectionTool> m_JetTileCorrectionTool_handle; //!


public:

  // this is a standard constructor
  JetCalibrator (std::string className = "JetCalibrator");

  // these are the functions inherited from Algorithm
  virtual EL::StatusCode setupJob (EL::Job& job);
  virtual EL::StatusCode fileExecute ();
  virtual EL::StatusCode histInitialize ();
  virtual EL::StatusCode changeInput (bool firstFile);
  virtual EL::StatusCode initialize ();
  virtual EL::StatusCode execute ();
  virtual EL::StatusCode postExecute ();
  virtual EL::StatusCode finalize ();
  virtual EL::StatusCode histFinalize ();

  /// @cond
  // this is needed to distribute the algorithm to the workers
  ClassDef(JetCalibrator, 1);
  /// @endcond

};

#endif
