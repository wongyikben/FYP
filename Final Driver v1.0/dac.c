#include "dac.h"




const u16 sin1[10][64]= {{392,411,429,448,465,483,499,514,528,540,552,561,569,576,580,583,584,583,580,576,569,561,552,540,528,514,499,483,465,448,429,411,392,373,355,336,319,301,285,270,256,244,232,223,215,208,204,201,200,201,204,208,215,223,232,244,256,270,285,301,319,336,355,373},{520,551,582,613,642,671,698,723,746,767,786,802,816,826,834,838,840,838,834,826,816,802,786,767,746,723,698,671,642,613,582,551,520,489,458,427,398,369,342,317,294,273,254,238,224,214,206,202,200,202,206,214,224,238,254,273,294,317,342,369,398,427,458,489},{648,692,735,778,819,859,897,932,965,994,1020,1043,1062,1077,1087,1094,1096,1094,1087,1077,1062,1043,1020,994,965,932,897,859,819,778,735,692,648,604,561,518,477,437,399,364,331,302,276,253,234,219,209,202,200,202,209,219,234,253,276,302,331,364,399,437,477,518,561,604},{776,832,888,943,996,1048,1096,1141,1183,1221,1255,1284,1308,1327,1341,1349,1352,1349,1341,1327,1308,1284,1255,1221,1183,1141,1096,1048,996,943,888,832,776,720,664,609,556,504,456,411,369,331,297,268,244,225,211,203,200,203,211,225,244,268,297,331,369,411,456,504,556,609,664,720},{904,973,1041,1108,1173,1236,1295,1351,1402,1448,1489,1525,1554,1578,1594,1605,1608,1605,1594,1578,1554,1525,1489,1448,1402,1351,1295,1236,1173,1108,1041,973,904,835,767,700,635,572,513,457,406,360,319,283,254,230,214,203,200,203,214,230,254,283,319,360,406,457,513,572,635,700,767,835},{1032,1114,1194,1274,1350,1424,1494,1560,1620,1675,1724,1766,1801,1828,1848,1860,1864,1860,1848,1828,1801,1766,1724,1675,1620,1560,1494,1424,1350,1274,1194,1114,1032,950,870,790,714,640,570,504,444,389,340,298,263,236,216,204,200,204,216,236,263,298,340,389,444,504,570,640,714,790,870,950},{1160,1254,1347,1439,1527,1613,1693,1769,1839,1902,1958,2007,2047,2079,2102,2115,2120,2115,2102,2079,2047,2007,1958,1902,1839,1769,1693,1613,1527,1439,1347,1254,1160,1066,973,881,793,707,627,551,481,418,362,313,273,241,218,205,200,205,218,241,273,313,362,418,481,551,627,707,793,881,973,1066},{1288,1395,1500,1604,1704,1801,1892,1978,2057,2129,2193,2248,2293,2329,2355,2371,2376,2371,2355,2329,2293,2248,2193,2129,2057,1978,1892,1801,1704,1604,1500,1395,1288,1181,1076,972,872,775,684,598,519,447,383,328,283,247,221,205,200,205,221,247,283,328,383,447,519,598,684,775,872,972,1076,1181},{1416,1535,1653,1769,1881,1989,2092,2187,2276,2356,2427,2488,2539,2580,2609,2626,2632,2626,2609,2580,2539,2488,2427,2356,2276,2187,2092,1989,1881,1769,1653,1535,1416,1297,1179,1063,951,843,740,645,556,476,405,344,293,252,223,206,200,206,223,252,293,344,405,476,556,645,740,843,951,1063,1179,1297},{1544,1676,1806,1934,2058,2178,2291,2397,2494,2583,2661,2729,2786,2830,2862,2882,2888,2882,2862,2830,2786,2729,2661,2583,2494,2397,2291,2178,2058,1934,1806,1676,1544,1412,1282,1154,1030,910,797,691,594,505,427,359,302,258,226,206,200,206,226,258,302,359,427,505,594,691,797,910,1030,1154,1282,1412},};
const u16 sin2[10][32]= {{392,429,465,499,528,552,569,580,584,580,569,552,528,499,465,429,392,355,319,285,256,232,215,204,200,204,215,232,256,285,319,355},{520,582,642,698,746,786,816,834,840,834,816,786,746,698,642,582,520,458,398,342,294,254,224,206,200,206,224,254,294,342,398,458},{648,735,819,897,965,1020,1062,1087,1096,1087,1062,1020,965,897,819,735,648,561,477,399,331,276,234,209,200,209,234,276,331,399,477,561},{776,888,996,1096,1183,1255,1308,1341,1352,1341,1308,1255,1183,1096,996,888,776,664,556,456,369,297,244,211,200,211,244,297,369,456,556,664},{904,1041,1173,1295,1402,1489,1554,1594,1608,1594,1554,1489,1402,1295,1173,1041,904,767,635,513,406,319,254,214,200,214,254,319,406,513,635,767},{1032,1194,1350,1494,1620,1724,1801,1848,1864,1848,1801,1724,1620,1494,1350,1194,1032,870,714,570,444,340,263,216,200,216,263,340,444,570,714,870},{1160,1347,1527,1693,1839,1958,2047,2102,2120,2102,2047,1958,1839,1693,1527,1347,1160,973,793,627,481,362,273,218,200,218,273,362,481,627,793,973},{1288,1500,1704,1892,2057,2193,2293,2355,2376,2355,2293,2193,2057,1892,1704,1500,1288,1076,872,684,519,383,283,221,200,221,283,383,519,684,872,1076},{1416,1653,1881,2092,2276,2427,2539,2609,2632,2609,2539,2427,2276,2092,1881,1653,1416,1179,951,740,556,405,293,223,200,223,293,405,556,740,951,1179},{1544,1806,2058,2291,2494,2661,2786,2862,2888,2862,2786,2661,2494,2291,2058,1806,1544,1282,1030,797,594,427,302,226,200,226,302,427,594,797,1030,1282},};
const u16 sin3[10][16]= {{482,555,618,659,674,659,618,555,482,409,346,305,290,305,346,409},{610,732,836,906,930,906,836,732,610,488,384,314,290,314,384,488},{738,909,1055,1152,1186,1152,1055,909,738,567,421,324,290,324,421,567},{866,1086,1273,1398,1442,1398,1273,1086,866,646,459,334,290,334,459,646},{994,1263,1492,1644,1698,1644,1492,1263,994,725,496,344,290,344,496,725},{1122,1440,1710,1891,1954,1891,1710,1440,1122,804,534,353,290,353,534,804},{1250,1617,1929,2137,2210,2137,1929,1617,1250,883,571,363,290,363,571,883},{1378,1794,2147,2383,2466,2383,2147,1794,1378,962,609,373,290,373,609,962},{1506,1971,2366,2629,2722,2629,2366,1971,1506,1041,646,383,290,383,646,1041},{1634,2148,2584,2876,2978,2876,2584,2148,1634,1120,684,392,290,392,684,1120},};

