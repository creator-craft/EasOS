#include "interrupts_handlers.h"

#define naked __attribute__((naked)) void

static inline void iret() { __asm__ volatile ( "iret" ); }


naked IRQ0() { iret(); }

naked IRQ1() { iret(); }

naked IRQ2() { iret(); }

naked IRQ3() { iret(); }

naked IRQ4() { iret(); }

naked IRQ5() { iret(); }

naked IRQ6() { iret(); }

naked IRQ7() { iret(); }

naked IRQ8() { iret(); }

naked IRQ9() { iret(); }

naked IRQ10() { iret(); }

naked IRQ11() { iret(); }

naked IRQ12() { iret(); }

naked IRQ13() { iret(); }

naked IRQ14() { iret(); }

naked IRQ15() { iret(); }

naked IRQ16() { iret(); }

naked IRQ17() { iret(); }

naked IRQ18() { iret(); }

naked IRQ19() { iret(); }

naked IRQ20() { iret(); }

naked IRQ21() { iret(); }

naked IRQ22() { iret(); }

naked IRQ23() { iret(); }

naked IRQ24() { iret(); }

naked IRQ25() { iret(); }

naked IRQ26() { iret(); }

naked IRQ27() { iret(); }

naked IRQ28() { iret(); }

naked IRQ29() { iret(); }

naked IRQ30() { iret(); }

naked IRQ31() { iret(); }

naked IRQ32() { iret(); }

naked IRQ33() { iret(); }

naked IRQ34() { iret(); }

naked IRQ35() { iret(); }

naked IRQ36() { iret(); }

naked IRQ37() { iret(); }

naked IRQ38() { iret(); }

naked IRQ39() { iret(); }

naked IRQ40() { iret(); }

naked IRQ41() { iret(); }

naked IRQ42() { iret(); }

naked IRQ43() { iret(); }

naked IRQ44() { iret(); }

naked IRQ45() { iret(); }

naked IRQ46() { iret(); }

naked IRQ47() { iret(); }



