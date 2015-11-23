/*
 * Modulo         : Modo
 * Fichero        : Modo.asm
 * Referirse a Mode.h para mas informacion
 */

.arm
.global getMode
getMode:
// Prologo
mov ip, sp
STMDB   sp!, {r4-r10,fp,ip,lr,pc}
sub fp, ip, #4

MRS r0, CPSR
and r0, #0xF // Tomamos los ultimos 4 bits

// Epilogo
LDMDB   fp, {r4-r10,fp,sp,pc}


.global doSWI2
doSWI2:
// Prologo
mov ip, sp
STMDB   sp!, {r4-r10,fp,ip,lr,pc} // Guardamos los parametros a tocar
sub fp, ip, #4

swi #2

// Epilogo
LDMDB   fp, {r4-r10,fp,sp,pc} // Recargamos los registros almacenados al inicio