const u16 DC[17] = {0,256,512, 768,1024,1280,1536,1792,2048,2304,2560,2816,3072,3328,3584,3840,4095};	


   
	
const u16* const sine[30] = {sin1[0],sin1[1],sin1[2],sin1[3],sin1[4],sin1[5],sin1[6],sin1[7],sin1[8],sin1[9],
sin2[0],sin2[1],sin2[2],sin2[3],sin2[4],sin2[5],sin2[6],sin2[7],sin2[8],sin2[9],
sin3[0],sin3[1],sin3[2],sin3[3],sin3[4],sin3[5],sin3[6],sin3[7],sin3[8],sin3[9],

};

u16 volt[] = {384,640,896,1152,1408,1664,1920,2176,2432,2688};

u8 volt_index = 0;

u16 frequency = 0;

GPIO_InitTypeDef GPIO_InitStructure;



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
	
	DMA_DeInit(DMA1_Stream5);
	DAC_Cmd(DAC_Channel_1, DISABLE);
	frequency = freq;
	
	GPIO_InitTypeDef GPIO_InitStructure;
  DAC_InitTypeDef  DAC_InitStructure;


  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);      
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
         
	
		/* Enable DAC clock */
	RCC->APB1ENR |= RCC_APB1ENR_DACEN;
	/* Enable DMA1 clock */
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
	
	
		//  calculation for period and voltage for timer and 
	u16 period = 0;
	u8 i = 0;
	DMA_InitTypeDef DMA_InitStruct;
{
		if(level>9){level=9;}
		volt_index = level;
		if(freq<50){freq=50;}
		if(freq>400){freq=400;}
		period = 1280/freq;
		while(period<10){
			i++;
			period = 1280*(0x01<<i)/freq;
		}
		DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)(sine[i*7+level]);
		DMA_InitStruct.DMA_BufferSize = 64/(0x01<<i);
	}
	


	//
	DAC_TIM4_Config(period);
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_T4_TRGO;    
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;        
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;       
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);        

	DAC_SetChannel1Data(DAC_Align_12b_R, 0x0000);
	DAC_Cmd(DAC_Channel_1, ENABLE);
	//DAC_DMACmd(DAC_Channel_1, ENABLE);

	
	
	
	

	
	DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
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

