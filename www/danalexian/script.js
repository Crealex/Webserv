document.querySelectorAll(".card button").forEach((btn) =>
  btn.addEventListener("click", async () => {
    const method = btn.dataset.method;
    const endpoint = btn.dataset.endpoint;
    const card = btn.closest(".card");
    const resultSpan = card.querySelector(".result");
    let response;
    if (method === "GET") {
      if (endpoint) response = await fetch(endpoint);
    } else if (method === "POST") {
      if (endpoint)
        response = await fetch(endpoint, {
          method: "POST",
          headers: {
            "Content-Type": "text/plain",
          },
          body: "Je suis le test de la method post, et si je réussis je suis content",
        });
    } else if (method === "DELETE") {
      if (endpoint) response = await fetch(endpoint, { method: "DELETE" });
    }
    if (!response) {
      resultSpan.textContent = "Méthode non valide";
      return;
    }
    const data = await response.text();
    resultSpan.textContent = `method: ${method}, endpoint: ${endpoint}, result: ${data}`;
  }),
);
