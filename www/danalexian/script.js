function uploadFile() {
  const input = document.querySelector("#inputFile");
  const form = new FormData();
  if (!input.files[0]) {
    document.querySelector(".errDisplay").innerHTML =
      `<p><strong>Missing file!</strong></p>`;
    return;
  }
  form.append("file", input.files[0]);
  return fetch("uploads/" + input.files[0].name, {
    method: "POST",
    body: form,
  });
}

function generateBigFile(sizeInKo) {
  const bytes = new Uint8Array(sizeInKo * 1024);
  bytes.fill(65);
  return new Blob([bytes], { type: "application/octet-stream" });
}

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
    const statusDiv = resultDiv.querySelector(".status");
    const timeDiv = resultDiv.querySelector(".time");
    const startTime = performance.now();

    console.log("debut de la fonction");
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
    } else if (method === "POSTFILE") {
      response = await uploadFile();
    } else if (method === "POSTBIGFILE") {
      const size = Number(btn.dataset.size);
      const file = generateBigFile(size);
      console.log(file);
      response = await fetch(endpoint, { method: "POST", body: file });
    }
    console.log("fin des premieres condition");
    if (!response) {
      resultDiv.textContent = "Méthode non valide";
      return;
    }
    console.log(response);
    const body = await response.text();
    console.log("here");
    const status = response.status;
    const length = response.headers.get("content-length");
    const type = response.headers.get("content-type");
    const date = response.headers.get("date");
    const lModified = response.headers.get("last-modified");
    const time = performance.now() - startTime;

    if (status >= 200 && status < 300)
      resultDiv.style.backgroundColor = "var(--green)";
    else resultDiv.style.backgroundColor = "var(--red)";

    console.log(status);
    statusDiv.innerHTML = `<strong>status: </strong>${status}`;
    methodDiv.innerHTML = `<strong>method:</strong> ${method}`;
    endpointDiv.innerHTML = `<strong>endpoint:</strong> ${endpoint}`;
    headerDiv.innerHTML = `<strong>content-length: </strong>${length}<br/><strong>content-type: </strong>${type}<br/><strong>date: </strong>${date}<br/><strong>last-modified: </strong>${lModified}`;
    bodyDiv.innerHTML = `<strong>body:</strong> ${body}`;
    timeDiv.innerHTML = `<strong>Response's time: </strong>${time}ms`;
  }),
);
