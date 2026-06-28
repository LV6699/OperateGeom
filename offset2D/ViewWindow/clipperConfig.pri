CPPath = /home/j-472/AlgDeve/clipper/clipper2_1
CPIncludePath = $$CPPath/include
CPLibPath = $$CPPath/lib
INCLUDEPATH += $$CPIncludePath
unix{
CPPath = /home/j-472/AlgDeve/clipper/clipper2_1
cpinclude = $$CPPath/include
cplib = $$CPPath/lib
}
INCLUDEPATH += $$cpinclude
CONFIG += C++17
LIBS += -L$$cplib -lClipper2
LIBS += -L$$cplib -lClipper2Z
HEADERS +=
SOURCES +=