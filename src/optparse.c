#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "optparse.h"
#include "flags.h"
#include "queue.h"
#include "tcheck.h"

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
/* verifica se ha numeros negativos em vargs
 * e os enfilera (getopt achara que sao opcoes) */
void enqueue_nums(struct bbx_queue *q, int argc, char *argv[]);
void backup_argv(struct bbx_queue *q, int argc, char *argv[]);
void print_ntype(char *num, bbx_flags flag);

static  const 
struct bbx_flag_s itags[NO_FLAGS] =
{
	{bbx_char, "Char"},
	{bbx_short, "ShortInt"},
	{bbx_int, "Int"},
	{bbx_float, "Float"},
	{bbx_word, "Word"},
	{bbx_str, "Str"}
};

static const
struct bbx_flag_s mtags[NO_MFLAGS] =
{
	{bbx_hex, "Hex"},
	{bbx_bin, "Bin"},
	{bbx_long, "Long"}
};

static const
struct bbx_flag_s ntypes[NO_NTYPES] =
{
	{bbx_issig, "Signed"},
	{bbx_isdec, "Dec"},
	{bbx_isbin, "Bin"},
	{bbx_isoct, "Oct"},
	{bbx_ishex, "Hex"},
	{bbx_isfp, "Float"}
};

const char bbx_types[] = "chifwxblsV";

struct bbx_queue *parse_opt(int argc, char *argv[])
{
	signed char opt;
	iflags = 0;
	xflags = 0;
	opterr = 0; //suprime mensagens de erro do getopt()
	/* ponteiro que sera retornado dependendo
	 * das opcoes selecionadas */
	struct bbx_queue *retptr; 	
	/* numeros em argv e uma copia de				
	 * argv antes da chamada de getopt()*/
	struct bbx_queue *nums, *argscpy; 	
						
	nums = bbx_queue_init(argc);
	argscpy = bbx_queue_init(argc);
	
	backup_argv(argscpy, argc, argv);
	enqueue_nums(nums, argc, argv);

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
			case 's': //string
				bbx_chtop(bbx_str);
				iter++;
				break;
			case 'V': //verbose (debug)
				bbx_set_flag(xflags, bbx_debug);
				iter++;
				break;
			default: //neste caso, possivelmente e um numero negativo
				if(optind < argc - 1){
					optind+=1; /* evita que as opcoes sejam lidas caso
						    *  o numero negativo seja hexadecimal */
				} else goto FIM_OPT;
				break;
		}
	}
	if(iter==0 || !bbx_flag_isset(iflags, bbx_any)){
		fprintf(stderr, "Erro: nenhum tipo especificado.\n"
				"Terminando...\n");
		exit(1);
	}
FIM_OPT:
	if(bbx_flag_isset(xflags, bbx_debug)) {
		fprintf(stderr, "%d opcoes foram lidas na entrada\n"
				"Flags para impressao:\t", iter);
		bbx_opt_printf();
	}
	if(bbx_flag_isset(iflags, bbx_str)) retptr = argscpy;
	else retptr = nums;
	return retptr;
}

/* change-to-option */
inline void bbx_chtop(bbx_flags fopt)
{
	if(bbx_flag_isset(iflags,  bbx_any)){
		bbx_unset_flag(iflags, bbx_any);
	}
	bbx_set_flag(iflags, fopt);
}
/* change-to-extra_flag */
inline void bbx_chtof(bbx_flags fopt)
{
	if(xflags & bbx_anyf){
		bbx_unset_flag(xflags, bbx_anyf);
	}
	bbx_set_flag(xflags, fopt);
}

inline void bbx_opt_printf()
{
	int impressos = 0;
	fputc('[', stderr);
	for(int i = 0; i < NO_MFLAGS; i++){
		if(bbx_flag_isset(xflags, mtags[i].bit)){
			if( impressos != 0 ) printf(", ");
			fprintf(stderr, "%s", mtags[i].tag);
			impressos++;
		}
	}
	for(int i = 0; i < NO_FLAGS; i++){
		if(bbx_flag_isset(iflags, itags[i].bit)){
			if( impressos != 0 ) printf(", ");
			fprintf(stderr, "%s", itags[i].tag);
			impressos++;
		}
	}
	fprintf(stderr, "]\n");
}

/*
inline void enqueue_nums(struct bbx_queue *q, int argc, char *argv[])
{
	char *str;
	int optc = 0;
	for(int i = 1; i < argc; i++){
		str=argv[i]; //i-esimo argv
		if( '0' <= str[0] && str[0] <= '9' ){
			if( bbx_enqueue(q, str) != Q_ERR ) continue;
			else{
				bbx_queue_show_stats(q);
				exit(1);
			}
		} else if(str[0] == '-'){
			if( '0' <= str[1] && str[1] <= '9' ){
				if( bbx_enqueue(q, str) != Q_ERR ) continue;
				else{
					bbx_queue_show_stats(q);
					exit(1);
				}
			} else optc += 1; //e uma opcao 
		}
	}
}
*/

inline void enqueue_nums(struct bbx_queue *q, int argc, char *argv[]){
	char *str;
	int optc = 0;
	for(int i = 1; i < argc; i++){
		str=argv[i]; //i-esimo argv
		bbx_flags numflags = 0;
		if((numflags = check_num(str))){
			print_ntype(str, numflags);
			if( bbx_enqueue(q, str) != Q_ERR ) continue;
			else{
				bbx_queue_show_stats(q);
				exit(1);
			}
		}
	}
	bbx_queue_show_stats(q);
}

inline void backup_argv(struct bbx_queue *q, int argc, char *argv[])
{
	char *str;
	for(int i = 1; i < argc; i++){
		str=argv[i]; //i-esimo argv
		if( bbx_enqueue(q, argv[i]) != Q_ERR ) continue;
		else{
			bbx_queue_show_stats(q);
			exit(1);
		}
	}
}

inline void print_ntype(char *num, bbx_flags flag){
	int i, impressos = 0;
	fprintf(stderr, "%s: ", num);
	fputc('[', stderr);
	for(int i = 0; i < NO_NTYPES; i++){
		if(bbx_flag_isset(flag, ntypes[i].bit)){
			if( impressos != 0 ) printf(", ");
			fprintf(stderr, "%s", ntypes[i].tag);
			impressos++;
		}
	}
	fprintf(stderr, "]\n");
}
