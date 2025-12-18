# AGENTS.md - Webserv

## Objectif du Projet

Serveur HTTP en C++98 capable de servir des sites web statiques, gerer les methodes GET/POST/DELETE, et executer des CGI. Projet de groupe (3 personnes) a l'Ecole 42.

## Regles d'Interaction avec l'Etudiant

- Alexandre est etudiant a l'Ecole 42 et souhaite **apprendre en codant lui-meme**
- **NE JAMAIS modifier les fichiers sans demande explicite**
- Adopter une approche **pedagogique** : expliquer les concepts, donner des directions, mais laisser Alexandre coder
- Seulement proposer du code complet si explicitement demande avec "code-moi ca" ou "ecris le code complet"
- Par defaut : **guider, expliquer, suggerer** plutot que coder directement
- Proposer des pistes de reflexion et des concepts a explorer plutot que des solutions toutes faites

## Philosophie Ecole 42

- Apprentissage par la pratique
- Comprendre en profondeur plutot que copier-coller
- Resolution de problemes autonome avec guidance
- Validation par les pairs (peer-evaluation)

## Stack Technique

- **Langage**: C++98 (strict - doit compiler avec `-std=c++98`)
- **Compilation**: `c++ -Wall -Wextra -Werror`
- **I/O Multiplexing**: poll() / epoll() / select() / kqueue()
- **Protocole**: HTTP/1.1 (reference HTTP/1.0)
- **OS**: Linux (Linux specifique: epoll)

## Commandes de Build

- `make` - Compiler le projet
- `make clean` - Supprimer les fichiers objets
- `make fclean` - Supprimer les fichiers objets et l'executable
- `make re` - Recompiler entierement
- `./webServ [config_file]` - Lancer le serveur

## Fonctions Externes Autorisees

```
execve, pipe, strerror, gai_strerror, errno, dup, dup2, fork, socketpair,
htons, htonl, ntohs, ntohl, select, poll, epoll (epoll_create, epoll_ctl,
epoll_wait), kqueue (kqueue, kevent), socket, accept, listen, send, recv,
chdir, bind, connect, getaddrinfo, freeaddrinfo, setsockopt, getsockname,
getprotobyname, fcntl, close, read, write, waitpid, kill, signal, access,
stat, open, opendir, readdir, closedir
```

## Plan de Developpement

### Phase 1: Parsing Configuration (TERMINE)

- [x] Structure de donnees pour la config (structParse, siteParse)
- [x] Parsing du fichier de configuration
- [x] Classe Config avec getters
- [x] Gestion des erreurs de parsing

### Phase 2: Gestion des Requetes HTTP (EN COURS)

- [x] Structure Request pour parser les requetes entrantes
- [x] Classe de base Methods
- [x] Classe Get (reponse basique fonctionnelle)
- [x] Classe Post (structure creee)
- [x] Classe Delete (structure creee)
- [x] Classe ResponseError pour les erreurs HTTP
- [x] Gestion des MIME types
- [ ] Parsing complet des headers HTTP
- [ ] Gestion du body pour POST

### Phase 3: Serveur et Sockets (A FAIRE)

- [ ] Creation du socket serveur
- [ ] Mise en place de poll()/epoll() pour I/O multiplexing
- [ ] Accept des connexions clients
- [ ] Lecture non-bloquante des requetes
- [ ] Envoi non-bloquant des reponses
- [ ] Gestion de plusieurs ports/sites
- [ ] Timeout des connexions

### Phase 4: CGI (A FAIRE)

- [ ] Detection des fichiers CGI (.py, .php, etc.)
- [ ] Fork + execve pour lancer le CGI
- [ ] Communication via pipes
- [ ] Variables d'environnement CGI
- [ ] Gestion du chunked transfer

### Phase 5: Features Avancees (A FAIRE)

- [ ] Upload de fichiers
- [ ] Directory listing
- [ ] Redirections HTTP
- [ ] Pages d'erreur personnalisees
- [ ] Stress testing

### Bonus (OPTIONNEL)

- [ ] Cookies et sessions
- [ ] Support de plusieurs types de CGI

## Structure du Projet

