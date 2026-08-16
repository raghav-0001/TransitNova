const API = "";

/* ============================================================
   GLOBAL STATE
   ============================================================ */

let backendBuses = {
    active: [],
    park1: [],
    park2: [],
    maintenance: [],
};

let liveBusPolling = null;
let popoverHideTimer = null;
let lastAnimationTime = 0;

let lastPark1Signature = "";
let lastPark2Signature = "";
let lastMaintenanceSignature = "";

let maintenanceRequestInProgress = false;

/*
 * Forces queue/maintenance DOM refresh after
 * an operation changes backend state.
 */
let forceQueueRender = false;

const visualBusStates = new Map();


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


function setText(id, value) {
    const element =
        document.getElementById(id);

    if (element) {
        element.textContent = value;
    }
}


function updateIcons() {
    if (window.lucide) {
        window.lucide.createIcons();
    }
}


function clamp(
    value,
    minimum,
    maximum
) {
    return Math.min(
        Math.max(value, minimum),
        maximum
    );
}


async function fetchJson(path) {
    const response =
        await fetch(`${API}${path}`);

    if (!response.ok) {
        throw new Error(
            `Request failed: ${path} (${response.status})`
        );
    }

    return response.json();
}


/* ============================================================
   THEME
   ============================================================ */

function applyTheme(theme) {
    const light =
        theme === "light";

    document.body.classList.toggle(
        "light-theme",
        light
    );

    document
        .getElementById("lightTheme")
        ?.classList.toggle(
            "active",
            light
        );

    document
        .getElementById("darkTheme")
        ?.classList.toggle(
            "active",
            !light
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
   CLOCK
   ============================================================ */

function updateClock() {
    const now =
        new Date();

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
            now.toLocaleTimeString(
                [],
                {
                    hour: "2-digit",
                    minute: "2-digit",
                    second: "2-digit",
                }
            );
    }

    if (dateElement) {
        dateElement.textContent =
            now.toLocaleDateString(
                [],
                {
                    month: "short",
                    day: "numeric",
                    year: "numeric",
                }
            );
    }
}


/* ============================================================
   BACKEND STATUS
   ============================================================ */

function setStatus(isOnline) {
    const status =
        document.getElementById(
            "status"
        );

    if (!status) {
        return;
    }

    status.classList.toggle(
        "status-online",
        isOnline
    );

    status.classList.toggle(
        "status-offline",
        !isOnline
    );

    status.innerHTML = `
        <span class="status-dot"></span>
        <span>
            ${isOnline
            ? "Backend Online"
            : "Backend Offline"
        }
        </span>
    `;
}


async function checkBackend() {
    try {
        const data =
            await fetchJson(
                "/api/status"
            );

        setStatus(
            data?.status === "ONLINE"
        );

        return (
            data?.status === "ONLINE"
        );

    } catch (error) {
        console.error(
            "Backend status check failed:",
            error
        );

        setStatus(false);

        return false;
    }
}


/* ============================================================
   DASHBOARD STATISTICS
   ============================================================ */

function renderDashboard() {
    const active =
        backendBuses.active;

    const park1 =
        backendBuses.park1;

    const park2 =
        backendBuses.park2;

    const maintenance =
        backendBuses.maintenance;

    const totalBuses =
        active.length +
        park1.length +
        park2.length +
        maintenance.length;

    const totalStops =
        document.querySelectorAll(
            "#routeMonitor .stop"
        ).length;

    setText(
        "totalBuses",
        totalBuses
    );

    setText(
        "runningBuses",
        active.length
    );

    setText(
        "ratnaparkCount",
        park1.length
    );

    setText(
        "bodeCount",
        park2.length
    );

    setText(
        "maintenanceCount",
        maintenance.length
    );

    setText(
        "totalStops",
        totalStops
    );
}


/* ============================================================
   BUS PARK QUEUES
   ============================================================ */

function renderParkQueue(
    containerId,
    buses,
    colorClass,
    terminalName
) {
    const container =
        document.getElementById(
            containerId
        );

    if (!container) {
        return;
    }

    const isPark1 =
        containerId ===
        "ratnaparkQueue";

    const signature =
        buses
            .map(
                (bus) =>
                    `${bus.id}:${bus.busNumber}:${bus.priority}:${bus.isMaintenance}`
            )
            .join("|");

    /*
     * Do not rebuild the DOM when
     * the queue has not changed,
     * unless an operation explicitly
     * requested a refresh.
     */
    if (
        !forceQueueRender &&
        isPark1 &&
        signature ===
        lastPark1Signature
    ) {
        return;
    }

    if (
        !forceQueueRender &&
        !isPark1 &&
        signature ===
        lastPark2Signature
    ) {
        return;
    }

    if (isPark1) {
        lastPark1Signature =
            signature;
    } else {
        lastPark2Signature =
            signature;
    }

    if (!buses.length) {
        container.innerHTML = `
            <div class="empty">
                No buses waiting at
                ${escapeHtml(
            terminalName
        )}.
            </div>
        `;

        return;
    }

    container.innerHTML =
        buses
            .map(
                (bus) => `
                <div
                    class="queue-row ${colorClass}"
                    data-bus-id="${escapeHtml(
                    bus.id
                )}">

                    <i data-lucide="bus-front"></i>

                    <b>
                        ${escapeHtml(
                    bus.busNumber
                )}
                    </b>

                    <span>
                        Idle
                    </span>

                    <button
                        type="button"
                        class="queue-maintenance-button"
                        data-bus-number="${escapeHtml(
                    bus.busNumber
                )}"
                        title="Send to maintenance"
                        aria-label="Send bus ${escapeHtml(
                    bus.busNumber
                )} to maintenance">

                        <i data-lucide="wrench"></i>

                    </button>

                </div>
            `
            )
            .join("");

    updateIcons();
}


function renderBusParks() {
    renderParkQueue(
        "ratnaparkQueue",
        backendBuses.park1,
        "yellow-row",
        "Ratnapark Bus Park"
    );

    renderParkQueue(
        "bodeQueue",
        backendBuses.park2,
        "purple-row",
        "Bode Bus Park"
    );

    setText(
        "ratnaparkTotal",
        backendBuses.park1.length
    );

    setText(
        "bodeTotal",
        backendBuses.park2.length
    );
}


/* ============================================================
   MAINTENANCE
   ============================================================ */

function renderMaintenance() {
    const container =
        document.getElementById(
            "maintenanceQueue"
        );

    if (!container) {
        return;
    }

    const buses =
        backendBuses.maintenance;

    const signature =
        buses
            .map(
                (bus) =>
                    `${bus.id}:${bus.busNumber}:${bus.priority}:${bus.isMaintenance}`
            )
            .join("|");

    /*
     * Do not rebuild the maintenance
     * list when nothing changed,
     * unless an operation explicitly
     * requested a refresh.
     */
    if (
        !forceQueueRender &&
        signature ===
        lastMaintenanceSignature
    ) {
        return;
    }

    lastMaintenanceSignature =
        signature;

    if (!buses.length) {
        container.innerHTML = `
            <div class="empty">
                No buses currently under maintenance.
            </div>
        `;

        setText(
            "maintenanceTotal",
            0
        );

        return;
    }

    container.innerHTML =
        buses
            .map(
                (bus) => `
                <div
                    class="maintenance-row"
                    data-bus-id="${escapeHtml(
                    bus.id
                )}">

                    <i data-lucide="bus-front"></i>

                    <div>
                        <b>
                            ${escapeHtml(
                    bus.busNumber
                )}
                        </b>

                        <span class="maintenance-tag">
                            Maintenance
                        </span>
                    </div>

                    <button
                        type="button"
                        class="maintenance-restore-button"
                        data-bus-number="${escapeHtml(
                    bus.busNumber
                )}"
                        title="Restore bus"
                        aria-label="Restore bus ${escapeHtml(
                    bus.busNumber
                )}">

                        <i data-lucide="rotate-ccw"></i>

                    </button>

                </div>
            `
            )
            .join("");

    setText(
        "maintenanceTotal",
        buses.length
    );

    updateIcons();
}


/* ============================================================
   ROUTE POSITION
   ============================================================ */

function getRoutePosition(bus) {
    const stopIndex =
        Number(
            bus.currentStopIndex ?? 0
        );

    const progress =
        clamp(
            Number(
                bus.progress ?? 0
            ),
            0,
            1
        );

    /*
     * There are 18 stops,
     * therefore 17 transitions.
     */

    const totalTransitions =
        17;

    let routePosition;

    if (
        bus.direction === "west"
    ) {
        routePosition =
            stopIndex -
            progress;
    } else {
        routePosition =
            stopIndex +
            progress;
    }

    return clamp(
        (
            routePosition /
            totalTransitions
        ) * 100,
        0,
        100
    );
}


/* ============================================================
   BUS ELEMENT
   ============================================================ */

function createBusElement(bus) {
    const node =
        document.createElement(
            "div"
        );

    const direction =
        bus.direction === "west"
            ? "west"
            : "east";

    node.className =
        `bus ${direction}`;

    node.dataset.busNumber =
        bus.busNumber;

    node.style.top =
        direction === "east"
            ? "54px"
            : "122px";

    node.innerHTML = `
        ${direction === "west"
            ? `<i data-lucide="bus-front"></i>`
            : ""
        }

        <span>
            ${escapeHtml(
            bus.busNumber
        )}
        </span>

        ${direction === "east"
            ? `<i data-lucide="bus-front"></i>`
            : ""
        }
    `;

    node.addEventListener(
        "mouseenter",
        () => {
            const latest =
                backendBuses.active.find(
                    (item) =>
                        String(item.id) ===
                        String(bus.id)
                );

            showBusTooltip(
                node,
                latest || bus
            );
        }
    );

    node.addEventListener(
        "mouseleave",
        hideBusTooltip
    );

    return node;
}


/* ============================================================
   BUS TARGETS
   ============================================================ */

function updateBusTargets() {
    const activeBuses =
        backendBuses.active;

    const activeIds =
        new Set(
            activeBuses.map(
                (bus) =>
                    String(bus.id)
            )
        );

    /*
     * Remove buses that have
     * left the active route.
     */

    for (
        const [id, state]
        of visualBusStates
    ) {
        if (
            !activeIds.has(id)
        ) {
            state.node.remove();

            visualBusStates.delete(
                id
            );
        }
    }

    /*
     * Add new buses or update
     * existing buses.
     */

    activeBuses.forEach(
        (bus) => {
            const id =
                String(bus.id);

            const target =
                getRoutePosition(
                    bus
                );

            let state =
                visualBusStates.get(
                    id
                );

            /*
             * New bus.
             */

            if (!state) {
                const node =
                    createBusElement(
                        bus
                    );

                const layer =
                    document.getElementById(
                        "liveBuses"
                    );

                if (!layer) {
                    return;
                }

                layer.appendChild(
                    node
                );

                state = {
                    node,
                    position: target,
                    target,
                    direction:
                        bus.direction,
                };

                visualBusStates.set(
                    id,
                    state
                );

                node.style.left =
                    `${target}%`;

                updateIcons();

                return;
            }

            /*
             * Update target from
             * backend.
             */

            state.target =
                target;

            /*
             * Update direction when
             * a bus changes direction.
             */

            if (
                state.direction !==
                bus.direction
            ) {
                state.direction =
                    bus.direction;

                state.node.className =
                    `bus ${bus.direction}`;

                state.node.style.top =
                    bus.direction ===
                        "east"
                        ? "54px"
                        : "122px";

                state.node.innerHTML = `
                    ${bus.direction ===
                        "west"
                        ? `<i data-lucide="bus-front"></i>`
                        : ""
                    }

                    <span>
                        ${escapeHtml(
                        bus.busNumber
                    )}
                    </span>

                    ${bus.direction ===
                        "east"
                        ? `<i data-lucide="bus-front"></i>`
                        : ""
                    }
                `;

                updateIcons();
            }
        }
    );
}


/* ============================================================
   30 FPS VISUAL BUS ANIMATION
   ============================================================ */

function animateLiveBuses(
    timestamp
) {
    if (
        timestamp -
        lastAnimationTime >=
        33
    ) {
        lastAnimationTime =
            timestamp;

        for (
            const state
            of visualBusStates.values()
        ) {
            const difference =
                state.target -
                state.position;

            state.position +=
                difference * 0.12;

            if (
                Math.abs(
                    difference
                ) < 0.01
            ) {
                state.position =
                    state.target;
            }

            state.node.style.left =
                `${state.position}%`;
        }
    }

    requestAnimationFrame(
        animateLiveBuses
    );
}


/* ============================================================
   BUS TOOLTIP
   ============================================================ */

function showBusTooltip(
    node,
    bus
) {
    let tooltip =
        document.getElementById(
            "busLiveTooltip"
        );

    if (!tooltip) {
        tooltip =
            document.createElement(
                "div"
            );

        tooltip.id =
            "busLiveTooltip";

        tooltip.className =
            "bus-live-tooltip";

        document.body.appendChild(
            tooltip
        );
    }

    const direction =
        bus.direction === "west"
            ? "Bode → Ratnapark"
            : "Ratnapark → Bode";

    const progress =
        Math.round(
            Number(
                bus.progress ?? 0
            ) * 100
        );

    tooltip.innerHTML = `
        <strong>
            🚌 Bus ${escapeHtml(
        bus.busNumber
    )}
        </strong>

        <div class="tooltip-row">
            <span>Direction</span>
            <b>${direction}</b>
        </div>

        <div class="tooltip-row">
            <span>Current stop</span>
            <b>
                ${escapeHtml(
        bus.currentStop ||
        "Unknown"
    )}
            </b>
        </div>

        <div class="tooltip-row">
            <span>Progress</span>
            <b>${progress}%</b>
        </div>

        <div class="tooltip-row">
            <span>Status</span>
            <b>Running</b>
        </div>
    `;

    const rect =
        node.getBoundingClientRect();

    let left =
        rect.left +
        rect.width / 2 -
        110;

    let top =
        rect.bottom + 12;

    left =
        Math.max(
            10,
            Math.min(
                left,
                window.innerWidth -
                230
            )
        );

    if (
        top + 150 >
        window.innerHeight
    ) {
        top =
            rect.top - 160;
    }

    tooltip.style.left =
        `${left}px`;

    tooltip.style.top =
        `${top}px`;

    tooltip.classList.add(
        "visible"
    );
}


function hideBusTooltip() {
    document
        .getElementById(
            "busLiveTooltip"
        )
        ?.classList.remove(
            "visible"
        );
}


/* ============================================================
   STOP POPOVER
   ============================================================ */

function positionStopPopover(
    map,
    popover,
    stop
) {
    const stopRect =
        stop.getBoundingClientRect();

    const popoverWidth =
        popover.offsetWidth;

    const estimatedHeight =
        popover.offsetHeight || 180;

    const margin = 10;

    /*
     * Position the popover directly
     * below the center of the stop.
     */
    let left =
        stopRect.left +
        stopRect.width / 2;

    let top =
        stopRect.bottom + 12;

    /*
     * Keep the popover inside the viewport.
     * Do NOT subtract half the width here
     * because CSS translateX(-50%) handles it.
     */
    const halfWidth =
        popoverWidth / 2;

    left = clamp(
        left,
        margin + halfWidth,
        window.innerWidth -
        margin -
        halfWidth
    );

    /*
     * If there isn't enough room below,
     * show it above the stop.
     */
    if (
        top +
        estimatedHeight >
        window.innerHeight -
        margin
    ) {
        top =
            stopRect.top -
            estimatedHeight -
            12;
    }

    top =
        Math.max(
            margin,
            top
        );

    popover.style.left =
        `${left}px`;

    popover.style.top =
        `${top}px`;

    /*
     * Keep the connector pointing
     * exactly at the hovered stop.
     */
    const connectorLeft =
        stopRect.left +
        stopRect.width / 2 -
        (left - popoverWidth / 2);

    popover.style.setProperty(
        "--connector-left",
        `${connectorLeft}px`
    );

    popover.classList.add(
        "visible"
    );
}


function showStopPopover(
    stop
) {
    clearTimeout(
        popoverHideTimer
    );

    const map =
        document.getElementById(
            "routeMonitor"
        );

    const popover =
        document.getElementById(
            "stopPopover"
        );

    const title =
        document.getElementById(
            "popoverStop"
        );

    const summary =
        document.getElementById(
            "popoverSummary"
        );

    const busList =
        document.getElementById(
            "popoverBusList"
        );

    if (
        !map ||
        !popover ||
        !title ||
        !summary ||
        !busList
    ) {
        return;
    }

    const stopName =
        stop.dataset.stop;

    const stopIndex =
        Number(
            stop.dataset.index
        );

    title.textContent =
        stopName;

    /* ========================================================
       BUSPARKS
       ======================================================== */

    if (
        stopIndex === 0 ||
        stopIndex === 17
    ) {
        const count =
            stopIndex === 0
                ? backendBuses.park1.length
                : backendBuses.park2.length;

        summary.innerHTML = `
            <div class="popover-total">
                <span>
                    Buses currently here
                </span>

                <b>
                    <i data-lucide="bus"></i>
                    ${count}
                </b>
            </div>
        `;

        busList.innerHTML =
            "";

        updateIcons();

        positionStopPopover(
            map,
            popover,
            stop
        );

        return;
    }

    /* ========================================================
       NORMAL STOPS
       ======================================================== */

    const busesAtStop =
        backendBuses.active.filter(
            (bus) =>
                Number(
                    bus.currentStopIndex
                ) === stopIndex
        );

    const eastBuses =
        busesAtStop.filter(
            (bus) =>
                bus.direction ===
                "east"
        );

    const westBuses =
        busesAtStop.filter(
            (bus) =>
                bus.direction ===
                "west"
        );

    summary.innerHTML = `
        <div class="popover-direction-row">

            <div>
                <span>To Bode</span>

                <b class="text-green">
                    <i data-lucide="bus"></i>
                    ${eastBuses.length}
                </b>
            </div>

            <div>
                <span>To Ratnapark</span>

                <b class="text-blue">
                    <i data-lucide="bus"></i>
                    ${westBuses.length}
                </b>
            </div>

        </div>

        <div class="popover-total">

            <span>
                Total buses here
            </span>

            <b>
                ${busesAtStop.length}
                <i data-lucide="users-round"></i>
            </b>

        </div>
    `;

    if (
        !busesAtStop.length
    ) {
        busList.innerHTML =
            "No buses currently at this stop.";
    } else {
        busList.innerHTML =
            busesAtStop
                .map(
                    (bus) => {
                        const direction =
                            bus.direction ===
                                "east"
                                ? "→ Bode"
                                : "→ Ratnapark";

                        return `
                            <div class="popover-bus">

                                <span>
                                    🚌
                                    ${escapeHtml(
                            bus.busNumber
                        )}
                                </span>

                                <b>
                                    ${direction}
                                </b>

                            </div>
                        `;
                    }
                )
                .join("");
    }

    updateIcons();

    positionStopPopover(
        map,
        popover,
        stop
    );
}


function hideStopPopover() {
    const popover =
        document.getElementById(
            "stopPopover"
        );

    if (!popover) {
        return;
    }

    popoverHideTimer =
        setTimeout(
            () => {
                popover.classList.remove(
                    "visible"
                );
            },
            100
        );
}


function bindStopInteractions() {
    document
        .querySelectorAll(
            "#routeMonitor .stop"
        )
        .forEach(
            (stop) => {
                stop.addEventListener(
                    "mouseenter",
                    () =>
                        showStopPopover(
                            stop
                        )
                );

                stop.addEventListener(
                    "mouseleave",
                    hideStopPopover
                );

                stop.addEventListener(
                    "focus",
                    () =>
                        showStopPopover(
                            stop
                        )
                );

                stop.addEventListener(
                    "blur",
                    hideStopPopover
                );
            }
        );
}


/* ============================================================
   MAINTENANCE ACTIONS
   ============================================================ */

function initializeMaintenanceButtons() {
    document.addEventListener(
        "pointerdown",
        async (event) => {

            /*
             * Only accept the primary
             * mouse button.
             */

            if (
                event.pointerType ===
                "mouse" &&
                event.button !== 0
            ) {
                return;
            }

            const button =
                event.target.closest(
                    ".queue-maintenance-button, .maintenance-restore-button"
                );

            if (!button) {
                return;
            }

            event.preventDefault();
            event.stopPropagation();

            if (
                maintenanceRequestInProgress ||
                button.disabled
            ) {
                return;
            }

            const busNumber =
                button.dataset.busNumber;

            if (!busNumber) {
                return;
            }

            const isRestore =
                button.classList.contains(
                    "maintenance-restore-button"
                );

            maintenanceRequestInProgress =
                true;

            button.disabled =
                true;

            try {
                const endpoint =
                    isRestore
                        ? "/api/maintenance/restore"
                        : "/api/maintenance";

                const response =
                    await fetch(
                        `${API}${endpoint}`,
                        {
                            method: "POST",

                            headers: {
                                "Content-Type":
                                    "application/json",
                            },

                            body:
                                JSON.stringify({
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
                        "Maintenance operation failed."
                    );
                }

                /*
                 * Backend state changed.
                 *
                 * Force the queue and maintenance
                 * containers to rebuild, including
                 * the important empty-state case.
                 */
                forceQueueRender = true;

                lastPark1Signature = "";
                lastPark2Signature = "";
                lastMaintenanceSignature = "";

                await loadLiveBusData();

                forceQueueRender = false;

            } catch (error) {
                console.error(
                    "Maintenance operation failed:",
                    error
                );

                alert(
                    error.message ||
                    "Unable to complete operation."
                );

                button.disabled =
                    false;

                forceQueueRender = false;

            } finally {
                maintenanceRequestInProgress =
                    false;
            }
        },
        true
    );
}


/* ============================================================
   LIVE BUS DATA
   ============================================================ */

async function loadLiveBusData() {
    try {
        const data =
            await fetchJson(
                "/api/buses"
            );

        backendBuses = {
            active:
                Array.isArray(
                    data.active
                )
                    ? data.active
                    : [],

            park1:
                Array.isArray(
                    data.park1
                )
                    ? data.park1
                    : [],

            park2:
                Array.isArray(
                    data.park2
                )
                    ? data.park2
                    : [],

            maintenance:
                Array.isArray(
                    data.maintenance
                )
                    ? data.maintenance
                    : [],
        };

        /*
         * Update dashboard state.
         */

        renderDashboard();

        renderBusParks();

        renderMaintenance();

        updateBusTargets();

        setStatus(true);

    } catch (error) {
        console.error(
            "Failed to load live bus data:",
            error
        );

        setStatus(false);
    }
}


/* ============================================================
   LIVE BUS POLLING
   ============================================================ */

function startLiveBusPolling() {
    if (liveBusPolling) {
        clearInterval(
            liveBusPolling
        );
    }

    /*
     * Backend state is sampled every
     * 250 ms.
     *
     * Visual movement is handled
     * independently at ~30 FPS.
     */

    liveBusPolling =
        setInterval(
            loadLiveBusData,
            250
        );
}


/* ============================================================
   INITIALIZATION
   ============================================================ */

async function initializeDashboard() {
    initializeTheme();

    updateClock();

    updateIcons();

    initializeMaintenanceButtons();

    bindStopInteractions();

    /*
     * Load initial backend state.
     */

    await loadLiveBusData();

    /*
     * Clock.
     */

    setInterval(
        updateClock,
        1000
    );

    /*
     * Backend connectivity.
     */

    setInterval(
        checkBackend,
        5000
    );

    /*
     * Live bus synchronization.
     */

    startLiveBusPolling();

    /*
     * Smooth bus animation.
     */

    requestAnimationFrame(
        animateLiveBuses
    );
}


document.addEventListener(
    "DOMContentLoaded",
    initializeDashboard
);