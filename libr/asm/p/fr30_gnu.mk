OBJ_FR30=asm_fr30_gnu.o
OBJ_FR30+=../arch/fr30/gnu/fr30-bitset-cgen.o
OBJ_FR30+=../arch/fr30/gnu/fr30-dis-cgen.o
OBJ_FR30+=../arch/fr30/gnu/fr30-opc-cgen.o
OBJ_FR30+=../arch/fr30/gnu/fr30-ibld.o
OBJ_FR30+=../arch/fr30/gnu/fr30-desc.o
OBJ_FR30+=../arch/fr30/gnu/fr30-dis.o
OBJ_FR30+=../arch/fr30/gnu/fr30-opc.o

TARGET_FR30=asm_fr30_gnu.${EXT_SO}
STATIC_OBJ+=${OBJ_FR30}

ifeq ($(WITHPIC),1)
ALL_TARGETS+=${TARGET_FR30}
${TARGET_FR30}: ${OBJ_FR30}
	${CC} $(call libname,asm_fr30) ${LDFLAGS} ${CFLAGS} \
		-o ${TARGET_FR30} ${OBJ_FR30}
endif

