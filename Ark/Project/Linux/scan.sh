#!/bin/sh

#参数
# $1	当前工程根路径


# 扫描$1目录中的.c文件，并记录所有目录
dir_have_c()
{
	WORK_DIR=$1
	
	# “..”目录比较特殊，需要给它的末尾增加一个“/”，便于后续统一处理
	if [ "$WORK_DIR" = ".." ]; then
		WORK_DIR=../
	fi
	
	ls $WORK_DIR/*.[cC] 1>/dev/null 2>/dev/null
	if [ $? = 0 ]; then
		SRC_C_DIR=$WORK_DIR" "$SRC_C_DIR
	fi
}

# 扫描$1目录中的.cpp文件，并记录所有目录
dir_have_cpp()
{
	WORK_DIR=$1
	
	ls $WORK_DIR/*.[cC][pP][pP] 1>/dev/null 2>/dev/null
	if [ $? = 0 ]; then
		SRC_CPP_DIR=$WORK_DIR" "$SRC_CPP_DIR
	fi
}

# 扫描$1目录中的.h文件，并记录所有目录
dir_have_h()
{
	WORK_DIR=$1
	
	ls $WORK_DIR/*.[hH] 1>/dev/null 2>/dev/null
	if [ $? = 0 ]; then
		SRC_H_DIR=$WORK_DIR" "$SRC_H_DIR
	fi
}

# 扫描$1目录中的源文件
dir_have_src()
{
	dir_have_c   $1
	dir_have_cpp $1
	dir_have_h	 $1
}

# 扫描$1目录中的源文件，迭代扫描子目录
search_dir()
{
	local CUR_DIR=$1
#此处去掉local	
	SUB_DIR=`ls -D $CUR_DIR`

	for i in $SUB_DIR
	do	
		if test -d $CUR_DIR/$i; then
				echo Search directory: $CUR_DIR/$i
				dir_have_src $CUR_DIR/$i
				search_dir $CUR_DIR/$i
		fi
	done	
}

# 暂存MYLIB中的源文件目录
save_mylib_dep()
{
	MYLIB_SRC_C_DIR=$SRC_C_DIR
	MYLIB_SRC_CPP_DIR=$SRC_CPP_DIR
	MYLIB_H_DIR=$SRC_H_DIR
}

# 处理mylib中需要忽略的子目录（定义在config.mk中的$IGNORE_DIR）
ignore_mylib_dir()
{
	echo
	echo Need ignore directory:
	
	for i in $1
	do
		echo $i
	
		DIR_SET_IGNORED=""
		for j in $MYLIB_SRC_C_DIR
		do
			expr match $j $i > /dev/null; \
			if [ $? != 0 ]; then
				DIR_SET_IGNORED=$DIR_SET_IGNORED" "$j
			fi
		done
		MYLIB_SRC_C_DIR=$DIR_SET_IGNORED
		
		DIR_SET_IGNORED=""
		for j in $MYLIB_SRC_CPP_DIR
		do
			expr match $j $i > /dev/null; \
			if [ $? != 0 ]; then
				DIR_SET_IGNORED=$DIR_SET_IGNORED" "$j
			fi
		done
		MYLIB_SRC_CPP_DIR=$DIR_SET_IGNORED

		DIR_SET_IGNORED=""
		for j in $MYLIB_H_DIR
		do
			expr match $j $i > /dev/null; \
			if [ $? != 0 ]; then
				DIR_SET_IGNORED=$DIR_SET_IGNORED" "$j
			fi
		done
		MYLIB_H_DIR=$DIR_SET_IGNORED

	done	
}

# 打印依赖关系到$1文件中
printf_dep()
{
	DEP_FILE=$2
	
	echo 													> $DEP_FILE

	printf_a_break_line
	echo '# '$1': the dependent relationship of mylib'	>> $DEP_FILE
	printf_a_break_line
	printf_a_blank_line

	CUR_SRC_C_DIR=$MYLIB_SRC_C_DIR
	CUR_SRC_CPP_DIR=$MYLIB_SRC_CPP_DIR
	
	printf_src_obj_dir $3

	printf_h_dir $3
	
	printf_src_obj $3

	printf_src_dep $1
}

# 打印一个空行
printf_a_blank_line()
{
	echo >> $DEP_FILE	
}

# 打印一行80个#，作为分隔符
printf_a_break_line()
{
	echo '################################################################################' >> $DEP_FILE
}

# 打印$1目录相关的源文件目录和.o目录
# $1: MYLIB
printf_src_obj_dir()
{
	echo $1'_SRC_C_DIR := '$CUR_SRC_C_DIR 		>> $DEP_FILE
	printf_a_blank_line
	echo $1'_SRC_CPP_DIR := '$CUR_SRC_CPP_DIR 	>> $DEP_FILE
	printf_a_blank_line

	# ../ -> null
	CUR_OBJ_C_DIR="`echo $CUR_SRC_C_DIR     | sed -n 's/\.\.\///gp '`"
	CUR_OBJ_CPP_DIR="`echo $CUR_SRC_CPP_DIR | sed -n 's/\.\.\///gp '`"

	echo $1'_OBJ_C_DIR := '$CUR_OBJ_C_DIR 		>> $DEP_FILE
	printf_a_blank_line
	echo $1'_OBJ_CPP_DIR := '$CUR_OBJ_CPP_DIR 	>> $DEP_FILE
	printf_a_blank_line
}

# 打印头文件目录
printf_h_dir()
{
	echo $1'_H_DIR := '$MYLIB_H_DIR		>> $DEP_FILE
	printf_a_blank_line
	
	echo 'COMMON_MYLIB_H_DIR = '"`echo $MYLIB_H_DIR | sed -n 's/\.\.\///gp '`" >> $DEP_FILE
}

# 打印$1目录相关的源文件和.o变量
# $1: MYLIB
printf_src_obj()
{
	echo '# Use to create subdir of .o'	>> $DEP_FILE
	echo $1'_OBJ_DIR := $(addprefix $(OBJ_DIR)/, $('$1'_OBJ_C_DIR) $('$1'_OBJ_CPP_DIR))' >> $DEP_FILE
	printf_a_blank_line

	echo $1'_SRC_C	:= $(foreach i, $('$1'_SRC_C_DIR), $(wildcard $(i)/*.c))' 		>> $DEP_FILE
	echo $1'_SRC_CPP := $(foreach i, $('$1'_SRC_CPP_DIR), $(wildcard $(i)/*.cpp))'	>> $DEP_FILE
	echo $1'_OBJ_C	:= $(patsubst %.c, %.o, $('$1'_SRC_C))' 		>> $DEP_FILE
	echo $1'_OBJ_CPP := $(patsubst %.cpp, %.o, $('$1'_SRC_CPP))' >> $DEP_FILE
	printf_a_blank_line
	
	echo $1'_OBJ := $('$1'_OBJ_C) $('$1'_OBJ_CPP)' 							>> $DEP_FILE
	echo $1'_OBJ := $(patsubst $(PROJ_ROOT)/%, $(OBJ_DIR)/%, $('$1'_OBJ))'	>> $DEP_FILE
	echo $1'_OBJ := $(patsubst $(OBJ_DIR)/..%, $(OBJ_DIR)%, $('$1'_OBJ))'	>> $DEP_FILE
	echo $1'_DEP := $('$1'_OBJ:.o=.d)'										>> $DEP_FILE
	printf_a_blank_line
}

# 打印.o和源文件的生成规则
printf_src_dep()
{
	for i in $CUR_OBJ_C_DIR
	do
		expr match $i 'SOFTMODULE' > /dev/null; \
		if [ $? != 0 ]; then
			echo '$(OBJ_DIR)/'"$i/%.o: $1/$i/%.c" 			>> $DEP_FILE
			echo '	@echo Compile $< ...'					>> $DEP_FILE
			echo '	$(MAKE_C) $< -o $@'						>> $DEP_FILE
			echo '	@echo'									>> $DEP_FILE	
			printf_a_blank_line
		else
			echo '$(OBJ_DIR)/'"$i/%.o: $1/$i/%.c" 			>> $DEP_FILE
			echo '	@echo Compile $< ...'					>> $DEP_FILE
			echo '	$(MAKE_C) $< -o $@'						>> $DEP_FILE
			echo '	@echo'									>> $DEP_FILE	
			printf_a_blank_line
		fi
	done	

	for i in $CUR_OBJ_CPP_DIR
	do
		expr match $i 'SOFTMODULE' > /dev/null; \
		if [ $? != 0 ]; then
			echo '$(OBJ_DIR)/'"$i/%.o: $1/$i/%.cpp"			>> $DEP_FILE
			echo '	@echo Compile $< ...'					>> $DEP_FILE
			echo '	$(MAKE_CPP) $< -o $@'						>> $DEP_FILE
			echo '	@echo'									>> $DEP_FILE
			printf_a_blank_line
		else
			echo '$(OBJ_DIR)/'"$i/%.o: $1/$i/%.cpp"			>> $DEP_FILE
			echo '	@echo Compile $< ...'					>> $DEP_FILE
			echo '	$(MAKE_CPP) $< -o $@'						>> $DEP_FILE
			echo '	@echo'									>> $DEP_FILE
			printf_a_blank_line
		fi
	done
}

# 产生依赖关系，并输出到.dep文件
# $1, project root directory.
generate_dep()
{
#	if [$# = 3];then
#		search_dir $1
#		save_mylib_dep
#		printf_dep $1 $2 $3
#	elif [$# = 4];then
		search_dir $1
		save_mylib_dep
		ignore_mylib_dir $4
		printf_dep $1 $2 $3
#	else
#		echo 'Parameter Num Wrong:'
#		echo $#
#		echo '$1='$1
#		echo '$2='$2
#		echo '$3='$3
#		echo '$4='$4
#	fi
}

# 传入参数说明：
#	$1:被编译的工程的跟路径
# $2:生成的依赖文件名称
# $3:依赖文件中的工程名称前缀
# $4:ignore dir
generate_dep $1 $2 $3 $4

exit 0
