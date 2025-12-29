style.css (tâches 1.x)

| #   | Tâche                                                                     | Priorité |
| --- | ------------------------------------------------------------------------- | -------- |
| X   | Changer la font: Cursive → 'Consolas', 'Monaco', 'Courier New', monospace | haute    |
| X   | Ajouter un reset CSS basique (margin/padding à 0, box-sizing border-box)  | haute    |
| X   | Styliser le header: titre Danalexian avec gradient purple/rose            | haute    |
| X   | Améliorer les cartes: box-shadow subtile + hover effect doux              | moyenne  |
| X   | Refaire le hover bouton: remplacer rotate(1800deg) par scale(1.05) + glow | moyenne  |
| X   | Styliser la zone résultat: box avec sections status/headers/body          | moyenne  |
| X   | Ajouter styles pour footer                                                | basse    |

index.html (tâches 2.x)

| #   | Tâche                                                                                   | Priorité |
| --- | --------------------------------------------------------------------------------------- | -------- |
| X   | Ajouter le header avec titre "Danalexian" + sous-titre                                  | haute    |
| X   | Organiser les cartes en 2 sections: "Tests Success" / "Tests Failure"                   | moyenne  |
| X   | Restructurer la zone résultat dans chaque carte (div pour status, headers, body, temps) | moyenne  |
| X   | Ajouter le footer: "Made by Dana, Kilian & Alexandre" + liens GitHub                    | basse    |

script.js (tâches 3.x)

| #   | Tâche                                                                              | Priorité |
| --- | ---------------------------------------------------------------------------------- | -------- |
| X   | Mesurer le temps de réponse avec performance.now()                                 | haute    |
| X   | Extraire et afficher les headers principaux (Content-Type, Content-Length, Server) | haute    |
| X   | Afficher le status code avec couleur (vert si 2xx, rouge sinon)                    | haute    |
| 3.4 | Ajouter try/catch et affichage propre des erreurs réseau                           | moyenne  |
| X   | Limiter l'affichage du body (truncate si trop long)                                | basse    |

---

bugs a fix:

Priorité haute (bugs)

| Fix | Fichier                | Description                                           |
| --- | ---------------------- | ----------------------------------------------------- |
| X   | index.html:15          | Typo "note site" → "notre site"                       |
| X   | index.html:52-60,72-79 | .result est dans .btn-container au lieu d'être à côté |

Priorité basse (optionnel)

| Fix | Fichier | Description |
|-----|---------|-------------|
| X| style.css:13 | Typo "Courrier New" → "Courier New" |
| X| style.css:81-83 | Réduire le glow (168px c'est énorme) |
| 3.4 | script.js | Ajouter try/catch |

---

- Trouver comment avoir la size d'un png (car comme je fait ca fonctionne pas)