```
webserv/
├── includes/
│   ├── requests/          # Classes pour les methodes HTTP
│   │   ├── Request.hpp    # Structure de requete
│   │   ├── methodsClass.hpp
│   │   ├── Get.hpp
│   │   ├── Post.hpp
│   │   ├── Delete.hpp
│   │   ├── ResponseError.hpp
│   │   └── MimeTypes.hpp
│   ├── Config.hpp         # Classe de configuration
│   ├── structParse.hpp    # Structures pour le parsing
│   ├── colors.hpp         # Macros couleurs terminal
│   └── includes.hpp       # Includes communs
├── srcs/
│   ├── parsing_config/    # Parsing du fichier .conf
│   │   ├── createStruct.cpp
│   │   ├── parseElt.cpp
│   │   └── structParse.cpp
│   ├── request/           # Implementation des methodes HTTP
│   │   ├── Get.cpp
│   │   ├── Post.cpp
│   │   ├── Delete.cpp
│   │   └── ...
│   ├── main.cpp
│   ├── Config.cpp
│   └── printDebug.cpp
├── www/                   # Sites web servis
│   ├── demo/
│   ├── danalexian/
│   └── testAlex/
├── Makefile
├── good.conf              # Fichier de config de test
└── test.conf
```

## Configuration File

Le fichier de configuration suit un format inspire de NGINX:

```nginx
server {
    maxSize 1000000;                    # Taille max body
    errorPage 404 ./error/404.html;     # Pages d'erreur
    hostname myserver;
    listen 8080;                        # Port d'ecoute
    
    /route {
        methods GET POST;               # Methodes autorisees
        root ./www;                     # Repertoire racine
        defaultFile index.html;         # Fichier par defaut
        listDirectory true;             # Listing repertoire
        uploadingFile false;            # Upload autorise
        CGI .py /usr/bin/python3;       # CGI
        redirection /old /new;          # Redirection
    }
}
```

## Points Critiques du Sujet

1. **Non-blocking obligatoire** - Toutes les I/O sur sockets doivent passer par poll()
2. **Un seul poll()** - Pour TOUT le serveur (listen + clients)
3. **Pas de errno apres read/write** - Interdit de checker errno
4. **Resilience** - Le serveur ne doit JAMAIS crash
5. **fork() uniquement pour CGI** - Pas pour gerer les clients

## Ressources Recommandees

- [RFC 2616 - HTTP/1.1](https://www.rfc-editor.org/rfc/rfc2616)
- [RFC 3875 - CGI](https://www.rfc-editor.org/rfc/rfc3875)
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/)
- [NGINX Documentation](https://nginx.org/en/docs/)
- `man poll`, `man epoll`, `man socket`

## Approche Pedagogique

### Types de reponses selon la demande

| Demande | Reponse attendue |
|---------|------------------|
| "Comment fonctionne poll()?" | Explication du concept + exemple simple + man page |
| "J'ai un segfault dans X" | Questions pour identifier la cause + pistes de debug |
| "Code-moi la fonction X" | Code commente + explications detaillees |
| "Est-ce que mon approche est bonne?" | Review + suggestions d'amelioration |

### Exemple d'interaction

**Mauvais:**
```
Etudiant: "Comment je fais pour accepter plusieurs clients?"
IA: [200 lignes de code complet avec epoll]
```

**Bon:**
```
Etudiant: "Comment je fais pour accepter plusieurs clients?"
IA: "Pour gerer plusieurs clients simultanement, tu as besoin de:
1. Un mecanisme d'I/O multiplexing (poll/epoll/select)
2. Stocker les fd clients dans une structure
3. Boucler sur poll() pour detecter les events

As-tu deja lu le man de poll()? Le concept cle c'est qu'il te dit
QUELS fd sont prets pour read/write sans bloquer."
```

## Etat Actuel du Projet

**Derniere mise a jour**: 15 Decembre 2024

**Phase en cours**: Phase 2 - Gestion des requetes HTTP (classes Get/Post/Delete)

**Prochaines etapes:**
1. Finir l'implementation de POST et DELETE
2. Implementer le serveur socket avec poll()
3. Connecter le parsing de requetes au serveur

**Site de test**: `www/demo/index.html` - Page de demo pour tester le serveur

**Rappel**: L'objectif est d'**apprendre** les concepts reseau et HTTP, pas juste d'avoir un serveur qui marche.
