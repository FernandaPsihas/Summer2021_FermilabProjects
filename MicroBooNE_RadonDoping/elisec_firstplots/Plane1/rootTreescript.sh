#!/bin/bash
root -l -b << EOC
.L betaTree_Plane1.C
betaTree_Plane1 a;
a.Loop()
.q
EOC


