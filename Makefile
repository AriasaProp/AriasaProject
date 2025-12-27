E		:= bin/exec
O		:= bin/objs
CS	:= c-src
PS	:= page-src
DFS := -DPROJECT_NAME="\"Fermity\""
CFS := -MMD -MP -std=gnu11 -Werror -Wall
LFS	:= -lc -lm
# MAKEFLAGS += -j4

.PHONY: start
start:
	python -m http.server -d . 8080
.PHONY: gp
gp: index.html customize.html
.PHONY: up
up:
	@git add .
	@git commit -m "up"
	@git push

#generate html files
%.html: $(E)/gen_page $(PS)/template.thtml $(PS)/%.chtml
	@./$< $(basename $@) 

$(E)/gen_page: $(O)/gen_page.c.o
	@echo "page gen exe build"
	@mkdir -p $(@D)
	@$(CC) $^ -o $@ $(LFS)
	@chmod +x $@
	
# generate object file from c
# should generate .d file to
$(O)/%.c.o: $(CS)/%.c
	@mkdir -p $(@D)
	@$(CC) -c $< -o $@ $(CFS) $(DFS)

# clean generated page
.PHONY: clean
clean:
	@echo "cleaning generated file page"
	@rm -rf *.html $(O)/ $(E)/



-include $(shell find $(O) -type f -name "*.o.d")

