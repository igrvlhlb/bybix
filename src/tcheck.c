#include "flags.h"
#include <ctype.h>
#include <string.h>
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
	int idx;
	char *restore_ptr = num;
	bbx_flags loc_flags = 0;
	if(num[0] == '-')
		num++;
	for(idx = 0; idx < strlen(num); idx++){
		char atu = num[idx];
		if(!isxdigit(atu)){
			if(atu == '.'){
				if(!bbx_flag_isset(loc_flags, bbx_isfp)){
					bbx_set_flag(loc_flags, bbx_isfp);
				} else{
					print_exit("Encontrado mais que um ponto/virgula em", restore_ptr);
					return 0;
				}
			} else if(atu == '-'){
				char ant = num[idx - 1];
				if(ant == 'e' || ant == 'E'){
					bbx_set_flag(loc_flags, bbx_isnc);
				} else{
					print_exit("Encontrado \'-\' em local inapropriado em", restore_ptr);
					return 0;
				}
			} else if(atu == 'x' || atu == 'X'){
				if(idx == 1 && num[0] == '0'){
					continue;
				} else{
					goto nao_numerico_exit;
				}
			} else{
nao_numerico_exit:
				print_exit("Caractere nao-numerico encontrado em", restore_ptr);
				return 0;
			}
		}
	}
	return 1;
}
