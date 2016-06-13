.SILENT:
.PHONY: release debug clean

AR=ar
RM=rm --force
LC_ALL=C

ifeq ($(CXX),g++)
  GCC_COLORS=always
  DEBUG_CPPFLAGS=-pipe -Wall -Wextra -Wpointer-arith -pedantic -Wno-long-long -I.
  DEBUG_CXXFLAGS=-fmax-errors=10 -ffast-math -msse4 -mfpmath=sse -mtune=native -g -fstack-check
  RELEASE_CPPFLAGS=-pipe -Wall -Wextra -Wpointer-arith -pedantic -Wno-long-long -DNDEBUG -I.
  RELEASE_CXXFLAGS=-fmax-errors=10 -ffast-math -msse4 -mfpmath=sse -mtune=native -O3
else
  ifeq ($(CXX),clang++)
    DEBUG_CPPFLAGS=-pipe -Weverything -pedantic -Wno-shadow -Wno-padded -Wno-long-long -I.
    DEBUG_CXXFLAGS=-g
    RELEASE_CPPFLAGS=-pipe -Weverything -pedantic -Wno-shadow -Wno-padded -Wno-long-long -DNDEBUG -I.
    RELEASE_CXXFLAGS=-O3
  else
    ifeq ($(CXX),icpc)
      DEBUG_CPPFLAGS=-Wall -Wextra -I.
      DEBUG_CXXFLAGS=-g
      RELEASE_CPPFLAGS=-Wall -Wextra -DNDEBUG -I.
      RELEASE_CXXFLAGS=-O3
    endif
  endif
endif

BASIC=Basic/Console.cpp Basic/Debug.cpp Basic/File.cpp Basic/Float.cpp Basic/Integer.cpp Basic/Memory.cpp Basic/Random.cpp Basic/Sort.cpp Basic/String.cpp Basic/Time.cpp
IMAGE=Image/Color.cpp Image/Font.cpp Image/FontRoboto8.cpp Image/FontRoboto10.cpp Image/FontRoboto12.cpp Image/FontRoboto14.cpp Image/FontRoboto18.cpp Image/FontRoboto24.cpp Image/Image.cpp
MATH=Math/GaussLegendreQuadrature.cpp Math/GaussPattersonQuadrature.cpp
SOURCES=$(BASIC) $(IMAGE) $(MATH)
OBJECTS=$(SOURCES:.cpp=.o)
OUTPUT=libGrok.a

release: CPPFLAGS=$(RELEASE_CPPFLAGS)
release: CXXFLAGS=$(RELEASE_CXXFLAGS)
release: info $(OUTPUT)

debug: CPPFLAGS=$(DEBUG_CPPFLAGS)
debug: CXXFLAGS=$(DEBUG_CXXFLAGS)
debug: info $(OUTPUT)

info:
	echo ""
	echo "----------------------------------------------------------------------"
	echo "$(OUTPUT)"
	echo "----------------------------------------------------------------------"
	echo "CXX = $(CXX)"
	echo "CPPFLAGS = $(CPPFLAGS)"
	echo "CXXFLAGS = $(CXXFLAGS)"
	echo "----------------------------------------------------------------------"

clean:
	$(RM) $(OUTPUT) $(OBJECTS)

$(OUTPUT): $(OBJECTS)
	echo AR $@
	$(AR) rcs $@ $(OBJECTS)

.cpp.o:
	echo CXX $<
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ -c $<
