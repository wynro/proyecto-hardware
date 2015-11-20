/*
 * Modulo        : Boton_antirrebotes
 * Fichero       : Boton_antirrebotes.h
 * Descripcion   : Modulo que gestiona los botones de la placa conectados
 *                 al puerto G
 * Mantenido por : guillerobles1995@gmail.com
 * Estabilidad   : experimental
 *
 * Gestiona los botones de la placa con la funcionalidad de que el derecho hace
 * que aumente el numero en el 8led, y el derecho levanta un flag accesible
 * desde fuera.
 */

// Descripcion demasiado simple??

#ifndef BUTTON_H_
#define BUTTON_H_

/**
 * Valor actual de la cuenta interna
 */
int Button_valor_actual(void);

/**
 * Configura el valor actual de la cuenta interna, actualizando la pantalla asociada
 */
void Button_set_valor_actual(int);

/**
 * Informa de si se ha de avanzar
 */
int Button_next(void);

/**
 * Baja la peticion de avanzado
 */
void Button_low_next(void);

/**
 * Iniciar el sistema de botones
 */
void Button_init(int min, int max);

/**
 * Accion asociada a la pulsacion del boton de codigo n
 */
void action(int n);

#endif /* BUTTON_H_ */
