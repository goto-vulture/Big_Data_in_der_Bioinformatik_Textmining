##### ##### ##### ##### ##### Makefile Skript fuer das Praktikumsprojekt "Bioinformatics_Textmining" ##### ##### ##### ##### #####

CC = gcc
RM = rm
MKDIR = mkdir
MAKE = make

# Pfad zu doxygen, falls es auf dem System vorhanden ist
DOXYGEN = doxygen
DOXYGEN_PATH = $(shell command -v $(DOXYGEN) 2> /dev/null)


# Flags, die sowohl im Debug- als auch im Release-Build, verwendet werden
CCFLAGS = -pedantic -Wall -Wextra -Wconversion -fmessage-length=0 -Wstack-protector -march=native

# Verwendete Libs
LIBS = -lm

# Weitere hilfreiche Compilerflags
# Programmabbruch bei Ueberlauf von vorzeichenbehafteten Integers
# CCFLAGS += -ftrapv => Funktioniert leider nicht wie erhofft :(
# Warnung, wenn Gleitkommazahlen auf Gleichheit mittels == getestet werden
CCFLAGS += -Wfloat-equal
# Warnung, wenn Variablen ueberdeckt werden
CCFLAGS += -Wshadow
# Warnung, wenn irgendwas von der Groesse einer Funktion oder des void-Typs abhaengt
CCFLAGS += -Wpointer-arith
# Fest codierte Strings bekommen den Typ const char*
CCFLAGS += -Wwrite-strings
# Warnung, wenn ein switch Ausdruck keinen default Pfad besitzt
CCFLAGS += -Wswitch-default
# Warnung, wenn bei einem switch Ausdruck ein enum Typ verwendet wird und nicht alle moeglichen Konstanten im switch Ausdruck verwendet werden
CCFLAGS += -Wswitch-enum
# Warnung, wenn Code existiert, der nie erreicht werden kann
CCFLAGS += -Wunreachable-code
# Warnung, wenn eine Funktion ohne Parameterangabe deklariert oder definiert wird
CCFLAGS += -Wstrict-prototypes
# Warnung, wenn Formatstrings dynamisch erzeugt werden und und nicht zur Kompilierungszeit bekannt sind
CCFLAGS += -Wformat-nonliteral
# Warnung, wenn ein Wert sich selbst zugewiesen wird
CCFLAGS += -Winit-self
# Warnung, wenn Funktionen, die einen Formatstring erwarten, diesen nicht bekommen
CCFLAGS += -Wformat-security
# Weitere Format String Ueberpruefungen
CCFLAGS += -Wformat=2
# Laut Doku: "If -Wformat is specified, also warn about strftime formats which may yield only a two-digit year."
# Die Warnung laesst sich im Code nicht vermeiden, wenn %c verwendet werden muss!
CCFLAGS += -Wno-format-y2k
# Einige weitere Moeglichkeiten den Code etwas sicherer zu machen => Diese Flags werden als systemspezifische Flags verwendet,
# da Windows mit der Standardauswahl nicht arbeiten kann ...
# CCFLAGS += -fstack-protector -Wl,-z,relro -Wl,-z,now

# Weitere Warnungsflags
CCFLAGS += -Wvolatile-register-var -Wdisabled-optimization -Winline -Wnested-externs -Wredundant-decls -Wpacked -Wwrite-strings -Wundef

# Debug Build: Keine Optimierung und das hoechste Debug Level
DEBUG_FLAGS = -O0 -g3 -D_FORTIFY_SOURCE=2
# Eine Praeprozessorkonstante setzen, wenn im Debug-Modus das Programm uebersetzt wird
DEBUG_FLAGS += -DDEBUG_BUILD


# Compiler Flags fuer die Nutzung von Code Coverage Tools
CODE_COVERAGE_FLAGS = -fprofile-arcs -ftest-coverage
CODE_COVERAGE_LINK_FLAGS = -lgcov

