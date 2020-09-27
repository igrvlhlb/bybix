#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include "optparse.h"
#include "flags.h"
#include "bbx-queue.h"

/*	importante:
 *		tipos = 	{c,h,i,f,p}
 *		modificadores = {x,b,l}
 *		
 *		seja algum x em tipos. se x = 1,
 *		para qualquer y em tipos tal que (y != x), y = 0
 *		
 *		flags{x,b} são disjuntos
 */

bbx_flags iflags;
bbx_flags xflags;

void bbx_chtop(bbx_flags fopt);
void bbx_chtof(bbx_flags fopt);
void bbx_opt_printf();
void enqueue_nums(int argc, char *argv[]); 	/* verifica se ha numeros negativos em vargs
						 *  e os enfilera (getopt achara que sao opcoes) */
				
struct bbx_queue bbxq;

static  const 
struct bbx_flag_s itags[8] =
{
	{bbx_char, "Char"},
	{bbx_short, "ShortInt"},
	{bbx_int, "Int"},
	{bbx_float, "Float"},
	{bbx_word, "Word"}
};
static const
struct bbx_flag_s mtags[3] =
{
	{bbx_hex, "Hex"},
	{bbx_bin, "Bin"},
	{bbx_long, "Long"},
};

const char bbx_types[] = "chifwxblV";

struct bbx_queue *parse_opt(int argc, char *argv[]){
	signed char opt;
	iflags = 0;
	xflags = 0;
	opterr = 0; //suprime mensagens de erro do getopt()

	bbx_queue_init(&bbxq);
	enqueue_nums(argc, argv);

	/* variaveis para fins de depuração */
	int iter = 0;

	/* so podem ser 'setadas' 2 flags e escolher 1 tipo: -l -<x|b> -<c|h|i|f|p> */
	while ( ( opt = getopt(argc, argv, bbx_types) ) != -1 ){
		switch(opt){
			case 'x': //hex
				bbx_chtof(bbx_hex);
				iter++;
				break;
			case 'b': //bin
				bbx_chtof(bbx_bin);
				iter++;
				break;
			case 'l': //long
				bbx_set_flag(xflags, bbx_long);
				iter++;
				break;
			case 'c': //char
				bbx_chtop(bbx_char);
				iter++;
				break;
			case 'h': //short
				bbx_chtop(bbx_short);
				iter++;
				break;
			case 'i': //int
				bbx_chtop(bbx_int);
				iter++;
				break;
			case 'f': //float
				bbx_chtop(bbx_float);
				iter++;
				break;
			case 'w': //word
				bbx_chtop(bbx_word);
				iter++;
				break;
			case 'V': //verbose (debug)
				bbx_set_flag(xflags, bbx_debug);
				iter++;
			default: //neste caso, possivelmente e um numero negativo
				if(optind < argc - 1){
					optind+=1; /* evita que as opcoes sejam lidas caso
						    *  o numero negativo seja hexadecimal */
				} else goto FIM_OPT;
				break;
		}
	}
FIM_OPT:
	if(bbx_flag_isset(xflags, bbx_debug)) {
		printf("%d opcoes foram lidas na entrada\nFlags para impressao:\t", iter);
		bbx_opt_printf();
	}
	return &bbxq;
}

/* change-to-option */
inline void bbx_chtop(bbx_flags fopt){
	if(bbx_flag_isset(iflags,  bbx_any)){
		bbx_unset_flag(iflags, bbx_any);
	}
	bbx_set_flag(iflags, fopt);
}
/* change-to-extra_flag */
inline void bbx_chtof(bbx_flags fopt){
	if(xflags & bbx_anyf){
		bbx_unset_flag(xflags, bbx_anyf);
	}
	bbx_set_flag(xflags, fopt);
}

inline void bbx_opt_printf(){
	int impressos = 0;
	putchar('[');
	for(int i = 0; i < NO_MFLAGS; i++){
		if(bbx_flag_isset(xflags, mtags[i].bit)){
			if( impressos != 0 ) printf(", ");
			printf("%s", mtags[i].tag);
			impressos++;
		}
	}
	for(int i = 0; i < NO_FLAGS; i++){
		if(bbx_flag_isset(iflags, itags[i].bit)){
			if( impressos != 0 ) printf(", ");
			printf("%s", itags[i].tag);
			impressos++;
		}
	}
	printf("]\n");
}

inline void enqueue_nums(int argc, char *argv[]){
	char *str;
	int optc = 0;
	for(int i = 1; i < argc; i++){
		str=argv[i]; //i-esimo argv
		if( '0' <= str[0] && str[0] <= '9' ){
			if( bbx_enqueue(&bbxq, str) != Q_ERR ) continue;
			else{
				bbx_queue_stats(&bbxq);
				exit(1);
			}
		} else if(str[0] == '-'){
			if( '0' <= str[1] && str[1] <= '9' ){
				if( bbx_enqueue(&bbxq, str) != Q_ERR ) continue;
				else{
					bbx_queue_stats(&bbxq);
					exit(1);
				}
			} else optc += 1; //e uma opcao 
		}
	}
}
