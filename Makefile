# Diretórios
SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin/Linux

# Arquivos de origem e cabeçalho
SRCS = $(wildcard $(SRCDIR)/*.cpp) $(SRCDIR)/glad.c
HEADERS = $(wildcard $(INCDIR)/*.h)

# Arquivos objeto
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o))

# Arquivo executável
OUTFILE = $(BINDIR)/main

# Compilador e flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wno-unused-function -g -I$(INCDIR)
LDFLAGS = ./lib-linux/libglfw3.a -lrt -lm -ldl -lX11 -lpthread -lXrandr -lXinerama -lXxf86vm -lXcursor

# Regra padrão para construir o executável
$(OUTFILE): $(OBJS)
	mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Regra para compilar arquivos objeto
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS)
	mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS)
	mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean run

# Regra para limpar os arquivos gerados
clean:
	rm -rf $(OBJDIR) $(OUTFILE)

# Regra para executar o programa
run: $(OUTFILE)
	cd $(BINDIR) && ./main