##### Siehe: https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html #####
##### Uebersicht ueber die OPtimierungsmoeglichkeiten von GCC #####
# Release Build: Hoechste Optimierung und keine Debug Informationen
RELEASE_FLAGS = -O3
# Optimierungen fuer den Linker (Ich wusste vorher gar nicht, dass es so ein Flag auch fuer den Linker gibt ... :o)
RELEASE_FLAGS += -Wl,-O1
# Positionsunabhaengigen Code erzeugen
RELEASE_FLAGS += -fPIE
# Eine Praeprozessorkonstante setzen, wenn im Release-Modus das Programm uebersetzt wird
RELEASE_FLAGS += -DRELEASE_BUILD
# IEEE Standard nicht mehr strikt verfolgen und Gleitkommaoperations u.a. durch fehlende NaN und +/-Inf Checks beschleunigen
RELEASE_FLAGS += -ffast-math
# Prolog von Vararg Funktionen optimieren
RELEASE_FLAGS += -fstdarg-opt
# Pointer Anlaysen durchfuehren, die bei groesseren Uebersetzungseinheiten viele Ressourcen in Anspruch nehmen koennen
RELEASE_FLAGS += -fipa-pta
# Weitere Optimierungen durch den Linker durchfuehren => Funktioniert nicht wie gewollt ... :(
#RELEASE_FLAGS += -flto

DEBUG = 0
RELEASE = 0

# Default C Standard: C11
CSTD = -std=c11

PROJECT_NAME = Bioinformatics_Textmining
DOCUMENTATION_PATH = ./Documentation
NO_DOCUMENTATION = 1

# addsuffix, welches einen String am Ende einer Variable anbringt, kann das Ergebnis NICHT einer Variablen zuweisen, wenn diese
# Variable im Aufruf von addsuffix vorhanden ist !
# D.h.: test += $(addsuffix _X_, $(test)) ist NICHT moeglich !
# Daher der Umweg ueber mehrere Variablen
TEMP_1 =
TARGET =

# Zusaetzliche Flags fuer Linux
# -D_POSIX_C_SOURCE=200112L:  Dies macht POSIX Funktionen verfuegbar, die nicht zum reinen C-Standard gehoeren (verwendet fuer fseeko()/ftello())
ADDITIONAL_LINUX_FLAGS = -fstack-protector-strong -Wl,-z,relro -Wl,-z,now -D_POSIX_C_SOURCE=200112L

# Zusaetzliche Flags fuer Windows
# Unter Windows gibt es bei Format-Strings einige Probleme !
# -Wno-pedantic-ms-format:    Bestimmte Warnungen, die die Format-Strings betreffen, abschalten
# -D__USE_MINGW_ANSI_STDIO=1: Standardmaessig ist der Formatstring "%z" fuer size_t Variablen abgeschaltet (warum auch immer) ...
#                             Dieses Flag schaltet das Element ein, sodass size_t Variablen richtig ausgegeben werden koennen
#                             Siehe: https://lists.gnu.org/archive/html/bug-gnulib/2014-09/msg00056.html
# Unter Windows fuehrt die Erzeugung des Stack-Protektors zu Segmentation Faults beim Aufruf von Funktionen (also wirklich
# WAEHREND des Aufrufes ! -> wird unter Windows nicht mehr verwendet.)
ADDITIONAL_WINDOWS_FLAGS = -Wno-pedantic-ms-format -D__USE_MINGW_ANSI_STDIO=1 # -fstack-protector

# Der Debug-Build ist die Standardvariante, wenn nichts anderes angegeben wurde
# Fuer den Release-Build muss die Variable "Release", "RELEASE" oder "release" auf 1 gesetzt werden
ifeq ($(Release), 1)
	CCFLAGS += $(RELEASE_FLAGS)
	TEMP_1 = $(addsuffix _Release_, $(PROJECT_NAME))
	RELEASE = 1
