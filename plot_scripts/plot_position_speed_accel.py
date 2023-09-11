import matplotlib.pyplot as plt

steps_per_turn = 800
ticks_per_second = 2000000

def avg(a,b):
    return (a + b) / 2

def ticks_to_sec(ticks):
    return float(ticks) / ticks_per_second

def steps_to_turns(steps):
    return float(steps) / steps_per_turn

ticks_per_step = \
[35355,21213,16499,13960,12318,11145,10253,9546,8967,8483,8069,7710,7395,7116,6867,6641,6437,6250,6079,5921,5775,5639,5512,5394,5283,5178,5079,4986,4898,4814,4735,4659,4587,4518,4452,4389,4328,4270,4214,4160,4109,4059,4011,3964,3920,3876,3834,3794,3755,3716,3679,3644,3609,3575,3542,3510,3479,3448,3419,3390,3362,3334,3307,3281,3256,3231,3206,3182,3159,3136,3114,3092,3071,3050,3029,3009,2989,2970,2951,2932,2914,2896,2879,2861,2844,2828,2811,2795,2779,2764,2748,2733,2719,2704,2690,2675,2662,2648,2634,2621,2608,2595,2583,2570,2558,2546,2534,2522,2510,2499,2487,2476,2465,2454,2443,2433,2422,2412,2402,2392,2382,2372,2362,2353,2343,2334,2325,2315,2306,2298,2289,2280,2271,2263,2254,2246,2238,2230,2222,2214,2206,2198,2190,2183,2175,2167,2160,2153,2145,2138,2131,2124,2117,2110,2103,2097,2090,2083,2077,2070,2064,2057,2051,2045,2038,2032,2026,2020,2014,2008,2002,1996,1991,1985,1979,1974,1968,1962,1957,1951,1946,1941,1935,1930,1925,1920,1914,1909,1904,1899,1894,1889,1884,1879,1875,1870,1865,1860,1856,1851,1846,1842,1837,1833,1828,1824,1819,1815,1811,1806,1802,1798,1793,1789,1785,1781,1777,1773,1769,1765,1761,1757,1753,1749,1745,1741,1737,1733,1729,1726,1722,1718,1715,1711,1707,1704,1700,1696,1693,1689,1686,1682,1679,1675,1672,1669,1665,1662,1658,1655,1652,1648,1645,1642,1639,1636,1632,1629,1626,1623,1620,1617,1614,1611,1607,1604,1601,1598,1595,1592,1590,1587,1584,1581,1578,1575,1572,1569,1567,1564,1561,1558,1555,1553,1550,1547,1544,1542,1539,1536,1534,1531,1529,1526,1523,1521,1518,1516,1513,1511,1508,1506,1503,1501,1498,1496,1493,1491,1488,1486,1484,1481,1479,1476,1474,1472,1469,1467,1465,1463,1460,1458,1456,1453,1451,1449,1447,1445,1442,1440,1438,1436,1434,1431,1429,1427,1425,1423,1421,1419,1417,1415,1413,1410,1408,1406,1404,1402,1400,1398,1396,1394,1392,1390,1388,1386,1385,1383,1381,1379,1377,1375,1373,1371,1369,1367,1366,1364,1362,1360,1358,1356,1354,1353,1351,1349,1347,1345,1344,1342,1340,1338,1337,1335,1333,1331,1330,1328,1326,1325,1323,1321,1319,1318,1316,1314,1313,1311,1310,1308,1306,1305,1303,1301,1300,1298,1297,1295,1293,1292,1290,1289,1287,1286,1284,1282,1281,1279,1278,1276,1275,1273,1272,1270,1269,1267,1266,1264,1263,1261,1260,1258,1257,1256,1254,1253,1251,1250,1248,1247,1245,1244,1243,1241,1240,1238,1237,1236,1234,1233,1232,1230,1229,1228,1226,1225,1223,1222,1221,1219,1218,1217,1215,1214,1213,1212,1210,1209,1208,1206,1205,1204,1203,1201,1200,1199,1197,1196,1195,1194,1193,1191,1190,1189,1188,1186,1185,1184,1183,1181,1180,1179,1178,1177,1175,1174,1173,1172,1171,1170,1168,1167,1166,1165,1164,1163,1161,1160,1159,1158,1157,1156,1155,1154,1152,1151,1150,1149,1148,1147,1146,1145,1144,1142,1141,1140,1139,1138,1137,1136,1135,1134,1133,1132,1131,1130,1129,1127,1126,1125,1124,1123,1122,1121,1120,1119,1118,1117,1116,1115,1114,1113,1112,1111,1110,1109,1108,1107,1106,1105,1104,1103,1102,1101,1100,1099,1098,1097,1096,1095,1094,1093,1092,1091,1091,1090,1089,1088,1087,1086,1085,1084,1083,1082,1081,1080,1079,1078,1077,1077,1076,1075,1074,1073,1072,1071,1070,1069,1068,1068,1067,1066,1065,1064,1063,1062,1061,1060,1060,1059,1058,1057,1056,1055,1054,1054,1053,1052,1051,1050,1049,1048,1048,1047,1046,1045,1044,1043,1043,1042,1041,1040,1039,1038,1038,1037,1036,1035,1034,1034,1033,1032,1031,1030,1030,1029,1028,1027,1026,1026,1025,1024,1023,1022,1022,1021,1020,1019,1019,1018,1017,1016,1016,1015,1014,1013,1012,1012,1011,1010,1009,1009,1008,1007,1006,1006,1005,1004,1003,1003,1002,1001,1001,1000,999,998,998,997,996,995,995,994,993,993,992,991,990,990,989,988,988,987,986,985,985,984,983,983,982,981,981,980,979,979,978,977,976,976,975,974,974,973,972,972,971,970,970,969,968,968,967,966,966,965,964,964,963,962,962,961,961,960,959,959,958,957,957,956,955,955,954,953,953,952,952,951,950,950,949,948,948,947,947,946,945,945,944,943,943,942,942,941,940,940,939,939,938,937,937,936,936,935,934,934,933,933,932,931,931,930,930,929,928,928,927,927,926,926,925,924,924,924,925,926,926,927,927,928,928,929,930,930,931,931,932,933,933,934,934,935,936,936,937,937,938,939,939,940,940,941,942,942,943,943,944,945,945,946,947,947,948,948,949,950,950,951,952,952,953,953,954,955,955,956,957,957,958,959,959,960,961,961,962,963,963,964,964,965,966,966,967,968,968,969,970,970,971,972,972,973,974,974,975,976,977,977,978,979,979,980,981,981,982,983,983,984,985,986,986,987,988,988,989,990,990,991,992,993,993,994,995,996,996,997,998,998,999,1000,1001,1001,1002,1003,1004,1004,1005,1006,1007,1007,1008,1009,1010,1010,1011,1012,1013,1013,1014,1015,1016,1016,1017,1018,1019,1019,1020,1021,1022,1023,1023,1024,1025,1026,1027,1027,1028,1029,1030,1031,1031,1032,1033,1034,1035,1035,1036,1037,1038,1039,1039,1040,1041,1042,1043,1044,1044,1045,1046,1047,1048,1049,1050,1050,1051,1052,1053,1054,1055,1055,1056,1057,1058,1059,1060,1061,1062,1062,1063,1064,1065,1066,1067,1068,1069,1070,1070,1071,1072,1073,1074,1075,1076,1077,1078,1079,1080,1080,1081,1082,1083,1084,1085,1086,1087,1088,1089,1090,1091,1092,1093,1094,1095,1096,1097,1098,1099,1099,1100,1101,1102,1103,1104,1105,1106,1107,1108,1109,1110,1111,1112,1113,1114,1115,1116,1117,1118,1119,1121,1122,1123,1124,1125,1126,1127,1128,1129,1130,1131,1132,1133,1134,1135,1136,1137,1138,1140,1141,1142,1143,1144,1145,1146,1147,1148,1149,1151,1152,1153,1154,1155,1156,1157,1158,1160,1161,1162,1163,1164,1165,1167,1168,1169,1170,1171,1172,1174,1175,1176,1177,1178,1180,1181,1182,1183,1184,1186,1187,1188,1189,1191,1192,1193,1194,1195,1197,1198,1199,1201,1202,1203,1204,1206,1207,1208,1210,1211,1212,1213,1215,1216,1217,1219,1220,1221,1223,1224,1225,1227,1228,1229,1231,1232,1234,1235,1236,1238,1239,1240,1242,1243,1245,1246,1248,1249,1250,1252,1253,1255,1256,1258,1259,1261,1262,1264,1265,1266,1268,1269,1271,1272,1274,1275,1277,1279,1280,1282,1283,1285,1286,1288,1289,1291,1293,1294,1296,1297,1299,1301,1302,1304,1305,1307,1309,1310,1312,1314,1315,1317,1319,1320,1322,1324,1325,1327,1329,1331,1332,1334,1336,1338,1339,1341,1343,1345,1346,1348,1350,1352,1354,1355,1357,1359,1361,1363,1365,1367,1368,1370,1372,1374,1376,1378,1380,1382,1384,1386,1388,1390,1391,1393,1395,1397,1399,1401,1403,1406,1408,1410,1412,1414,1416,1418,1420,1422,1424,1426,1428,1431,1433,1435,1437,1439,1441,1444,1446,1448,1450,1453,1455,1457,1459,1462,1464,1466,1469,1471,1473,1476,1478,1480,1483,1485,1487,1490,1492,1495,1497,1500,1502,1505,1507,1510,1512,1515,1517,1520,1522,1525,1528,1530,1533,1536,1538,1541,1544,1546,1549,1552,1554,1557,1560,1563,1566,1568,1571,1574,1577,1580,1583,1586,1589,1591,1594,1597,1600,1603,1606,1610,1613,1616,1619,1622,1625,1628,1631,1635,1638,1641,1644,1647,1651,1654,1657,1661,1664,1667,1671,1674,1678,1681,1685,1688,1692,1695,1699,1703,1706,1710,1713,1717,1721,1725,1728,1732,1736,1740,1744,1748,1752,1756,1759,1764,1768,1772,1776,1780,1784,1788,1792,1797,1801,1805,1809,1814,1818,1823,1827,1832,1836,1841,1845,1850,1854,1859,1864,1869,1873,1878,1883,1888,1893,1898,1903,1908,1913,1918,1924,1929,1934,1939,1945,1950,1956,1961,1967,1972,1978,1984,1989,1995,2001,2007,2013,2019,2025,2031,2037,2043,2050,2056,2062,2069,2075,2082,2089,2095,2102,2109,2116,2123,2130,2137,2144,2151,2159,2166,2174,2181,2189,2197,2204,2212,2220,2228,2236,2245,2253,2261,2270,2279,2287,2296,2305,2314,2323,2332,2342,2351,2361,2370,2380,2390,2400,2410,2421,2431,2442,2453,2463,2475,2486,2497,2509,2520,2532,2544,2556,2568,2581,2594,2606,2620,2633,2646,2660,2674,2688,2702,2717,2732,2747,2762,2778,2793,2810,2826,2843,2860,2877,2894,2912,2931,2949,2968,2988,3007,3027,3048,3069,3090,3112,3134,3157,3180,3204,3229,3254,3279,3305,3332,3359,3388,3416,3446,3476,3508,3540,3573,3606,3641,3677,3714,3752,3791,3832,3874,3917,3962,4008,4056,4106,4158,4211,4267,4325,4386,4449,4515,4584,4656,4732,4811,4895,4983,5076,5175,5279,5391,5509,5636,5771,5918,6075,6247,6433,6637,6862,7112,7391,7705,8064,8477,8962,9540,10247,11138,12310,13951,16488,21199,]


n = len(ticks_per_step)
sec_per_step = [ticks_to_sec(ticks) for ticks in ticks_per_step]

# speed (steps/sec)
steps_per_sec = [1/t for t in sec_per_step] + [0]
# speed (ticks/sec)
turns_per_sec = [0] + [steps_to_turns(steps) for steps in steps_per_sec] + [0]

# time (sec)
time = [0]
for i in range(n):
    time.append(time[i]+sec_per_step[i])

# position (turns)
position = [0]
for i in range(n):
    position.append(position[i]+avg(turns_per_sec[i],turns_per_sec[i+1])*sec_per_step[i])

acceleration = [float('inf')]
for i in range(n):
    acceleration.append((avg(turns_per_sec[i+1],turns_per_sec[i+2]) - avg(turns_per_sec[i],turns_per_sec[i+1]))/sec_per_step[i])

plt.plot(time, position, label="position (turns)")
turns_per_sec.pop()
turns_per_sec.pop()
plt.plot(time, turns_per_sec, label="speed (turns/sec)")
plt.plot(time, acceleration, label="acceleration (turns/sec/sec)")
plt.legend()
plt.show()