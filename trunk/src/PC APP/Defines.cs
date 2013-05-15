using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace PTO_PC_APP
{
    class Defines
    {
        public const string IDN = "IDN_";

        /*
        public const string STM32F051 = "STM32F051";
        public const string STM32F100 = "STM32F100";
        public const string STM32F303 = "STM32F303";
        public const string STM32F407 = "STM32F407";
        public const string ADuC843 = "ADuC843";
        */

        /* KOMUNIKACE */
        // obecne
        public const string IDNRequest = "IDN?";
        public const string START = "SRAT";
        public const string STOP = "STOP";
        public const string ACKNOWLEDGE = "ACKN";
        public const string ERROR = "ERRR";
        
        public const string RISE = "RISE";
        public const string FALL = "FALL";

        //frekvence
        public const string FREQ_1K = "1K__";
        public const string FREQ_2K = "2K__";
        public const string FREQ_5K = "5K__";
        public const string FREQ_10K = "10K_";
        public const string FREQ_20K = "20K_";
        public const string FREQ_50K = "50K_";
        public const string FREQ_100K = "100K";
        public const string FREQ_200K = "200K";
        public const string FREQ_500K = "500K";
        public const string FREQ_1M = "1M__";
        public const string FREQ_2M = "2M__";
        public const string FREQ_5M = "5M__";


        //scope
        public const string SCOPE_8BIT = "OSC8";
        public const string SCOPE = "OSCP";
        public const string SCOPE_TRIG = "TRIG";
        public const string SCOPE_TRIG_LEVEL = "LEVL";
        public const string SCOPE_EDGE = "EDGE";
        public const string SCOPE_PRETRIGGER = "PRET";
        public const string SCOPE_FREQUENCY = "FREQ";

        public const string SCOPE_TRIG_NORMAL = "NORM";
        public const string SCOPE_TRIG_AUTO = "AUTO";
        public const string SCOPE_TRIG_SINGLE = "SING";


        //generator
        public const string GENERATOR = "GEN_";
        public const string GEN_AMPLITUDE = "AMPL";
        public const string GEN_OFFSET = "OFFS";
        public const string GEN_DUTY = "DUTY";
        public const string GEN_FREQUENCY = "FREQ";
        public const string GEN_TYPE = "TYPE";

        public const string GEN_SINE = "SINE";
        public const string GEN_SQUARE = "SQRE";
        public const string GEN_TRIANGLE = "TRIA";

        /*	
        //ZATIM NENAPSANA KOMUNIKACE
         * 	REGISTER_WID(1,K,US,US),
        REGISTER_WID(I,D,N,QM),	//
	REGISTER_WID(E,R,R,R),	//
	REGISTER_WID(G,E,N,US),	//
	REGISTER_WID(G,P,I,O),	//
	REGISTER_WID(L,O,G,US),	//
	REGISTER_WID(C,O,N,T),	//
	
	REGISTER_WID(M,E,A,S),	//
	REGISTER_WID(V,O,L,T),	//

	REGISTER_WID(S,E,T,V),
	REGISTER_WID(G,E,T,QM),
	
	
	REGISTER_WID(D,A,T,A),
	
	REGISTER_WID(T,Y,P,E),
	REGISTER_WID(A,M,P,L),
	REGISTER_WID(O,F,F,S),
	REGISTER_WID(D,U,T,Y),
	REGISTER_WID(D,E,P,T),

	REGISTER_WID(C,H,A,N),

	REGISTER_WID(F,E,Q,QM),   //Prepis FRQ? kvuli 8051
	REGISTER_WID(P,L,S,QM),

	REGISTER_WID(V,A,L,QM),



	REGISTER_WID(0,8,B,US),
	REGISTER_WID(1,6,B,US),

	REGISTER_WID(S,I,N,E),
	REGISTER_WID(S,Q,R,E),
	REGISTER_WID(T,R,I,A),
	REGISTER_WID(A,R,B,T),

	REGISTER_WID(A,N,D,US),
	REGISTER_WID(O,R,US,US),
	
	REGISTER_WID(C,O,T,F), //prepis CNTF kvuli 8051 
	REGISTER_WID(C,N,T,P)*/


        //obecne pro umisteni XML souboru
        public const string confPath = "../../";


    }
}
