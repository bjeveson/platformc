################################################################################
# 全局变量配置
################################################################################

MYLIB_DEP_FILE := mylib.dep

# 设置单板工程根路径
PROJ_ROOT = ../..

# 设置编译过程文件存在目录
OBJ_DIR = ./debug

#包含依赖文件
-include $(MYLIB_DEP_FILE)

#生成静态库的名称
LibName = MyLib.a

#依赖文件中的模块名称前缀
PREFIXPROJECTNAME = MYLIB

#存储符号表的文件
SymbolFile = libsymbol.x
################################################################################
# 设置mylib目录中编译时需要忽略的目录（包括子目录）
################################################################################

ifeq ($(IGNORE_DIR), )

endif

################################################################################
# 归档工具
################################################################################
AR := $(CROSS_COMPILE)ar
AR_CREATE := $(AR) rcs
AR_EXTRACT := $(AR) x

################################################################################
# 一些shell命令封装
################################################################################
RM := rm -rf
CP := cp -rf
LS := ls -l
CD := cd
TAR := tar czf
CC:= gcc

################################################################################
# 公共编译规则
################################################################################
CFLAG := -c -g
MYLIB_INC := $(addprefix -I, $(MYLIB_H_DIR))

MYLIB_OBJ_DIR := $(addprefix $(OBJ_DIR)/, $(MYLIB_OBJ_C_DIR) $(MYLIB_OBJ_CPP_DIR))

MAKE_C := @$(CC) $(CFLAG) $(MYLIB_INC)

################################################################################
# 目标
################################################################################
mylib: mylib_create_subdir $(MYLIB_OBJ) createlib listsymbol

createlib:
	@echo Create Static Lib :$(OBJ_DIR)/$(LibName)
	@$(AR_CREATE) $(OBJ_DIR)/$(LibName) $(MYLIB_OBJ) 
listsymbol:
	@nm -a -S -s -A -l $(OBJ_DIR)/$(LibName) > $(OBJ_DIR)/$(SymbolFile)

depend: 
	@echo Target $(MYLIB_DEP_FILE) ...
	@./scan.sh $(PROJ_ROOT) $(MYLIB_DEP_FILE) $(PREFIXPROJECTNAME)

mapfile:
	@echo "Create Func MapFile..."
	@./CreateMapFile.sh "$(MYLIB_SRC_C)"
	
clean:
	@echo Target $@ ...
	@$(RM) $(OBJ_DIR) $(MYLIB_DEP_FILE) *.bak
	@echo

mylib_create_subdir:
	@echo Target $@ ...
	@for i in $(MYLIB_OBJ_DIR); \
	do \
		if test ! -d $$i; then \
			mkdir -p $$i; \
		fi; \
	done
	@echo