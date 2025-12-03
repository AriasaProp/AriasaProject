E		:= bin/exec
O		:= bin/objs
CS	:= c-src
PS	:= page-src
DFS := -DPROJECT_NAME="\"Ariasa Project\""
CFS := -ggdb -MMD -MP -std=c11 -Werror -Wall
LFS	:= -lc -lm
# MAKEFLAGS += -j4

.PHONY: start
start:
	python -m http.server -d . 8080

.PHONY: generate_pages
generate_pages: index.html theme.html

#generate html files
%.html: $(E)/gen_page $(PS)/template.thtml $(PS)/%.chtml
	@./$< $(basename $@) 

$(E)/gen_page: $(O)/gen_page.c.o
	@mkdir -p $(@D)
	@$(CC) $^ -o $@ $(LFS)
	@chmod +x $@
	
# generate object file from c
# should generate .d file to
$(O)/%.c.o: $(CS)/%.c
	@mkdir -p $(@D)
	@$(CC) -c $< -o $@ $(CFS) $(DFS)
#
# clean generated page
.PHONY: clean
clean:
	@echo "cleaning generated file page"
	@rm -rf *.html $(O)/ $(E)/



-include $(shell find $(O) -type f -name "*.o.d")

