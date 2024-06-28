### NOTE: Script still WIP, with these cuts have very poor background rejection
### Likely want to add more jet variables & perform additional or completely different cuts

#Input directory where the files produced at the stage1 level are
#inputDir  = "/eos/experiment/fcc/ee/analyses/case-studies/bsm/LLPs/H_SS_4b/Reco_output_stage1/"
#inputDir  = "/eos/experiment/fcc/ee/analyses_storage/BSM/LLPs/ExoticHiggsDecays/Reco_output_stage1/"
#inputDir = "Reco_output_stage1/exoticHiggs_scalar_ms20GeV_sine-5/"
#inputDir = "/eos/experiment/fcc/ee/analyses/case-studies/bsm/LLPs/H_SS_4b/Reco_output_stage1"
#inputDir = "/eos/experiment/fcc/ee/analyses/case-studies/bsm/LLPs/exoticHiggsSamplesCLD/Reco_output_stage1_H"
inputDir = "/eos/user/m/mlarson/Reco_output_stage1_H"
#Output directory where the files produced at the final-selection level are
outputDir = "Reco_output_final_H/"


# # #Integrated luminosity for scaling number of events (required only if setting doScale to true)
intLumi = 5.0e6 #pb^-1

# # #Scale event yields by intLumi and cross section (optional)
doScale = True

# # #Save event yields in a table (optional)
# saveTabular = True

#Mandatory: List of processes
processList = {

        #privately-produced signals
        'exoticHiggs_scalar_ms20GeV_sine-5_H':{},
        'exoticHiggs_scalar_ms20GeV_sine-6_H':{},
        'exoticHiggs_scalar_ms20GeV_sine-7_H':{},
        'exoticHiggs_scalar_ms60GeV_sine-5_H':{},
        'exoticHiggs_scalar_ms60GeV_sine-6_H':{},
        'exoticHiggs_scalar_ms60GeV_sine-7_H':{},

        # #centrally produced backgrounds
        'p8_ee_ZZ_ecm240':{},   
        'p8_ee_WW_ecm240':{},   
        'wzp6_ee_ccH_HWW_ecm240':{},   
        'wzp6_ee_qqH_HWW_ecm240':{},   
        'wzp6_ee_bbH_HWW_ecm240':{},   
        'wzp6_ee_ssH_HWW_ecm240':{},   
        'wzp6_ee_ssH_Hbb_ecm240':{},   
        'wzp6_ee_qqH_Hbb_ecm240':{},   
        'wzp6_ee_bbH_Hbb_ecm240':{},   
        'wzp6_ee_ccH_Hbb_ecm240':{}   

}

