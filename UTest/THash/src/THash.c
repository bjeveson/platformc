/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : THash.c                                                        */
/*  Descript :                                                                */
/*  API Pref :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.29                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Descript                                 */
/*  1.                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Include Head File,Order:Lib Head File,Public Head File,Private Head File   */
/*============================================================================*/
#include <stdio.h>
#include <stdlib.h>

#include "VarType.h"
#include "ErrorCode.h"
#include "TestDrv.h"
#include "Log.h"
#include "List.h"
#include "MemoryPool.h"
#include "Hash.h"
#include "THash.h"
/*============================================================================*/
/* Macro Define                                                               */
/*============================================================================*/

/*============================================================================*/
/* Gloabal Var                                                                */
/*============================================================================*/
static THashNode *pHashTable = NULL;

static UINT32 Data[] = {
                            0x1cae80b0,
                            0x1cae80f0,
                            0x1cae8130,
                            0x1cae8170,
                            0x1cae81b0,
                            0x1cae81f0,
                            0x1cae8230,
                            0x1cae8270,
                            0x1cae82b0,
                            0x1cae82f0,
                            0x1cae8330,
                            0x1cae8370,
                            0x1cae83b0,
                            0x1cae83f0,
                            0x1cae8430,
                            0x1cae8470,
                            0x1cae84b0,
                            0x1cae84f0,
                            0x1cae8530,
                            0x1cae8570,
                            0x1cae85b0,
                            0x1cae85f0,
                            0x1cae8630,
                            0x1cae8670,
                            0x1cae86b0,
                            0x1cae86f0,
                            0x1cae8730,
                            0x1cae8770,
                            0x1cae87b0,
                            0x1cae87f0,
                            0x1cae8830,
                            0x1cae8870,
                            0x1cae88b0,
                            0x1cae88f0,
                            0x1cae8930,
                            0x1cae8970,
                            0x1cae89b0,
                            0x1cae89f0,
                            0x1cae8a30,
                            0x1cae8a70,
                            0x1cae8ab0,
                            0x1cae8af0,
                            0x1cae8b30,
                            0x1cae8b70,
                            0x1cae8bb0,
                            0x1cae8bf0,
                            0x1cae8c30,
                            0x1cae8c70,
                            0x1cae8cb0,
                            0x1cae8cf0,
                            0x1cae8d30,
                            0x1cae8d70,
                            0x1cae8db0,
                            0x1cae8df0,
                            0x1cae8e30,
                            0x1cae8e70,
                            0x1cae8eb0,
                            0x1cae8ef0,
                            0x1cae8f30,
                            0x1cae8f70,
                            0x1cae8fb0,
                            0x1cae8ff0,
                            0x1cae9030,
                            0x1cae9070,
                            0x1cae90b0,
                            0x1cae90f0,
                            0x1cae9130,
                            0x1cae9170,
                            0x1cae91b0,
                            0x1cae91f0,
                            0x1cae9230,
                            0x1cae9270,
                            0x1cae92b0,
                            0x1cae92f0,
                            0x1cae9330,
                            0x1cae9370,
                            0x1cae93b0,
                            0x1cae93f0,
                            0x1cae9430,
                            0x1cae9470,
                            0x1cae94b0,
                            0x1cae94f0,
                            0x1cae9530,
                            0x1cae9570,
                            0x1cae95b0,
                            0x1cae95f0,
                            0x1cae9630,
                            0x1cae9670,
                            0x1cae96b0,
                            0x1cae96f0,
                            0x1cae9730,
                            0x1cae9770,
                            0x1cae97b0,
                            0x1cae97f0,
                            0x1cae9830,
                            0x1cae9870,
                            0x1cae98b0,
                            0x1cae98f0,
                            0x1cae9930,
                            0x1cae9970,
                            0x1cae99b0,
                            0x1cae99f0,
                            0x1cae9a30,
                            0x1cae9a70,
                            0x1cae9ab0,
                            0x1cae9af0,
                            0x1cae9b30,
                            0x1cae9b70,
                            0x1cae9bb0,
                            0x1cae9bf0,
                            0x1cae9c30,
                            0x1cae9c70,
                            0x1cae9cb0,
                            0x1cae9cf0,
                            0x1cae9d30,
                            0x1cae9d70,
                            0x1cae9db0,
                            0x1cae9df0,
                            0x1cae9e30,
                            0x1cae9e70,
                            0x1cae9eb0,
                            0x1cae9ef0,
                            0x1cae9f30,
                            0x1cae9f70,
                            0x1cae9fb0,
                            0x1cae9ff0,
                            0x1caea030,
                            0x1caea070,
                            0x1caea0b0,
                            0x1caea0f0,
                            0x1caea130,
                            0x1caea170,
                            0x1caea1b0,
                            0x1caea1f0,
                            0x1caea230,
                            0x1caea270,
                            0x1caea2b0,
                            0x1caea2f0,
                            0x1caea330,
                            0x1caea370,
                            0x1caea3b0,
                            0x1caea3f0,
                            0x1caea430,
                            0x1caea470,
                            0x1caea4b0,
                            0x1caea4f0,
                            0x1caea530,
                            0x1caea570,
                            0x1caea5b0,
                            0x1caea5f0,
                            0x1caea630,
                            0x1caea670,
                            0x1caea6b0,
                            0x1caea6f0,
                            0x1caea730,
                            0x1caea770,
                            0x1caea7b0,
                            0x1caea7f0,
                            0x1caea830,
                            0x1caea870,
                            0x1caea8b0,
                            0x1caea8f0,
                            0x1caea930,
                            0x1caea970,
                            0x1caea9b0,
                            0x1caea9f0,
                            0x1caeaa30,
                            0x1caeaa70,
                            0x1caeaab0,
                            0x1caeaaf0,
                            0x1caeab30,
                            0x1caeab70,
                            0x1caeabb0,
                            0x1caeabf0,
                            0x1caeac30,
                            0x1caeac70,
                            0x1caeacb0,
                            0x1caeacf0,
                            0x1caead30,
                            0x1caead70,
                            0x1caeadb0,
                            0x1caeadf0,
                            0x1caeae30,
                            0x1caeae70,
                            0x1caeaeb0,
                            0x1caeaef0,
                            0x1caeaf30,
                            0x1caeaf70,
                            0x1caeafb0,
                            0x1caeaff0,
                            0x1caeb030,
                            0x1caeb070,
                            0x1caeb0b0,
                            0x1caeb0f0,
                            0x1caeb130,
                            0x1caeb170,
                            0x1caeb1b0,
                            0x1caeb1f0,
                            0x1caeb230,
                            0x1caeb270,
                            0x1caeb2b0,
                            0x1caeb2f0,
                            0x1caeb330,
                            0x1caeb370,
                            0x1caeb3b0,
                            0x1caeb3f0,
                            0x1caeb430,
                            0x1caeb470,
                            0x1caeb4b0,
                            0x1caeb4f0,
                            0x1caeb530,
                            0x1caeb570,
                            0x1caeb5b0,
                            0x1caeb5f0,
                            0x1caeb630,
                            0x1caeb670,
                            0x1caeb6b0,
                            0x1caeb6f0,
                            0x1caeb730,
                            0x1caeb770,
                            0x1caeb7b0,
                            0x1caeb7f0,
                            0x1caeb830,
                            0x1caeb870,
                            0x1caeb8b0,
                            0x1caeb8f0,
                            0x1caeb930,
                            0x1caeb970,
                            0x1caeb9b0,
                            0x1caeb9f0,
                            0x1caeba30,
                            0x1caeba70,
                            0x1caebab0,
                            0x1caebaf0,
                            0x1caebb30,
                            0x1caebb70,
                            0x1caebbb0,
                            0x1caebbf0,
                            0x1caebc30,
                            0x1caebc70,
                            0x1caebcb0,
                            0x1caebcf0,
                            0x1caebd30,
                            0x1caebd70,
                            0x1caebdb0,
                            0x1caebdf0,
                            0x1caebe30,
                            0x1caebe70,
                            0x1caebeb0,
                            0x1caebef0,
                            0x1caebf30,
                            0x1caebf70,
                            0x1caebfb0,
                            0x1caebff0,
                            0x1caec030,
                            0x1caec070,
                            0x1caec0b0,
                            0x1caec0f0,
                            0x1caec130,
                            0x1caec170,
                            0x1caec1b0,
                            0x1caec1f0,
                            0x1caec230,
                            0x1caec270,
                            0x1caec2b0,
                            0x1caec2f0,
                            0x1caec330,
                            0x1caec370,
                            0x1caec3b0,
                            0x1caec3f0,
                            0x1caec430,
                            0x1caec470,
                            0x1caec4b0,
                            0x1caec4f0,
                            0x1caec530,
                            0x1caec570,
                            0x1caec5b0,
                            0x1caec5f0,
                            0x1caec630,
                            0x1caec670,
                            0x1caec6b0,
                            0x1caec6f0,
                            0x1caec730,
                            0x1caec770,
                            0x1caec7b0,
                            0x1caec7f0,
                            0x1caec830,
                            0x1caec870,
                            0x1caec8b0,
                            0x1caec8f0,
                            0x1caec930,
                            0x1caec970,
                            0x1caec9b0,
                            0x1caec9f0,
                            0x1caeca30,
                            0x1caeca70,
                            0x1caecab0,
                            0x1caecaf0,
                            0x1caecb30,
                            0x1caecb70,
                            0x1caecbb0,
                            0x1caecbf0,
                            0x1caecc30,
                            0x1caecc70,
                            0x1caeccb0,
                            0x1caeccf0,
                            0x1caecd30,
                            0x1caecd70,
                            0x1caecdb0,
                            0x1caecdf0,
                            0x1caece30,
                            0x1caece70,
                            0x1caeceb0,
                            0x1caecef0,
                            0x1caecf30,
                            0x1caecf70,
                            0x1caecfb0,
                            0x1caecff0,
                            0x1caed030,
                            0x1caed070,
                            0x1caed0b0,
                            0x1caed0f0,
                            0x1caed130,
                            0x1caed170,
                            0x1caed1b0,
                            0x1caed1f0,
                            0x1caed230,
                            0x1caed270,
                            0x1caed2b0,
                            0x1caed2f0,
                            0x1caed330,
                            0x1caed370,
                            0x1caed3b0,
                            0x1caed3f0,
                            0x1caed430,
                            0x1caed470,
                            0x1caed4b0,
                            0x1caed4f0,
                            0x1caed530,
                            0x1caed570,
                            0x1caed5b0,
                            0x1caed5f0,
                            0x1caed630,
                            0x1caed670,
                            0x1caed6b0,
                            0x1caed6f0,
                            0x1caed730,
                            0x1caed770,
                            0x1caed7b0,
                            0x1caed7f0,
                            0x1caed830,
                            0x1caed870,
                            0x1caed8b0,
                            0x1caed8f0,
                            0x1caed930,
                            0x1caed970,
                            0x1caed9b0,
                            0x1caed9f0,
                            0x1caeda30,
                            0x1caeda70,
                            0x1caedab0,
                            0x1caedaf0,
                            0x1caedb30,
                            0x1caedb70,
                            0x1caedbb0,
                            0x1caedbf0,
                            0x1caedc30,
                            0x1caedc70,
                            0x1caedcb0,
                            0x1caedcf0,
                            0x1caedd30,
                            0x1caedd70,
                            0x1caeddb0,
                            0x1caeddf0,
                            0x1caede30,
                            0x1caede70,
                            0x1caedeb0,
                            0x1caedef0,
                            0x1caedf30,
                            0x1caedf70,
                            0x1caedfb0,
                            0x1caedff0,
                            0x1caee030,
                            0x1caee070,
                            0x1caee0b0,
                            0x1caee0f0,
                            0x1caee130,
                            0x1caee170,
                            0x1caee1b0,
                            0x1caee1f0,
                            0x1caee230,
                            0x1caee270,
                            0x1caee2b0,
                            0x1caee2f0,
                            0x1caee330,
                            0x1caee370,
                            0x1caee3b0,
                            0x1caee3f0,
                            0x1caee430,
                            0x1caee470,
                            0x1caee4b0,
                            0x1caee4f0,
                            0x1caee530,
                            0x1caee570,
                            0x1caee5b0,
                            0x1caee5f0,
                            0x1caee630,
                            0x1caee670,
                            0x1caee6b0,
                            0x1caee6f0,
                            0x1caee730,
                            0x1caee770,
                            0x1caee7b0,
                            0x1caee7f0,
                            0x1caee830,
                            0x1caee870,
                            0x1caee8b0,
                            0x1caee8f0,
                            0x1caee930,
                            0x1caee970,
                            0x1caee9b0,
                            0x1caee9f0,
                            0x1caeea30,
                            0x1caeea70,
                            0x1caeeab0,
                            0x1caeeaf0,
                            0x1caeeb30,
                            0x1caeeb70,
                            0x1caeebb0,
                            0x1caeebf0,
                            0x1caeec30,
                            0x1caeec70,
                            0x1caeecb0,
                            0x1caeecf0,
                            0x1caeed30,
                            0x1caeed70,
                            0x1caeedb0,
                            0x1caeedf0,
                            0x1caeee30,
                            0x1caeee70,
                            0x1caeeeb0,
                            0x1caeeef0,
                            0x1caeef30,
                            0x1caeef70,
                            0x1caeefb0,
                            0x1caeeff0,
                            0x1caef030,
                            0x1caef070,
                            0x1caef0b0,
                            0x1caef0f0,
                            0x1caef130,
                            0x1caef170,
                            0x1caef1b0,
                            0x1caef1f0,
                            0x1caef230,
                            0x1caef270,
                            0x1caef2b0,
                            0x1caef2f0,
                            0x1caef330,
                            0x1caef370,
                            0x1caef3b0,
                            0x1caef3f0,
                            0x1caef430,
                            0x1caef470,
                            0x1caef4b0,
                            0x1caef4f0,
                            0x1caef530,
                            0x1caef570,
                            0x1caef5b0,
                            0x1caef5f0,
                            0x1caef630,
                            0x1caef670,
                            0x1caef6b0,
                            0x1caef6f0,
                            0x1caef730,
                            0x1caef770,
                            0x1caef7b0,
                            0x1caef7f0,
                            0x1caef830,
                            0x1caef870,
                            0x1caef8b0,
                            0x1caef8f0,
                            0x1caef930,
                            0x1caef970,
                            0x1caef9b0,
                            0x1caef9f0,
                            0x1caefa30,
                            0x1caefa70,
                            0x1caefab0,
                            0x1caefaf0,
                            0x1caefb30,
                            0x1caefb70,
                            0x1caefbb0,
                            0x1caefbf0,
                            0x1caefc30,
                            0x1caefc70,
                            0x1caefcb0,
                            0x1caefcf0,
                            0x1caefd30,
                            0x1caefd70,
                            0x1caefdb0,
                            0x1caefdf0,
                            0x1caefe30,
                            0x1caefe70,
                            0x1caefeb0,
                            0x1caefef0,
                            0x1caeff30,
                            0x1caeff70,
                            0x1caeffb0,
                            0x1caefff0,
                            0x1caf0030,
                            0x1caf0070,
                            0x1caf00b0,
                            0x1caf00f0,
                            0x1caf0130,
                            0x1caf0170,
                            0x1caf01b0,
                            0x1caf01f0,
                            0x1caf0230,
                            0x1caf0270,
                            0x1caf02b0,
                            0x1caf02f0,
                            0x1caf0330,
                            0x1caf0370,
                            0x1caf03b0,
                            0x1caf03f0,
                            0x1caf0430,
                            0x1caf0470,
                            0x1caf04b0,
                            0x1caf04f0,
                            0x1caf0530,
                            0x1caf0570,
                            0x1caf05b0,
                            0x1caf05f0,
                            0x1caf0630,
                            0x1caf0670,
                            0x1caf06b0,
                            0x1caf06f0,
                            0x1caf0730,
                            0x1caf0770,
                            0x1caf07b0,
                            0x1caf07f0,
                            0x1caf0830,
                            0x1caf0870,
                            0x1caf08b0,
                            0x1caf08f0,
                            0x1caf0930,
                            0x1caf0970,
                            0x1caf09b0,
                            0x1caf09f0,
                            0x1caf0a30,
                            0x1caf0a70,
                            0x1caf0ab0,
                            0x1caf0af0,
                            0x1caf0b30,
                            0x1caf0b70,
                            0x1caf0bb0,
                            0x1caf0bf0,
                            0x1caf0c30,
                            0x1caf0c70,
                            0x1caf0cb0,
                            0x1caf0cf0,
                            0x1caf0d30,
                            0x1caf0d70,
                            0x1caf0db0,
                            0x1caf0df0,
                            0x1caf0e30,
                            0x1caf0e70,
                            0x1caf0eb0,
                            0x1caf0ef0,
                            0x1caf0f30,
                            0x1caf0f70,
                            0x1caf0fb0,
                            0x1caf0ff0,
                            0x1caf1030,
                            0x1caf1070,
                            0x1caf10b0,
                            0x1caf10f0,
                            0x1caf1130,
                            0x1caf1170,
                            0x1caf11b0,
                            0x1caf11f0,
                            0x1caf1230,
                            0x1caf1270,
                            0x1caf12b0,
                            0x1caf12f0,
                            0x1caf1330,
                            0x1caf1370,
                            0x1caf13b0,
                            0x1caf13f0,
                            0x1caf1430,
                            0x1caf1470,
                            0x1caf14b0,
                            0x1caf14f0,
                            0x1caf1530,
                            0x1caf1570,
                            0x1caf15b0,
                            0x1caf15f0,
                            0x1caf1630,
                            0x1caf1670,
                            0x1caf16b0,
                            0x1caf16f0,
                            0x1caf1730,
                            0x1caf1770,
                            0x1caf17b0,
                            0x1caf17f0,
                            0x1caf1830,
                            0x1caf1870,
                            0x1caf18b0,
                            0x1caf18f0,
                            0x1caf1930,
                            0x1caf1970,
                            0x1caf19b0,
                            0x1caf19f0,
                            0x1caf1a30,
                            0x1caf1a70,
                            0x1caf1ab0,
                            0x1caf1af0,
                            0x1caf1b30,
                            0x1caf1b70,
                            0x1caf1bb0,
                            0x1caf1bf0,
                            0x1caf1c30,
                            0x1caf1c70,
                            0x1caf1cb0,
                            0x1caf1cf0,
                            0x1caf1d30,
                            0x1caf1d70,
                            0x1caf1db0,
                            0x1caf1df0,
                            0x1caf1e30,
                            0x1caf1e70,
                            0x1caf1eb0,
                            0x1caf1ef0,
                            0x1caf1f30,
                            0x1caf1f70,
                            0x1caf1fb0,
                            0x1caf1ff0,
                            0x1caf2030,
                            0x1caf2070,
                            0x1caf20b0,
                            0x1caf20f0,
                            0x1caf2130,
                            0x1caf2170,
                            0x1caf21b0,
                            0x1caf21f0,
                            0x1caf2230,
                            0x1caf2270,
                            0x1caf22b0,
                            0x1caf22f0,
                            0x1caf2330,
                            0x1caf2370,
                            0x1caf23b0,
                            0x1caf23f0,
                            0x1caf2430,
                            0x1caf2470,
                            0x1caf24b0,
                            0x1caf24f0,
                            0x1caf2530,
                            0x1caf2570,
                            0x1caf25b0,
                            0x1caf25f0,
                            0x1caf2630,
                            0x1caf2670,
                            0x1caf26b0,
                            0x1caf26f0,
                            0x1caf2730,
                            0x1caf2770,
                            0x1caf27b0,
                            0x1caf27f0,
                            0x1caf2830,
                            0x1caf2870,
                            0x1caf28b0,
                            0x1caf28f0,
                            0x1caf2930,
                            0x1caf2970,
                            0x1caf29b0,
                            0x1caf29f0,
                            0x1caf2a30,
                            0x1caf2a70,
                            0x1caf2ab0,
                            0x1caf2af0,
                            0x1caf2b30,
                            0x1caf2b70,
                            0x1caf2bb0,
                            0x1caf2bf0,
                            0x1caf2c30,
                            0x1caf2c70,
                            0x1caf2cb0,
                            0x1caf2cf0,
                            0x1caf2d30,
                            0x1caf2d70,
                            0x1caf2db0,
                            0x1caf2df0,
                            0x1caf2e30,
                            0x1caf2e70,
                            0x1caf2eb0,
                            0x1caf2ef0,
                            0x1caf2f30,
                            0x1caf2f70,
                            0x1caf2fb0,
                            0x1caf2ff0,
                            0x1caf3030,
                            0x1caf3070,
                            0x1caf30b0,
                            0x1caf30f0,
                            0x1caf3130,
                            0x1caf3170,
                            0x1caf31b0,
                            0x1caf31f0,
                            0x1caf3230,
                            0x1caf3270,
                            0x1caf32b0,
                            0x1caf32f0,
                            0x1caf3330,
                            0x1caf3370,
                            0x1caf33b0,
                            0x1caf33f0,
                            0x1caf3430,
                            0x1caf3470,
                            0x1caf34b0,
                            0x1caf34f0,
                            0x1caf3530,
                            0x1caf3570,
                            0x1caf35b0,
                            0x1caf35f0,
                            0x1caf3630,
                            0x1caf3670,
                            0x1caf36b0,
                            0x1caf36f0,
                            0x1caf3730,
                            0x1caf3770,
                            0x1caf37b0,
                            0x1caf37f0                                
                        };
