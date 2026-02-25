#!/bin/bash

echo 'Content-Type: text/html'
printf '\r\n\r\n'
echo "<!DOCTYPE html><html><head>"
echo "<meta charset=\"UTF-8\" />"
echo "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" />"
echo "<link href=\"/style.css\" rel=\"stylesheet\" />"
echo "<title>Danalexian</title></head>"
echo "<body>"
echo "<div class=\"intro\">"
echo "<h1>Test CGI bash</h1>"

echo "<p>Date/Heure"
date  +%m-%d_%H:%M:%S
echo " </p>"


echo "</div></body></html>"