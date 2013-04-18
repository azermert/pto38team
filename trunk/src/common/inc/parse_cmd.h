/**
  ******************************************************************************
  * @file    parse_cmd.h
  * @author  PTO Team
  * @version V1.0.0
  * @date    11/3/2013
  * @brief   This file contains all the functions prototypes for the SCPI firmware 
  *          library.   
  ******************************************************************************
*/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PARSE_CMD_H
#define __PARSE_CMD_H

/* Includes ------------------------------------------------------------------*/
/* Zadne includy zde nebudou!!!*/
#include "abc.h"

typedef enum uint32_t{		  	//QM = question mark (?) ;  US = underscore	(_)
															//WID_IDNQM=('?' << 24)|('N' << 16)|('D' << 8)|('I'),
	REGISTER_WID(I,D,N,QM),	//
	REGISTER_WID(E,R,R,R),	//
	REGISTER_WID(G,E,N,US),	//
	REGISTER_WID(G,P,I,O),	//
	REGISTER_WID(O,S,C,P),	//
	REGISTER_WID(L,O,G,US),	//
	REGISTER_WID(C,O,N,T),	//
	
	REGISTER_WID(M,E,A,S),	//
	REGISTER_WID(V,O,L,T),	//

	REGISTER_WID(S,E,T,V),
	REGISTER_WID(G,E,T,QM),
	
	REGISTER_WID(T,R,I,G),
	REGISTER_WID(L,E,V,L),
	REGISTER_WID(E,D,G,E),
	REGISTER_WID(F,R,E,Q),
	REGISTER_WID(P,R,E,T),
	REGISTER_WID(D,A,T,A),
	REGISTER_WID(S,T,R,T),
	REGISTER_WID(S,T,O,P),
	
	REGISTER_WID(T,Y,P,E),
	REGISTER_WID(A,M,P,L),
	REGISTER_WID(O,F,F,S),
	REGISTER_WID(D,U,T,Y),
	REGISTER_WID(D,E,P,T),

	REGISTER_WID(C,H,A,N),

	REGISTER_WID(F,R,Q,QM),
	REGISTER_WID(P,L,S,QM),

	REGISTER_WID(V,A,L,QM),

	REGISTER_WID(N,O,R,M),
	REGISTER_WID(A,U,T,O),
	REGISTER_WID(S,I,N,G),

	REGISTER_WID(R,I,S,E),
	REGISTER_WID(F,A,L,L),

	REGISTER_WID(1,K,US,US),
	REGISTER_WID(2,K,US,US),
	REGISTER_WID(5,K,US,US),
	REGISTER_WID(1,0,K,US),
	REGISTER_WID(2,0,K,US),
	REGISTER_WID(5,0,K,US),
	REGISTER_WID(1,0,0,K),
	REGISTER_WID(2,0,0,K),
	REGISTER_WID(5,0,0,K),
	REGISTER_WID(1,M,US,US),
	REGISTER_WID(2,M,US,US),
	REGISTER_WID(5,M,US,US),
	REGISTER_WID(1,0,M,US),

	REGISTER_WID(0,8,B,US),
	REGISTER_WID(1,6,B,US),

	REGISTER_WID(S,I,N,E),
	REGISTER_WID(S,Q,R,E),
	REGISTER_WID(T,R,I,A),
	REGISTER_WID(A,R,B,T),

	REGISTER_WID(A,N,D,US),
	REGISTER_WID(O,R,US,US),
	
	REGISTER_WID(C,N,T,F),
	REGISTER_WID(C,N,T,P)
}WORD_ID;


#define IS_OSCP_TRIG(WID_TRIG) (((WID_TRIG) == WID_NORM) || \
                            ((WID_TRIG) == WID_AUTO) || \
														((WID_TRIG) == WID_SING))

#define IS_OSCP_EDGE(WID_EDGE) (((WID_EDGE) == WID_RISE) || \
															 ((WID_EDGE) == WID_FALL))

#define IS_OSCP_FREQ(WID_FREQ) (((WID_FREQ) == WID_1KUSUS) || \
                            ((WID_FREQ) == WID_2KUSUS) || \
														((WID_FREQ) == WID_5KUSUS) || \
														((WID_FREQ) == WID_10KUS) || \
														((WID_FREQ) == WID_20KUS) || \
														((WID_FREQ) == WID_50KUS) || \
														((WID_FREQ) == WID_100K) || \
														((WID_FREQ) == WID_200K) || \
														((WID_FREQ) == WID_500K) || \
														((WID_FREQ) == WID_1MUSUS) || \
														((WID_FREQ) == WID_2MUSUS) || \
														((WID_FREQ) == WID_5MUSUS) || \
														((WID_FREQ) == WID_10MUS))

#define IS_OSCP_DEPT(WID_DEPT) (((WID_DEPT) == WID_08BUS) || \
													((WID_DEPT) == WID_16BUS))
													
#define IS_GENUS_TYPE(WID_TYPE) (((WID_TYPE) == WID_SINE) || \
                            ((WID_TYPE) == WID_SQRE) || \
														((WID_TYPE) == WID_TRIA) || \
														((WID_TYPE) == WID_ARBT))										

#define IS_LOGUS_EDGE(WID_EDGE) (((WID_EDGE) == WID_RISE) || \
															 ((WID_EDGE) == WID_FALL))	

#define IS_LOGUS_TYPE(WID_TYPE) (((WID_TYPE) == WID_ANDUS) || \
															 ((WID_TYPE) == WID_ORUSUS))
															 
#define IS_LOGUS_FREQ(WID_FREQ) (((WID_FREQ) == WID_1KUSUS) || \
                            ((WID_FREQ) == WID_2KUSUS) || \
														((WID_FREQ) == WID_5KUSUS) || \
														((WID_FREQ) == WID_10KUS) || \
														((WID_FREQ) == WID_20KUS) || \
														((WID_FREQ) == WID_50KUS) || \
														((WID_FREQ) == WID_100K) || \
														((WID_FREQ) == WID_200K) || \
														((WID_FREQ) == WID_500K) || \
														((WID_FREQ) == WID_1MUSUS) || \
														((WID_FREQ) == WID_2MUSUS) || \
														((WID_FREQ) == WID_5MUSUS) || \
														((WID_FREQ) == WID_10MUS))
															 
/* COMM_Exported_Functions */
COMM_CMD SCPI_try_parse_cmd(void); /* Pokusi se v bufferu najit prikaz, rozpoznat ho a vratit*/

#endif /*__PARSE_CMD_H */

/************************ END OF FILE *****************************************/
