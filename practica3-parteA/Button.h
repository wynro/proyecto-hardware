/** @file Button.h
 *  @brief Modulo que gestiona los rebotes de los botones
 * 
 * Modulo que se encarga de gestionar los botones y su uso en el
 * proyecto, inicializarlos y sincronizar el 7 segmentos con el valor
 * interno del boton.
 * 
 * @author Guillermo Robles Gonzalez
 */

#ifndef BUTTON_H_
#define BUTTON_H_

/**
 * Valor actual de la cuenta interna
 *
 * @return valor actual
 */
int Button_valor_actual(void);

/**
 * Informa de si se ha de avanzar
 *
 * @return 1 en caso de que el flag este activo, 0 en caso contrario
 */
int Button_next(void);

/**
 * Iniciar el sistema de botones, poniendo la pantalla al valor minimo pasado
 *
 * @param[in] min Valor minimo que aparecera en la pantalla
 * @param[in] max Valor maximo que aparecera en la pantalla
 */
void Button_init(int min, int max);

/**
 * Accion asociada a la pulsacion del boton de codigo n
 *
 * @param[in] n Codigo de boton pulsado
 */
void action(int n);

/**
 * Ajusta el valor de la cuenta interna del boton, actualizando el
 * display. Ha de pertenecer al rango al cual esta actualmente
 * configurado el boton.
 *
 * @param[in] n nuevo valor a poner
 */
void Button_set_valor_actual(int n);

/**
 * Baja el flag interno de next
 */
void Button_low_next(void);

/**
 * Indica si ha sido realizada alguna accion que afecte al estado interno
 * del boton, tanto la pulsacion de un boton como la repeticion de una accion
 *
 * @return 1 en caso de que el flag este activo, 0 en caso contrario
 */
int Button_update_screen();

/**
 * Baja el flag interno indicando que se han realizado acciones
 */
void Button_low_update_screen();

/**
 * Reconfigura el rango que maneja el boton, solo seran validos valores en el rango [0,15]
 *
 * @param[in] new_min Nuevo minimo
 * @param[in] new_max Nuevo maximo
 */
void Button_reconfigure_range(int new_min, int new_max);

#endif /* BUTTON_H_ */
