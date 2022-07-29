# Normally this makefile shouldn't be called directly and we expect the output
# path to be on a certain location to fit together with the other OP-TEE
# gits and helper scripts.
ifeq ($O,)
$(error output path should be specified when calling this makefile)
endif

# Prevent use of LDFLAGS from the environment. For example, yocto exports
# LDFLAGS that are suitable for the client applications, not for TAs
LDFLAGS=

CA_DIRS := hello
CA_DIRS += parameters

.PHONY: all
all: ca

.PHONY: ca
ca:
	$(q)$(foreach dir,$(CA_DIRS), $(MAKE) -C $(dir) O=$(O)/$(dir) CROSS_COMPILE="$(CROSS_COMPILE_CA)" q=$(q) &&) true

.PHONY: clean
clean:
	$(q)$(foreach dir,$(CA_DIRS), $(MAKE) -C $(dir) O=$(O)/$(dir) $@ &&) true

