style.css (tâches 1.x)

| #   | Tâche                                                                     | Priorité |
| --- | ------------------------------------------------------------------------- | -------- |
| X   | Changer la font: Cursive → 'Consolas', 'Monaco', 'Courier New', monospace | haute    |
| X   | Ajouter un reset CSS basique (margin/padding à 0, box-sizing border-box)  | haute    |
| X   | Styliser le header: titre Danalexian avec gradient purple/rose            | haute    |
| X   | Améliorer les cartes: box-shadow subtile + hover effect doux              | moyenne  |
| X   | Refaire le hover bouton: remplacer rotate(1800deg) par scale(1.05) + glow | moyenne  |
| X   | Styliser la zone résultat: box avec sections status/headers/body          | moyenne  |
| 1.7 | Ajouter styles pour footer                                                | basse    |

index.html (tâches 2.x)

| #   | Tâche                                                                                   | Priorité |
| --- | --------------------------------------------------------------------------------------- | -------- |
| X   | Ajouter le header avec titre "Danalexian" + sous-titre                                  | haute    |
| X   | Organiser les cartes en 2 sections: "Tests Success" / "Tests Failure"                   | moyenne  |
| X   | Restructurer la zone résultat dans chaque carte (div pour status, headers, body, temps) | moyenne  |
| 2.4 | Ajouter le footer: "Made by Dana, Kilian & Alexandre" + liens GitHub                    | basse    |

script.js (tâches 3.x)

| # | Tâche | Priorité |
|---|-------|----------|
| 3.1 | Mesurer le temps de réponse avec performance.now() | haute |
| X | Extraire et afficher les headers principaux (Content-Type, Content-Length, Server) | haute |
| X | Afficher le status code avec couleur (vert si 2xx, rouge sinon) | haute |
| 3.4 | Ajouter try/catch et affichage propre des erreurs réseau | moyenne |
| 3.5 | Limiter l'affichage du body (truncate si trop long) | basse |

---

- trouver comment utiliser performance.now() correctement
- faire le footer
- demander a claude ce qu'il pense de se que j'ai fait