else
	ifeq ($(RELEASE), 1)
		CCFLAGS += $(RELEASE_FLAGS)
		TEMP_1 = $(addsuffix _Release_, $(PROJECT_NAME))
		RELEASE = 1
	else
		ifeq ($(release), 1)
			CCFLAGS += $(RELEASE_FLAGS)
			TEMP_1 = $(addsuffix _Release_, $(PROJECT_NAME))
			RELEASE = 1
		else
			CCFLAGS += $(DEBUG_FLAGS)
			TEMP_1 = $(addsuffix _Debug_, $(PROJECT_NAME))
			CCFLAGS += $(CODE_COVERAGE_FLAGS)
			LIBS += $(CODE_COVERAGE_LINK_FLAGS)
			DEBUG = 1
		endif
	endif
endif

ifeq ($(STD), 99)
	CSTD = -std=c99
endif
ifeq ($(std), 99)
	CSTD = -std=c99
endif
ifeq ($(std), c99)
	CSTD = -std=c99
endif
ifeq ($(STD), c99)
	CSTD = -std=c99
endif
ifeq ($(std), C99)
	CSTD = -std=c99
endif
ifeq ($(STD), C99)
	CSTD = -std=c99
endif
CCFLAGS += $(CSTD)

# Soll die Dokumentation mittels Doxygen erzeugt werden ? Die Erzeugung der Dokumentation benoetigt mit Abstand die meiste
# Zeit bei der Erstellung des Programms
# "NO_DOCUMENTATION", "NO_DOCU", "NO_DOCS": Alle CLI-Parameter schalten die Erzeugung der Doxygen-Dokumentation ab
ifeq ($(NO_DOCUMENTATION), 1)
	NO_DOCUMENTATION = 1
endif
ifeq ($(NO_DOCU), 1)
	NO_DOCUMENTATION = 1
endif
ifeq ($(NO_DOCS), 1)
	NO_DOCUMENTATION = 1
endif

# Neu:
# Das Erzeugen der Dokumentation muss explizit angestrossen werden
ifeq ($(DOCUMENTATION), 1)
	NO_DOCUMENTATION = 0
endif
ifeq ($(DOCU), 1)
	NO_DOCUMENTATION = 0
endif
ifeq ($(DOCS), 1)
	NO_DOCUMENTATION = 0
endif

# Unter Windows wird u.a. das Flag "-Wno-pedantic-ms-format" benoetigt, da die MinGW Implementierung nicht standardkonforme
# Formatstrings verwendet. Mit diesem Compilerflag wird die Warnung, dass die MinGW Formatstrings nicht dem Standard
# entsprechen, entfernt
#
# Unter Linux werden einige Flags verwendet, die unter Windows nicht verwendet werden koennen
ifeq ($(OS), Windows_NT)
	CCFLAGS += $(ADDITIONAL_WINDOWS_FLAGS)
	TARGET = $(addsuffix Win, $(TEMP_1))
else
	CCFLAGS += $(ADDITIONAL_LINUX_FLAGS)
	TARGET = $(addsuffix Linux, $(TEMP_1))
endif

##### ##### ##### BEGINN Uebersetzungseinheiten ##### ##### #####
MAIN_C = ./src/main.c

STR2INT_H = ./src/str2int.h
STR2INT_C = ./src/str2int.c

INT2STR_H = ./src/int2str.h
INT2STR_C = ./src/int2str.c

DYNAMIC_MEMORY_H = ./src/Error_Handling/Dynamic_Memory.h
DYNAMIC_MEMORY_C = ./src/Error_Handling/Dynamic_Memory.c

TINYTEST_H = ./src/Tests/tinytest.h
TINYTEST_C = ./src/Tests/tinytest.c

# Hiervon gibt es nur eine Headerdatei !
ASSERT_MSG_H = ./src/Error_Handling/Assert_Msg.h

ARGPARSE_H = ./src/argparse.h
ARGPARSE_C = ./src/argparse.c

CLI_PARAMETER_H = ./src/CLI_Parameter.h
CLI_PARAMETER_C = ./src/CLI_Parameter.c

