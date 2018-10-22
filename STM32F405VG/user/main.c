#include "main.h"

#define sense_buffer 100

const uint16_t Sine12bit[64] = {
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
											0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
											0, 0, 0, 0, 0, 4095, 4095, 4095, 4095, 4095, 4095,
                      4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,4095, 4095, 4095, 4095, 4095, 4095,
                      4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095};
/*
const u16 sin1[64] = {484,522,559,595,631,665,697,728,756,781,803,823,839,851,861,866,868,866,861,851,839,823,803,781,756,728,697,665,631,595,559,522,484,446,409,373,337,303,271,240,212,187,165,145,129,117,107,102,100,102,107,117,129,145,165,187,212,240,271,303,337,373,409,446};
const u16 sin2[64] = {740,803,865,926,985,1042,1096,1146,1193,1235,1272,1304,1331,1352,1368,1377,1380,1377,1368,1352,1331,1304,1272,1235,1193,1146,1096,1042,985,926,865,803,740,677,615,554,495,438,384,334,287,245,208,176,149,128,112,103,100,103,112,128,149,176,208,245,287,334,384,438,495,554,615,677};
const u16 sin3[64] = {996,1084,1171,1256,1339,1418,1494,1564,1630,1689,1741,1786,1824,1853,1875,1888,1892,1888,1875,1853,1824,1786,1741,1689,1630,1564,1494,1418,1339,1256,1171,1084,996,908,821,736,653,574,498,428,362,303,251,206,168,139,117,104,100,104,117,139,168,206,251,303,362,428,498,574,653,736,821,908};
const u16 sin4[64] = {1252,1365,1477,1586,1693,1795,1892,1983,2067,2143,2210,2268,2316,2354,2382,2398,2404,2398,2382,2354,2316,2268,2210,2143,2067,1983,1892,1795,1693,1586,1477,1365,1252,1139,1027,918,811,709,612,521,437,361,294,236,188,150,122,106,100,106,122,150,188,236,294,361,437,521,612,709,811,918,1027,1139};
const u16 sin5[64] = {1508,1646,1783,1917,2047,2172,2290,2401,2504,2596,2679,2750,2809,2855,2889,2909,2916,2909,2889,2855,2809,2750,2679,2596,2504,2401,2290,2172,2047,1917,1783,1646,1508,1370,1233,1099,969,844,726,615,512,420,337,266,207,161,127,107,100,107,127,161,207,266,337,420,512,615,726,844,969,1099,1233,1370};
const u16 sin6[64] = {1764,1927,2089,2247,2401,2548,2688,2820,2941,3050,3148,3232,3301,3356,3396,3420,3428,3420,3396,3356,3301,3232,3148,3050,2941,2820,2688,2548,2401,2247,2089,1927,1764,1601,1439,1281,1127,980,840,708,587,478,380,296,227,172,132,108,100,108,132,172,227,296,380,478,587,708,840,980,1127,1281,1439,1601};
const u16 sin7[64] = {2020,2208,2395,2577,2755,2925,3087,3238,3378,3504,3616,3713,3794,3857,3903,3931,3940,3931,3903,3857,3794,3713,3616,3504,3378,3238,3087,2925,2755,2577,2395,2208,2020,1832,1645,1463,1285,1115,953,802,662,536,424,327,246,183,137,109,100,109,137,183,246,327,424,536,662,802,953,1115,1285,1463,1645,1832};
*/
/*
const u16 sin1[32] = {484,559,631,697,756,803,839,861,868,861,839,803,756,697,631,559,484,409,337,271,212,165,129,107,100,107,129,165,212,271,337,409};
const u16 sin2[32] = {740,865,985,1096,1193,1272,1331,1368,1380,1368,1331,1272,1193,1096,985,865,740,615,495,384,287,208,149,112,100,112,149,208,287,384,495,615};
const u16 sin3[32] = {996,1171,1339,1494,1630,1741,1824,1875,1892,1875,1824,1741,1630,1494,1339,1171,996,821,653,498,362,251,168,117,100,117,168,251,362,498,653,821};
const u16 sin4[32] = {1252,1477,1693,1892,2067,2210,2316,2382,2404,2382,2316,2210,2067,1892,1693,1477,1252,1027,811,612,437,294,188,122,100,122,188,294,437,612,811,1027};
const u16 sin5[32] = {1508,1783,2047,2290,2504,2679,2809,2889,2916,2889,2809,2679,2504,2290,2047,1783,1508,1233,969,726,512,337,207,127,100,127,207,337,512,726,969,1233};
const u16 sin6[32] = {1764,2089,2401,2688,2941,3148,3301,3396,3428,3396,3301,3148,2941,2688,2401,2089,1764,1439,1127,840,587,380,227,132,100,132,227,380,587,840,1127,1439};
const u16 sin7[32] = {2020,2395,2755,3087,3378,3616,3794,3903,3940,3903,3794,3616,3378,3087,2755,2395,2020,1645,1285,953,662,424,246,137,100,137,246,424,662,953,1285,1645};
*/

