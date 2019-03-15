CXX=g++
CXX+=-DDEBUG -g
CXXFLAGS=-c -Wall
LDFLAGS=
SOURCES=main.C Event.C EventDict.C
OBJDIR=obj
OBJECTS=$(SOURCES:%.C=$(OBJDIR)/%.o)
EXECUTABLE=JP

CXXFLAGS += $(shell $(ROOTSYS)/bin/root-config --cflags)
LDFLAGS += $(shell $(ROOTSYS)/bin/root-config --libs)

CXXFLAGS += -I$(JSAPSYS)/Analysis/

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

$(OBJECTS):$(OBJDIR)/%.o: %.C | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $< -o $@

$(OBJDIR):
	mkdir $@

clean:
	rm -f $(OBJDIR)/*.o; rm -f $(EXECUTABLE)
