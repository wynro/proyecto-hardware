.global getMode
.arm
getMode:
// Es innecesario, pero la costumbre
mov ip, sp
STMDB   sp!, {r4-r10,fp,ip,lr} // Guardamos los parametros a tocar
sub fp, ip, #4
MRS r0, CPSR
and r0, #0xF // Tomamos los ultimos 4 bits
LDMDB   fp, {r4-r10,fp,sp,pc} // Recargamos los registros almacenados al inicio


.global doSWI2
doSWI2:
mov ip, sp
STMDB   sp!, {r4-r10,fp,ip,lr} // Guardamos los parametros a tocar
sub fp, ip, #4

swi #2

LDMDB   fp, {r4-r10,fp,sp,pc} // Recargamos los registros almacenados al inicio
