# 1. Test pour savoir si le serveur

ps aux | grep webServ

# 2. Test pour savoir si le port est ouvert

ss -tlnp | grep 8080

# 3. Test la co avecn netcat

nc -zv localhost 8080
