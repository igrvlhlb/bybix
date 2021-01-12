#ifndef _BBX_FLAGS_H
#define _BBX_FLAGS_H 1

/* flags uteis para a leitura */
#define		bbx_isdec		1	/* decimal */
#define		bbx_isbin		2	/* binario */
#define		bbx_isoct		4	/* octal */
#define		bbx_ishex		8	/* hexadecimal */
#define		bbx_isstr		16	/* nao e numero. ou
						   seja, e string */
#define		bbx_issig		32	/* signed */
#define		bbx_isfp		64	/* floating point */
#define		bbx_isnc		128	/* notacao cientifica */

#define		bbx_isany	((bbx_isdec) | (bbx_isbin)\
				|(bbx_isoct) | (bbx_ishex))
#define		bbx_isanyf	((bbx_isfp) | (bbx_issn))

/* opcoes para impressao */
#define 	bbx_char 		1	/* char (1 byte) */
#define 	bbx_short 		2
#define 	bbx_int 		4	   
#define 	bbx_float 		8	   
#define 	bbx_word		16	/* (1 word) */
#define		bbx_str			32

/* opcoes extras  */			
#define 	bbx_hex			1
#define		bbx_bin			2
#define 	bbx_long		4
#define		bbx_debug		8

/* conveniencia */
#define 	bbx_lint	((bbx_long) | (bbx_int))
#define 	bbx_double	((bbx_long) | (bbx_float))

#define	 bbx_any	((bbx_char) | (bbx_short) | (bbx_int)\
			|(bbx_float) | (bbx_word) | (bbx_str))

#define	 bbx_anyf	((bbx_hex) | (bbx_bin))

/* macros para operacoes bit a bit */
#define	bbx_set_flag(flag, bit) (flag) = (flag) | ((typeof(flag))bit)

#define	bbx_unset_flag(flag, bit) (flag) = (flag) & ~((typeof(flag))bit)

#define	bbx_flag_isset(flag, bit) ((flag) & ((typeof(flag))bit))

typedef unsigned char bbx_flags;

/* serao definidas em optparse.c */
extern bbx_flags iflags; //flags para informar a forma
	       		// que a impressao deve ser feita
extern bbx_flags xflags; //opcoes extras

/* par valor - nome das opcoes */
struct bbx_flag_s
{
	bbx_flags bit;
	char tag[16];
};

#endif /* _BBX_FLAGS */
