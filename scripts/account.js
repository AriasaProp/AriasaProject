

function checkLoginStatus() {
    const isLoggedIn = localStorage.getItem("isLoggedIn") === "true";
    if (isLoggedIn) {
        console.log("Pengguna sudah login.");
    } else {
        console.log("Pengguna belum login.");
    }
}

function login() {
    localStorage.setItem("isLoggedIn", "true");
    console.log("Pengguna berhasil login.");
}

function logout() {
    localStorage.setItem("isLoggedIn", "false");
    console.log("Pengguna berhasil logout.");
}
