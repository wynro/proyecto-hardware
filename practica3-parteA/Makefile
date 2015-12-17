.SUFFIXES: .c .h .o
# Los fuentes son todo fichero acabados en .c
SRCS=$(shell ls *.c)


.PHONY: editdoxy doxyclean
edit:
	@emacs -nw $(SRCS)

# Generar el pdf de documentacion
doxy:
	doxygen
	$(MAKE) -C latex
	@cp latex/refman.pdf refman.pdf
	@rm -r latex

# Eliminar documentacion
doxyclean:
	@rm -f -r html/ latex/ refman.pdf