/*											
const u16 sin1[7][64]= {{484,522,559,595,631,665,697,728,756,781,803,823,839,851,861,866,868,866,861,851,839,823,803,781,756,728,697,665,631,595,559,522,484,446,409,373,337,303,271,240,212,187,165,145,129,117,107,102,100,102,107,117,129,145,165,187,212,240,271,303,337,373,409,446},{740,803,865,926,985,1042,1096,1146,1193,1235,1272,1304,1331,1352,1368,1377,1380,1377,1368,1352,1331,1304,1272,1235,1193,1146,1096,1042,985,926,865,803,740,677,615,554,495,438,384,334,287,245,208,176,149,128,112,103,100,103,112,128,149,176,208,245,287,334,384,438,495,554,615,677},{996,1084,1171,1256,1339,1418,1494,1564,1630,1689,1741,1786,1824,1853,1875,1888,1892,1888,1875,1853,1824,1786,1741,1689,1630,1564,1494,1418,1339,1256,1171,1084,996,908,821,736,653,574,498,428,362,303,251,206,168,139,117,104,100,104,117,139,168,206,251,303,362,428,498,574,653,736,821,908},{1252,1365,1477,1586,1693,1795,1892,1983,2067,2143,2210,2268,2316,2354,2382,2398,2404,2398,2382,2354,2316,2268,2210,2143,2067,1983,1892,1795,1693,1586,1477,1365,1252,1139,1027,918,811,709,612,521,437,361,294,236,188,150,122,106,100,106,122,150,188,236,294,361,437,521,612,709,811,918,1027,1139},{1508,1646,1783,1917,2047,2172,2290,2401,2504,2596,2679,2750,2809,2855,2889,2909,2916,2909,2889,2855,2809,2750,2679,2596,2504,2401,2290,2172,2047,1917,1783,1646,1508,1370,1233,1099,969,844,726,615,512,420,337,266,207,161,127,107,100,107,127,161,207,266,337,420,512,615,726,844,969,1099,1233,1370},{1764,1927,2089,2247,2401,2548,2688,2820,2941,3050,3148,3232,3301,3356,3396,3420,3428,3420,3396,3356,3301,3232,3148,3050,2941,2820,2688,2548,2401,2247,2089,1927,1764,1601,1439,1281,1127,980,840,708,587,478,380,296,227,172,132,108,100,108,132,172,227,296,380,478,587,708,840,980,1127,1281,1439,1601},{2020,2208,2395,2577,2755,2925,3087,3238,3378,3504,3616,3713,3794,3857,3903,3931,3940,3931,3903,3857,3794,3713,3616,3504,3378,3238,3087,2925,2755,2577,2395,2208,2020,1832,1645,1463,1285,1115,953,802,662,536,424,327,246,183,137,109,100,109,137,183,246,327,424,536,662,802,953,1115,1285,1463,1645,1832},};
const u16 sin2[7][32]= {{484,559,631,697,756,803,839,861,868,861,839,803,756,697,631,559,484,409,337,271,212,165,129,107,100,107,129,165,212,271,337,409},{740,865,985,1096,1193,1272,1331,1368,1380,1368,1331,1272,1193,1096,985,865,740,615,495,384,287,208,149,112,100,112,149,208,287,384,495,615},{996,1171,1339,1494,1630,1741,1824,1875,1892,1875,1824,1741,1630,1494,1339,1171,996,821,653,498,362,251,168,117,100,117,168,251,362,498,653,821},{1252,1477,1693,1892,2067,2210,2316,2382,2404,2382,2316,2210,2067,1892,1693,1477,1252,1027,811,612,437,294,188,122,100,122,188,294,437,612,811,1027},{1508,1783,2047,2290,2504,2679,2809,2889,2916,2889,2809,2679,2504,2290,2047,1783,1508,1233,969,726,512,337,207,127,100,127,207,337,512,726,969,1233},{1764,2089,2401,2688,2941,3148,3301,3396,3428,3396,3301,3148,2941,2688,2401,2089,1764,1439,1127,840,587,380,227,132,100,132,227,380,587,840,1127,1439},{2020,2395,2755,3087,3378,3616,3794,3903,3940,3903,3794,3616,3378,3087,2755,2395,2020,1645,1285,953,662,424,246,137,100,137,246,424,662,953,1285,1645},};
const u16 sin3[7][16]= {{484,631,756,839,868,839,756,631,484,337,212,129,100,129,212,337},{740,985,1193,1331,1380,1331,1193,985,740,495,287,149,100,149,287,495},{996,1339,1630,1824,1892,1824,1630,1339,996,653,362,168,100,168,362,653},{1252,1693,2067,2316,2404,2316,2067,1693,1252,811,437,188,100,188,437,811},{1508,2047,2504,2809,2916,2809,2504,2047,1508,969,512,207,100,207,512,969},{1764,2401,2941,3301,3428,3301,2941,2401,1764,1127,587,227,100,227,587,1127},{2020,2755,3378,3794,3940,3794,3378,2755,2020,1285,662,246,100,246,662,1285},};
*/


