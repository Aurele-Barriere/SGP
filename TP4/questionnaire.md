# TP4 : Prise en main de Nachos

### Mécanisme d'appel système

Les programmes utilisateurs ont accès à un ensemble d'appels systèmes dont les définitions sont disponibles dans `userlib/syscall.h`. Il suffit à l'utilisateur d'appeller une de ces procédures.
L'instruction MIPS `syscall` est alors appelée, ce qui déclenche l'appel de la routine de traitement des exceptions `ExceptionHandler` située dans `kernel/exception.cc`. Grâce à la valeur située dans le registre _r2_, la routine est capable de déterminer quel appel système a été effectué. Les paramètres sont eux situés dans les registres _r4_, _r5_, _r6_ et _r7_.

### Gestion de threads et de processus

1. Le rôle de ces deux fonctions est de sauvegarde et restorer l'état de tous les registres du processeur (**à l'exception des registres r2 r4 r5 r6 r7 qui sont utilisés dans les appels systèmes ????**)
Il faut sauvegarder l'état du processeur et celui du simulateur (cf question 7?), puisqu'on n'exécute pas le système d'exploitation directement sur une machine.

2. Le _scheduler_ contient une liste de tous les threads qui sont prêts à s'éxécuter : `readyList`. Cette liste ne contient pas le thread qui s'éxécute actuellement. Celui-ci est accessible grâce à la variable globale `g_current_thread` définie dans `kernel/system.cc` qui est un pointeur vers le thread actuel.

3. La varialbe `g_alive` est une liste globale de tous les threads existants. Contrairement à readyList, celle-ci peut contenir des threads n'étant pas prêts à s'éxécuter.

4. Pas sûr de comprendre la question. Elle n'alloue pas les objets chaînés, et se content de pointeurs. Pourquoi -> parce qu'elles sont utilisés dans le noyau qui a sa propre gestion mémoire ? Parce qu'on ne veut pas copier des threads mais juste les organiser ?? **à vérifier**  
Comme expliqué dans utility/list.h, on ne désalloue pas les objets à l'intérieur des ListElements, on se contente de désallouer les ListElements. De manière générale, seuls les ListElements (qui contiennent un pointeur sur l'objet concerné) sont alloués/désalloués. On veut pouvoir créer une list d'objets puis supprimer la liste sans supprimer les objets, pour pouvoir utiliser un même objet dans plusieurs listes sans avoir recours à une copie profonde.

5. Un objet thread est placé **nulle part** quand il est bloqué par un sémaphore. C'est au sémaphore de le replacer sur la liste des processus prêts quand il est débloqué. (**à vérifier**) avec la fonction `void ReadyToRun(Thread *thread)` définie dans kernel/scheduler.h

6. Il faut appeller la fonction `g_machine->interrupt->SetStatus` avec pour paramètre `INTERRUPTS_OFF`. On se place ainsi dans un mode où les interruptions sont désactivées.  
Il faut bien sûr les réactiver ensuite avec la même fonction et le paramètre `INTERRUPTS_ON`. (dans la fonction StartThread ou à l'initialisation. expliquer pourquoi?)

7. Le _scheduler_ est défini dans les fichiers `kernel/scheduler.cc` et `kernel/scheduler.h`.
Cette classe contient en particulier une méthode `void SwitchTo(Thread *nextThread)` qui effectue un changement de contexte.  
Il est nécessaire de sauvegarder deux types de contextes à cause du fonctionnement de Nachos: le contexte noyau et le contexte utilisateur.  
Le contexte noyau est celui du simulateur, manipulé par `SaveSimulatorState` et `RestoreSimulatorState`.  
Les méthodes `SaveProcessorState` et `RestoreProcessorState` devront quant à elles se charger de sauvegarder et restorer les registres du processeur.

8. Le champ `ObjectTypeId` de chaque objet permet de vérifier que les objets passés aux méthodes du noyau, sous forme de pointeurs, sont bien les objets attendus.

### Environnement de développement

1. On peut utiliser les options en ligne de commande de l'exécutable nachos. Par exemple, l'option -s lance une exécution pas à pas, avec affichage de l'état des registres entre chaque instruction.  
Cette version de Nachos contient également une alternative aux fonctions de la libc dans le dossier userlib. On y définit par exemple des fonctions sur les chaînes de caractères, sur les input/output etc.

2. Oui. Nachos s'éxécute directement sur notre PC. gdb peut y accéder facilement.

3. Non. Les programmes sont executés à l'intérieur de la machine virtuelle Nachos. Il faudrait apprendre à gdb comment Nachos attribue la mémoire, gère les processus, etc.