###Dictionary for prettier names of processes (optional)
processLabels = {
    #signals
    'exoticHiggs_scalar_ms20GeV_sine-5_H': "$m_S$ = 20 GeV, sin $\theta = 1 * 10^{-5}$",
    'exoticHiggs_scalar_ms20GeV_sine-6_H': "$m_S$ = 20 GeV, sin $\theta = 1 * 10^{-6}$",
    'exoticHiggs_scalar_ms20GeV_sine-7_H': "$m_S$ = 20 GeV, sin $\theta = 1 * 10^{-7}$",
    'exoticHiggs_scalar_ms60GeV_sine-5_H': "$m_S$ = 60 GeV, sin $\theta = 1 * 10^{-5}$",
    'exoticHiggs_scalar_ms60GeV_sine-6_H': "$m_S$ = 60 GeV, sin $\theta = 1 * 10^{-6}$",
    'exoticHiggs_scalar_ms60GeV_sine-7_H': "$m_S$ = 60 GeV, sin $\theta = 1 * 10^{-7}$",

    # #backgrounds
    'p8_ee_WW_ecm240': "e^{-}e^{+} $\rightarrow$ WW",
    'p8_ee_ZZ_ecm240': "e^{-}e^{+} $\rightarrow$ ZZ",
    'p8_ee_ZH_ecm240': "e^{-}e^{+} $\rightarrow$ ZH",
    'p8_ee_ZZ_ecm240': "e^{-}e^{+} $\rightarrow$ WW",
    'p8_ee_WW_ecm240': "e^{-}e^{+} $\rightarrow$ ZZ",
    'wzp6_ee_ccH_HWW_ecm240': "e^{-}e^{+} $\rightarrow$ ZH, Z $\rightarrow$ cc, H $\rightarrow$ WW",
    'wzp6_ee_qqH_HWW_ecm240': "e^{-}e^{+} $\rightarrow$ ZH, Z $\rightarrow$ 1/2(uu + dd), H $\rightarrow$ WW",
    'wzp6_ee_bbH_HWW_ecm240': "e^{-}e^{+} $\rightarrow$ ZH, Z $\rightarrow$ bb, H $\rightarrow$ WW",
    'wzp6_ee_ssH_HWW_ecm240': "e^{-}e^{+} $\rightarrow$ ZH, Z $\rightarrow$ ss, H $\rightarrow$ WW",
    'wzp6_ee_ssH_Hbb_ecm240': "e^{-}e^{+} $\rightarrow$ ZH, Z $\rightarrow$ ss, H $\rightarrow$ bb",
    'wzp6_ee_qqH_Hbb_ecm240': "e^{-}e^{+} $\rightarrow$ ZH, Z $\rightarrow$ 1/2(uu + dd), H $\rightarrow$ bb",
    'wzp6_ee_bbH_Hbb_ecm240': "e^{-}e^{+} $\rightarrow$ ZH, Z $\rightarrow$ bb, H $\rightarrow$ bb",
    'wzp6_ee_ccH_Hbb_ecm240': "e^{-}e^{+} $\rightarrow$ ZH, Z $\rightarrow$ cc, H $\rightarrow$ bb"
}

#Link to the dictonary that contains all the cross section information etc...
procDict = "FCCee_procDict_winter2023_IDEA.json"

#Add MySample_p8_ee_ZH_ecm240 as it is not an offical process
#OBS NUMBEROFEVENTS AND SUMOFWEIGHTS HAS BEEN MODIFIED FOR DEBUGGING, REMEMBER TO CHANGE BACK
procDictAdd={
    'exoticHiggs_scalar_ms20GeV_sine-5_H': {"numberOfEvents": 10000, "sumOfWeights": 10000, "crossSection": 1.5943e-4, "kfactor": 1.0, "matchingEfficiency": 1.0}, # NOTE cross sections use updated kappa value of 7e-4, which was used for sample generation 
    'exoticHiggs_scalar_ms20GeV_sine-6_H': {"numberOfEvents": 10000, "sumOfWeights": 10000, "crossSection": 1.5943e-4, "kfactor": 1.0, "matchingEfficiency": 1.0},
    'exoticHiggs_scalar_ms20GeV_sine-7_H': {"numberOfEvents": 10000, "sumOfWeights": 10000, "crossSection": 1.5943e-4, "kfactor": 1.0, "matchingEfficiency": 1.0},
    'exoticHiggs_scalar_ms60GeV_sine-5_H': {"numberOfEvents": 10000, "sumOfWeights": 10000, "crossSection": 4.7134e-5, "kfactor": 1.0, "matchingEfficiency": 1.0},
    'exoticHiggs_scalar_ms60GeV_sine-6_H': {"numberOfEvents": 10000, "sumOfWeights": 10000, "crossSection": 4.7134e-5, "kfactor": 1.0, "matchingEfficiency": 1.0},
    'exoticHiggs_scalar_ms60GeV_sine-7_H': {"numberOfEvents": 10000, "sumOfWeights": 10000, "crossSection": 4.7134e-5, "kfactor": 1.0, "matchingEfficiency": 1.0},
}

#Number of CPUs to use
nCPUS = 2

