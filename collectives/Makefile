all: simple_analysis.pdf

%.tex: %.org
	emacs -batch --eval "(setq enable-local-eval t)" --eval "(setq enable-local-variables t)" --eval "(setq org-export-babel-evaluate nil)" $^  --funcall org-latex-export-to-latex
	mv $@ $@.bak
	echo '\def\\raggedright{}' > $@
	echo "\PassOptionsToPackage{svgnames}{xcolor}" >> $@
	sed -e 's/\\begin{verbatim}/\\begin{pyglist}[language=r,style=tango,bgcolor=Moccasin,numbers=left,numbersep=5pt]/' -e 's/\\end{verbatim}/\\end{pyglist}/' $@.bak >> $@

%.pdf: %.tex
	pdflatex -shell-escape $<
#	bibtex `basename $< .tex`
	pdflatex -shell-escape $<
#	pdflatex -shell-escape $<
