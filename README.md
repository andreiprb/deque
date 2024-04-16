# deque
C++ implementation of a double ended queue.

Documentation (in Romanian):

Clasa Block:


	* membru static constant de tip intreg "BLOCKSIZE"
		- initializat la crearea template-ului cu 512 / sizeof(T)
		- in implementarea gcc, deque foloseste block-uri de 512 biti

	* membru de tip T* (template) "data"
		- initializat cu nullptr
		- menit sa stocheze un block de dimensiune "BLOCKSIZE"

	* constructor si destructor pentru alocare si dealocare de memorie
		- constructorul aloca un block de dimensiune "BLOCKSIZE"
		- destructorul dealoca memoria, daca aceasta exista

Clasa Deque:

	* membru de tip Block<T>** (template) "header"
		- pointer catre buffer-ul de block-uri al deque-ului
		- la nivel inalt, se poate intelege ca o matrice bidiemnsionala de tip T

	* membrii de tip intreg "first_used_block", "last_used_block"
		- salveaza index-ul (in buffer-ul de block-uri) primului / ultimului block folosit 
		- index-ul primului block folosit poate fi 0 sau 1 (vezi constructor + functia push_front())
		- index-ul ultimului block folosit poate fi n-1 sau n-2, unde n este dimensiunea buffer-ului de block-uri (vezi constructor + functia push_back())

	* membrii de tip intreg "first_index", "last_index"
		- salveaza index-ul primului / ultimului element in block-ul propriu
		- initializati cu "BLOCKSIZE" / -1 (vezi constructor si functiile push_front(), push_back())
		- fiecare element inserat va fi inserat in primul / ultimul block folosit la index-ul --first_index / ++last_index
		- daca index-ul iese din limite, se schimba block-ul folosit si se reseteaza index-ul la "BLOCKSIZE" / -1

	* membru de tip intreg "last_block"
		- salveaza index-ul ultimului block existent in buffer
		- poate fi interpretat ca lungimea buffer-ului - 1

	* membru de tip intreg "count"
		- salveasa numarul total de elemente din deque

	* constructor si destructor pentru initializare, alocare si dealocare de memorie
		- counstructorul: 
			- creeaza obiectul cu un buffer de 2 bucket-uri si le aloca memorie
			- initializeaza "first_index" si "last_index" cu predecesorul, respectiv cu succesorul intervalului de indexi ale unui block de tip Block<T> (template): [0, BLOCKSIZE - 1]
			- pe scurt, variabilele se initializeaza cu -1 si "BLOCKSIZE"
			- initializeaza "first_used_block" si "last_used_block" cu 0 si 1 (cei 2 indexi ai buffer-ului de lungime 2)
			- initializeaza "last_block" cu 1 si count cu 0
			- complexitate de timp: O(1)

		-destructorul
			- apeleaza destructorul pentru fiecare block din buffer
			- elibereaza memoria header-ului
			- commplexitate: O(n), unde n este numarul de elemente din deque

	* functia push_front()
		- functia adauga la inceput valoarea trimisa prin parametru
		- se incrementeaza "count"
		- se decrementeaza "first_index" si se verifica daca a ajuns la -1
			- daca "first_index" este egal cu -1, se reseteaza la "BLOCKSIZE" - 1
			- se decrementeaza "first_used_block"

		- se verifica daca "first_used_block" este mai mic ca 0
			- daca "first_used_block" este mai mic ca 0, trebuie alocat un nou block la inceputul deque-ului
			- se incrementeaza "first_used_block" (cum se realoca un nou block la inceput, acesta preia index-ul 0 si impinge block-ul curent pe index-ul 1)
			- se incrementeaza "last_used_block" si "last_block" (insertia block-ului la inceput creste lungimea deque_ului)
			- se aloca o noua zona de memorie pentru buffer, de dimensiune mai mare cu 1 decat cea existenta
			- se copiaza pointerii catre block-urile existente in zona noua
			- se elibereaza zona veche de memorie a buffer-ului
			- se instantiaza block-ul abia introdus, pe pozitia 0 in buffer

		- se introduce elementul pe pozitia stabilita mai sus (in block_ul "first_used_block", pe pozitia "firsst_index")
		- se verifica daca "last_index" este egal cu 1
			- daca "last_index" este egal cu 1, elementul introdus este primul element introdus in deque
			- "last_used_block" primeste valoarea lui "first_used_block"
			- "last_index" primeste valoarea lui "first_used_index"

		- complexitate de timp medie: O(1)
		- complexitate de timp worst-case: O(n), unde n este numarul de block-uri din buffer (situatie intalnita la redimensionarea buffer-ului)

	* functia push_back()
		- functia adauga la final valoarea trimisa prin parametru
		- se incrementeaza "count"
		- se incrementeaza "last_index" si se verifica daca a ajuns la "BLOCKSIZE"
			- daca "last_index" este egal cu "BLOCKSIZE", se reseteaza la 0
			- se incrementeaza "last_used_block"

		- se verifica daca "last_used_block" este mai mare ca "last_block"
			- daca "last_used_block" este mai mare ca "last_block", trebuie alocat un nou block la finalul deque-ului
			- se incrementeaza "last_block" (insertia block-ului la final creste lungimea deque-ului)
			- se aloca o noua zona de memorie pentru buffer, de dimensiune mai mare cu 1 decat cea existenta
			- se copiaza pointerii catre block-urile existente in zona noua
			- se elibereaza zona veche de memorie a buffer-ului
			- se instantiaza block-ul abia introdus, pe pozitia "last_block" in buffer

		- se introduce elementul pe pozitia stabilita mai sus (in block_ul "last_used_block", pe pozitia "last_index")
		- se verifica daca "first_index" este egal cu "BLOCKSIZE"
			- daca "first_index" este egal cu "BLOCKSIZE", elementul introdus este primul element introdus in deque
			- "first_used_block" primeste valoarea lui "last_used_block"
			- "first_index" primeste valoarea lui "last_used_index"

		- complexitate de timp medie: O(1)
		- complexitate de timp worst-case: O(n), unde n este numarul de block-uri din buffer (situatie intalnita la redimensionarea buffer-ului)

	* functia pop_front()
		- daca "count" este 0, se apeleaza functia clear()
		- altfel, se decrementeaza "count", se incrementeaza "first_index" si se verifica daca "first_index" este egal cu "BLOCKSIZE"
			- daca "first_index" este egal cu "BLOCKSIZE", blocul curent nu mai stocheaza niciun element
			- "first_index" primeste 0
			- se incrementeaza "first_used_block"

		- daca "first_used_block" este mai mare ca 1, putem elibera block-ul 0
			- se decrementeaza "first_used_block" (cum elimina block-ul de la inceput, block-ul curent coboara cu o pozitie in buffer)
			- se decrementeaza "last_used_block" si "last_block" (stergerea block-ului de la inceput micsoreaza lungimea deque_ului)
			- se aloca o noua zona de memorie pentru buffer, de dimensiune mai mica cu 1 decat cea existenta
			- se copiaza pointerii catre block-urile existente in zona noua
			- se elibereaza block-ul de pe pozitia 0 in buffer-ul vechi
			- se elibereaza zona veche de memorie a buffer-ului

		- complexitate de timp medie: O(1)
		- complexitate de timp worst-case: O(n), unde n este numarul de block-uri din buffer (situatie intalnita la redimensionarea buffer-ului)

	* functia pop_back()
		- daca "count" este 0, se apeleaza functia clear()
		- altfel, se decrementeaza "count", se decrementeaza "last_index" si se verifica daca "last_index" este egal cu -1
			- daca "last_index" este egal cu -1, blocul curent nu mai stocheaza niciun element
			- "last_index" primeste "BLOCKSIZE"" - 1
			- se decrementeaza "last_used_block"

		- daca "last_used_block" este mai mic ca "last_block" - 1, putem elibera block-ul de pe pozitia "last_block"
			- se decrementeaza "last_block" (stergerea block-ului de la final micsoreaza lungimea deque-ului)
			- se aloca o noua zona de memorie pentru buffer, de dimensiune mai mica cu 1 decat cea existenta
			- se copiaza pointerii catre block-urile existente in zona noua
			- se elibereaza block-ul de pe pozitia "last_block" in buffer-ul vechi
			- se elibereaza zona veche de memorie a buffer-ului

		- complexitate de timp medie: O(1)
		- complexitate de timp worst-case: O(n), unde n este numarul de block-uri din buffer (situatie intalnita la redimensionarea buffer-ului)

	* functia insert()
		- daca index-ul introdus este mai mic ca 0, elementul se introduce prin functia push_front()
		- daca index-ul introdus este mai mar sau egala cu "count", elementul se introduce prin functia push_back()
		- daca index-ul introdus este in intervalul [0, count - 1], se verifica fata de ce capat al deque-ului este mai apropiata pozitia pe care vrem sa introducem elementul
			- daca pozitia este in prima jumatate a deque-ului, se adauga prin push_front() primul element din deque
			- fiecare element de la index 0 pana la "index" - 1 primeste valoarea elementului de dupa el (de la dreapta lui)
			- elementul de pe pozitia "index" primeste valoarea pasata ca parametru

			- daca pozitia este in a doua jumatate a deque-ului, se adauga prin push_back() ultimul element din deque
			- fiecare element de la index "count" - 1 pana la "index" + 1 primeste valoarea elementului de dianintea lui (de la stanga lui)
			- elementul de pe pozitia "index" primeste valoarea pasata ca parametru
		
		- complexitate de timp: O(n/2), unde n este numarul de elemente din deque

	* functia remove()
		- daca index-ul pasat ca parametru nu se afla in intervalul [0, count - 1], se iese din functie fara a se modifica nimic
		- altfel, se verifica fata de ce capat al deque-ului este mai apropiata pozitia
			- daca pozitia este in prima jumatate a deque-ului, fiecare element de la index-ul "index" pana la 1 primeste valoarea elementului de dinaintea lui (de la stanga lui)
			- se elimina primul element cu pop_front()

			- daca pozitia este in a doua jumatate a deque-ului, fiecare element de la index-ul "index" pana la "count" - 1 primeste valoarea elementului de dupa el (de la dreapta lui)
			- se elimina ultimul element cu pop_back()

		- complexitate de timp: O(n/2), unde n este numarul de elemente din deque

	* functiile front() si back()
		- returneaza primul, respectiv ultimul element din deque
		- complexitate de timp: O(1)

	* functiile size() si empty()
		- returneaza lungimea deque-ului (valoarea membrului "count"), respectiv valoarea booleana a membrului "count"
		- complexitate de timp: O(1)

	* functia clear()
		- apeleaza destructorul si reia pasii din constructor
		- complexitate de timp: O(n), unde n este numarul de block-uri din buffer

	* operatorul []
		- returneaza elementul de pe pozitia pasata ca parametru
		- se initializeaza o variabila "current_block" cu valoarea membrului "first_used_block"
		- se creste valoarea variabilei "index" cu valoarea membrului "first_index", pentru a obtine offset-ul necesar, in cazul in care primul element din deque nu este pe pozitia 0 in block-ul sau
		- se creste valoarea variabilei "current_block" cu partea intrega obtinuta prin impartiea valorii variabilei "index" la "BLOCKSIZE"
		- se returneaza elementul din block-ul cu index-ul "current_index", de pe pozitia "index" % "BLOCKSIZE"
		- complexitate de timp: O(1)

	* functia print()
		- itereaza pe baza de index in intervalul [0, "count" - 1] prin deque si afiseaza fiecare element
		- complexitate de timp: O(n), unde n este numarul de elemente din deque
