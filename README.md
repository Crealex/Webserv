# Webserv
Create a webserver in C and CPP, with beautiful people

## Github tips:

### Branch

On essaye de créer une branche par feature et on merge (via pull request) une fois la feature finie.

### Pull request

Une pull request créer un espace "bac a sable" qui permet de tester l'integration de la branch avec le main sans risquer de casser le main.

Possibilité de creer des scripts pour tester automatiquement notre programme afin de se faciliter le travail de test en chaque pull.

Tout est gerable via l'interface graphique de github ou directement en ligne de commande (voir [[#Utils cmds]])

### Workflow de base:
1. On crée une branche pour la feature voulue
2. Une fois la feature finie, on crée une pull request pour demander de merge avec le main
3. Les autres membres de la team valide la pull request (apres avoir lu le code et fait des tests)
4. On croise les doigts pour que tout ne soit pas cassé :..)

### Utils cmds
- **Create and go to new branch:** `git -b checkout <new-branch>`
- **Setup new branch when push:** `git push --set-upstream origin <branch> `
- **Create new pull request:** `gh pr create --title "Title of pull request" --body "Explain the pull request"`

### Warning

Comme toujours bien communiquer (même si en théorie les pulls requests nous empeche de faire trop de betises)

Si plusieurs personnes travaillent sur la meme feature a definir si:
1. On cree des sous-branches
2. On travaille en asychrone
3. Inshallah

Definir si besoin de seulement une personne pour valider la pull request ou si il faut que **Toute la team** la valide.

Avant d'utilser les commandes `gh` il faut faire `gh login` pour se log à son compte github (c'est pas très compliqué promis)

### Liens utiles

- [https://docs.github.com/fr/pull-requests/collaborating-with-pull-requests/proposing-changes-to-your-work-with-pull-requests/creating-a-pull-request?tool=cli]
