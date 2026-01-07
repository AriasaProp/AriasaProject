E		:= bin/exec
WS  := bin/wasm32
O		:= bin/objs
CS	:= c-src
PS	:= page-src
CFS := -MMD -MP -DPROJECT_NAME="\"Fermity\"" -I./c-src

# MAKEFLAGS += -j4
# Free variable
AFLAGS ?=
LFLAGS ?=

.PHONY: gwasm
gwasm: $(WS)/hello_triangle.wasm
	
.PHONY: up
up:
	@git add .
	@git commit -m "up"
	@git push

.PHONY: start
start:
	python -m http.server -d . 8080

.PHONY: gp
gp: $(E)/gen_page
	./$<

# executable of page generator
$(E)/gen_page: AFLAGS	= -Werror
$(E)/gen_page: LFLAGS	= -lc -lm
$(E)/gen_page: $(O)/gen_page.c.o
	@echo "page gen exe build"
	@mkdir -p $(@D)
	@$(CC) -o $@ $(LFLAGS) $^
	@chmod +x $@


# executable of test
$(E)/test: AFLAGS = -I./c-src -Werror
$(E)/test: $(O)/test.c.o
	@$(CC) -o $@ $(LFLAGS) $<
	@chmod +x $@
	@./$@

#generate html files
%.html: $(E)/gen_page $(PS)/%.chtml
	@./$< $(basename $@) 

#generate game wasm file
$(WS)/%.wasm: $(CS)/games/%.c
	@echo "generated wasm game $@"
	@mkdir -p $(@D)
	clang --target=wasm32 \
      -nostdlib \
      -Wl,--no-entry \
      -Wl,--export-all \
      -o $@ $<

# generate object file from c and not link
# should generate .d file to
$(O)/%.c.o: $(CS)/%.c
	@mkdir -p $(@D)
	@$(CC) -o $@ $(CFS) -c $(AFLAGS) $<

# clean generated page
.PHONY: clean
clean:
	@echo "cleaning generated file page"
	@rm -rf *.html bin

-include $(shell find $(O) -type f -name "*.o.d")