const u16 sin1[7][64]= {{892,911,929,948,965,983,999,1014,1028,1040,1052,1061,1069,1076,1080,1083,1084,1083,1080,1076,1069,1061,1052,1040,1028,1014,999,983,965,948,929,911,892,873,855,836,819,801,785,770,756,744,732,723,715,708,704,701,700,701,704,708,715,723,732,744,756,770,785,801,819,836,855,873},{1020,1051,1082,1113,1142,1171,1198,1223,1246,1267,1286,1302,1316,1326,1334,1338,1340,1338,1334,1326,1316,1302,1286,1267,1246,1223,1198,1171,1142,1113,1082,1051,1020,989,958,927,898,869,842,817,794,773,754,738,724,714,706,702,700,702,706,714,724,738,754,773,794,817,842,869,898,927,958,989},{1148,1192,1235,1278,1319,1359,1397,1432,1465,1494,1520,1543,1562,1577,1587,1594,1596,1594,1587,1577,1562,1543,1520,1494,1465,1432,1397,1359,1319,1278,1235,1192,1148,1104,1061,1018,977,937,899,864,831,802,776,753,734,719,709,702,700,702,709,719,734,753,776,802,831,864,899,937,977,1018,1061,1104},{1276,1332,1388,1443,1496,1548,1596,1641,1683,1721,1755,1784,1808,1827,1841,1849,1852,1849,1841,1827,1808,1784,1755,1721,1683,1641,1596,1548,1496,1443,1388,1332,1276,1220,1164,1109,1056,1004,956,911,869,831,797,768,744,725,711,703,700,703,711,725,744,768,797,831,869,911,956,1004,1056,1109,1164,1220},{1404,1473,1541,1608,1673,1736,1795,1851,1902,1948,1989,2025,2054,2078,2094,2105,2108,2105,2094,2078,2054,2025,1989,1948,1902,1851,1795,1736,1673,1608,1541,1473,1404,1335,1267,1200,1135,1072,1013,957,906,860,819,783,754,730,714,703,700,703,714,730,754,783,819,860,906,957,1013,1072,1135,1200,1267,1335},{1532,1614,1694,1774,1850,1924,1994,2060,2120,2175,2224,2266,2301,2328,2348,2360,2364,2360,2348,2328,2301,2266,2224,2175,2120,2060,1994,1924,1850,1774,1694,1614,1532,1450,1370,1290,1214,1140,1070,1004,944,889,840,798,763,736,716,704,700,704,716,736,763,798,840,889,944,1004,1070,1140,1214,1290,1370,1450},{1660,1754,1847,1939,2027,2113,2193,2269,2339,2402,2458,2507,2547,2579,2602,2615,2620,2615,2602,2579,2547,2507,2458,2402,2339,2269,2193,2113,2027,1939,1847,1754,1660,1566,1473,1381,1293,1207,1127,1051,981,918,862,813,773,741,718,705,700,705,718,741,773,813,862,918,981,1051,1127,1207,1293,1381,1473,1566},};
const u16 sin2[7][32]= {{342,379,415,449,478,502,519,530,534,530,519,502,478,449,415,379,342,305,269,235,206,182,165,154,150,154,165,182,206,235,269,305},{470,532,592,648,696,736,766,784,790,784,766,736,696,648,592,532,470,408,348,292,244,204,174,156,150,156,174,204,244,292,348,408},{598,685,769,847,915,970,1012,1037,1046,1037,1012,970,915,847,769,685,598,511,427,349,281,226,184,159,150,159,184,226,281,349,427,511},{726,838,946,1046,1133,1205,1258,1291,1302,1291,1258,1205,1133,1046,946,838,726,614,506,406,319,247,194,161,150,161,194,247,319,406,506,614},{854,991,1123,1245,1352,1439,1504,1544,1558,1544,1504,1439,1352,1245,1123,991,854,717,585,463,356,269,204,164,150,164,204,269,356,463,585,717},{982,1144,1300,1444,1570,1674,1751,1798,1814,1798,1751,1674,1570,1444,1300,1144,982,820,664,520,394,290,213,166,150,166,213,290,394,520,664,820},{1110,1297,1477,1643,1789,1908,1997,2052,2070,2052,1997,1908,1789,1643,1477,1297,1110,923,743,577,431,312,223,168,150,168,223,312,431,577,743,923},};
const u16 sin3[7][16]= {{342,415,478,519,534,519,478,415,342,269,206,165,150,165,206,269},{470,592,696,766,790,766,696,592,470,348,244,174,150,174,244,348},{598,769,915,1012,1046,1012,915,769,598,427,281,184,150,184,281,427},{726,946,1133,1258,1302,1258,1133,946,726,506,319,194,150,194,319,506},{854,1123,1352,1504,1558,1504,1352,1123,854,585,356,204,150,204,356,585},{982,1300,1570,1751,1814,1751,1570,1300,982,664,394,213,150,213,394,664},{1110,1477,1789,1997,2070,1997,1789,1477,1110,743,431,223,150,223,431,743},};

