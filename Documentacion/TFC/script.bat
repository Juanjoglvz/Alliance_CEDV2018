pdflatex -synctex=1 -interaction=nonstopmode TFC.tex > NUL
bibtex TFC.aux > NUL
pdflatex -synctex=1 -interaction=nonstopmode TFC.tex > NUL
pdflatex -synctex=1 -interaction=nonstopmode TFC.tex > NUL

DEL TFC.aux TFC.synctex.gz TFC.toc TFC.log TFC.bbl TFC.blg TFC.lof TFC.lol TFC.lot
explorer.exe "TFC.pdf"