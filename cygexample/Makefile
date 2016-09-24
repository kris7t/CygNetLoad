
.PHONY: all clean

# Add -static -static-libgcc -static-libstdc++ -lc after -static to link everything
# (including libstdc++) statically, it may help to fix some mysterious SIGSEGVs.
LDFLAGS = -shared -Wl,--export-all-symbols -Wl,--enable-auto-import

ifeq ($(VS_CONFIGURATION),Debug)
	CXXFLAGS += -O0 -g3 -ggdb
	CFLAGS += -O0 -g3 -ggdb
	LDFLAGS += -O0 -g3 -ggdb
else
	CXXFLAGS += -O2
	CFLAGS += -O2
	LDFLAGS += -O2
endif

all: cygexample.dll
	# Copy the output file so that .NET can find it.
	cp cygexample.dll ../CygNetHost

clean:
	rm -f *.o *.dll

cygexample.dll: example.o

%.dll:
	$(CXX) $(LDFLAGS) $< -o $@

