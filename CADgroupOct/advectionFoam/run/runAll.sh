#!/bin/sh -e

rm -rf [0-9]* constant/polyMesh
blockMesh
add2dMountain
gmtFoam mesh
gv constant/mesh.pdf
setVelocityField
rm -r [1-9]*
cp init0/T 0
setScalarOverOrography
advectionFoamTest -explicit
gmtFoam T
eps2gif T.gif 0/T.pdf ????/T.pdf ?????/T.pdf &

