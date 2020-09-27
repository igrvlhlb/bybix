#include<unistd.h>
#include<stdio.h>
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

inline void bbx_chtop(bbx_flags fopt);
inline void bbx_chtof(bbx_flags fopt);
inline void bbx_opt_printf();
inline void enqueue_signed(); 	/* verifica se ha numeros negativos em vargs
				 *  e os enfilera (getopt achara que sao opcoes) */
				
extern struct bbx_queue bbxq;

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

int parse_opt(int argc, char *argv[]){
	signed char opt;
	iflags = 0;
	xflags = 0;

	bbx_queue_init(&bbxq);

	/* variaveis para fins de depuração */
	int iter = 0;

	/* so podem ser 'setadas' 2 flags e escolher 1 tipo: -l -<x|b> -<c|h|i|f|p> */
	while ( ( opt = getopt(argc, argv, bbx_types) ) != -1 ){
		switch(opt){
			case 'x': //hex
				bbx_chtof(bbx_hex);
				break;
			case 'b': //bin
				bbx_chtof(bbx_bin);
				break;
			case 'l': //long
				bbx_set_flag(xflags, bbx_long);
				break;
			case 'c': //char
				bbx_chtop(bbx_char);
				break;
			case 'h': //short
				bbx_chtop(bbx_short);
				break;
			case 'i': //int
				bbx_chtop(bbx_int);
				break;
			case 'f': //float
				bbx_chtop(bbx_float);
				break;
			case 'w': //word
				bbx_chtop(bbx_word);
				break;
			case 'V': //verbose (debug)
				bbx_set_flag(xflags, bbx_debug);
			default: //neste caso, possivelmente e um numero negativo
				if( 0 <= opt && opt <= 9 ){
					
				}
				break;
		}
		iter++;
	}
	if(bbx_flag_isset(xflags, bbx_debug)) {
		printf("%d opcoes foram lidas na entrada\nFlags para impressao:\t", iter);
		bbx_opt_printf();
	}
	return optind;
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
	/*
	for(int i = NO_FLAGS-1; i >= 0; i--){
		if(itags[i].bit & flag) printf("%s", itags[i].tag);
	}
	putchar('\n'); //ok. ja imprimiu o necessario
	*/
}

inline void enqueue_signed(int argc, char *argv[]){
	char *str;
	int optc = 0;
	for(int i = 1; i < argc; i++){
		str=argv[i]; //i-esimo argv
		if(str[0] == '-'){
			if( 0 <= str[1] && str[1] <= 9 ){
				//TODO: dizer a posicao do numero, tirando o offset das opcoes
			} else{ //e uma opcao
				optc += 1;
			}
		}
	}
}
