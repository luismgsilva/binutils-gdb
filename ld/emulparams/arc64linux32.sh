ARCH=arc

SCRIPT_NAME=elf
ELFSIZE=32
OUTPUT_FORMAT="elf32-littlearc64"

TEMPLATE_NAME=elf

GENERATE_SHLIB_SCRIPT=yes
GENERATE_PIE_SCRIPT=yes

MAXPAGESIZE="CONSTANT (MAXPAGESIZE)"
COMMONPAGESIZE="CONSTANT (COMMONPAGESIZE)"

TEXT_START_ADDR=0x10000

ENTRY=__start

# To support RELRO security feature.
NO_SMALL_DATA=yes
SEPARATE_GOTPLT="SIZEOF (.got.plt) >= 12 ? 12 : 0"
GENERATE_COMBRELOC_SCRIPT=yes