/*============================================================================*/
/* Data Type In This File                                                     */
/*============================================================================*/

/*============================================================================*/
/* Static Function Declare                                                    */
/*============================================================================*/
static void Hash_InitTable_Case(void *Para);

static UINT32 KeyFunction(const void *pStr);

static UINT32 CompareFunc(void *data, void *nextdata);

static void Hash_AddNodeToTable_Case(void *Para);

static void Hash_Search_Case(void *Para);

/*============================================================================*/
/* Non-Static Function Code                                                   */
/*============================================================================*/
/*============================================================================*/
/*  Function : TList_AddTestCase                                              */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.29                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 THash_AddTestCase(void)
{
    TTestCase TestCase[] = 
    {
        {
            "Hash_InitTable_Case",
            Hash_InitTable_Case,
            NULL
        },   
        {
            "Hash_AddNodeToTable_Case",
            Hash_AddNodeToTable_Case,
            NULL
        },     
        {
            "Hash_Search_Case",
            Hash_Search_Case,
            NULL
        },        
        {
            "Over",
            NULL,
            NULL
        }
    };
    
    return TD_AddTestCase(TestCase);
}

/*============================================================================*/
/* Static Function Code                                                       */
/*============================================================================*/
/*============================================================================*/
/*  Function : List_Init_Case                                                 */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.29                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static void Hash_InitTable_Case(void *Para)
{
    pHashTable = Hash_InitTable(HASH_TABLE_LENGTH);
    TD_ASSERT_NOT_EQUAL(pHashTable, NULL);
    
    return;
}

