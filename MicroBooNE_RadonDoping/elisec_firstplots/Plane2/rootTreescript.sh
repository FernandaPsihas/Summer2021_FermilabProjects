#!/bin/bash
root -l -b << EOC
.L betaTree_Plane2.C
betaTree_Plane2 a;
a.Loop()
.q
EOC