const u16* const sine[21] = {sin1[0],sin1[1],sin1[2],sin1[3],sin1[4],sin1[5],sin1[6],
sin2[0],sin2[1],sin2[2],sin2[3],sin2[4],sin2[5],sin2[6],
sin3[0],sin3[1],sin3[2],sin3[3],sin3[4],sin3[5],sin3[6]

};
	
	
//const u16* sine[7]={sin1,sin2,sin3,sin4,sin5,sin6,sin7};

void DAC_TIM4_Config(u16 period)
{
        
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
        
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	TIM_DeInit(TIM4);
     
// TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
  TIM_TimeBaseStructure.TIM_Period = period;        //100 => 13k 20=>62.5k 30=>42.37k  10=>120k      9=>130
  TIM_TimeBaseStructure.TIM_Prescaler = 0;                                                               
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;                                                   
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;         
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

  TIM_SelectOutputTrigger(TIM4, TIM_TRGOSource_Update);

  TIM_Cmd(TIM4, ENABLE);	
}
											
											
											
void dac_init(u16 freq,u8 level){
	
	
	
	GPIO_InitTypeDef GPIO_InitStructure;
  DAC_InitTypeDef  DAC_InitStructure;


  RCC_AHB2PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);      
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
         
	//  calculation for period and voltage for timer and 
	u16 period = 0;
	if(level>6){level=6;}
	if(freq<50){freq=50;}
	if(freq>400){freq=400;}
	
	u8 i = 0;
	period = 1280/freq;
	while(period<10){
		i++;
		period = 1280*(0x01<<i)/freq;
	}

	//
	DAC_TIM4_Config(period);
	
	
		/* Enable DAC clock */
	RCC->APB1ENR |= RCC_APB1ENR_DACEN;
	/* Enable DMA1 clock */
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
	
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_T4_TRGO;    
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;        
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;       
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);        

	DAC_SetChannel1Data(DAC_Align_12b_R, 0x0000);
	DAC_Cmd(DAC_Channel_1, ENABLE);
	//DAC_DMACmd(DAC_Channel_1, ENABLE);

	
	
	DMA_InitTypeDef DMA_InitStruct;
	
	DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)(sine[i*7+level]);
	DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStruct.DMA_BufferSize = 64/(0x01<<i);
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;
	DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	
	
	
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&DAC->DHR12R1;
		
			/* Disable DMA */
	DMA_DeInit(DMA1_Stream5);
			
			/* Set channel used */
	DMA_InitStruct.DMA_Channel = DMA_Channel_7;
		
			/* Initialize DMA */
	DMA_Init(DMA1_Stream5, &DMA_InitStruct);
			
			/* Enable DMA Stream for DAC Channel 1 */
	DMA_Cmd(DMA1_Stream5, ENABLE);

			/* Enable DAC Channel 1 */
	DAC_Cmd(DAC_Channel_1, ENABLE);

			/* Enable DMA for DAC Channel 1 */
	DAC_DMACmd(DAC_Channel_1, ENABLE);
	
	
	TIM4->CR1 |= TIM_CR1_CEN;
}