/*============================================================================*/
/*  函数名 : Hash_AddNodeToTable_Case                                         */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.001                                                        */
/*  日  期 : 2014.11.4                                                        */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
static void Hash_AddNodeToTable_Case(void *Para)
{
    UINT32 loop = 0;
    UINT32 EleNum = sizeof(Data) / sizeof(Data[0]);
    UINT32 ret;
    PLNode pNode, pHead;

    for (loop = 0; loop < EleNum; loop++)
    {
        ret = Hash_AddNodeToTable(pHashTable,
                                  HASH_TABLE_LENGTH,
                                  (void *)&Data[loop],
                                  (void *)&Data[loop],
                                  KeyFunction);
        TD_ASSERT_EQUAL(ret, LIB_ERR_SUCCESS);
    }

#if 0
    for (loop = 0; loop < HASH_TABLE_LENGTH; loop++)
    {
        if (pHashTable[loop].ConflictCount != 0)
        {
            printf("%d  %d   ", loop, pHashTable[loop].ConflictCount);
            
            pHead = pHashTable[loop].ConflictNode;
            printf("0x%x ", *(UINT32 *)pHead->Data);
            
            pNode = pHashTable[loop].ConflictNode->Next;
            
            
            while (pNode != pHashTable[loop].ConflictNode)
            {
                printf("0x%x ", *(UINT32 *)pNode->Data);
                pNode = pNode->Next;
            }
            printf("\n");
        }
    }
#endif
    return;
}

