#!/usr/bin/python3
import os
import sys
import datetime

print("Content-Type: text/html\r\n\r\n")
print("<!DOCTYPE html><html><body>")
print("<h1>Test CGI : GET & POST</h1>")

# 1. Affiche la méthode reçue (C'est la preuve pour l'évaluateur)
method = os.environ.get("REQUEST_METHOD", "UNKNOWN")
print(f"<h2>Méthode détectée : <span style='color:red'>{method}</span></h2>")

# 2. Affiche l'heure (Preuve que c'est dynamique)
print(f"<p>Date/Heure : {datetime.datetime.now()}</p>")

# 3. Si c'est un POST, on essaie de lire un petit body (Preuve ultime)
if method == "POST":
    try:
        # On lit stdin si CONTENT_LENGTH est présent
        length = os.environ.get("CONTENT_LENGTH")
        if length:
            data = sys.stdin.read(int(length))
            print(f"<p>Données reçues en POST : <b>{data}</b></p>")
        else:
            print("<p>Pas de Content-Length reçu.</p>")
    except:
        print("<p>Erreur lecture stdin (Pas grave pour ce test)</p>")

print("</body></html>")