PRINT_TOOLS_H = ./src/Print_Tools.h
PRINT_TOOLS_C = ./src/Print_Tools.c

STRING_TOOLS_H = ./src/String_Tools.h
STRING_TOOLS_C = ./src/String_Tools.c

DOCUMENT_WORD_LIST_H = ./src/Document_Word_List.h
DOCUMENT_WORD_LIST_C = ./src/Document_Word_List.c

TEST_DOCUMENT_WORD_LIST_H = ./src/Tests/TEST_Document_Word_List.h
TEST_DOCUMENT_WORD_LIST_C = ./src/Tests/TEST_Document_Word_List.c

CREATE_TEST_DATA_H = ./src/Tests/Create_Test_Data.h
CREATE_TEST_DATA_C = ./src/Tests/Create_Test_Data.c

INTERSECTION_APPROACHES_H = ./src/Intersection_Approaches.h
INTERSECTION_APPROACHES_C = ./src/Intersection_Approaches.c

FILE_READER_H = ./src/File_Reader.h
FILE_READER_C = ./src/File_Reader.c

TOKEN_INT_MAPPING_H = ./src/Token_Int_Mapping.h
TOKEN_INT_MAPPING_C = ./src/Token_Int_Mapping.c

CJSON_H = ./src/JSON_Parser/cJSON.h
CJSON_C = ./src/JSON_Parser/cJSON.c

TEST_CJSON_PARSER_H = ./src/Tests/TEST_cJSON_Parser.h
TEST_CJSON_PARSER_C = ./src/Tests/TEST_cJSON_Parser.c

MISC_H = ./src/Misc.h
MISC_C = ./src/Misc.c

EXEC_INTERSECTION_H = ./src/Exec_Intersection.h
EXEC_INTERSECTION_C = ./src/Exec_Intersection.c

STOP_WORDS_H = ./src/Stop_Words/Stop_Words.h
STOP_WORDS_C = ./src/Stop_Words/Stop_Words.c

TWO_DIM_C_STRING_ARRAY_H = ./src/Two_Dim_C_String_Array.h
TWO_DIM_C_STRING_ARRAY_C = ./src/Two_Dim_C_String_Array.c

MD5_SUM_H = ./src/Tests/md5.h
MD5_SUM_C = ./src/Tests/md5.c

TEST_FILE_READER_H = ./src/Tests/TEST_File_Reader.h
TEST_FILE_READER_C = ./src/Tests/TEST_File_Reader.c

EXEC_CONFIG_H = ./src/Exec_Config.h
EXEC_CONFIG_C = ./src/Exec_Config.c

TEST_EXEC_INTERSECTION_H = ./src/Tests/TEST_Exec_Intersection.h
TEST_EXEC_INTERSECTION_C = ./src/Tests/TEST_Exec_Intersection.c

TEST_ETC_H = ./src/Tests/TEST_Etc.h
TEST_ETC_C = ./src/Tests/TEST_Etc.c

UTF8_H = ./src/UTF8/utf8.h
UTF8_C = ./src/UTF8/utf8.c

ANSI_ESC_SEQ_H = ./src/ANSI_Esc_Seq.h
ANSI_ESC_SEQ_C = ./src/ANSI_Esc_Seq.c

TEST_TWO_DIM_C_STRING_ARRAY_H = ./src/Tests/TEST_Two_Dim_C_String_Array.h
TEST_TWO_DIM_C_STRING_ARRAY_C = ./src/Tests/TEST_Two_Dim_C_String_Array.c
##### ##### ##### ENDE Uebersetzungseinheiten ##### ##### #####



# Komplettes Projekt erstellen
all: $(TARGET)
	@echo

# Wenn doxygen auf den System nicht vorhanden ist, dann wird die Kompilierung fortgesetzt; aber ohne Erzeugung der Dokumentation
ifndef DOXYGEN_PATH
	@echo No $(DOXYGEN) in $(PATH). Skip the creation of the documentation.
	@echo If you want the program documentation, try to install $(DOXYGEN) with apt-get install $(DOXYGEN).
