CHARMC=$(CHARM_HOME)/bin/charmc
OPTS=-O3
CXX=$(CHARMC)
CXXFLAGS=$(OPTS)

BINARY=oddeven

all: $(BINARY)

$(BINARY): $(BINARY).o
	$(CHARMC) -o $@ $< $(OPTS)

$(BINARY).o: $(BINARY).C $(BINARY).decl.h
$(BINARY).decl.h: $(BINARY).ci
	$(CHARMC) $<

test: $(BINARY)
	$(call run, +p4 ./$(BINARY) 50 5 )

clean:
	rm -f *.o *.decl.h *.def.h $(BINARY) charmrun
