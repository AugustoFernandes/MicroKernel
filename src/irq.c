#include "irq.h"
#include "keyboard.h"
#include "vga.h"

static handler_t pirqHandlers[MAX_IRQ_HANDLERS] = {0};

handler_t	irqRegisterHandler(uint32_t dwIRQ, handler_t pHandler)
{
  handler_t	prevHandler = (handler_t) -1;

  if (dwIRQ < MAX_IRQ_HANDLERS)
    {
      prevHandler = pirqHandlers[dwIRQ];
      pirqHandlers[dwIRQ] = pHandler;
    }
  return prevHandler;
}

handler_t	irqUnregisterHandler(uint32_t dwIRQ)
{
  handler_t	prevHandler = (handler_t) -1;

  if (dwIRQ < MAX_IRQ_HANDLERS)
    {
      prevHandler = pirqHandlers[dwIRQ];
      pirqHandlers[dwIRQ] = (handler_t) 0;
    }
  return prevHandler;
}

void		STDCALL InterruptRequestCHandler(t_context *pctx)
{
  if (pctx->uiNumber >= 32
      && pctx->uiNumber <= 47
      && pirqHandlers[pctx->uiNumber - 32])
    pirqHandlers[pctx->uiNumber - 32](pctx);

  SendEndOfInterrupt(pctx->uiNumber - 32);
}

static uint32_t    	irqClockHandler(t_context *pctx)
{
  UNUSED(pctx);
  //printf("clock\n");
  return 0;
}

static uint32_t    	irqKeyboardHandler(t_context *pctx)
{
  uint32_t vk;
  uint8_t c = 0;

  UNUSED(pctx);
  //printf("%p\n", GetScanCode());
  //return;

  vk = GetVirtualKey();
  if (vk && !(vk & VK_KEYUP))
    {
      c = MapVirtualKey(vk, MAPVK_VK_TO_VSC, KEYBOARD_LAYOUT_FR);
      if (c)
	terminal_putchar(get_terminal_instance(), c);
    }

  return 0;
}

void		irqInitialize(void)
{
  //irqRegisterHandler(IRQ_CLOCK, &irqClockHandler);
  irqRegisterHandler(IRQ_KEYBOARD, &irqKeyboardHandler);
}
