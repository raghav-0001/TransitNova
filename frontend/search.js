const API = "";


/* ============================================================
   STATE
   ============================================================ */

let fleet = {
    active: [],
    park1: [],
    park2: [],
    maintenance: [],
};


/* ============================================================
   HELPERS
   ============================================================ */

function escapeHtml(value = "") {
    return String(value)
        .replaceAll("&", "&amp;")
        .replaceAll("<", "&lt;")
        .replaceAll(">", "&gt;")
        .replaceAll('"', "&quot;")
        .replaceAll("'", "&#039;");
}


function updateIcons() {
    if (window.lucide) {
        window.lucide.createIcons();
    }
}


function setMessage(
    message,
    type = ""
) {
    const element =
        document.getElementById(
            "searchMessage"
        );

    if (!element) {
        return;
    }

    element.textContent = message;

    element.className =
        `search-message ${type}`;
}


async function fetchJson(path) {
    const response =
        await fetch(
            `${API}${path}`,
            {
                cache: "no-store",
            }
        );

    if (!response.ok) {
        throw new Error(
            `${path} returned ${response.status}`
        );
    }

    return response.json();
}


/* ============================================================
   CLOCK
   ============================================================ */

function updateClock() {
    const now = new Date();

    const timeElement =
        document.getElementById(
            "liveTime"
        );

    const dateElement =
        document.getElementById(
            "currentDate"
        );

    if (timeElement) {
        timeElement.textContent =
            now.toLocaleTimeString([], {
                hour: "2-digit",
                minute: "2-digit",
                second: "2-digit",
            });
    }

    if (dateElement) {
        dateElement.textContent =
            now.toLocaleDateString([], {
                month: "short",
                day: "numeric",
                year: "numeric",
            });
    }
}


/* ============================================================
   THEME
   ============================================================ */

function applyTheme(theme) {
    const isLight =
        theme === "light";

    document.body.classList.toggle(
        "light-theme",
        isLight
    );

    document
        .getElementById("lightTheme")
        ?.classList.toggle(
            "active",
            isLight
        );

    document
        .getElementById("darkTheme")
        ?.classList.toggle(
            "active",
            !isLight
        );

    localStorage.setItem(
        "transitNovaTheme",
        theme
    );
}


function initializeTheme() {
    const savedTheme =
        localStorage.getItem(
            "transitNovaTheme"
        );

    applyTheme(
        savedTheme === "light"
            ? "light"
            : "dark"
    );

    document
        .getElementById("lightTheme")
        ?.addEventListener(
            "click",
            () => applyTheme("light")
        );

    document
        .getElementById("darkTheme")
        ?.addEventListener(
            "click",
            () => applyTheme("dark")
        );
}


/* ============================================================
   BACKEND STATUS
   ============================================================ */

async function checkBackend() {
    const status =
        document.getElementById(
            "status"
        );

    if (!status) {
        return;
    }

    try {
        const data =
            await fetchJson(
                "/api/status"
            );

        if (
            data &&
            data.status === "ONLINE"
        ) {
            status.innerHTML = `
                <span class="status-dot"></span>
                <span>Backend Online</span>
            `;

            status.classList.add(
                "status-online"
            );

            status.classList.remove(
                "status-offline"
            );

            return;
        }

        throw new Error(
            "Unexpected backend status"
        );

    } catch (error) {
        console.error(
            "Backend status check failed:",
            error
        );

        status.innerHTML = `
            <span class="status-dot"></span>
            <span>Backend Offline</span>
        `;

        status.classList.remove(
            "status-online"
        );

        status.classList.add(
            "status-offline"
        );
    }
}


/* ============================================================
   LOAD REAL FLEET DATA
   ============================================================ */

async function loadFleet() {
    const data =
        await fetchJson(
            "/api/buses"
        );

    fleet = {
        active:
            Array.isArray(data.active)
                ? data.active
                : [],

        park1:
            Array.isArray(data.park1)
                ? data.park1
                : [],

        park2:
            Array.isArray(data.park2)
                ? data.park2
                : [],

        maintenance:
            Array.isArray(data.maintenance)
                ? data.maintenance
                : [],
    };
}


/* ============================================================
   FIND BUS
   ============================================================ */

function findBus(busNumber) {
    const target =
        busNumber
            .trim()
            .toLowerCase();

    if (!target) {
        return null;
    }

    const activeBus =
        fleet.active.find(
            (bus) =>
                String(
                    bus.busNumber
                ).toLowerCase() === target
        );

    if (activeBus) {
        return {
            bus: activeBus,
            category: "active",
        };
    }

    const ratnaparkBus =
        fleet.park1.find(
            (bus) =>
                String(
                    bus.busNumber
                ).toLowerCase() === target
        );

    if (ratnaparkBus) {
        return {
            bus: ratnaparkBus,
            category: "park1",
        };
    }

    const bodeBus =
        fleet.park2.find(
            (bus) =>
                String(
                    bus.busNumber
                ).toLowerCase() === target
        );

    if (bodeBus) {
        return {
            bus: bodeBus,
            category: "park2",
        };
    }

    const maintenanceBus =
        fleet.maintenance.find(
            (bus) =>
                String(
                    bus.busNumber
                ).toLowerCase() === target
        );

    if (maintenanceBus) {
        return {
            bus: maintenanceBus,
            category: "maintenance",
        };
    }

    return null;
}