void test_init(void){

	
	
	
	GPIO_InitTypeDef GPIO_InitStructure;
  DAC_InitTypeDef  DAC_InitStructure;


  RCC_AHB2PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);      
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
         
	//  calculation for period and voltage for timer and 


	//
	DAC_TIM4_Config(60000);
	
	
		/* Enable DAC clock */
	RCC->APB1ENR |= RCC_APB1ENR_DACEN;
	/* Enable DMA1 clock */
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
	
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_T4_TRGO;    
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;        
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;       
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);        

	DAC_SetChannel1Data(DAC_Align_12b_R, 0x0000);
	DAC_Cmd(DAC_Channel_1, ENABLE);
	//DAC_DMACmd(DAC_Channel_1, ENABLE);

	
	
	DMA_InitTypeDef DMA_InitStruct;
	
	DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)Sine12bit;
	DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStruct.DMA_BufferSize = 64;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;
	DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	
	
	
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&DAC->DHR12R1;
		
			/* Disable DMA */
	DMA_DeInit(DMA1_Stream5);
			
			/* Set channel used */
	DMA_InitStruct.DMA_Channel = DMA_Channel_7;
		
			/* Initialize DMA */
	DMA_Init(DMA1_Stream5, &DMA_InitStruct);
			
			/* Enable DMA Stream for DAC Channel 1 */
	DMA_Cmd(DMA1_Stream5, ENABLE);

			/* Enable DAC Channel 1 */
	DAC_Cmd(DAC_Channel_1, ENABLE);

			/* Enable DMA for DAC Channel 1 */
	DAC_DMACmd(DAC_Channel_1, ENABLE);
	
	
	TIM4->CR1 |= TIM_CR1_CEN;
}



