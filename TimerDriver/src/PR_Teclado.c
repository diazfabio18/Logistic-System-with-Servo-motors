#include "PR_Teclado.h"

uint8_t GetKey(void)
{
	uint8_t keye = Tecla;
	Tecla = NO_KEY;
	return keye;
}




