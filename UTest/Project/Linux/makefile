################################################################################
# 全局变量配置
################################################################################

#本工程的依赖文件
PROJECT_DEP_FILE := unittest.dep

# 本工程根路径，不带/
PROJ_ROOT = ../..

# 编译过程文件存放目录
OBJ_DIR = ./debug

#生成目标的名称
ExeName = UnitTest

#依赖文件中的本工程名称前缀
PREFIXPROJECTNAME = UT

#库的相对跟路径
MYLIB_PATH_ROOT = ../../../Ark

#库的依赖文件
MYLIB_DEP_FILE:=$(MYLIB_PATH_ROOT)/Project/Linux/mylib.dep

#包含依赖文件
-include $(PROJECT_DEP_FILE)
-include $(MYLIB_DEP_FILE)

#调用公共makfile生成规则
-include $(MYLIB_PATH_ROOT)/Project/Linux/Makefile.Common