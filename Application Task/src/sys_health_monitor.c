/*
 * sys_health_monitor_task.c
 *
 * Implements the System Health Monitor Task, responsible for:
 * 1. Monitoring the MCU's internal temperature and notifying the Error Handler if thresholds are exceeded.
 * 2. Regularly checking the status of critical tasks and resetting the watchdog timer if they are operational.
 */

#include "sys_health_monitor.h"
#include "error_handler_task.h"
#include "gpio.h"
#include "adc.h"
#include "tim.h"
#include "iwdg.h"

// Global health flags
volatile bool g_sensors_task_ok = false;
// ... Add more flags for other critical tasks if needed

// MCU temperature health flag
static volatile bool mcu_temp_ok = true;

// Optional: flag & variable used for demonstrating obtaining the updated MCU temperature
static volatile uint8_t ADC1_CONVERSION_COMPLETE  = 0;
static volatile uint16_t adc_temp_val             = 0;

/*
 * this function used to convert the ADC value to the celsius
 * currelty used to set the analog watchdog thershold
 * @param temp_celsius temperature in celsius o be converted to ADC value
 * @return the equivalent ADC value for the given temperature

*/

static uint16_t temperature_to_adc(float temp_celsius){

  //constants that are required to convert temperature to the ADC as per STM32 datasheet pg 144
  const float TEMP_SENSOR_AVG_SLOPE = 0.0025;
  const float TEMP_SENSOR_V25       = 0.76;
  const float VREF                  = 3.3;
  // CALCULATE THE sensor voltage for the given temperature
  // formula for reffrence manual pg:382: voltage = ( temperature difference from 25 C * slope) + voltage at 25
  float voltage = ((temp_celsius - 25) * TEMP_SENSOR_AVG_SLOPE) + TEMP_SENSOR_V25;

  //CONVERT THE VOLTAGE TO THE ADC value
  uint16_t adc_value = (voltage / VREF)* 4095.0;

  return adc_value;

}

/*
 * this funtion do the exact opp of temp_tot_adc
 * it convert the adc_to_temp
 * */

static float adc_to_temperature(uint16_t adc_value){

  //constants that are required to convert temperature to the ADC as per STM32 datasheet pg 144
  const float TEMP_SENSOR_AVG_SLOPE = 0.0025;
  const float TEMP_SENSOR_V25       = 0.76;
  const float VREF                  = 3.3;


 // float voltage  = (VREF/adc_value)* 4095.0;

  //float temp_celsius = (voltage - TEMP_SENSOR_V25) / TEMP_SENSOR_AVG_SLOPE * 25.0;

  float voltage = ((float)adc_value / 4095.0f) * VREF;

  float temp_celsius =
      ((voltage - TEMP_SENSOR_V25) /
       TEMP_SENSOR_AVG_SLOPE) + 25.0f;

  return temp_celsius;


}


/**
 * System Health Monitor Task which monitors the MCU temperature and,
 * if critical tasks are operational, periodically kicks the Independent Watchdog.
 * @param param Parameters passed to the task (unused).
 */
static void sys_health_monitor_task(void *param)
{
  float internal_temp;

  // Convert the temperature high/low thresholds to ADC values using temperature_to_adc()

  uint16_t awd_htr = temperature_to_adc(ADC1_AWD_HT_TEMP_CELSIUS);
  uint16_t awd_ltr = temperature_to_adc(ADC1_AWD_LT_TEMP_CELSIUS);


  // Initialize the analog watchdog based on the thresholds using adc_awd_init()

  adc_awd_init(ADC1, ADC_CH18, awd_htr, awd_ltr);


  // Initialize the IWDG here if not in Debug Mode
 //if ((CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) == 0)
  //{
    // Debugger not connected, safe to enable IWDG
   iwdg_init();
  //}

  // Variable used to check the system health
  bool system_healthy;

  // Specify the frequency of task execution and,
  // Initialize the xLasatWakeTime variable with the current time
  const TickType_t xFrequency = pdMS_TO_TICKS(1000);
  TickType_t xLastWakeTime = xTaskGetTickCount();

  while (1)
  {
    // Wait for the next cycle
    xTaskDelayUntil(&xLastWakeTime, xFrequency);


    // Optional: Get the temperature for demonstration purposes
    if (ADC1_CONVERSION_COMPLETE)
    {

      internal_temp = adc_to_temperature(adc_temp_val);
      ADC1_CONVERSION_COMPLETE = 0;
    }

    // Check if all critical tasks are running correctly

    system_healthy = g_sensors_task_ok && mcu_temp_ok;


    if (system_healthy)
    {
     iwdg_reset();

      g_sensors_task_ok = false;
      // ... reset any other task flags as well
    }
  //  else
    //{
      // Handle the system health issue (e.g., log error to FRAM, take corrective action)
   // }

    // Get rid of unused variable warning
    (void)internal_temp;
  }
}

void sys_health_monitor_task_start(void)
{
  // Setup for getting MCU temperature (TIM2 and ADC)
   tim2_init();
   adc_init();

  configASSERT(xTaskCreate(sys_health_monitor_task,
                           "System Health Monitor Task",
                           SYS_HEALTH_MONITOR_TASK_STACK_SIZE,
                           NULL,
                           SYS_HEALTH_MONITOR_TASK_PRIORITY,
                           NULL) == pdPASS);
}

// Define ADC_IRQHandler()

void ADC_IRQHandler(void){

  //check the analog watchdog bit

  if (ADC1->SR & ADC_SR_AWD){

      //CLEAR THE ANALOG WATCHDOG BIT
      ADC1->SR &= ~ADC_SR_AWD;

      //ALERT THE ERROR HANDLER DUE TO AWD EVENT
      error_handler_send_msg_from_isr(EVT_SYS_HEALTH_AWDG_THRESHOLD_EXCEEDED);

      //SET THE TEMPERATURE HEALTH FLAG TO FALSE
      mcu_temp_ok = false;


  }
  // CHECK THE END OF THE CONVERTION FLAG
  if (ADC1->SR & ADC_SR_EOC){


      //toggel the led (can be used for debugging and signal processing)

  //   gpio_toggle_pin(USER_LED_PORT, USER_LED_PIN);

     //read the ADC value from the temperature sensor
     adc_temp_val = ADC1->DR;

     //set the convertion flag to 1
     ADC1_CONVERSION_COMPLETE = 1;

      //CLEAR THE eoc flag
     ADC1->SR &= ~ADC_SR_EOC;
  }

  // CHECK THE OVERRUN FLAG
  if(ADC1->SR & ADC_SR_OVR){

      //CLEAR THE OVERRUN FLAG
      ADC1->SR &= ~ADC_SR_OVR;
  }


}













































