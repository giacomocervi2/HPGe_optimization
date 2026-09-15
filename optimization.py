import subprocess
import os
import numpy as np
import uproot
import lmfit

# =========================================================
# ISOTOPES DEFINITION
# =========================================================
Eu152 = {
      
    #most intense gamma lines (no 121 keV)
    "energies_keV": [244.70, 344.28, 778.90, 964.08, 1112.07, 1408.01], 
        
    #EXPERIMENTAL EFFICIENCIES AT THOSE ENERGIES
    "exp_eff": [0.007482, 0.005876, 0.002885, 0.002453, 0.002262, 0.001809], 

    #EXPERIMENAL EFFICIENCY ERRORS
    "exp_err": [0.00023544, 0.00017995, 0.00009159, 0.00007756, 0.00007218, 0.00005681],
        
    #Branching Ratios
    "BR": [0.0755, 0.2658, 0.1296, 0.1462, 0.1340, 0.2085] 
    
}

N_EVENTS = 500000
ROOT_OUTPUT = "output0.root" 

# =========================================================
# GEANT4 INTERFACE
# =========================================================
def run_geant4(p_dict):
    if os.path.exists(ROOT_OUTPUT):
        os.remove(ROOT_OUTPUT)
        
    with open("opt_run.mac", "w") as f:
        f.write(f"/mycmd/setDetLength {p_dict['det_length']} mm\n")
        f.write(f"/mycmd/setDetRadius {p_dict['det_radius']} mm\n")
        f.write(f"/mycmd/setHoleRadius {p_dict['hole_radius']} mm\n")
        f.write(f"/mycmd/setHoleLength {p_dict['hole_length']} mm\n")
        f.write("/mycmd/update\n")

        f.write("/run/initialize\n")
        f.write("/process/had/rdm/thresholdForVeryLongDecayTime 1.0e+3 year\n")
        f.write("/process/activate RadioactiveDecay\n")
        
        f.write("/gps/particle ion\n")
        f.write(f"/gps/ion 63 152 0 0\n")
        f.write("/gps/energy 0. keV\n") 

        f.write("/gps/pos/type Point\n")
        f.write("/gps/pos/centre -1.3 0.0 8.5 cm\n")
        f.write("/gps/pos/confine physPlastic\n")
        f.write("/gps/ang/type iso\n")
        
        f.write(f"/run/beamOn {N_EVENTS}\n")

    subprocess.run(["./build/sim", "opt_run.mac"], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

# =========================================================
# EFFICIENCY EXTRACTION
# =========================================================
def extract_efficiency(root_filename, energies_keV, br_list):

    effs = []

    try:
        with uproot.open(root_filename) as file:
            hist = file["Edep"]
            counts = hist.values()
            edges = hist.axes[0].edges() * 1000.0 #multiplication for 1000 since the Geant4 histo is in MeV

            for E, br in zip(energies_keV, br_list):
                peak_roi = (edges[:-1] >= E - 3.0) & (edges[1:] <= E + 3.0)
                
                peak_counts = np.sum(counts[peak_roi])
                
                effs.append(peak_counts / (N_EVENTS * br))
                
    except Exception as e:
        print(f"ROOT error: {e}")
        effs = [0.0] * len(energies_keV)
        
    return effs

# =========================================================
# MINIMIZATION
# =========================================================
def residuals(params):
    p_dict = params.valuesdict()
    print(f"Step -> Len:{p_dict['det_length']:.2f}, R_ext:{p_dict['det_radius']:.2f}, "
          f"Hole_R:{p_dict['hole_radius']:.2f}, Hole_L:{p_dict['hole_length']:.2f}")
    
    res = []
    
    run_geant4(p_dict)
        
    sim_effs = extract_efficiency(ROOT_OUTPUT, Eu152["energies_keV"], Eu152["BR"])
        
    for sim, exp, err in zip(sim_effs, Eu152["exp_eff"], Eu152["exp_err"]):
        res.append((sim - exp) / err)
            
    chi2 = np.sum(np.array(res)**2)
    print(f"--> Current Chi-squared: {chi2:.2f}")

    return np.array(res)

if __name__ == "__main__":
    params = lmfit.Parameters()
    params.add('det_length',  value=28.0, min=26.0, max=30.0)
    params.add('det_radius',  value=28.0, min=26.0, max=30.0)
    params.add('hole_radius', value=6.0,  min=2.0,  max=9.0)
    params.add('hole_length', value=22.0, vary=False)
    
    print("Starting geometric optimization...")
    
    
    result = lmfit.minimize(residuals, params, method='powell')
    
    print("\n====================================")
    print(lmfit.fit_report(result))
    
