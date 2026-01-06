## Todolist Phase 2 - Upload & Test gros fichiers

Phase 2A : Frontend (HTML/CSS/JS)

| #     | Fichier    | Tâche                                                                                  | Priorité |
| ----- | ---------- | -------------------------------------------------------------------------------------- | -------- |
| X     | index.html | Ajouter une carte "Upload File" avec <input type="file"> + bouton "Upload"             | haute    |
| X     | index.html | Ajouter une carte "Test Big File" avec 3 boutons: 100Ko, 500Ko, 1Mo                    | haute    |
| X     | index.html | Ajouter zone résultat dans les 2 nouvelles cartes (même structure que GET/POST/DELETE) | haute    |
| X     | style.css  | Styliser l'input file (il est moche par défaut, utiliser label + opacity trick)        | moyenne  |
| X     | style.css  | Styliser les boutons de taille (100Ko/500Ko/1Mo)                                       | basse    |
| p2a.6 | script.js  | Créer fonction uploadFile() qui envoie via FormData API                                | haute    | | p2a.7 | script.js  | Créer fonction generateBigFile(sizeInKo) qui génère un Blob                            | haute    |
| p2a.8 | script.js  | Brancher les event listeners sur les nouveaux boutons                                  | haute    |
| p2a.9 | script.js  | Afficher le résultat (status, headers, temps)                                          | moyenne  |

---

Phase 2B : Backend (C++)

| #     | Fichier       | Tâche                                                                    | Priorité |
| ----- | ------------- | ------------------------------------------------------------------------ | -------- |
| p2b.1 | -             | Créer le dossier www/danalexian/uploads/                                 | haute    |
| p2b.2 | MimeTypes.cpp | Ajouter multipart/form-data dans la map                                  | haute    |
| p2b.3 | Post.cpp      | Parser le header Content-Type pour extraire le boundary                  | haute    |
| p2b.4 | Post.cpp      | Parser le body multipart - trouver les boundaries et extraire le contenu | haute    |
| p2b.5 | Post.cpp      | Extraire le filename depuis Content-Disposition                          | haute    |
| p2b.6 | Post.cpp      | Écrire le fichier dans uploads/ (mode binaire!)                          | haute    |
| p2b.7 | Post.cpp      | Vérifier Content-Length vs maxSize AVANT de lire le body                 | haute    |
| p2b.8 | Post.cpp      | Retourner 413 Payload Too Large si fichier trop gros                     | moyenne  |
| p2b.9 | Post.cpp      | Retourner 201 Created avec le path du fichier uploadé si succès          | moyenne  |

---

# Notes techniques pour t'aider

Frontend - FormData API (p2a.6)
const formData = new FormData();
formData.append('file', fileInput.files[0]);
fetch('/uploads', {
method: 'POST',
body: formData // Le navigateur ajoute automatiquement le Content-Type avec boundary
});
Frontend - Générer un gros fichier (p2a.7)
function generateBigFile(sizeInKo) {
const bytes = new Uint8Array(sizeInKo _1024);
// Remplir avec des données (ex: 'A' répété)
bytes.fill(65); // 65 = 'A' en ASCII
return new Blob([bytes], { type: 'application/octet-stream' });
}
CSS - Styliser input file (p2a.4)
/_ Cacher le vrai input _/
.file-input {
opacity: 0;
position: absolute;
}
/_ Styliser le label qui le déclenche _/
.file-label {
/_ tes styles ici \*/
cursor: pointer;
}
Backend - Extraire boundary (p2b.3)
Le header ressemble à : Content-Type: multipart/form-data; boundary=----WebKitFormBoundary7MA4YWxk
Tu dois parser après boundary= pour récupérer la chaîne.

---

Ordre recommandé

1. p2a.1 → p2a.3 : Structure HTML d'abord
2. p2a.4 → p2a.5 : Un peu de CSS pour que ce soit joli
3. p2a.6 → p2a.9 : La logique JS
4. Test avec le navigateur : Tu verras les requêtes partir (même si le serveur ne répond pas encore correctement)
5. p2b.1 → p2b.9 : Le backend

---
