#!/bin/sh

fmap=../FuncNameMap/src/FuncNameMap.c

CreateMapFile()
{
	echo "#include <stdio.h>"			>$fmap
	echo "#include \"VarType.h\""  >> $fmap
	echo "#include \"FuncName.h\"" >> $fmap
	echo " " >> $fmap
	
	j=0;
	for i in $1
	do
		while read line
		do
			name=$(echo "$line" | awk '/[a-zA-Z_]\w*\([^=><;]*\)\s*\r/ {if ($1 == "if" || $1 == "for"){exit 1} else if ($1 == "static" || $1 == "const") {funcname=$3} else {funcname=$2} num=split(funcname,str,""); for(i=1;i<=num;i++){if(str[i]=="("){break} else if(str[i]=="*"){continue} else {printf str[i]}}print ""}')
			if [ -n "$name" ];then
				funcarray[j++]=$name
				echo $name
			fi
		done < $i
	done
	
	i=0;
	while [ $i -lt $j ]
	do 
		echo "extern void" ${funcarray[$i]}"();" >> $fmap
		let i++
	done
	
	echo " " >> $fmap
	echo "static TFuncName g_FuncMap[] =" >> $fmap
	echo "{" 														>> $fmap

	i=0;
	while [ $i -lt $j ]
	do 
		echo "	{"${funcarray[$i]}"," \"${funcarray[$i]}\" "}," >> $fmap
		let i++
	done

	echo "	{""NULL""," \"NULL\" "}," >> $fmap
	echo "};" >> $fmap
	
	CreateFunc_GetInfo
}

CreateFunc_GetInfo()
{
	echo "TFuncName Func_GetInfo(UINT8 *pFuncDesc)" 							>>$fmap
	echo "{"																											>>$fmap
	echo "	UINT32 i = 0;"																				>>$fmap
	echo "	UINT32 Arr_len = ARR_SIZE(g_FuncMap);"								>>$fmap
	echo " "																											>>$fmap
  echo "	if (NULL == pFuncDesc)"																>>$fmap
  echo "	{"																										>>$fmap
  echo "		return g_FuncMap[Arr_len - 1];"											>>$fmap
  echo "	}"																										>>$fmap
	echo " "																											>>$fmap
  echo "	for (i = 0; i < Arr_len; i++)"												>>$fmap
  echo "	{"																										>>$fmap
  echo "		if (0 == strcmp(g_FuncMap[i].pdesc, pFuncDesc))"		>>$fmap
  echo "		{"																									>>$fmap
  echo "			break;"																						>>$fmap
  echo "		}"																									>>$fmap
  echo "	}"																										>>$fmap
	echo " "																											>>$fmap
  echo "	if (i == ARR_SIZE(g_FuncMap))"												>>$fmap	
  echo "	{"																										>>$fmap
  echo "		return g_FuncMap[Arr_len - 1];"											>>$fmap
  echo "	}"																										>>$fmap
  echo " "  																										>>$fmap
  echo "	return g_FuncMap[i];"																	>>$fmap
	echo "}"																											>>$fmap
	echo " "																											>>$fmap
}

CreateMapFile "$1"

exit 0