volatile u16 on9ADC[sense_buffer];
volatile u8 flag=0;

void DMA2_Stream0_IRQHandler(void) {
	//DMA_ClearITPendingBit(DMA2_Stream0,DMA_IT_TCIF0); // reset the interupt
	 DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TEIF0 | DMA_IT_DMEIF0 | DMA_IT_FEIF0 | DMA_IT_TCIF0 | DMA_IT_HTIF0);
	 DMA_Cmd(DMA2_Stream0, DISABLE); // doesn't matter
	 flag=1;
}	



void adc_gpio_init(){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

}


void adc_dma_init(){
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	ADC_DeInit();
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	
	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_3Cycles);
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	
	DMA_InitStructure.DMA_Channel =  DMA_Channel_0;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &(ADC1)->DR;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) &(on9ADC);
	DMA_InitStructure.DMA_BufferSize = sense_buffer;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	
	

	
	DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);
	DMA_Cmd(DMA2_Stream0, ENABLE);
	
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);
	ADC_SoftwareStartConv(ADC1);

}



u32 median(u8 n, u32* x) {
    u16 temp;
    u8 i, j;
    // the following two loops sort the array x in ascending order
    for(i=0; i<n-1; i++) {
        for(j=i+1; j<n; j++) {
            if(x[j] < x[i]) {
                // swap elements
                temp = x[i];
                x[i] = x[j];
                x[j] = temp;
            }
        }
    }

    if(n%2==0) {
        // if there is an even number of elements, return mean of the two elements in the middle
        return((x[n/2] + x[n/2 - 1]) >>1);
    } else {
        // else return the element in the middle
        return x[n/2];
    }
}




u32 pk2pk(volatile u16* input){
	u32 temp = 0;
	for(u8 i=2;i<sense_buffer-3;i++){
		temp = input[i]+input[i+1]+input[i+2]+input[i+3];
		temp = temp>>2;
		input[i] = temp;
	}

	u8 peak_num[2]={0,0};
	u32 peak[2]={0,0};
	
	for(u8 i=3;i<sense_buffer-4;i++){ 
		if(input[i-1]>input[i]&&input[i+1]>input[i]){ //min
			peak[0]+=input[i];
			peak_num[0]++;
		}
		if(input[i-1]<input[i]&&input[i+1]<input[i]){ //max
			peak[1]+=input[i];
			peak_num[1]++;
		}
	}
	
	peak[0]/=peak_num[0];
	peak[1]/=peak_num[1];
	
	return peak[1]-peak[0];
}



void on9_adc(){
	ADC_CommonInitTypeDef ADC_CommonInitStruct; 
	NVIC_InitTypeDef NVIC_InitStructure;


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); 
	
	adc_gpio_init();
		
	
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_1;
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStruct);
	
	//ADC init
	adc_dma_init();
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_Init(&NVIC_InitStructure);

}

void reset_dma_adc(){
		DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TEIF0 | DMA_IT_DMEIF0 | DMA_IT_FEIF0 | DMA_IT_TCIF0 | DMA_IT_HTIF0);
		adc_dma_init();
}



int main(void) {
	SystemInit();
	SystemCoreClockUpdate();

	
	//ticks_init();
		dac_init(100,3);
		//timer_init();
		uart_init(COM1, 115200);
		//adc_init();
		on9_adc();
		btn_init();
	_delay_ms(10);
	while(1)
	{

		if(flag){
		for(u16 j=2;j<sense_buffer;j++){
		 uart_tx(COM1,"%d,",on9ADC[j]);
			_delay_ms(10);
		}
		uart_tx(COM1,"\n");
		uart_tx(COM1,"%d;",pk2pk(on9ADC));
		flag = 0;
		}
	if(btn_pressed(BUTTON_1)){
			reset_dma_adc();
	}	
	}	
	
	/*u16 i=100;
	while(1){
			for(u8 j=0;j<7;j++){
				dac_init(i,j);
				_delay_ms(2000);
			}
			i+=10;
	}*/
	

}
