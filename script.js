document.addEventListener("DOMContentLoaded", () => {
  const nameInput = document.getElementById("nameInput");
  const statusSelect = document.getElementById("statusSelect");
  const addBtn = document.getElementById("addBtn");
  const attendanceList = document.getElementById("attendanceList");

  addBtn.addEventListener("click", () => {
    const name = nameInput.value.trim();
    const status = statusSelect.value;

    if (!name) {
      alert("Please enter a name.");
      return;
    }

    const li = document.createElement("li");

    const nameSpan = document.createElement("span");
    nameSpan.textContent = name;

    const statusSpan = document.createElement("span");
    statusSpan.className = "status";
    statusSpan.textContent = status;

    li.appendChild(nameSpan);
    li.appendChild(statusSpan);

    attendanceList.appendChild(li);

    // Clear input
    nameInput.value = "";
    statusSelect.selectedIndex = 0;
  });
});
