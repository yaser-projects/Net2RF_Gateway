// =========================
// METAL BRAIN QUICK-START
// =========================

let step = 1;
const steps = 5;

function showStep(n) {
    document.querySelectorAll(".qs-step").forEach(s => s.classList.remove("active"));
    document.getElementById("step" + n).classList.add("active");

    document.querySelectorAll(".dot").forEach(d => d.classList.remove("active"));
    document.querySelectorAll(".dot")[n - 1].classList.add("active");
}

// WebSocket اتصال
const proto = location.protocol === "https:" ? "wss://" : "ws://";
const ws = new WebSocket(proto + location.host + "/ws");

// مرحله ۱: start
document.getElementById("startBtn").onclick = () => {
    step = 2;
    showStep(step);

    ws.send(JSON.stringify({
        setting: "AP",
        action: "read",
        fields: ["SSID","Pre-Shared Key","AP Hidden"]
    }));
};

// مرحله ۲ → save AP
document.getElementById("saveAp").onclick = () => {
    ws.send(JSON.stringify({
        setting: "AP",
        action: "write",
        "SSID": document.getElementById("apSsid").value,
        "Pre-Shared Key": document.getElementById("apPass").value,
        "AP Hidden": document.getElementById("apHidden").checked ? "1" : "0"
    }));

    step = 3;
    showStep(step);

    ws.send(JSON.stringify({
        setting: "Modem",
        action: "read",
        fields: ["Scan Networks"]
    }));
};

// مرحله ۳ → save Station
document.getElementById("saveSta").onclick = () => {
    step = 4;
    showStep(step);
};

// مرحله ۴ → save Mode
document.getElementById("nextToStep5").onclick = () => {
    step = 5;
    showStep(step);

    ws.send(JSON.stringify({
        setting: "device",
        action: "read",
        fields: ["Device Info"]
    }));
};

// مرحله ۵ → finish
document.getElementById("finishBtn").onclick = () => {
    alert("Configuration completed!");
    window.location.href = "index.html";
};

// WebSocket Listener
ws.onmessage = (e) => {
    const data = JSON.parse(e.data);

    if (data["SSID"]) document.getElementById("apSsid").value = data["SSID"];
    if (data["Pre-Shared Key"]) document.getElementById("apPass").value = data["Pre-Shared Key"];

    if (data["Scan Networks"]) {
        const sel = document.getElementById("staNetworks");
        sel.innerHTML = "";
        data["Scan Networks"].forEach(net => {
            const op = document.createElement("option");
            op.textContent = net;
            sel.appendChild(op);
        });
    }

    if (data["Device Info"]) {
        document.getElementById("summaryBox").innerText = JSON.stringify(data["Device Info"], null, 2);
    }
};

// نمایش گام اول
showStep(step);
