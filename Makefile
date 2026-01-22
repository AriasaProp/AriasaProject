include .env
SEM_VER := $(VERSION.MAJOR).$(VERSION.MINOR).$(VERSION.PATCH)-$(VERSION.EXTRA)

# folder alias
E		:= bin/exec
WS  := bin/wasm32
O		:= bin/objs
CRT := bin/cert
CS	:= c-src
PS	:= page-src
# c flags
CFS := -MMD -MP \
	-DPROJECT_NAME="\"$(PROJECT_NAME)\"" \
	-DSEM_VER="\"${SEM_VER}\"" \
	-I./c-src


# MAKEFLAGS += -j4

.PHONY: gwasm
gwasm: $(WS)/hello_triangle.wasm

.PHONY: up
up:
	@git add .
	@if [ -z "$(MSG)" ]; then \
		git commit -m "update"; \
	else \
		git commit -m "$(MSG)"; \
	fi
	@git push

.PHONY: todo
todo:
	@grep TODO -rno c-src/*.c > TODOs.txt

.PHONY: start_server
start_server: AFLAGS	= -Wall -Werror -DSERVER_HOSTNAME="\"$(SERVER.HOSTNAME)\"" -DSERVER_PORT=$(SERVER.PORT)
start_server: $(O)/start_server.c.o $(O)/page_gen.c.o $(CRT)/crt.pem $(CRT)/key.pem
	@echo "Generate server"
	@mkdir -p $(E)
	@$(CC) -o $(E)/$@ -lc -lm -lssl $(filter %.c.o, $^)
	@chmod +x $(E)/$@
	./$(E)/$@

# generate static pages
.PHONY: gsp
gsp: AFLAGS	= -Wall -Werror
gsp: SP = bin/static_pages
gsp: $(O)/gen_static_page.c.o $(O)/page_gen.c.o
	@echo "Generate static pages"
	@mkdir -p $(E)
	@mkdir -p $(SP)
	@ln -s ../../assets $(SP)/assets
	@$(CC) -o $(E)/gen_static_page -lc -lm $^
	@chmod +x $(E)/gen_static_page
	./$(E)/gen_static_page $(SP)

# executable of test
$(E)/test: AFLAGS = -I./c-src -Werror
$(E)/test: $(O)/test.c.o
	@$(CC) -o $@ $(LFLAGS) $<
	@chmod +x $@
	@./$@

#generate game wasm file
$(WS)/%.wasm: $(CS)/games/%.c
	@echo "generated wasm game $@"
	@mkdir -p $(@D)
	clang --target=wasm32 \
	    -O0 \
      -nostdlib \
      -Wl,--no-entry \
      -Wl,--export=get_value \
      -Wl,--export-table \
      -o $@ $<

# generate object file from c and not link
# should generate .d file to
$(O)/%.c.o: $(CS)/%.c
	@mkdir -p $(@D)
	@$(CC) -o $@ $(CFS) -c $(AFLAGS) $<

$(CRT)/key.pem:
	@mkdir -p $(@D)
	@openssl genrsa -out $@ $(CERT.KeyLen)

$(CRT)/crt.pem: server-san.cnf $(CRT)/key.pem
	@mkdir -p $(@D)
	@Subject="/C=$(CERT.Country)/ST=$(CERT.State)/L=$(CERT.City)/O=$(CERT.O)/OU=$(PROJECT_NAME)/CN=$(CERT.CN)"
	@openssl req -new -key $(word 2, $^) -out $(@D)/crt-req.csr -subj $Subject -config $< -reqexts san
	@openssl x509 -req -days $(CERT.Life) -in $(@D)/crt-req.csr -signkey $(word 2, $^) -out $@ -subj $Subject
	@unset Subject

# clean generated page
.PHONY: clean
clean:
	@echo "cleaning generated file page"
	@rm -rf *.html bin

-include $(shell find $(O) -type f -name "*.o.d")

