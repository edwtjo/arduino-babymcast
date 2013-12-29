SUBDIRS := $(sort $(dir $(wildcard lib/*/.))) src/.
TARGETS := all clean destroy

SUBDIRS_TARGETS := \
	$(foreach t,$(TARGETS),$(addsuffix $t,$(SUBDIRS)))

.PHONY : $(TARGETS) $(SUBDIRS_TARGETS)

$(TARGETS) : % : $(addsuffix %,$(SUBDIRS))
	@echo 'Done "$*" target'

$(SUBDIRS_TARGETS) :
	$(MAKE) -C $(@D) $(@F:.%=%)