#produces ROOT TTrees, default is False
doTree = False

###Dictionnay of the list of cuts. The key is the name of the selection that will be added to the output file
cutList = {
    # For plotting
    #"selNone": "n_tracks > -1",

    # For event selection
    ### preSel "done" at reco stage by clustering 2 jets 
    "selJetEnergy": "((jets_kt_e_sum > 50) && (jets_kt_e_sum < 140))",
    "selJetEnergy+nDVs_seltracks": "((jets_kt_e_sum > 50) && (jets_kt_e_sum < 140)) && filter_n_DVs_seltracks > 1",
    "selJetEnergy+nDVs_merge": "((jets_kt_e_sum > 50) && (jets_kt_e_sum < 140)) && filter_n_DVs_merge > 1"
}

###Dictionary for prettier names of cuts (optional)
cutLabels = {
    # For plotting
    #"selNone": "Before selection",

    # For event selection
    ### preSel "done" at reco stage by clustering 2 jets 
    "selJetEnergy": "50 < $E_jet1 + E_jet2$ < 140 GeV",
    "selJetEnergy+nDVs_seltracks": "n DVs $\geq$ 2",
    "selJetEnergy+nDVs_merge": "n DVs $\geq$ 2 (merged)",
}

###Dictionary for the ouput variable/hitograms. The key is the name of the variable in the output files. "name" is the name of the variable in the input file, "title" is the x-axis label of the histogram, "bin" the number of bins of the histogram, "xmin" the minimum x-axis value and "xmax" the maximum x-axis value.

