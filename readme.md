# 🚍 TransitNova

## Real-Time Bus Transit Operations, Live Tracking & Journey Planning System

**TransitNova** is a web-based bus transit simulation and management system designed to model a real-world public transportation network between **Ratnapark Buspark and Bode Buspark, Kathmandu**.

The project combines a **C++20 backend**, custom data structures and algorithms, a continuous bus simulation engine, an HTTP API, and a modern **HTML, CSS and JavaScript frontend**. The backend acts as the core of the system, maintaining buses, routes, terminal queues, maintenance states, travel information and the live simulation, while the frontend provides an interactive interface for users to monitor and interact with the transit system.

The current system simulates a fleet of **22 buses operating across 18 stops** in both directions. Buses can be dispatched from either terminal, travel through the route, stop at intermediate locations, reach the opposite terminal, return to a departure queue, and continue operating. Buses can also be placed into maintenance and removed from active service.

TransitNova is developed as a **Data Structures and Algorithms project**, with the data structures integrated directly into the application's functionality rather than being implemented as isolated examples. **Linked Lists** are used for dynamic bus collections and operational lists, a **Doubly Linked List** represents the bidirectional route, a **Binary Search Tree** provides efficient bus searching, and **Queues** manage terminal departures. The backend also uses **threads and synchronization** to keep the bus simulation running while handling HTTP requests.

### ✨ Main Features

- 🚌 **Live Bus Tracking** — Monitor active buses, their current stops, directions and route progress.
- 🗺️ **Interactive Route Map** — Visualize the complete 18-stop Ratnapark–Bode route.
- 🧭 **Journey Planner** — Select pickup and destination stops and find suitable buses with estimated travel information.
- 🔎 **Bus Search** — Search buses by bus number and view their current operational state.
- 🅿️ **Terminal Management** — Manage buses waiting at Ratnapark and Bode through departure queues.
- 🔧 **Maintenance Management** — Track buses currently unavailable for service.
- ⏱️ **Travel-Time Estimation** — Calculate estimated bus arrival and journey duration.
- 🔄 **Live Simulation** — Continuously simulate bus movement and terminal operations.
- 🔌 **HTTP API** — Connect the frontend with the C++ backend using API requests and JSON data.
- 🌗 **Modern Interface** — Responsive dashboard with live statistics, interactive components and dark/light themes.

### 🛣️ Route

```text
Ratnapark Buspark
       ↓
Singha Durbar
       ↓
Maitighar
       ↓
Bijulibazar
       ↓
Baneshwor
       ↓
Civil
       ↓
Shantinagar
       ↓
Tinkune
       ↓
Koteshwor
       ↓
Jadibuti
       ↓
Lokanthali
       ↓
Kaushaltar
       ↓
Gatthaghar
       ↓
Thimi
       ↓
Bode Chardobato
       ↓
Bode Planning
       ↓
Jaya School
       ↓
Bode Buspark
````

**Eastbound:** Ratnapark Buspark → Bode Buspark


**Westbound:** Bode Buspark → Ratnapark Buspark

### 🏗️ How It Works

```text
                 🚍 TransitNova
                       │
          ┌────────────┴────────────┐
          │                         │
     🌐 Frontend                ⚙️ Backend
   HTML/CSS/JavaScript          C++20/HTTP
          │                         │
          └────────── HTTP ─────────┘
                    │
                    ▼
               🧠 BusManager
                    │
       ┌────────────┼────────────┐
       │            │            │
     🚌 Buses     🛣️ Route     🅿️ Queues
       │            │            │
       └────────────┼────────────┘
                    ▼
             🔄 Simulation
```

The **C++ backend is the source of truth** for the transit system. It continuously updates the live state of buses while simultaneously serving API requests from the frontend. The browser retrieves this information and updates the dashboard, allowing users to observe the simulated transit network in real time.

### 🧠 Data Structures

| Data Structure         | Application                                            |
| ---------------------- | ------------------------------------------------------ |
| **Linked List**        | Active buses, maintenance and dynamic collections      |
| **Doubly Linked List** | Bidirectional route and stop traversal                 |
| **Binary Search Tree** | Bus searching and lookup                               |
| **Queue**              | Terminal departure management                          |
| **Stack**              | Stack-based operations                                 |
| **Threads + Mutex**    | Continuous simulation and shared-state synchronization |

### 🛠️ Technology Stack

**Backend:** C++20, cpp-httplib, nlohmann/json, C++ Threads
**Frontend:** HTML5, CSS3, JavaScript, Lucide Icons
**Development:** Git, GitHub, GCC/G++, VS Code

### 📂 Project Structure

```text
TransitNova/
├── backend/
│   ├── include/
│   └── src/
├── frontend/
│   ├── images/
│   ├── index.html
│   ├── journey.html
│   ├── journey.js
│   ├── search.html
│   ├── search.js
│   ├── script.js
│   └── style.css
├── build.txt
├── .gitignore
└── readme.md
```

### 🚀 Running Locally

Clone the repository:

```bash
git clone https://github.com/raghav-0001/TransitNova.git
cd TransitNova
```

Compile the C++ backend:

```bash
cd backend/src
g++ -std=c++20 *.cpp -o TransitNova -pthread
```

Run:

```bash
./TransitNova
```

On Windows with MinGW:

```bash
g++ *.cpp -std=c++20 -lws2_32 -pthread -o TransitNova
```

```powershell
.\TransitNova.exe
```

The C++ server handles the backend and serves the frontend, so a separate frontend development server is not required for the current setup.

---

## 📌 Project Status

**Active Development — Version 1.0**

TransitNova currently includes live bus simulation, route visualization, bus searching, journey planning, terminal queues, maintenance management and frontend/backend API integration.

---

## 👨‍💻 Team

| Name                | Roll Number |
| ------------------- | ----------- |
| **Raghav Phuyal**   | 081BCT063   |
| **Shrawan Katuwal** | 081BCT077   |
| **Unique Kharel**   | 081BCT092   |

---

<div align="center">

### 🚍 TransitNova

**Smart · Reliable · Efficient**

*Built with C++20 and Web Technologies*

</div>
```
