Navod na instalaci SVN a pripojeni repozitare:

1. Stazeni instalace SVN tortoise

http://ftp-stahuj.centrum.cz/dl/ef0bb3f4431fd6283edab50ff97b1bb9/51235fc3/stahuj/download/software/secured/t/tortoisesvn/1711/32-bit/TortoiseSVN-1.7.11.23600-win32-svn-1.7.8.msi

2. Instalace s beznym nastavenim

3. Vytvoreni pracovní slozky (kdekoliv na disku ale nejak inteligentne :) )

4. Pravym mysitkem na slozku a zvolit "SVN checkout"

5. Vyplneni udaju:
	URL of repository: https://pto38team.googlecode.com/svn/trunk
	Checkout directory: -vase slozka-
	checkout depth: fully recursive
	revision: HEAD

6. Hotovo. Ve slozce se objevi adresarova struktura projektu a nejake soubory mimo jine i tento navod.



Commit (nahrani) uprav:

1. Davejte pri commitu pozor at neupravite neco co nechcete!!! upravene soubory a slozky jsou oznaceny cervenou znackou s vykricnikem

2. Na korenovou slozku kde mate vse ulozeno zase pravym a dat "SVN commit"

3. Do message vypltnit kratky popis toho co bylo kde zmeneno (napr comm.c - implementovano SCPI)
4. Vybrat soubory ktere chci upravit (dvjoklikem se lze podivat na zmeny) a OK

5. Zadat uzivatelske jmeno a heslo pro commit
jmeno: pto38team
heslo: nX5MB2pb4ST9

6. NEZAPOMEN odskrtnout remember password (heslo se kdyztak da najit v documentes/svn_account.txt)



Update (stazeni aktualni revize):

1. Pravym tlacitkem na korenovou slozku a dát "SVN update"


Revert zmen:

1. Revertovat se daji pouze zmeny ve vasem pracovnim adresari!!!

2. Pravým na slozku -> "TortoiseSVN/revert"

3. Vybrat soubory, ve kterych chci zrusit zmeny



Dalsi uzitecne veci z nabidky TortoiseSVN urcite budouY:
Show log - zobrazi historii ruznych revizi
Update to revision - moznost stahnout urcitou revizi
create/apply patch - vytvori patch s aktualnimi upravami










