

# Note: CXX, CXXFLAGS, LD, LDFLAGS, SOFLAGS, LIBS, GLIBS are defined here

CXX = c++

include $(ROOTSYS)/etc/Makefile.arch

graph4_donors: graph4_donors.cpp
	$(CXX) $(CXXFLAGS) graph4_donors.cpp $(LIBS) -o a.out

plot: plot.cpp
	$(CXX) $(CXXFLAGS) plot.cpp $(LIBS) -o plot

