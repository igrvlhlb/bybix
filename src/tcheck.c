#include "flags.h"
#include <ctype.h>
#include <string.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>


void print_exit(char *str, char *num)
{
	fprintf(stderr, "%s \'%s\'\n", str, num);
}

/* verifica se a string passada como parametro e um numero (base 2, 10 ou 16)
 * caso nao seja, manda uma mensagem para stderr avisando sobre o problema
 * encontrado */
int check_num(char *num){
	int idx = 0;
	char dec_sep = (char) *(localeconv()->decimal_point);
	char *restore_ptr = num;
	bbx_flags loc_flags = 0;
	if(num[0] == '-')
		num++;
	for(; idx < strlen(num); idx++){
		if(!isxdigit(num[idx])){
			if(num[idx] == dec_sep){
				if(!bbx_flag_isset(loc_flags, bbx_isfp)){
					bbx_set_flag(loc_flags, bbx_isfp);
				} else{
					print_exit("Encontrado mais que um ponto/virgula em", restore_ptr);
					return 0;
				}
			} else if(num[idx] == '-'){
				if(num[idx - 1] == 'e' || num[idx - 1] == 'E'){
					bbx_set_flag(loc_flags, bbx_isnc);
				} else{
					print_exit("Encontrado mais que um \'-\' em", restore_ptr);
					return 0;
				}
			} else if(num[idx] == 'x' || num[idx] == 'X'){
				if(idx == 1 && num[0] == '0'){
					continue;
				} else{
					print_exit("Caractere nao-numerico encontrado em", restore_ptr);
					return 0;
				}
			}
		}
	}
}
