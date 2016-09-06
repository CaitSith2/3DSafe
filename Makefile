.PHONY : stage1 arm11 stage2 launcher

OUTDIR = out

all : $(OUTDIR) stage1 arm11 launcher stage2

$(OUTDIR):
	@[ -d $(OUTDIR) ] || mkdir -p $(OUTDIR)

arm11:
	@$(MAKE) -C arm11

launcher:
	@$(MAKE) -C launcher

stage1:
	@$(MAKE) -C payload_stage1
	@mv payload_stage1/payload_stage1.bin $(OUTDIR)

stage2:
	@[ -d payload_stage2/data ] || mkdir -p payload_stage2/data
	@cp arm11/arm11.bin payload_stage2/data
	@$(MAKE) -C payload_stage2
	@mv payload_stage2/payload_stage2.bin $(OUTDIR)

clean:
	@$(MAKE) -C payload_stage1 clean
	@$(MAKE) -C arm11 clean
	@$(MAKE) -C launcher clean
	@$(MAKE) -C payload_stage2 clean
	@rm -rf $(OUTDIR)
