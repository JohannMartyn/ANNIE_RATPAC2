# ANNRI-Gd gamma-cascade generator

Gamma cascades of thermal neutron capture on Gd-155 and Gd-157, from the ANNRI-Gd collaboration.
Ported from the standalone ANNRI program (folder `ANNRIGdFeb2019`); used by `src/NeutronCapture.cc`
(macro switch `/annie/physics/annriGd`, on by default). Lookup tables: `ratdb/ANNRIGd/`.

## Condition of use

The authors state: "Both the source code and the input data files are open for general use in any
research, provided that our papers are referred to in your publications."

Every publication that uses results simulated with ANNRI-Gd enabled must cite:

1. Kaito Hagiwara, Takatomi Yano, Tomoyuki Tanaka, Mandeep Singh Reen, Pretam Kumar Das, Sebastian Lorenz,
   Iwa Ou, Takashi Sudo, Yoshiyuki Yamada, Takaaki Mori, Tsubasa Kayano, Rohit Dhir, Yusuke Koshio,
   Makoto Sakuda (Okayama), Atsushi Kimura, Shoji Nakamura, Nobuyuki Iwamoto, Hideo Harada (JAEA),
   Michael Wurm (Mainz), William Focillon, Michel Gonin (Ecole Polytechnique), Ajmi Ali,
   Gianmaria Collazuol (Padova) (ANNRI-Gd Collab.), Gamma-ray spectrum from thermal neutron capture on
   gadolinium-157, PTEP 2019, 023D01, 29pp. https://doi.org/10.1093/ptep/ptz002
2. T. Tanaka et al. (ANNRI-Gd Collab.), Gamma-ray spectra from thermal neutron capture on gadolinium-155
   and natural gadolinium, PTEP 2020, 043D02, 15pp. https://doi.org/10.1093/ptep/ptaa015

## Changes to the original code

`ANNRIGd_158GdContinuumModelV2.cc` and `ANNRIGd_158GdDiscreteModel.cc` wrote each cascade into global
variables of the standalone program (`NumGamma`, `GammaEnergies`, `outf`); those lines were removed.
All other files are unchanged. Not ported: the Geant4 10 glue (`GdNeutronHPCapture`,
`GdNeutronHPCaptureFS`), `ANNRIGd_GeneratorConfigurator`, `ANNRIGd_OutputConverter`, the ggarnet and
glg4sim models, and the ANNRI detector program.
