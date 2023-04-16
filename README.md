# PCAD - Architetture e parallelismo
Relazione dello svolgimento del primo laboratorio del corso di PCAD

Gruppo (a.k.a. thread_ement):
- Andrea Valenzano 4548315
- Lorenzo Contino 4832500
- Eugenio Pallestrini 4878184

## Progetto
Il progetto si compone principalmente di due parti: 
- Implementazioni e operazioni sulle matrici. (matrix.h)
- Bencmark e stampa dei risultati (doomBench.h + main.c)

Essendo la libreria molto piccola e non presentando quindi problemi temporali in fase di compilazione e' stata interamente implementata all'interno dell'header file.
La libreria doomBench e' stata integrata nel progetto facendo parte della nostra codebase.

## Scelte ingegneristiche
Per l'implementazione della richiesta abbiamo effettuato le seguenti scelte:
- Suddividere il calcolo non decomponendolo per righe ma per entrate della matrice risultante(righa x colonna). Questo ci ha consentinto di avere maggiore capillarita' e di poter suddividere piu equamente il carico di lavoro.
- Il calcolo della matrice finale e' stato suddiviso in due calcoli distinti in modo da poter generalizzare il piu possibile le funzioni implementate (rimane comunque invariato l'utilizzo delle barriere).
 
## Implementazione
Le matrici sono sono state implementate come puntatori ad array di float allocate dinamicamente, e per la loro gestione abbiamo implementato specifiche funzioni ausiliarie.

L'implementazione della moltiplicazione tra matrici e' stata implementata in due diverse modalita': singe thread e multithread.
Questo per evitare di avere un overhead sulla creazione dei thread quando non e' necessario.

La versione multithread della multiply va ad allocare una quantita' di thread come passato tramite parametro.
Va quindi a dividere il carico di lavoro in parti uguali tra i thread tenendo lo scarto per il main.
Per fare questo la matrice risultante viene divisa in blocchi pari al numero di thread richiesti, oguno di essi performera' il calcolo della moltiplicazione vettoriale per tutti i valori compresi nel suo blocco.
Per implementare la sincronizazione di questo processo abbiamo utilizzato due barriere: una per prevenire race condition sull'identificatore del blocco di pertinenza al thread, la seconda per attendere il completamento del calcolo da parte di tutti i thread.
Dopo che quindi tutte le bariere vengono distrutte tutti i thread hanno completato il loro lavoro e possono essere joinati e liberati.

## Bencmark
Il bencmark e' stato svolto utilizzando matrici di dimensioni significative in modo da rendere apprezzabile il miglioramento temporale dell'esecuzione multithread.
Testando il codice (main.c) abbiamo quindi ottenuto i seguenti risultati:


```
Compiler: clang -O3
CPU: Ryzen 9 (24 threads)

A->( 1000 x 1500 )
B-> ( 1500 x 1200 )
C-> ( 1200 x 1500 )

== MATRIX MULTIPLY BENCMARK ===============================
Function Name: multiply_singe_thread
Execuiton time: 2189ms
===========================================================
Function Name: multiply_multi_pthread_2
Execuiton time: 1103ms
===========================================================
Function Name: multiply_multi_pthread_4
Execuiton time: 576ms
===========================================================
Function Name: multiply_multi_pthread_8
Execuiton time: 324ms
===========================================================
Function Name: multiply_multi_pthread_16
Execuiton time: 161ms
===========================================================
```

Da notare che all'incremento del numero di thread (nel nostro esempio esponenziale in base 2) anche il tempo di esecuzione decresce proporzionalmente.

## Approfondimento
Siccome il calcolo matriciale intenso e' oggigiorno eseguito quasi unicamente grazie all'utilizzo delle GPU, abbiamo implementato la moltiplicazione sfruttando le capacita' offerte da CUDA.
Abbiamo quindi ricompilato il sorgente usando il compilatore di nVidia (main.cu) ottenendo i seguenti risultati:

```
Compiler: nvcc
CPU: Ryzen 9 (24 threads)
GPU: GTX 1660 Super

A->( 1000 x 1500 )
B-> ( 1500 x 1200 )
C-> ( 1200 x 1500 )

== MATRIX MULTIPLY BENCMARK ===============================
Function Name: multiply_multi_pthread_2
Execuiton time: 4230ms
===========================================================
Function Name: multiply_multi_pthread_4
Execuiton time: 2163ms
===========================================================
Function Name: multiply_multi_pthread_8
Execuiton time: 1132ms
===========================================================
Function Name: multiply_multi_pthread_16
Execuiton time: 681ms
===========================================================
== MATRIX MULTIPLY CUDA BENCMARK ==========================
Function Name: cuda_multiply
Execuiton time: 27ms
===========================================================
```

Notando la differenza di prestazioni abbiamo quindi incrementato ogni dimensione delle matrici di un fattore 10 per stressare maggiormente l'algoritmo:


```
Compiler: nvcc
CPU: Ryzen 9 (24 threads)
GPU: GTX 1660 Super

A->( 10000 x 15000 )
B-> ( 15000 x 12000 )
C-> ( 12000 x 15000 )

== MATRIX MULTIPLY BENCMARK ===============================
Function Name: multiply_multi_pthread_16
Execuiton time: 662659ms
===========================================================
== MATRIX MULTIPLY CUDA BENCMARK ==========================
Function Name: cuda_multiply
Execuiton time: 4897ms
===========================================================
```

Da questi risultati capiamo che in caso di un utilizzo intenso della moltiplicazione tra matrici una soluzione basata sul multithreading della CPU non e' comunque sufficiente.
