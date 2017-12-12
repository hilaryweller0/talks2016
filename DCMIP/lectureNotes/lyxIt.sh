#!/bin/bash -e

file=Weller

# create the test version
sed 's/%VERSION/\\testversion/g' studentVersionTMP > studentVersion.tex
lyx --export pdflatex -f all ${file}.lyx
lualatex ${file}.tex
bibtex ${file}
lualatex ${file}.tex
lualatex ${file}.tex
mv ${file}.pdf output/${file}_test.pdf


# create the online version
sed 's/%VERSION/\\onlineversion/g' studentVersionTMP > studentVersion.tex
lyx --export pdflatex -f all ${file}.lyx
lualatex ${file}.tex
bibtex ${file}
lualatex ${file}.tex
lualatex ${file}.tex
mv ${file}.pdf output/${file}_online.pdf

# create the students version
sed 's/%VERSION/\\studentversion/g' studentVersionTMP > studentVersion.tex
lyx --export pdflatex -f all ${file}.lyx
lualatex ${file}.tex
bibtex ${file}
lualatex ${file}.tex
lualatex ${file}.tex
pdfjam --nup 1x2 --scale 0.95 --a4paper $file.pdf --outfile output/${file}_2_student.pdf

# create the lecturers version
cp studentVersionTMP studentVersion.tex
lyx --export pdflatex -f all ${file}.lyx
lualatex ${file}.tex
bibtex ${file}
lualatex ${file}.tex
lualatex ${file}.tex
pdfjam --nup 1x2 --scale 0.95 --a4paper $file.pdf --outfile output/${file}_2_lec.pdf

rmtexall ${file}.tex


