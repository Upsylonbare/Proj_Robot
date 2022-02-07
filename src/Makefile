
PACKAGES = commando telco

SRC  = $(wildcard commando/*.c)
SRC  += $(wildcard telco/*.c)

OBJ = $(SRC:.c=.o)

TELCO = ../$(PROG_TELCO)
COMMANDO = ../$(PROG_COMMANDO)

CCFLAGS += -I.

all:
	@for p in $(PACKAGES); do (cd $$p; $(MAKE) $@); done
	@$(MAKE) CCFLAGS="$(CCFLAGS)" LDFLAGS="$(LDFLAGS)" $(COMMANDO)
	@$(MAKE) CCFLAGS="$(CCFLAGS)" LDFLAGS="$(LDFLAGS)" $(TELCO)

$(COMMANDO): $(OBJ)
	$(CC) $(CCFLAGS) $(OBJ) -o $(COMMANDO) $(LDFLAGS)

$(TELCO): $(OBJ)
	$(CC) $(CCFLAGS) $(OBJ) -o $(TELCO) $(LDFLAGS)

.PHONY: clean

clean:
	@for p in $(PACKAGES); do (cd $$p; $(MAKE) $@); done

