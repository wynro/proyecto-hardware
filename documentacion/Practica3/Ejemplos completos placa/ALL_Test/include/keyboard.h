#ifndef __keyboard_H__
#define __keyboard_H__

#define KEY_VALUE_MASK	0x0f

/* keyboard control address */
volatile UCHAR *keyboard_base = (UCHAR *)0x06000000;

void Test_Keyboard(void);

#endif /* __keyboard_H__ */
