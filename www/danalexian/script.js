document.querySelectorAll(".card button").forEach((btn) =>
  btn.addEventListener("click", async () => {
    const method = btn.dataset.method;
    const endpoint = btn.dataset.endpoint;
    const card = btn.closest(".card");
    const resultDiv = card.querySelector(".result");
    const methodDiv = resultDiv.querySelector(".method");
    const endpointDiv = resultDiv.querySelector(".endpoint");
    const headerDiv = resultDiv.querySelector(".header");
    const bodyDiv = resultDiv.querySelector(".body");
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
      resultDiv.textContent = "Méthode non valide";
      return;
    }
    const body = await response.text();
    const length = response.headers.get("content-length");
    const type = response.headers.get("content-type");
    const date = response.headers.get("date");
    const lModified = response.headers.get("last-modified");
    methodDiv.innerHTML = `<strong>method:</strong> ${method}`;
    endpointDiv.innerHTML = `<strong>endpoint:</strong> ${endpoint}`;
    headerDiv.innerHTML = `content-length: ${length}<br/>content-type: ${type}<br/>date: ${date}<br/>last-modified: ${lModified}`;
    bodyDiv.textContent = `body: ${body}`;
  }),
);
