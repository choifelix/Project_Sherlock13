Original work on gitlab : https://gitlab.com/felixchoi/Project_Scherlock
Bonjour, vous êtes dans le readme du jeux Sherlock sh13 par Felix CHOI et Thizirie OULD AMER.

Les includes :

#include <SDL2/SDL.h>        
#include <SDL2/SDL_image.h>        
#include <SDL2_ttf/SDL_ttf.h>        
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

Assurez vous que toutes les dépendances sont installées.

Pour jouer au jeu, il faut t tout d’abord compiler le server et client. Un script bash est mis a disposition il suffit de taper :

>$ ./cmd.sh

Une fois compilé, pour lancer le server il faut préciser un numéro de port:

>$ ./server <port>

Ex: >$ ./server 32000

Pour lancer le client, donc un joueur, il lancer sh13, en précisant l’adresse IP du server, le numéro de port du server, l’adresse IP du joueur, le numéro de port du joueur, et un pseudo :

>$ ./sh13 <serverIPadress> <serverPort> <clientIPadress> <clientPort> <pseudo>

Par exemple sur un même ordinateur on pourra écrire :
>$ ./sh13 localhost 32000 localhost 10001 joueur0

Une fois 4 joueur connectés au serveur, le jeu peu commencer.







Tutoriel du jeu :

Jeu à 4 joueurs. 1. Chacun doit se connecter sur son terminal après le lance- ment du serveur. 2. En lançant le serveur il faut spécifier le numéro de port qu’il va prendre. Sachant que pour l’adresse IP, il prendra celle de la machine sur laquelle il est lancé. 3. Chaque joueur devra, par contre, spécifier 5 champs. Le port du serveur, suivie de son adresse IP. Ensuite le numéro de port du joueur et son IP à lui. Puis enfin son identifiant. 4. Le premier à s’identifier sera le joueur 0 ... etc. 5. Une fenetre s’affichera pour chaque joueur, et ils devront tous se connecter. 6. Chaque joueur aura 3 cartes sur les 13 possibles. Le but et de trouver la seule qui n’a pas été distribuée = le coupable. Chaque carte a des éléments "caractéristiques". 7. Le joueur 0 va commencer à jouer. et ainsi de suite. 8. Trois choix s’offrent à chacun. Demander combien de "caractéristique X" a la personne Z. Demander si tous les autres joueurs ont le caractéristique X. Proposer un coupable. 9. Le gagnant sera celui qui trouvera le coupable en premier. Happy Sherlock games. 



Juin 2018 - Polytech Sorbonne (Sorbonne Université)