void DAC_enable_init(void){
	// PA5,6,7

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);      
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	DAC_enable(ALL_DISABLE);
}





void DAC_enable(u8 channel){
	
	GPIOC->ODR = (GPIOC->ODR&(~(DAC_A|DAC_B|DAC_C)))|channel;
}


bool DAC_on(void){
	if(GPIOC->ODR&0x3CF8){
		return true;
	}
	return false;
}





void FET_GPIO_init(void){
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);      
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);      
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9|GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}


void FET_gnd(u16 input){
	FET_GPIO_init();
	
	switch(input){
		case (FET_A):{
			GPIOA->ODR = (GPIOA->ODR&0xF9FF)|0x0400;
			GPIOC->ODR = (GPIOC->ODR&0xFDFF);
		  break;
		}
		case (FET_B):{
			GPIOA->ODR = (GPIOA->ODR&0xF9FF)|0x0200;
			GPIOC->ODR = (GPIOC->ODR&0xFDFF);
			break;
		}
		case (FET_C):{
			GPIOA->ODR = (GPIOA->ODR&0xF9FF);
			GPIOC->ODR = (GPIOC->ODR&0xFDFF)|0x0200;
			break;			
		}
		default:{
			GPIOA->ODR = GPIOA->ODR&0xF9FF;
			GPIOC->ODR = GPIOC->ODR&0xFDFF;
			break;
		}
	}
	

	
}

u16 get_freq(void){
	return frequency-2;
}

u32 get_voltage(void){
	return (u32)volt[volt_index];
}

u16 get_interval(void){
	return 818/frequency;
}

void diode_gnd(u8 input){

	if(input==0){
		GPIOC->ODR |= 0x0800;
		return;
	}
	GPIOC->ODR &= ~(0x0800);
	
}

void diode_init(void){

	GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);      
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;// fuck you;
	//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void HFI_init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);      
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;// fuck you;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void BEMF_init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);      
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;// fuck you;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void HFI_read(u8 input){
	if(input==1){
		GPIOC->ODR |= 0x2000;
		return;
	}
	GPIOC->ODR &= ~(0x2000);
}

void BEMF_read(u8 input){
	if(input==1){
		GPIOC->ODR |= 0x0400;
		return;
	}
	GPIOC->ODR &= ~(0x0400);
}

void Scale_init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);      
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;// fuck you;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIOC->ODR &= ~(0x1000);
}