/* ============================================================
   BUS STATUS
   ============================================================ */

function getBusStatus(
    category,
    bus
) {
    if (
        category === "maintenance"
    ) {
        return "Maintenance";
    }

    if (
        category === "park1" ||
        category === "park2"
    ) {
        return "At Buspark";
    }

    if (
        bus.available === false
    ) {
        return "Unavailable";
    }

    return "Running";
}


/* ============================================================
   DIRECTION
   ============================================================ */

function getDirectionText(bus) {
    if (
        bus.direction === "east"
    ) {
        return "Ratnapark → Bode";
    }

    if (
        bus.direction === "west"
    ) {
        return "Bode → Ratnapark";
    }

    return "Direction unavailable";
}


/* ============================================================
   PROGRESS
   ============================================================ */

function getProgressText(bus) {
    const progress =
        Number(
            bus.progress
        );

    if (
        !Number.isFinite(
            progress
        )
    ) {
        return "—";
    }

    return `${Math.round(
        progress * 100
    )}%`;
}


/* ============================================================
   MAINTENANCE ACTION
   ============================================================ */

async function performMaintenanceAction(
    busNumber,
    action
) {
    const restore =
        action === "restore";

    const endpoint =
        restore
            ? "/api/maintenance/restore"
            : "/api/maintenance";

    setMessage(
        restore
            ? "Restoring bus..."
            : "Sending bus to maintenance..."
    );

    try {
        const response =
            await fetch(
                `${API}${endpoint}`,
                {
                    method: "POST",

                    headers: {
                        "Content-Type":
                            "application/json",
                    },

                    body: JSON.stringify({
                        busNumber,
                    }),
                }
            );

        let data = {};

        try {
            data =
                await response.json();
        } catch {
            data = {};
        }

        if (!response.ok) {
            throw new Error(
                data.message ||
                "Bus operation failed."
            );
        }

        setMessage(
            data.message ||
            (
                restore
                    ? "Bus restored successfully."
                    : "Bus sent to maintenance successfully."
            ),
            "success"
        );

        await loadFleet();

        const result =
            findBus(busNumber);

        if (result) {
            renderBusResult(result);
        } else {
            renderEmptyResult();
        }

    } catch (error) {
        console.error(
            "Maintenance action failed:",
            error
        );

        setMessage(
            error.message ||
            "Unable to complete the operation.",
            "error"
        );
    }
}


/* ============================================================
   RESULT CARD
   ============================================================ */

