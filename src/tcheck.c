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
/*
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
*/

int check_is_bin(const char *num){
	char c;
	int i;
	bbx_flags flag = 0;
	if (num[0] != 'b') return 0;
	for (i = 1; (c = num[i]) != '\0'; i++) {
		if (c != '1' && c != '0') {
			if (c == '.' && !bbx_flag_isset(flag, bbx_isfp)) {
				bbx_set_flag(flag, bbx_isfp);
			} else {
				return 0;
			}
		}
	}
	bbx_set_flag(flag, bbx_isbin);
	return flag;
}
int check_is_hex(const char *num){
	char c;
	int i;
	bbx_flags flag = 0;
	if (num[0] != 'x') return 0;
	for (i = 1; (c = num[i]) != '\0'; i++) {
		if (!isxdigit(c)) {
			if (c == '.' && !bbx_flag_isset(flag, bbx_isfp)) {
				bbx_set_flag(flag, bbx_isfp);
			} else {
				return 0;
			}
		}
	}
	bbx_set_flag(flag, bbx_ishex);
	return flag;
}
int check_is_oct(const char *num){
	char c;
	int i;
	bbx_flags flag = 0;
	for (i = 0; (c = num[i]) != '\0'; i++) {
		if (!(('0' < c) && (c < '7'))) {
			if (c == '.' && !bbx_flag_isset(flag, bbx_isfp)) {
				bbx_set_flag(flag, bbx_isfp);
			} else {
				return 0;
			}
		}
	}
	bbx_set_flag(flag, bbx_isoct);
	return flag;
}
int check_is_dec(const char *num){
	char c;
	int i;
	bbx_flags flag = 0;
	for (i = 0; (c = num[i]) != '\0'; i++) {
		if (!isdigit(c)) {
			if (c == '.' && !bbx_flag_isset(flag, bbx_isfp)) {
				bbx_set_flag(flag, bbx_isfp);
			} else if ((c == 'e') || (c == '-' && num[i-1] == 'e')) {
				bbx_set_flag(flag, bbx_isnc);
				continue;
			} else {
				return 0;
			}
		}
	}
	bbx_set_flag(flag, bbx_isdec);
	return flag;
}


bbx_flags check_num(char *num){
	char *aux = num;
	bbx_flags loc_flags = 0;
	bbx_flags tmp_flags = 0;

	if(aux[0] == '-') {
		bbx_set_flag(loc_flags, bbx_issig);
		aux = num + 1; // avanca o sinal (aponta para digito apos sinal)
	}
	if(aux[0] == '0'){
		aux += 1; // avanca o 0 inicial
		if ((tmp_flags = check_is_bin(aux))) {
			bbx_set_flag(loc_flags, tmp_flags);
		} else if ((tmp_flags = check_is_hex(aux))) {
			bbx_set_flag(loc_flags, tmp_flags);
		} else if ((tmp_flags = check_is_oct(aux))) {
			bbx_set_flag(loc_flags, tmp_flags);
		}
	} else {
		if ((tmp_flags = check_is_dec(num))){
			bbx_set_flag(loc_flags, tmp_flags);
		}
	}
	if(!bbx_flag_isset(loc_flags, bbx_isany))
		loc_flags = 0;
	return loc_flags;
}
