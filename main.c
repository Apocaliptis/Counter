/**
  ******************************************************************************
  * @file STM8_main.c
  * @brief RC Touch Sensing Library for STM8 CPU family.
  * Application example.
  * @author STMicroelectronics - MCD Application Team
  * @version V0.2.0
  * @date 19-DEC-2008
  ******************************************************************************
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2008 STMicroelectronics</center></h2>
  * @image html logo.bmp
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "stm8_tsl_rc_api.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void CLK_Configuration(void);
void GPIO_Configuration(void);
void ExtraCode_Init(void);
void ExtraCode_StateMachine(void);


/* Private functions ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
u8 numberOFclicks = 0;

/* Public functions ----------------------------------------------------------*/

/**
  ******************************************************************************
  * @brief Main function.
  * @par Parameters:
  * None
  * @retval void None
  * @par Required preconditions:
  * None
  ******************************************************************************
  */
void main(void)
{
  /* Configures clocks */
  CLK_Configuration();

  /* Configures GPIOs */
  GPIO_Configuration();

  /* Initialize Touch Sensing library */
  TSL_Init();
  
	/* Initialize all the Touch Sensing keys */
  ExtraCode_Init();

  /* Start the 100ms timebase Timer */
  TSL_Tick_Flags.b.User_Start_100ms = 1;

  for (;;)
  {

    /* User code */
    ExtraCode_StateMachine();
   
	  /* Main function of the Touch Sensing library */
    TSL_Action();

  }

}


/**
  ******************************************************************************
  * @brief Initialize all the TS keys
  * @par Parameters:
  * None
  * @retval void None
  * @par Required preconditions:
  * None
  ******************************************************************************
  */
void ExtraCode_Init(void)
{

  u8 i;

  /* All keys are implemented and enabled */

  for (i = 0; i < NUMBER_OF_SINGLE_CHANNEL_KEYS; i++)
  {
    sSCKeyInfo[i].Setting.b.IMPLEMENTED = 1;
    sSCKeyInfo[i].Setting.b.ENABLED = 1;
    sSCKeyInfo[i].DESGroup = 0x01; /* Put 0x00 to disable the DES on these pins */
  }

#if NUMBER_OF_MULTI_CHANNEL_KEYS > 0
  for (i = 0; i < NUMBER_OF_MULTI_CHANNEL_KEYS; i++)
  {
    sMCKeyInfo[i].Setting.b.IMPLEMENTED = 1;
    sMCKeyInfo[i].Setting.b.ENABLED = 1;
    sMCKeyInfo[i].DESGroup = 0x01; /* Put 0x00 to disable the DES on these pins */
  }
#endif

  enableInterrupts();
}


/**
  ******************************************************************************
  * @brief Adjustable led blinking speed using touch sensing keys
  * KEY1: LED1 is blinking
  * KEY1: LED1 is blinking faster
  * KEY1: LED1 don't blink anymore
  * @par Parameters:
  * None
  * @retval void None
  * @par Required preconditions:
  * None
  ******************************************************************************
  */
void ExtraCode_StateMachine(void)
{
  if ((TSL_GlobalSetting.b.CHANGED) && (TSLState == TSL_IDLE_STATE))
  {
    TSL_GlobalSetting.b.CHANGED = 0;

    if (sSCKeyInfo[0].Setting.b.DETECTED) /* KEY 1 touched */
    {
      numberOFclicks++;
      numberOFclicks = numberOFclicks % 0x11;
     
      GPIO_Write(GPIOA, numberOFclicks << 3);
     }
  }
}

/**
  ******************************************************************************
  * @brief Configures clocks
  * @par Parameters:
  * None
  * @retval void None
  * @par Required preconditions:
  * None
  ******************************************************************************
  */
void CLK_Configuration(void)
{

  /* Fmaster = 16MHz */
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);

}

/**
  ******************************************************************************
  * @brief Configures GPIOs
  * @par Parameters:
  * None
  * @retval void None
  * @par Required preconditions:
  * None
  ******************************************************************************
  */
void GPIO_Configuration(void)
{
  /* GPIOA reset */
  GPIO_DeInit(GPIOA);

  /* Configure PD0 (LED1) as output push-pull low (led switched on) */
  GPIO_Init(GPIOA, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(GPIOA, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(GPIOA, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(GPIOA, GPIO_PIN_6, GPIO_MODE_OUT_PP_LOW_FAST);

}