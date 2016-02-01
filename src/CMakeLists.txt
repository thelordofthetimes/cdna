# setup source files
file(GLOB CDNA_SOURCE_FILES ./com/*.cpp ./kernel/*.cpp ./shell/*.cpp)

# define libaries
add_library(cdna SHARED ${CDNA_SOURCE_FILES})
