# WarehouseManager


>Fiecare ROBOT are un inventar(Manifest) care reprezinta lista
pachetelor asociate robotului nostru.
	
  > Manifest
  
	| 		|
	|   pachet 1  	|
	|		|
  
	| 		|
	|   pachet 2  	|
	|		|
  
	| 		|
	|   pachet 3  	|
	|		|

>
> pachetul se poate accesa prin campurile next si prev.
>
>  ROBOT_LOAD_OAN_PACKAGE:
>
>	    1. daca robotul este plin, nu avem ce face
>	  
>		2. creez un nou nod pentru robot(Mnifest*), caruia ii asociem pachetul primit la intrarea in functie;
>   
>		3. daca robotul este gol primul nod din robot este cel creat (2.);
>
>		4. first - > reprezinta nodul cu care iterez prin nodurile listei de pachete(Manifest), adica iau fiecare pachet din inventar.
>
>		5. daca nu am inserat la inceputul listei, incerc sa inserez in interiorul ei, pentru acest lucru verific ca prioriatea pachetului pe care il vreau inserat in lista sa fie mai mare decat valoarea prioritatii pachetului la care sunt in acest moment si simultan sa fie si mai mica decat elementul imiediat urmator;altfel nu am cum sa inserez astfel si merg mai departe;\
>
>	    6. daca gasesc un pachet in lista care sa aibe aceeasi prioritate cu pachetul pe care vreau sa il inserez se va face cmparatie dupa destnatia pachetului, nu dupa prioritatea acestuia; deci folosesc strcmp pentru a verifica  care este pachetul mai mare alfabetic=>astfel, in functie de acest rezultat nodul se va insera ininte sau dupa pachetul in cauza(cel cu care facem comparatia, cel care are ac prioritate cu cel inserat).


