// ======================================================
// TransitNova
// Journey Planner
// ======================================================

const API = "";


// ======================================================
// Icons
// ======================================================

function updateIcons() {
    if (window.lucide) {
        window.lucide.createIcons();
    }
}


// ======================================================
// Clock
// ======================================================

function updateClock() {
    const now = new Date();

    const timeElement =
        document.getElementById("liveTime");

    const dateElement =
        document.getElementById("currentDate");

    if (timeElement) {
        timeElement.textContent =
            now.toLocaleTimeString([], {
                hour: "2-digit",
                minute: "2-digit",
                second: "2-digit"
            });
    }

    if (dateElement) {
        dateElement.textContent =
            now.toLocaleDateString([], {
                month: "short",
                day: "numeric",
                year: "numeric"
            });
    }
}


// ======================================================
// Theme
// ======================================================

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


// ======================================================
// Backend Status
// ======================================================

async function checkBackend() {
    const status =
        document.getElementById("status");

    if (!status) {
        return;
    }

    try {
        const response =
            await fetch(
                `${API}/api/status`,
                {
                    cache: "no-store"
                }
            );

        if (!response.ok) {
            throw new Error(
                "Backend unavailable"
            );
        }

        const data =
            await response.json();

        if (
            data.status !== "ONLINE"
        ) {
            throw new Error(
                "Backend returned unexpected status"
            );
        }


        status.innerHTML = `
            <span class="status-dot"></span>
            <span>Backend Online</span>
        `;

        status.classList.remove(
            "status-offline"
        );

        status.classList.add(
            "status-online"
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


// ======================================================
// Swap Stops
// ======================================================

function swapStops() {
    const fromStop =
        document.getElementById("fromStop");

    const toStop =
        document.getElementById("toStop");

    if (!fromStop || !toStop) {
        return;
    }

    const fromValue =
        fromStop.value;

    const toValue =
        toStop.value;

    fromStop.value = toValue;
    toStop.value = fromValue;
}


// ======================================================
// Format Relative Time
// ======================================================

function formatRelativeMinutes(minutes) {
    const value =
        Math.max(
            0,
            Math.round(
                Number(minutes) || 0
            )
        );

    if (value === 0) {
        return "Now";
    }

    return `${value} min`;
}


// ======================================================
// Format Final Destination Time
//
// ONLY the final destination uses a clock time.
// Bus arrival remains relative.
// ======================================================

function formatClockTime(date) {
    return date.toLocaleTimeString([], {
        hour: "2-digit",
        minute: "2-digit"
    });
}


// ======================================================
// HTML Escape
// ======================================================

function escapeHtml(value) {
    return String(value ?? "")
        .replaceAll("&", "&amp;")
        .replaceAll("<", "&lt;")
        .replaceAll(">", "&gt;")
        .replaceAll('"', "&quot;")
        .replaceAll("'", "&#039;");
}


// ======================================================
// Journey API
// ======================================================

async function fetchJourney(from, to) {
    const response =
        await fetch(
            `${API}/api/journey`,
            {
                method: "POST",

                headers: {
                    "Content-Type":
                        "application/json"
                },

                body: JSON.stringify({
                    from,
                    to
                })
            }
        );


    let data;

    try {
        data =
            await response.json();

    } catch {
        throw new Error(
            "Invalid response from backend."
        );
    }


    if (!response.ok) {
        throw new Error(
            data?.message ||
            "Unable to calculate this journey."
        );
    }


    return data;
}


// ======================================================
// Render Available Buses
//
// Maximum: 3
//
// Bus #1 = Recommended
// Bus #2 = Normal
// Bus #3 = Normal
// ======================================================

// ======================================================
// Render Available Buses
//
// Maximum: 3
//
// Bus #1 = Recommended
// Bus #2 = Normal
// Bus #3 = Normal
//
// Each bus now shows:
// - Current stop
// - Arrival time
// - Travel time
// - Total journey time
// - Estimated destination arrival
// ======================================================

function renderAvailableBuses(
    buses,
    travelMinutes
) {
    const container =
        document.getElementById(
            "availableBuses"
        );

    if (!container) {
        return;
    }


    container.innerHTML = "";


    const journeyTravelMinutes =
        Number(travelMinutes) || 0;


    buses
        .slice(0, 3)
        .forEach(
            (bus, index) => {

                const recommended =
                    index === 0;


                const arrivalMinutes =
                    Number(
                        bus.arrivalMinutes
                    ) || 0;


                const totalMinutes =
                    arrivalMinutes +
                    journeyTravelMinutes;


                const destinationArrival =
                    new Date(
                        Date.now() +
                        totalMinutes *
                        60 *
                        1000
                    );


                const card =
                    document.createElement(
                        "article"
                    );


                card.className =
                    "available-bus-card panel" +
                    (
                        recommended
                            ? " recommended-bus-card"
                            : ""
                    );


                card.innerHTML = `

                    ${recommended
                        ? `
                                <div class="recommended-label">

                                    <i data-lucide="sparkles"></i>

                                    <span>
                                        Recommended
                                    </span>

                                </div>
                            `
                        : ""
                    }


                    <div class="available-bus-content">

                        <!-- Bus icon -->
                        <div class="available-bus-icon">

                            <i data-lucide="bus-front"></i>

                        </div>


                        <!-- Bus information -->
                        <div class="available-bus-info">

                            <strong>
                                ${escapeHtml(
                        bus.busNumber
                    )}
                            </strong>

                            <span>
                                Currently at
                                ${escapeHtml(
                        bus.currentStop ||
                        "Unknown"
                    )}
                            </span>

                        </div>


                        <!-- Arrival -->
                        <div class="available-bus-arrival">

                            <small>
                                Arrives in
                            </small>

                            <strong>
                                ${escapeHtml(
                        formatRelativeMinutes(
                            arrivalMinutes
                        )
                    )}
                            </strong>

                        </div>

                    </div>


                    <!-- ======================================
                         BUS JOURNEY DETAILS
                         ====================================== -->

                    <div class="bus-journey-details">


                        <!-- Travel time -->
                        <div class="bus-time-detail">

                            <i data-lucide="timer"></i>

                            <div>

                                <small>
                                    Travel
                                </small>

                                <strong>
                                    ${escapeHtml(
                        journeyTravelMinutes
                    )} min
                                </strong>

                            </div>

                        </div>


                        <!-- Total journey -->
                        <div class="bus-time-detail">

                            <i data-lucide="route"></i>

                            <div>

                                <small>
                                    Total
                                </small>

                                <strong>
                                    ${escapeHtml(
                        totalMinutes
                    )} min
                                </strong>

                            </div>

                        </div>


                        <!-- Destination arrival -->
                        <div class="bus-time-detail">

                            <i data-lucide="flag"></i>

                            <div>

                                <small>
                                    Destination
                                </small>

                                <strong>
                                    ${escapeHtml(
                        formatClockTime(
                            destinationArrival
                        )
                    )}
                                </strong>

                            </div>

                        </div>


                    </div>

                `;


                container.appendChild(
                    card
                );
            }
        );


    updateIcons();
}


// ======================================================
// Display Journey
// ======================================================

function displayJourney(data) {
    const resultSection =
        document.getElementById(
            "journeyResult"
        );


    const buses =
        Array.isArray(data.buses)
            ? data.buses
            : [];


    // --------------------------------------------------
    // No available buses
    // --------------------------------------------------

    if (
        !data.found ||
        buses.length === 0
    ) {
        if (resultSection) {
            resultSection.hidden = true;
        }

        throw new Error(
            "No available buses were found for this journey right now."
        );
    }


    // --------------------------------------------------
    // Top 3 buses
    // --------------------------------------------------

    const availableBuses =
        buses.slice(0, 3);

    const travelMinutes =
        Number(
            data.travelMinutes
        ) || 0;

    renderAvailableBuses(
        availableBuses,
        travelMinutes
    );

    // --------------------------------------------------
    // Recommended bus
    // --------------------------------------------------

    const recommendedBus =
        availableBuses[0];


    const arrivalMinutes =
        Number(
            recommendedBus.arrivalMinutes
        ) || 0;


    // --------------------------------------------------
    // Travel time
    // --------------------------------------------------




    // --------------------------------------------------
    // Total time
    //
    // Wait for bus
    // +
    // Travel to destination
    // --------------------------------------------------

    const totalMinutes =
        arrivalMinutes +
        travelMinutes;


    // --------------------------------------------------
    // Final destination arrival
    //
    // Current real time
    // +
    // total journey duration
    // --------------------------------------------------

    const destinationArrival =
        new Date(
            Date.now() +
            totalMinutes *
            60 *
            1000
        );


    // --------------------------------------------------
    // DOM elements
    // --------------------------------------------------

    const resultFrom =
        document.getElementById(
            "resultFromStop"
        );

    const resultTo =
        document.getElementById(
            "resultToStop"
        );

    const stopCount =
        document.getElementById(
            "stopCount"
        );

    const travelTime =
        document.getElementById(
            "travelTime"
        );

    const totalTime =
        document.getElementById(
            "totalTime"
        );

    const destinationTime =
        document.getElementById(
            "destinationTime"
        );


    // --------------------------------------------------
    // Boarding stop
    // --------------------------------------------------

    if (resultFrom) {
        resultFrom.textContent =
            data.pickup ||
            "";
    }


    // --------------------------------------------------
    // Destination
    // --------------------------------------------------

    if (resultTo) {
        resultTo.textContent =
            data.destination ||
            "";
    }


    // --------------------------------------------------
    // Stop count
    // --------------------------------------------------

    if (stopCount) {
        const count =
            Number(
                data.stopCount
            ) || 0;

        stopCount.textContent =
            `${count} ${count === 1
                ? "stop"
                : "stops"
            }`;
    }


    // --------------------------------------------------
    // Travel time
    // --------------------------------------------------

    if (travelTime) {
        travelTime.textContent =
            `${travelMinutes} min`;
    }


    // --------------------------------------------------
    // Total time
    // --------------------------------------------------

    if (totalTime) {
        totalTime.textContent =
            `${totalMinutes} min`;
    }


    // --------------------------------------------------
    // Final arrival
    // --------------------------------------------------

    if (destinationTime) {
        destinationTime.textContent =
            formatClockTime(
                destinationArrival
            );
    }


    // --------------------------------------------------
    // Show results
    // --------------------------------------------------

    if (resultSection) {
        resultSection.hidden = false;
    }


    updateIcons();
}


// ======================================================
// Find Journey
// ======================================================

async function findJourney() {
    const fromStop =
        document.getElementById(
            "fromStop"
        );

    const toStop =
        document.getElementById(
            "toStop"
        );

    const message =
        document.getElementById(
            "journeyMessage"
        );

    const resultSection =
        document.getElementById(
            "journeyResult"
        );


    if (
        !fromStop ||
        !toStop
    ) {
        return;
    }


    const from =
        fromStop.value;

    const to =
        toStop.value;


    // --------------------------------------------------
    // Validation
    // --------------------------------------------------

    if (!from || !to) {

        if (message) {
            message.textContent =
                "Please choose both your pickup and destination stops.";

            message.classList.add(
                "error"
            );
        }

        if (resultSection) {
            resultSection.hidden = true;
        }

        return;
    }


    if (from === to) {

        if (message) {
            message.textContent =
                "Please choose two different stops.";

            message.classList.add(
                "error"
            );
        }

        if (resultSection) {
            resultSection.hidden = true;
        }

        return;
    }


    // --------------------------------------------------
    // Loading
    // --------------------------------------------------

    if (message) {
        message.textContent =
            "Finding the best buses for your journey...";

        message.classList.remove(
            "error"
        );
    }


    if (resultSection) {
        resultSection.hidden = true;
    }


    try {

        // ------------------------------------------------
        // REAL BACKEND REQUEST
        // ------------------------------------------------

        const data =
            await fetchJourney(
                from,
                to
            );


        // ------------------------------------------------
        // Render backend result
        // ------------------------------------------------

        displayJourney(
            data
        );


        // ------------------------------------------------
        // Result message
        // ------------------------------------------------

        const count =
            Math.min(
                Array.isArray(data.buses)
                    ? data.buses.length
                    : 0,
                3
            );


        if (message) {
            message.textContent =
                `${count} ${count === 1
                    ? "bus"
                    : "buses"
                } found for your journey.`;

            message.classList.remove(
                "error"
            );
        }

    } catch (error) {

        console.error(
            "Journey planner error:",
            error
        );


        if (message) {
            message.textContent =
                error.message ||
                "Something went wrong while finding your journey.";

            message.classList.add(
                "error"
            );
        }


        if (resultSection) {
            resultSection.hidden = true;
        }
    }
}


// ======================================================
// Initialize
// ======================================================

function initializeJourneyPlanner() {

    const form =
        document.getElementById(
            "journeyForm"
        );

    const swapButton =
        document.getElementById(
            "swapStops"
        );

    const resultSection =
        document.getElementById(
            "journeyResult"
        );


    // --------------------------------------------------
    // Hide results initially
    // --------------------------------------------------

    if (resultSection) {
        resultSection.hidden = true;
    }


    // --------------------------------------------------
    // Theme
    // --------------------------------------------------

    initializeTheme();


    // --------------------------------------------------
    // Clock
    // --------------------------------------------------

    updateClock();

    setInterval(
        updateClock,
        1000
    );


    // --------------------------------------------------
    // Icons
    // --------------------------------------------------

    updateIcons();


    // --------------------------------------------------
    // Journey form
    // --------------------------------------------------

    if (form) {
        form.addEventListener(
            "submit",
            (event) => {

                event.preventDefault();

                findJourney();

            }
        );
    }


    // --------------------------------------------------
    // Swap button
    // --------------------------------------------------

    if (swapButton) {
        swapButton.addEventListener(
            "click",
            swapStops
        );
    }
}


// ======================================================
// Start
// ======================================================

document.addEventListener(
    "DOMContentLoaded",
    () => {

        initializeJourneyPlanner();

        checkBackend();

    }
);