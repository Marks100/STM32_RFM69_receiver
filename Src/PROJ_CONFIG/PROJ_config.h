#ifndef PROJ_CONFIG_H
#define PROJ_CONFIG_H

#define STM32 							 1
#define RFM69HW
#define RFM69_DEFAULT_TX_POWER_LEVEL     0
#define RTC_EXT_DEFAULT_WAKEUP_TIME_SEC  10
#define MAX_NUM_RF_CONFIGS 				 4

#define AUTO_DEBUG_MODE					 1
#define MULTI_SPI_WRITE_CONFIG			 1
#define SENSOR_ID 						 NODE_OWN_ADDRESS


#if !defined ( RFM69HW ) && !defined ( RFM69W ) && !defined ( RFM69HW_SUPER )
	#error "Please define one of the following RFM69W || RFM69HW || RFM69HW_SUPER"
#endif


#endif /* PROJ_CONFIG_H multiple inclusion guard */