histoList = {
    "n_tracks":                             {"name":"n_tracks",                 "title":"Number of reconstructed tracks",                          "bin":100, "xmin":-0.5,"xmax":99.5},
    "n_RecoedPrimaryTracks":                {"name":"n_RecoedPrimaryTracks",    "title": "Number of reconstructed primary tracks",                 "bin":10, "xmin":-0.5,"xmax":9.5},
    'n_seltracks_DVs':                      {"name":"n_seltracks_DVs",           "title":"Number of DVs",                                           "bin":12, "xmin":-0.5, "xmax":11.5},
    'n_trks_seltracks_DVs':                 {"name":'n_trks_seltracks_DVs',       "title":"Number of tracks from the DVs",                          "bin":30, "xmin":1.5, "xmax":29.5},
    'invMass_seltracks_DVs':                {"name":'invMass_seltracks_DVs',      "title":"Invariant mass at the DVs [GeV]",                           "bin":40, "xmin":-0.5, "xmax":39.5},
    "DV_evt_seltracks_chi2":                {"name":"DV_evt_seltracks_chi2",    "title":"The #chi^{2} distribution of the DVs",                    "bin":100, "xmin":-0.5, "xmax":11.5},
    "Reco_seltracks_DVs_Lxy":               {"name":"Reco_seltracks_DVs_Lxy",     "title":"Transverse distance between PV and DVs [mm]",               "bin":100, "xmin":0, "xmax":300},
    "Reco_seltracks_DVs_Lxyz":              {"name":"Reco_seltracks_DVs_Lxyz",    "title":"Distance between PV and DVs [mm]",                          "bin":100, "xmin":0, "xmax":2000},
    "DV_evt_seltracks_normchi2":            {"name":"DV_evt_seltracks_normchi2",    "title":"The normalised #chi^{2} distribution of the DVs",      "bin":40, "xmin":0, "xmax":10},
    "merged_DVs_n":                         {"name":"merged_DVs_n",              "title":"Number of DVs",                                           "bin":10, "xmin":-0.5, "xmax":9.5},
    'n_trks_merged_DVs':                    {"name":'n_trks_merged_DVs',       "title":"Number of tracks from the DVs from sel tracks + merge",   "bin":30, "xmin":1.5, "xmax":29.5},
    'invMass_merged_DVs':                   {"name":'invMass_merged_DVs',      "title":"Invariant mass at the DVs [GeV]",                           "bin":40, "xmin":-0.5, "xmax":39.5},
    "merged_DVs_chi2":                      {"name":"merged_DVs_chi2",          "title":"The #chi^{2} distribution of the merged DVs",              "bin":100, "xmin":-0.5, "xmax":11.5},
    "merged_DVs_normchi2":                  {"name":"merged_DVs_normchi2",       "title":"The normalised #chi^{2} distribution of the merged DVs",    "bin":40, "xmin":0, "xmax":10},
    "Reco_DVs_merged_Lxy":                  {"name":"Reco_DVs_merged_Lxy",     "title":"Transverse distance between PV and DVs [mm]",               "bin":100, "xmin":0, "xmax":300},
    "Reco_DVs_merged_Lxyz":                 {"name":"Reco_DVs_merged_Lxyz",    "title":"Distance between PV and DVs [mm]",                          "bin":100, "xmin":0, "xmax":2000},

    #"jets_kt_e":                            {"name":"jets_kt_e",    "title":"Reco. Jet Energy [GeV]",                                               "bin":100, "xmin":0, "xmax":100},
    #"jets_kt_px":                           {"name":"jets_kt_px",    "title":"Reco. Jet Momenutm (along x-axis) [GeV]",                             "bin":100, "xmin":-50, "xmax":50},
    #"jets_kt_py":                           {"name":"jets_kt_py",    "title":"Reco. Jet Momenutm (along y-axis) [GeV]",                             "bin":100, "xmin":-50, "xmax":50},
    #"jets_kt_pz":                           {"name":"jets_kt_pz",    "title":"Reco. Jet Momenutm (along z-axis) [GeV]",                             "bin":100, "xmin":-50, "xmax":50},
    #"jets_kt_m":                            {"name":"jets_kt_m",    "title":"Reco. Jet Mass [GeV]",                                                 "bin":100, "xmin":0, "xmax":100},
    #"jetconstituents_kt":                   {"name":"jetconstituents_kt",    "title":"Jet Constituent Objects (showing # Reco. jets)",              "bin":20, "xmin":-10, "xmax":10},

    "jets_kt_e_sum":                        {"name":"jets_kt_e_sum",    "title":"Total Reco. Jet Energy [GeV]",                                     "bin":100, "xmin":0, "xmax":200},
    "jets_kt_px_sum":                       {"name":"jets_kt_px_sum",    "title":"Total Reco. Jet Momenutm (along x-axis) [GeV]",                   "bin":100, "xmin":-100, "xmax":100},
    "jets_kt_py_sum":                       {"name":"jets_kt_py_sum",    "title":"Total Reco. Jet Momenutm (along y-axis) [GeV]",                   "bin":100, "xmin":-100, "xmax":100},
    "jets_kt_pz_sum":                       {"name":"jets_kt_pz_sum",    "title":"Total Reco. Jet Momenutm (along z-axis) [GeV]",                   "bin":100, "xmin":-100, "xmax":100},

    "jets_kt_e_avg":                        {"name":"jets_kt_e_avg",    "title":"Avg. Reco. Jet Energy [GeV]",                                      "bin":100, "xmin":0, "xmax":100},
    "jets_kt_px_avg":                       {"name":"jets_kt_px_avg",    "title":"Avg. Reco. Jet Momenutm (along x-axis) [GeV]",                    "bin":100, "xmin":-50, "xmax":50},
    "jets_kt_py_avg":                       {"name":"jets_kt_py_avg",    "title":"Avg. Reco. Jet Momenutm (along y-axis) [GeV]",                    "bin":100, "xmin":-50, "xmax":50},
    "jets_kt_pz_avg":                       {"name":"jets_kt_pz_avg",    "title":"Avg. Reco. Jet Momenutm (along z-axis) [GeV]",                    "bin":100, "xmin":-50, "xmax":50},
    
}
