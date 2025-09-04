#include "interrupts_handlers.h"

#define necked __attribute__((naked)) void

static inline void iret() { __asm__ volatile ( "iret" ); }


necked IRQ0() { iret(); }

necked IRQ1() { iret(); }

necked IRQ2() { iret(); }

necked IRQ3() { iret(); }

necked IRQ4() { iret(); }

necked IRQ5() { iret(); }

necked IRQ6() { iret(); }

necked IRQ7() { iret(); }

necked IRQ8() { iret(); }

necked IRQ9() { iret(); }

necked IRQ10() { iret(); }

necked IRQ11() { iret(); }

necked IRQ12() { iret(); }

necked IRQ13() { iret(); }

necked IRQ14() { iret(); }

necked IRQ15() { iret(); }

necked IRQ16() { iret(); }

necked IRQ17() { iret(); }

necked IRQ18() { iret(); }

necked IRQ19() { iret(); }

necked IRQ20() { iret(); }

necked IRQ21() { iret(); }

necked IRQ22() { iret(); }

necked IRQ23() { iret(); }

necked IRQ24() { iret(); }

necked IRQ25() { iret(); }

necked IRQ26() { iret(); }

necked IRQ27() { iret(); }

necked IRQ28() { iret(); }

necked IRQ29() { iret(); }

necked IRQ30() { iret(); }

necked IRQ31() { iret(); }

necked IRQ32() { iret(); }

necked IRQ33() { iret(); }

necked IRQ34() { iret(); }

necked IRQ35() { iret(); }

necked IRQ36() { iret(); }

necked IRQ37() { iret(); }

necked IRQ38() { iret(); }

necked IRQ39() { iret(); }

necked IRQ40() { iret(); }

necked IRQ41() { iret(); }

necked IRQ42() { iret(); }

necked IRQ43() { iret(); }

necked IRQ44() { iret(); }

necked IRQ45() { iret(); }

necked IRQ46() { iret(); }

necked IRQ47() { iret(); }