/*============================================================================*/
/*  函数名 : Hash_Search_Case                                                 */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.001                                                        */
/*  日  期 : 2014.11.4                                                        */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
static void Hash_Search_Case(void *Para)
{
    void *pHashData = NULL;
    void *pGetHashData;
    UINT32 Value, loop;
    UINT32 EleNum = sizeof(Data) / sizeof(Data[0]);

    for (loop = 0; loop < EleNum; loop++)
    {
        pHashData = (void *)&Data[loop];
        
        pGetHashData = Hash_Search(pHashTable, 
                                   HASH_TABLE_LENGTH, 
                                   pHashData, 
                                   KeyFunction, 
                                   CompareFunc);

        Value = *(UINT32 *)pGetHashData;
        TD_ASSERT_EQUAL(Value, Data[loop]);
    }
    
    return;
}

/*============================================================================*/
/*  函数名 : CompareFunc                                                      */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.001                                                        */
/*  日  期 : 2014.11.4                                                        */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
static UINT32 CompareFunc(void *data, void *nextdata)
{
    UINT32 Data1 = *(UINT32 *)data;
    UINT32 Data2 = *(UINT32 *)nextdata;

    if (Data1 == Data2)
    {
        return LIST_DATA_EQUAL;
    }
    else
    {
        return LIST_DATA_NOT_EQUAL;
    }
}

/*============================================================================*/
/*  函数名 : KeyFunction                                                      */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.001                                                        */
/*  日  期 : 2014.11.4                                                        */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
static UINT32 KeyFunction(const void *pStr)
{
    UINT32 Num = *(UINT32 *)pStr;

    return (Num >> 4) & (HASH_TABLE_LENGTH - 1);
}
