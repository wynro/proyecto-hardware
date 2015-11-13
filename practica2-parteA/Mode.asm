.global getMode
.arm
getMode:
// Es innecesario, pero la costumbre
mov ip, sp
STMDB   sp!, {r4-r10,fp,ip,lr,pc} // Guardamos los parametros a tocar
sub fp, ip, #4
MRS r0, CPSR
and r0, #0xF // Tomamos los ultimos 4 bits
LDMDB   fp, {r4-r10,fp,sp,pc} // Recargamos los registros almacenados al inicio


.global doSWI
doSWI:
mov ip, sp
STMDB   sp!, {r4-r10,fp,ip,lr,pc} // Guardamos los parametros a tocar
sub fp, ip, #4

swi #2

LDMDB   fp, {r4-r10,fp,sp,pc} // Recargamos los registros almacenados al inicio
