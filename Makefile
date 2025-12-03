# LuxBracer

SRCDIR = src
INCDIR = include
EXTDIR = ext
OBJDIR = obj
BINDIR = bin
DOCDIR = doc
TSTDIR = test
RELDIR = release
COVERAGEDIR = coverage

DOX = doxygen
DOXYFILE = Doxyfile

CC = g++
CFLAGS =  -Wall -g -std=c++20 -Wno-psabi -O0 -fprofile-arcs -ftest-coverage -ffunction-sections -fdata-sections
RELEASE_CFLAGS = -Wall -std=c++20 -Wno-psabi -O3 -ffunction-sections -fdata-sections
LDFLAGS = -Wl,--gc-sections
INCLUDES = -I. -I/usr/include/jsoncpp

LIB = \
      -lgcov \
      -lcivetweb \
      -ljsoncpp \

ENGINE_FILES = Main.cpp TimeManager.cpp Entity.cpp Universe.cpp Engine.cpp System.cpp Area.cpp Modifier.cpp
WEBUI_FILES = WebUI.cpp
COMMON_FILES = Logger.cpp Utils.cpp MySocket.cpp

TEST_FILES = test_main.cpp

COMMON_SRC = $(addprefix $(SRCDIR)/,$(COMMON_FILES))
COMMON_OBJ = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o, $(COMMON_SRC))

ENGINE_SRC = $(addprefix $(SRCDIR)/,$(ENGINE_FILES))
ENGINE_OBJ = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o, $(ENGINE_SRC))
ENGINE_BIN = $(BINDIR)/arta

WEBUI_SRC = $(addprefix $(SRCDIR)/,$(WEBUI_FILES))
WEBUI_OBJ = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o, $(WEBUI_SRC))
WEBUI_BIN = $(BINDIR)/webui

TST = $(addprefix $(TSTDIR)/,$(TEST_FILES))
TSTOBJ = $(patsubst $(TSTDIR)/%.cpp,$(OBJDIR)/%.o, $(TST))
TSTBIN = $(BINDIR)/test

.PHONY: all release clean dox test test_ci arta webui

all: arta webui

arta: $(ENGINE_BIN)

webui: ${WEBUI_BIN}

release:
	$(MAKE) all CFLAGS="$(RELEASE_CFLAGS)"

$(ENGINE_BIN): $(ENGINE_OBJ) $(COMMON_OBJ)| $(BINDIR)
	$(CC) $(LDFLAGS) -o $(ENGINE_BIN) $(ENGINE_OBJ) $(COMMON_OBJ) $(LIB)

$(WEBUI_BIN): $(WEBUI_OBJ) $(COMMON_OBJ)| $(BINDIR)
	$(CC) $(LDFLAGS) -o $(WEBUI_BIN) $(WEBUI_OBJ) $(COMMON_OBJ) $(LIB)

$(ENGINE_OBJ): $(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CC) -o $@ $(CFLAGS) $(INCLUDES) -c $<

$(WEBUI_OBJ): $(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CC) -o $@ $(CFLAGS) $(INCLUDES) -c $<

$(COMMON_OBJ): $(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CC) -o $@ $(CFLAGS) $(INCLUDES) -c $<

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(BINDIR):
	@mkdir -p $(BINDIR)

$(DOCDIR):
	@mkdir -p $(DOCDIR)

clean:
	@rm -rf $(OBJDIR) $(BINDIR) $(RELDIR)

containers: engine_container webui_container

engine_container:
	@docker buildx build --file cicd/engine.Dockerfile -t arta-engine:dev .
#	@docker buildx build --platform linux/amd64 --file cicd/engine.Dockerfile -t arta-engine:dev .

webui_container:
	@docker buildx build --file cicd/webui.Dockerfile -t arta-webui:dev .
#	@docker buildx build --platform linux/amd64 --file cicd/engine.Dockerfile -t arta-webui:dev .

dox:  | $(DOCDIR)
	@rm -rf $(DOCDIR)
	$(DOX) $(DOXYFILE)

cpplint:
	@cpplint --linelength=120 --filter=-build/c++20,-whitespace/indent,-runtime/indentation_namespace $(COMMON_SRC) $(WEBUI_SRC) $(ENGINE_SRC) $(INCDIR)/*

cppcheck:
	@cppcheck $(COMMON_SRC) $(WEBUI_SRC) $(ENGINE_SRC) --force --enable=all --inconclusive --error-exitcode=1 --suppress=unmatchedSuppression --suppress=missingIncludeSystem --suppress=missingInclude -I$(INCDIR) $(EXTDIR)

$(TSTOBJ): $(OBJDIR)/%.o: $(TSTDIR)/%.cpp | $(OBJDIR)
	$(CC) -o $@ $(CFLAGS) -DUNITTEST $(INCLUDES) -c $<

$(TSTBIN): $(TSTOBJ) $(filter-out $(OBJDIR)/main.o, $(OBJ)) | $(BINDIR)
	$(CC) -o $(TSTBIN) $(TSTOBJ) $(filter-out $(OBJDIR)/main.o, $(OBJ)) $(LIB)

test_ci: $(TSTBIN)
	bin/test --reporter junit --out results.xml

test: $(TSTBIN)
	bin/test

coverage: test
	@rm -rf ./$(COVERAGEDIR)
	lcov --capture --directory . --output-file coverage.info
	lcov --remove coverage.info '/usr/*' '*/test/test_*' '*/ext/*' --output-file coverage_clean.info
	genhtml coverage_clean.info --output-directory $(COVERAGEDIR)

runner:
	@docker build -t github:latest -f cicd/github-runner.Dockerfile .

serialization_check:
	@./serial_check.sh $(INCDIR)/*