else
ifeq ($(NO_DOCUMENTATION), 0)
	@echo Generating the documentation ...
	$(DOXYGEN_PATH) ./Doxyfile
endif
endif

	@echo
	@echo $(PROJECT_NAME) build completed !

$(TARGET): main.o str2int.o int2str.o Dynamic_Memory.o tinytest.o argparse.o CLI_Parameter.o Print_Tools.o String_Tools.o Document_Word_List.o TEST_Document_Word_List.o Create_Test_Data.o Intersection_Approaches.o File_Reader.o Token_Int_Mapping.o cJSON.o TEST_cJSON_Parser.o Misc.o  Exec_Intersection.o Stop_Words.o Two_Dim_C_String_Array.o md5.o TEST_File_Reader.o Exec_Config.o TEST_Exec_Intersection.o TEST_Etc.o utf8.o ANSI_Esc_Seq.o TEST_Two_Dim_C_String_Array.o
	@echo
	@echo Linking object files ...
	@echo
	$(CC) $(CCFLAGS) -o $(TARGET) main.o str2int.o int2str.o Dynamic_Memory.o tinytest.o argparse.o CLI_Parameter.o Print_Tools.o String_Tools.o Document_Word_List.o TEST_Document_Word_List.o Create_Test_Data.o Intersection_Approaches.o File_Reader.o Token_Int_Mapping.o cJSON.o TEST_cJSON_Parser.o Misc.o Exec_Intersection.o Stop_Words.o Two_Dim_C_String_Array.o md5.o TEST_File_Reader.o Exec_Config.o TEST_Exec_Intersection.o TEST_Etc.o utf8.o ANSI_Esc_Seq.o TEST_Two_Dim_C_String_Array.o $(LIBS)

##### BEGINN Die einzelnen Uebersetzungseinheiten #####
main.o: $(MAIN_C)
	@echo Build target: $(TARGET).
	@echo

	@echo ">>> Build information <<<"
ifeq ($(OS), Windows_NT)
	@echo OS: Windows
else
	@echo OS: Linux
endif

ifeq ($(RELEASE), 1)
	@echo Type: Release
else
	@echo Type: Debug
endif

	@echo C Std: $(CSTD)

ifeq ($(NO_DOCUMENTATION), 1)
	@echo Documentation: NO
else
	@echo Documentation: YES
endif

	@echo
	$(CC) $(CCFLAGS) -c $(MAIN_C)

str2int.o: $(STR2INT_C)
	$(CC) $(CCFLAGS) -c $(STR2INT_C)

int2str.o: $(INT2STR_C)
	$(CC) $(CCFLAGS) -c $(INT2STR_C)

Dynamic_Memory.o: $(DYNAMIC_MEMORY_C)
	$(CC) $(CCFLAGS) -c $(DYNAMIC_MEMORY_C)

tinytest.o: $(TINYTEST_C)
	$(CC) $(CCFLAGS) -c $(TINYTEST_C)

argparse.o: $(ARGPARSE_C)
	$(CC) $(CCFLAGS) -c $(ARGPARSE_C)

CLI_Parameter.o: $(CLI_PARAMETER_C)
	$(CC) $(CCFLAGS) -c $(CLI_PARAMETER_C)

Print_Tools.o: $(PRINT_TOOLS_C)
	$(CC) $(CCFLAGS) -c $(PRINT_TOOLS_C)

String_Tools.o: $(STRING_TOOLS_C)
	$(CC) $(CCFLAGS) -c $(STRING_TOOLS_C)

Document_Word_List.o: $(DOCUMENT_WORD_LIST_C)
	$(CC) $(CCFLAGS) -c $(DOCUMENT_WORD_LIST_C)

