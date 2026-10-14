/**
 * @file    main.c
 * @brief   STM32F103 standard-library FreeRTOS port verification.
 * @details One application task toggles PC13 every 500 ms. The idle task
 *          runs while the LED task is blocked. No UART or motor is required.
 */
#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"

#define LED_HALF_PERIOD_MS   500UL
#define LED_STACK_WORDS      128U
#define LED_TASK_PRIORITY    2U

/* Debugger observables: 0=normal, 1=clock, 2=task creation, 3=scheduler
 * return, 4=allocation failure, 5=stack overflow, 6=assertion failure. */
volatile uint32_t g_fault_code = 0U;
const char * volatile g_assert_file = NULL;
volatile unsigned long g_assert_line = 0UL;

/* --------------------------------------------------------------------------
 * GPIO setup and fault handling
 * PC13 is assumed to drive an active-low board LED. On a fault, interrupts
 * are disabled and the CPU stops here; inspect g_fault_code in the debugger.
 * -------------------------------------------------------------------------- */
static void Led_Init(void)
{
    GPIO_InitTypeDef gpio;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_SetBits(GPIOC, GPIO_Pin_13);
    gpio.GPIO_Pin = GPIO_Pin_13;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOC, &gpio);
}

static void Error_Stop(uint32_t code)
{
    __disable_irq();
    g_fault_code = code;
    GPIO_SetBits(GPIOC, GPIO_Pin_13);
    for (;;)
    {
        /* Set a breakpoint here to identify the failure reason. */
    }
}

/* --------------------------------------------------------------------------
 * Application task
 * vTaskDelay blocks this task rather than busy-waiting. The delay is expressed
 * in milliseconds and converted to ticks so it tracks configTICK_RATE_HZ.
 * 128 StackType_t words occupy 512 bytes on this Cortex-M3 port, excluding TCB.
 * -------------------------------------------------------------------------- */
static void Led_Task(void *argument)
{
    (void)argument;

    for (;;)
    {
        GPIO_ResetBits(GPIOC, GPIO_Pin_13);
        vTaskDelay(pdMS_TO_TICKS(LED_HALF_PERIOD_MS));
        GPIO_SetBits(GPIOC, GPIO_Pin_13);
        vTaskDelay(pdMS_TO_TICKS(LED_HALF_PERIOD_MS));
    }
}

/* --------------------------------------------------------------------------
 * FreeRTOS diagnostic hooks
 * These hooks must not allocate memory or block. The stack-overflow callback
 * uses the signature declared in task.h. They stop execution for debugging;
 * production fault recovery is outside this minimal example's scope.
 * -------------------------------------------------------------------------- */
void vApplicationMallocFailedHook(void)
{
    Error_Stop(4U);
}

void vApplicationStackOverflowHook(TaskHandle_t task, char *task_name)
{
    (void)task;
    (void)task_name;
    Error_Stop(5U);
}

void vAssertCalled(const char *file, unsigned long line)
{
    g_assert_file = file;
    g_assert_line = line;
    Error_Stop(6U);
}

/* --------------------------------------------------------------------------
 * Startup
 * SystemInit is called by startup_stm32f10x_md.s before main. Confirm the
 * runtime HCLK matches the configured 72 MHz before starting the tick timer.
 * Priority group 4 assigns all implemented NVIC bits to preemption priority.
 * -------------------------------------------------------------------------- */
int main(void)
{
    BaseType_t result;

    SystemCoreClockUpdate();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    Led_Init();

    if (SystemCoreClock != configCPU_CLOCK_HZ)
    {
        Error_Stop(1U);
    }

    result = xTaskCreate(Led_Task, "led", LED_STACK_WORDS, NULL,
                         LED_TASK_PRIORITY, NULL);
    if (result != pdPASS)
    {
        Error_Stop(2U);
    }

    vTaskStartScheduler();

    /* The scheduler should not return in a successfully started system. */
    Error_Stop(3U);
    return 0;
}