function renderBusResult(result) {
    const container =
        document.getElementById(
            "searchResult"
        );

    if (
        !container ||
        !result
    ) {
        return;
    }

    const {
        bus,
        category,
    } = result;

    const status =
        getBusStatus(
            category,
            bus
        );

    const direction =
        getDirectionText(bus);

    const progress =
        getProgressText(bus);

    let locationLabel =
        "Current stop";

    let location =
        bus.currentStop ||
        "Unknown";

    if (
        category === "park1"
    ) {
        locationLabel =
            "Current location";

        location =
            "Ratnapark Buspark";
    }

    if (
        category === "park2"
    ) {
        locationLabel =
            "Current location";

        location =
            "Bode Buspark";
    }

    const action =
        category === "maintenance"
            ? `
                <button
                    type="button"
                    class="search-bus-action restore"
                    data-maintenance-action="restore"
                    data-bus-number="${escapeHtml(
                bus.busNumber
            )}">

                    <i data-lucide="rotate-ccw"></i>

                    <span>
                        Restore Bus
                    </span>

                </button>
            `
            : `
                <button
                    type="button"
                    class="search-bus-action maintenance"
                    data-maintenance-action="send"
                    data-bus-number="${escapeHtml(
                bus.busNumber
            )}">

                    <i data-lucide="wrench"></i>

                    <span>
                        Send to Maintenance
                    </span>

                </button>
            `;

    container.innerHTML = `
        <article
            class="search-result-card">

                <div class="bus-visual">
                    <img
                        src="images/${encodeURIComponent(bus.busNumber)}.jpg"
                        alt="Bus ${escapeHtml(bus.busNumber)}"
                        onerror="
                            if (this.src.endsWith('.jpg')) {
                                this.src = 'images/${encodeURIComponent(bus.busNumber)}.jpeg';
                            } else if (this.src.endsWith('.jpeg')) {
                                this.src = 'images/${encodeURIComponent(bus.busNumber)}.png';
                            } else {
                                this.style.display = 'none';
                                this.nextElementSibling.style.display = 'flex';
                            }
                        "
                    >

                    <i
                        data-lucide="bus-front"
                        style="display: none;"
                    ></i>
                </div>


            <div class="bus-info">

                <p class="bus-label">
                    ${category === "active"
            ? "Live bus"
            : "Bus record"
        }
                </p>


                <h2>
                    ${escapeHtml(
            bus.busNumber
        )}
                </h2>


                <p class="route-name">
                    Kathmandu - Bode Route
                </p>


                <div
                    class="bus-meta-grid">

                    <div>
                        <span>
                            ${locationLabel}
                        </span>

                        <strong>
                            ${escapeHtml(
            location
        )}
                        </strong>
                    </div>


                    <div>
                        <span>
                            Direction
                        </span>

                        <strong>
                            ${direction}
                        </strong>
                    </div>


                    <div>
                        <span>
                            Status
                        </span>

                        <strong>
                            ${status}
                        </strong>
                    </div>


                    <div>
                        <span>
                            Progress
                        </span>

                        <strong>
                            ${progress}
                        </strong>
                    </div>


                    <div>
                        <span>
                            Priority
                        </span>

                        <strong>
                            ${bus.priority ?? "—"}
                        </strong>
                    </div>


                    <div>
                        <span>
                            Available
                        </span>

                        <strong>
                            ${bus.available === true
            ? "Yes"
            : bus.available === false
                ? "No"
                : "—"
        }
                        </strong>
                    </div>

                </div>


                <div class="search-bus-actions">
                    ${action}
                </div>

            </div>

        </article>
    `;

    updateIcons();

    const button =
        container.querySelector(
            "[data-maintenance-action]"
        );

    if (!button) {
        return;
    }

    button.addEventListener(
        "click",
        async () => {
            const busNumber =
                button.dataset.busNumber;

            const action =
                button.dataset
                    .maintenanceAction;

            if (!busNumber) {
                return;
            }

            button.disabled = true;

            await performMaintenanceAction(
                busNumber,
                action === "restore"
                    ? "restore"
                    : "send"
            );
        }
    );
}


/* ============================================================
   EMPTY RESULT
   ============================================================ */

function renderEmptyResult() {
    const container =
        document.getElementById(
            "searchResult"
        );

    if (!container) {
        return;
    }

    container.innerHTML = `
        <div class="empty">

            <i data-lucide="bus-front"></i>

            <span>
                Enter a bus number to
                view its live details.
            </span>

        </div>
    `;

    updateIcons();
}


/* ============================================================
   NOT FOUND
   ============================================================ */

function renderNotFound(busNumber) {
    const container =
        document.getElementById(
            "searchResult"
        );

    if (!container) {
        return;
    }

    container.innerHTML = `
        <div class="empty">

            <i data-lucide="search-x"></i>

            <span>
                No bus found with number
                <strong>
                    ${escapeHtml(
        busNumber
    )}
                </strong>.
            </span>

        </div>
    `;

    updateIcons();
}


/* ============================================================
   SEARCH
   ============================================================ */

async function searchBus(busNumber) {
    const query =
        busNumber.trim();

    if (!query) {
        setMessage(
            "Please enter a bus number.",
            "error"
        );

        renderEmptyResult();

        return;
    }

    setMessage(
        "Searching live buses..."
    );

    try {
        await loadFleet();

        const result =
            findBus(query);

        if (!result) {
            setMessage(
                `No bus found with number ${query}.`,
                "error"
            );

            renderNotFound(query);

            return;
        }

        renderBusResult(result);

        setMessage(
            "Live bus information loaded.",
            "success"
        );

    } catch (error) {
        console.error(
            "Bus search failed:",
            error
        );

        setMessage(
            "Unable to connect to the TransitNova backend.",
            "error"
        );

        const container =
            document.getElementById(
                "searchResult"
            );

        if (container) {
            container.innerHTML = `
                <div class="empty">

                    <i data-lucide="wifi-off"></i>

                    <span>
                        Live bus data is
                        currently unavailable.
                    </span>

                </div>
            `;

            updateIcons();
        }
    }
}


/* ============================================================
   FORM
   ============================================================ */

function initializeSearch() {
    const form =
        document.getElementById(
            "busSearchForm"
        );

    const input =
        document.getElementById(
            "busID"
        );

    if (
        !form ||
        !input
    ) {
        return;
    }

    form.addEventListener(
        "submit",
        async (event) => {
            event.preventDefault();

            await searchBus(
                input.value
            );
        }
    );
}


/* ============================================================
   INITIALIZATION
   ============================================================ */

function initializeSearchPage() {
    initializeTheme();

    updateClock();

    updateIcons();

    renderEmptyResult();

    initializeSearch();

    checkBackend();

    setInterval(
        updateClock,
        1000
    );

    setInterval(
        checkBackend,
        5000
    );
}


document.addEventListener(
    "DOMContentLoaded",
    initializeSearchPage
);