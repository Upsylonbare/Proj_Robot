
PACKAGES = commando telco

SRC_COMMANDO  = $(wildcard commando/*.c)
SRC_TELCO = $(wildcard telco/*.c)

OBJ_COMMANDO = $(SRC_COMMANDO:.c=.o)
OBJ_TELCO = $(SRC_TELCO:.c=.o)

TELCO = ../$(PROG_TELCO)
COMMANDO = ../$(PROG_COMMANDO)

CCFLAGS += -I.

all:
	@for p in $(PACKAGES); do (cd $$p; $(MAKE) $@); done
	@$(MAKE) CCFLAGS="$(CCFLAGS)" LDFLAGS="$(LDFLAGS)" $(COMMANDO)
	@$(MAKE) CCFLAGS="$(CCFLAGS)" LDFLAGS="$(LDFLAGS)" $(TELCO)

$(COMMANDO): $(OBJ_COMMANDO)
	$(CC) $(CCFLAGS) $(OBJ_COMMANDO) -o $(COMMANDO) $(LDFLAGS)

$(TELCO): $(OBJ_TELCO)
	$(CC) $(CCFLAGS) $(OBJ_TELCO) -o $(TELCO) $(LDFLAGS)

.PHONY: clean

clean:
	@for p in $(PACKAGES); do (cd $$p; $(MAKE) $@); done