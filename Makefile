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

LIB = -ldpp \
      -lgcov \
      -lcivetweb \
      -ljsoncpp \

FILES = Main.cpp TimeManager.cpp Entity.cpp Universe.cpp Engine.cpp System.cpp Area.cpp Modifier.cpp
DISCORD_FILES = DiscordMain.cpp DiscordBot.cpp DiscordChannel.cpp DiscordGuild.cpp DiscordCommands.cpp
WEBUI_FILES = WebUI.cpp
COMMON_FILES = Logger.cpp Utils.cpp MySocket.cpp

TEST_FILES = test_main.cpp

COMMON_SRC = $(addprefix $(SRCDIR)/,$(COMMON_FILES))
COMMON_OBJ = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o, $(COMMON_SRC))

SRC = $(addprefix $(SRCDIR)/,$(FILES))
OBJ = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o, $(SRC))
BIN = $(BINDIR)/arta

DISCORD_SRC = $(addprefix $(SRCDIR)/,$(DISCORD_FILES))
DISCORD_OBJ = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o, $(DISCORD_SRC))
DISCORD_BIN = $(BINDIR)/discord

WEBUI_SRC = $(addprefix $(SRCDIR)/,$(WEBUI_FILES))
WEBUI_OBJ = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o, $(WEBUI_SRC))
WEBUI_BIN = $(BINDIR)/webui

TST = $(addprefix $(TSTDIR)/,$(TEST_FILES))
TSTOBJ = $(patsubst $(TSTDIR)/%.cpp,$(OBJDIR)/%.o, $(TST))
TSTBIN = $(BINDIR)/test

.PHONY: all release clean dox test test_ci arta discord webui

all: arta webui

arta: $(BIN)

discord: $(DISCORD_BIN)

webui: ${WEBUI_BIN}

release:
	$(MAKE) all CFLAGS="$(RELEASE_CFLAGS)"

$(BIN): $(OBJ) $(COMMON_OBJ)| $(BINDIR)
	$(CC) $(LDFLAGS) -o $(BIN) $(OBJ) $(COMMON_OBJ) $(LIB)

$(DISCORD_BIN): $(DISCORD_OBJ) $(COMMON_OBJ)| $(BINDIR)
	$(CC) $(LDFLAGS) -o $(DISCORD_BIN) $(DISCORD_OBJ) $(COMMON_OBJ) $(LIB)

$(WEBUI_BIN): $(WEBUI_OBJ) $(COMMON_OBJ)| $(BINDIR)
	$(CC) $(LDFLAGS) -o $(WEBUI_BIN) $(WEBUI_OBJ) $(COMMON_OBJ) $(LIB)

$(OBJ): $(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CC) -o $@ $(CFLAGS) $(INCLUDES) -c $<

$(DISCORD_OBJ): $(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
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

dox:  | $(DOCDIR)
	@rm -rf $(DOCDIR)
	$(DOX) $(DOXYFILE)

cpplint:
	@cpplint --linelength=120 --filter=-build/c++20,-whitespace/indent,-runtime/indentation_namespace $(SRC) $(INCDIR)/*

cppcheck:
	@cppcheck $(SRC) --force --enable=all --inconclusive --error-exitcode=1 --suppress=unmatchedSuppression --suppress=missingIncludeSystem --suppress=missingInclude -I$(INCDIR) $(EXTDIR)

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
