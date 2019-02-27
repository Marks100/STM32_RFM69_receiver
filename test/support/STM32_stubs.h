
typedef struct
{
} GPIO_TypeDef;

typedef enum
{ 
  FLASH_BUSY = 1,
  FLASH_ERROR_PG,
  FLASH_ERROR_WRP,
  FLASH_COMPLETE,
  FLASH_TIMEOUT
}FLASH_Status;



typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;

void asm( );