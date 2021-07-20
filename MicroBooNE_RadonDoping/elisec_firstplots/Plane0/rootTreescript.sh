#!/bin/bash
root -l -b << EOC
.L betaTree_Plane0.C
betaTree_Plane0 a;
a.Loop()
.q
EOC


