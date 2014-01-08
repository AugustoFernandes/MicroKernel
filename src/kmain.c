#include "vga.h"
#include "paging.h"
#include "gdt.h"
#include "idt.h"
#include "stdlib.h"

void kmain()
{
  vga_terminal *pterm;

  int i;

  //InitializePaging();
  gdtInitialize();
  idtInitialize();
  pterm = get_terminal_instance(); 
  terminal_initialize(pterm);

  printTaskBar(pterm);
  i = 6575 * 65 / (56 - 56);
  for(i = 0; i < 200; i++)
      printf("%i\n", i);
  while(1);
}