TEST_Document_Word_List.o: $(TEST_DOCUMENT_WORD_LIST_C)
	$(CC) $(CCFLAGS) -c $(TEST_DOCUMENT_WORD_LIST_C)

Create_Test_Data.o: $(CREATE_TEST_DATA_C)
	$(CC) $(CCFLAGS) -c $(CREATE_TEST_DATA_C)

Intersection_Approaches.o: $(INTERSECTION_APPROACHES_C)
	$(CC) $(CCFLAGS) -c $(INTERSECTION_APPROACHES_C)

File_Reader.o: $(FILE_READER_C)
	$(CC) $(CCFLAGS) -c $(FILE_READER_C)

Token_Int_Mapping.o: $(TOKEN_INT_MAPPING_C)
	$(CC) $(CCFLAGS) -c $(TOKEN_INT_MAPPING_C)

cJSON.o: $(CJSON_C)
	$(CC) $(CCFLAGS) -c $(CJSON_C)

TEST_cJSON_Parser.o: $(TEST_CJSON_PARSER_C)
	$(CC) $(CCFLAGS) -c $(TEST_CJSON_PARSER_C)

Misc.o: $(MISC_C)
	$(CC) $(CCFLAGS) -c $(MISC_C)

Exec_Intersection.o: $(EXEC_INTERSECTION_C)
	$(CC) $(CCFLAGS) -c $(EXEC_INTERSECTION_C)

Stop_Words.o: $(STOP_WORDS_C)
	$(CC) $(CCFLAGS) -c $(STOP_WORDS_C)

Two_Dim_C_String_Array.o: $(TWO_DIM_C_STRING_ARRAY_C)
	$(CC) $(CCFLAGS) -c $(TWO_DIM_C_STRING_ARRAY_C)

md5.o: $(MD5_SUM_C)
	$(CC) $(CCFLAGS) -c $(MD5_SUM_C)

TEST_File_Reader.o: $(TEST_FILE_READER_C)
	$(CC) $(CCFLAGS) -c $(TEST_FILE_READER_C)

Exec_Config.o: $(EXEC_CONFIG_C)
	$(CC) $(CCFLAGS) -c $(EXEC_CONFIG_C)

TEST_Exec_Intersection.o: $(TEST_EXEC_INTERSECTION_C)
	$(CC) $(CCFLAGS) -c $(TEST_EXEC_INTERSECTION_C)

TEST_Etc.o: $(TEST_ETC_C)
	$(CC) $(CCFLAGS) -c $(TEST_ETC_C)

utf8.o: $(UTF8_C)
	$(CC) $(CCFLAGS) -c $(UTF8_C)

ANSI_Esc_Seq.o: $(ANSI_ESC_SEQ_C)
	$(CC) $(CCFLAGS) -c $(ANSI_ESC_SEQ_C)

TEST_Two_Dim_C_String_Array.o: $(TEST_TWO_DIM_C_STRING_ARRAY_C)
	$(CC) $(CCFLAGS) -c $(TEST_TWO_DIM_C_STRING_ARRAY_C)
##### ENDE Die einzelnen Uebersetzungseinheiten #####

# Kompilierung des Programms im Debug Modus mit direkter Ausfuehrung der Tests
test:
	$(MAKE) clean
	@echo
	$(MAKE) all STD=11 NO_DOCU=1
	@echo
	@echo Run all tests ...
	@echo
	./$(TARGET) -T

# Alles wieder aufraeumen
clean:
	@echo Clean $(PROJECT_NAME) build.
	@echo
	@echo \> Deleting compilation files:
	$(RM) -f $(PROJECT_NAME)* *.o ./src/Error_Handling/*.gch gmon.out
	@echo
	@echo \> Deleting doxygen documentation:
	$(RM) -rf $(DOCUMENTATION_PATH)
	$(MKDIR) $(DOCUMENTATION_PATH)
	@echo
	@echo \> Deleting code coverage data:
	$(RM) -f *.gcda *.gcno
	@echo
	@echo $(PROJECT_NAME) build cleaned.
